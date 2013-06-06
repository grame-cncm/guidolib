/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/*
	Tuplet positionning:
		
		Manual: a least one parameter was specified in a \tuplet tag. Keeps
				compatibility with older versions of the library. 

		Automatic: decides if tuplet braces are required or not, decides
				the "direction" of the tuplet marks (above or below the note group),
				minimize collisions.
*/

#include <iostream>
#include <string.h>

#include "ARTuplet.h"

#include "GRNewTuplet.h"
#include "GRStaff.h"
#include "GRPage.h"
#include "GRVoice.h"
#include "VGDevice.h"
#include "FontManager.h"
#include "secureio.h"

using namespace std;

// #include "NEPointerList.h"
// ----------------------------------------------------------------------------
GRNewTuplet::GRNewTuplet ( GRStaff * inStaff, ARTuplet * artuplet )
				   : GRPTagARNotationElement(artuplet)
{
	// (JB) require mGrStaff = inStaff ?

	assert(artuplet);
	assert(inStaff);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	
	GRNewTupletSaveStruct * st = new GRNewTupletSaveStruct;

	sse->p = (void *) st;

	mStartEndList.AddTail(sse);
	mDirection = dirUP;

	mShowLeftBrace = false;
	mShowRightBrace = false;
}

// ----------------------------------------------------------------------------
GRNewTuplet::~GRNewTuplet()
{
	delete mAssociated;	// base destructor does a more complex job, should be removed ? 
	mAssociated = 0;
}

// ----------------------------------------------------------------------------
const ARTuplet * GRNewTuplet::getARTuplet() const
{
	return static_cast/*dynamic cast*/<ARTuplet *>(mAbstractRepresentation);
}

// ----------------------------------------------------------------------------
void 
GRNewTuplet::tellPosition(GObject * caller, const NVPoint & inPos )
{
	const ARTuplet * arTuplet = getARTuplet();

	if( arTuplet->isFormatSet() || arTuplet->isDySet())
	{
		manualPosition( caller, inPos );
	}
	else
	{
		// automaticDirection();
		automaticPosition( caller, inPos );
	}
}

// ----------------------------------------------------------------------------
void 
GRNewTuplet::manualPosition(GObject * caller, const NVPoint & inPos )
{
	GREvent * event = GREvent::cast( caller );
	if( event == 0 ) return;

	GRStaff * staff = event->getGRStaff();
	if( staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if( sse == 0 ) return;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;

	// if ( openLeftRange && openRightRange ) return;

	GRNewTupletSaveStruct * st = (GRNewTupletSaveStruct *)sse->p;

	const ARTuplet * arTuplet = getARTuplet();

	float dy1 = arTuplet->isDySet() ? arTuplet->getDy1() : float(1.25) * LSPACE;
	float dy2 = arTuplet->isDySet() ? arTuplet->getDy2() : float(1.25) * LSPACE;

	if(( dy1 > 0 ) || ( dy2 > 0 ))
		mDirection = dirUP;

	const float halfNoteWidth = LSPACE * float(0.65); // harcoded
	if (event == startElement)
	{
		st->p1 = startElement->getPosition();		
		st->p1.x -= halfNoteWidth; // to enclose all the element width
		st->p1.y -= dy1;
	}
	else if (event == endElement)
	{
		st->p2 = endElement->getPosition();
		st->p2.x += halfNoteWidth; // to enclose all the element width
		st->p2.y -= dy2;
	}
	
	if( event == endElement || ( endElement == 0 && event == startElement) )
	{
		if (startElement && endElement)
		{
			const float posx = (st->p2.x - st->p1.x) * 0.5f + st->p1.x;
			const float posy = st->p2.y > st->p1.y ? (st->p2.y) : (st->p1.y);

			// posy += 2*LSPACE;
		
			st->textpos.x = posx;
			st->textpos.y = posy;
		}
		else
			st->textpos = inPos;
	}

//	if( arTuplet->isFormatSet())
//	{
		mShowLeftBrace = arTuplet->getLeftBrace();
		mShowRightBrace = arTuplet->getRightBrace();
//	}

}

/*
// ----------------------------------------------------------------------------
void 
GRNewTuplet::automaticDirection()
{
	GuidoPos pos = mAssociated->GetHeadPosition();
	while( pos )
	{
		GRNotationElement * el = mAssociated->GetNext(pos);
		if( el && ( el != startElement ) && ( el != endElement ))
		{
		}
	}

//	mDirection = event->getStemDirection();

}
*/

// ----------------------------------------------------------------------------
/** Places the tuplet bracket and/or numeral, close to its group of notes.

	Calculates the positions of the two possible tuplet bracket (above 
	 and below) then choose the best one.

*/
void  GRNewTuplet::automaticPosition(GObject * caller, const NVPoint & inPos )
{
	GREvent * callerEv = GREvent::cast( caller );
	if( callerEv == 0 ) return;

	GRStaff * staff = callerEv->getGRStaff();
	if( staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if( sse == 0 ) return;

	GRNewTupletSaveStruct * st = (GRNewTupletSaveStruct *)sse->p;
	if( st == 0 ) return;

	GREvent * startElement = GREvent::cast( sse->startElement );
	if( startElement == 0 ) return;

	GREvent * endElement = GREvent::cast( sse->endElement );
	if( endElement == 0 ) return;

	// - Accept being positioned only once, if possible by the last tuplet element.
	//if(( callerEv != endElement ) && ((callerEv != startElement) || (endElement != 0)))
	if( callerEv != endElement )
		return;
	
	// - Check for beams
	const bool firstBeamed = (startElement->getBeamCount() > 0);
	const bool lastBeamed = (endElement->getBeamCount() > 0);

	mShowLeftBrace = !(firstBeamed && lastBeamed );
	mShowRightBrace = mShowLeftBrace;

	// - Get first and last element positions to work with
	float startX, endX;
	startX = endX = 0;
	float startUpY, startDownY, endUpY, endDownY;

		// x positions
	const float halfNoteWidth = LSPACE * float(0.65); // harcoded
	startX = startElement->getPosition().x;
	endX = endElement->getPosition().x;

//	if( endX == startX ) return;		// DF commented: results in strange vertical bars

	startX -= halfNoteWidth;
	endX += halfNoteWidth;
		
		// y positions
	const NVRect & leftBox = startElement->getBoundingBox();
	const NVRect & rightBox = endElement->getBoundingBox();
	const float elPos1y = startElement->getPosition().y;
	const float elPos2y = endElement->getPosition().y;

	startUpY = leftBox.top + elPos1y;
	endUpY = rightBox.top + elPos2y;

	startDownY = leftBox.bottom + elPos1y;
	endDownY = rightBox.bottom + elPos2y;

	// - Calculate a virtual line from the first to the last element,
	
	int stemStats = 0;
	const float deltaX = (endX - startX);

	float slopeUp = (endUpY - startUpY) / deltaX; // slope of the virtal line above
	float slopeDown = (endDownY - startDownY) / deltaX; // slope of the virtal line below

	float x, yUp, yDown, distUp, distDown;
	float mxUp = 0, myUp = 0, mxDown = 0, myDown = 0;	// 'extreme' middle point
	bool collideUp = false;
	bool collideDown = false;

	GuidoPos pos = mAssociated->GetHeadPosition();
	while( pos )
	{
		GREvent * el = GREvent::cast( mAssociated->GetNext(pos));
		if( el == 0 ) continue;

		// - Generate stats about stem directions
		GDirection stemDir = el->getStemDirection();
		if( stemDir == dirUP ) ++ stemStats;
		else if( stemDir == dirDOWN ) -- stemStats; 
		
		if(( el != startElement ) && ( el != endElement ))
		{
			// - Get the element box
			const NVRect & elBox = el->getBoundingBox();
			const NVPoint & elPos = el->getPosition();
			
			x = elPos.x;
			yUp = elBox.top + elPos.y;
			yDown = elBox.bottom + elPos.y;
			
			// - Calculate the y-distance between the element and the virtual line.
			// distY = startY - (y - slope * (x - startX)); 

			distUp = (startUpY - yUp) + (x - startX) * slopeUp;
			distDown = (startDownY - yDown) + (x - startX) * slopeDown;

			if( distUp > 0 )	// then the point is above the virtual line
			{
				mxUp = x;
				myUp = yUp; 
				collideUp = true;
				//startUpY -= distUp;	// TODO: better handling of this collision
				//endUpY -= distUp;
			}
		
			if( distDown < 0 )	// then the point is below the virtual line
			{
				mxDown = x;
				myDown = yDown; 
				collideDown = true;
				//startDownY -= distDown;
				//endDownY -= distDown;
			}
		}
	}

	// - Adjust the brace to avoid collisions. It must be above (or below) all elements, 
	// while remaining close to those elements, and avoid being in the staff.
	// brace 1 (upward)
	if( collideUp )
	{
		if(( myUp <= startUpY ) && ( myUp <= endUpY ))	// middle point above start and end points
		{
			// slopeUp = 0; // horizontal
			startUpY = myUp;
			endUpY = myUp;
		}
		else	
		{
			if( myUp <= endUpY ) // middle point above end point only: shift the end point up
			{
				slopeUp = (myUp - startUpY) / (mxUp - startX);
				endUpY = startUpY + (deltaX * slopeUp);
			}
			else // middle point above start point only: shift the start point up
			{
				slopeUp = (endUpY - myUp) / (endX - mxUp);
				startUpY = endUpY - (deltaX * slopeUp);
			}
		}
	}

	// brace 2 (downward)
	if( collideDown )
	{
		if(( myDown >= startDownY ) && ( myDown >= endDownY )) // middle point below start and end points
		{
			// slopeDown = 0; // horizontal
			startDownY = myDown;
			endDownY = myDown;
		}
		else
		{
			if( myDown >= endDownY ) // middle point below end point only: shift the end point down
			{
				slopeDown = (myDown - startDownY) / (mxDown - startX);
				endDownY = startDownY + (deltaX * slopeDown);
			}
			else // middle point below start point only: shift the start point down
			{
				slopeDown = (endDownY - myDown) / (endX - mxDown);
				startDownY = endDownY - (deltaX * slopeDown);
			}
		}
	}

	// - Avoid being inside staff
	if( startUpY > 0 )	startUpY = 0;	
	if( endUpY > 0 )	endUpY = 0;	

	const float staffHeight = staff->getDredgeSize();
	if( startDownY < staffHeight )	startDownY = staffHeight;	
	if( endDownY < staffHeight )	endDownY = staffHeight;	

	// - Tune
	float marginY = LSPACE * float(1.25);

	startUpY -= marginY;
	endUpY -= marginY;
	
	startDownY += marginY;
	endDownY += marginY;

	// - Choose the best solution (above or below)
	// We put the brace and the tuplet numeral on the stem side.

	float startY;
	float endY;
	
	if( stemStats >= 0 ) // stems tend to be up.
	{
		mDirection = dirUP;
		startY = startUpY;
		endY = endUpY;
	}
	else
	{
		mDirection = dirDOWN;
		startY = startDownY;
		endY = endDownY;
	}

	// - Store results
	const float textOffsetY = LSPACE;

	st->p1.x = startX;
	st->p1.y = startY;
	st->p2.x = endX;
	st->p2.y = endY;

	st->textpos.x = (startX + endX) * float(0.5);
	st->textpos.y = (startY + endY) * float(0.5) + textOffsetY;
}


// ----------------------------------------------------------------------------
void GRNewTuplet::OnDraw(VGDevice & hdc) const
{ 
	if(!mDraw)
		return;
	assert(gCurSystem);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0)
		return;

	GRNewTupletSaveStruct * st = (GRNewTupletSaveStruct *)sse->p;	

	const ARTuplet * arTuplet = getARTuplet();

	int charCount = 0;

	// - Draws the number
	const int numerator = arTuplet->getNumerator();
	if( numerator > 0 )
	{
		char buffer[8];
		const int denominator = arTuplet->getDenominator(); 
		if( denominator > 0 )
			snprintf( buffer, 8, "%d:%d", numerator, denominator ); // should be cached somewhere
		else
			snprintf( buffer, 8, "%d", numerator );
	
		charCount = (int)strlen( buffer );

		hdc.SetTextFont( FontManager::gFontText );
		hdc.SetFontAlign( VGDevice::kAlignCenter | VGDevice::kAlignBottom );
		hdc.DrawString( st->textpos.x, st->textpos.y, buffer, charCount );
	}

	// - Draws the braces
	const float middleX = (st->p1.x + st->p2.x) * 0.5f;
	const float middleY = (st->p1.y + st->p2.y) * 0.5f;
	const float slope = (st->p2.y - st->p1.y) / (st->p2.x - st->p1.x); //<- could be stored
	const float textSpace = ((float)charCount + float(0.5)) * LSPACE * float(0.5);

	if( mShowLeftBrace | mShowRightBrace )
	{
		const float bracketThick = LSPACE * float(0.08);
		hdc.PushPenWidth( bracketThick );
		
		if( mShowLeftBrace ) //arTuplet->getLeftBrace()) // (mBraceState & BRACELEFT)
		{
			if( sse->startflag == GRSystemStartEndStruct::LEFTMOST)
			{
				hdc.Line(st->p1.x, st->p1.y + 0.5f * LSPACE * mDirection, st->p1.x, st->p1.y);
			}
			hdc.Line( st->p1.x, st->p1.y, middleX - textSpace, middleY - slope * textSpace );
		}

		if( mShowRightBrace ) //arTuplet->getRightBrace()) // (mBraceState & BRACERIGHT)
		{
			hdc.Line( middleX + textSpace, middleY + slope * textSpace, st->p2.x, st->p2.y );
			if( sse->endflag == GRSystemStartEndStruct::RIGHTMOST)
			{
				hdc.Line( st->p2.x, st->p2.y, st->p2.x, st->p2.y + 0.5f * LSPACE * (float)mDirection);
			}
		}
		hdc.PopPenWidth();
	}
}

// ----------------------------------------------------------------------------
void GRNewTuplet::print() const
{
}

/* First new version
// ----------------------------------------------------------------------------
void 
GRNewTuplet::automaticPosition(GObject * caller, const NVPoint & inPos )
{
	GREvent * event = GREvent::cast( caller );
	if( event == 0 ) return;

	GRStaff * staff = event->getGRStaff();
	if( staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if( sse == 0 ) return;

	GRNewTupletSaveStruct * st = (GRNewTupletSaveStruct *)sse->p;
	if( st == 0 ) return;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;

	// - Accept being positioned only once, end-element prefered, if possible.
	if(( caller != endElement ) && ((caller != startElement) || (endElement != 0)))
		return;
	
	// - Choose the direction: up (above) or down (below)

	mDirection = event->getStemDirection();
	const bool upward = (mDirection != dirDOWN);

	// - Get first and last element positions to work with
	float startX, startY, endX, endY;
	const float halfNoteWidth = LSPACE * float(0.65); // harcoded

	const NVRect & leftBox = startElement->getBoundingBox();
	const NVRect & rightBox = endElement->getBoundingBox();

	startX = startElement->getPosition().x - halfNoteWidth;
	endX = endElement->getPosition().x + halfNoteWidth;

	if( upward )
	{
		startY = leftBox.top;
		endY = rightBox.top;
	}
	else
	{
		startY = leftBox.bottom;
		endY = rightBox.bottom;
	}

	startY += startElement->getPosition().y;
	endY += endElement->getPosition().y;

	// - Calculates the position of the tuplet bracket.
	// - The idea is to calculate a virtual line from the first to the last element,
	// and to shift this line up (or down) to avoid intersections with other elements.
	if( endX == startX ) return;

	float slope = (endY - startY) / (endX - startX); // slope of the virtal line.

	float x, y, distY;

	GuidoPos pos = mAssociated->GetHeadPosition();
	while( pos )
	{
		GRNotationElement * el = mAssociated->GetNext(pos);
		if( el && ( el != startElement ) && ( el != endElement ))
		{
			// - Get the element box
			const NVRect & elBox = el->getBoundingBox();
			const NVPoint & elPos = el->getPosition();
			
			x = elPos.x;
			y = upward ?  elBox.top : elBox.bottom;
			y += elPos.y;

			// - Calculate the y-distance between the element and the virtual line.
			// distY = startY - (y - slope * (x - startX)); 

			distY = (startY - y) + (x - startX) * slope;

			if(( upward && (distY > 0)) || ( !upward && (distY < 0)))
			{
				startY -= distY;
				endY -= distY;
			}
		}
	}

	// - Avoid being inside staff
	if( upward )
	{
		if( startY > 0 )	startY = 0;	
		if( endY > 0 )		endY = 0;	
	}
	else
	{
		const float staffHeight = staff->getDredgeSize();
		if( startY < staffHeight )	startY = staffHeight;	
		if( endY < staffHeight )	endY = staffHeight;	
	}

	// - Tune
	float textOffsetY;
	float marginY;

	if( upward )
	{
		marginY = - LSPACE * float(1.25);
		textOffsetY = LSPACE;
	}
	else
	{
		marginY = LSPACE * float(1.25);
		textOffsetY = LSPACE;
	}

	startY += marginY;
	endY += marginY;
	
	// - Store results

	st->p1.x = startX;
	st->p1.y = startY;
	st->p2.x = endX;
	st->p2.y = endY;

	st->textpos.x = (startX + endX) * float(0.5);
	st->textpos.y = (startY + endY) * float(0.5) + textOffsetY;
}*/

/* Original version:
void GRNewTuplet::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if((grel == 0) || (grel->getGRStaff() == 0))
	{
		assert(false);
		return;
	}

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grel->getGRStaff()->getGRSystem());
	assert(sse);
	if (!sse)
		return;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;

	// if ( openLeftRange && openRightRange ) return;

	GRNewTupletSaveStruct * st = (GRNewTupletSaveStruct *)sse->p;

	const ARTuplet * arTuplet = getARTuplet();

	float dy1 = arTuplet->isDySet() ? -arTuplet->getDy1() : float(1.25) * LSPACE;
	float dy2 = arTuplet->isDySet() ? -arTuplet->getDy2() : float(1.25) * LSPACE;

	if (grel == startElement)
	{
		st->p1 = startElement->getPosition();		
		st->p1.y += (GCoord)(dy1);
	}
	if (grel == endElement)
	{
		st->p2 = endElement->getPosition();
		st->p2.y += (GCoord)(dy2);
	}
	if ( grel == endElement || ( endElement == 0 && grel == startElement) )
	{

		if (startElement && endElement)
		{
			const float posx = (st->p2.x - st->p1.x) * 0.5f + st->p1.x;
			const float posy = st->p2.y > st->p1.y ? (st->p2.y) : (st->p1.y);

			// posy += 2*LSPACE;
		
			st->textpos.x = (GCoord)posx;
			st->textpos.y = (GCoord)posy;
		}
		else
			st->textpos = newPosition;
	}
}
*/

/* Original version:
void GRNewTuplet::OnDraw(VGDevice & hdc) const
{ 

	assert(gCurSystem);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (!sse)
		return;

	GRNewTupletSaveStruct * st = (GRNewTupletSaveStruct *) sse->p;	

	// The first implementation justs draws the string ...
	char buffer[30];
	char * mybuf = &buffer[0];
	strncpy(buffer,getARTuplet()->getName(),30);
	if (buffer[0] == '-')
		mybuf = &buffer[1];
	int length = (int)strlen(mybuf);
	if (mybuf[ length - 1 ] == '-')
	{
		mybuf[ length - 1 ] = 0;
		-- length;
	}

	// TODO: SelectPen !
	
	if (mBraceState & BRACELEFT 
		&& sse->startflag == GRSystemStartEndStruct::LEFTMOST)
	{
	  hdc.MoveTo(st->p1.x, st->p1.y - 0.5f * LSPACE);
	  hdc.LineTo(st->p1.x, st->p1.y);
	}

	// now draw the lines ...
	hdc.MoveTo(st->p1.x,st->p1.y);

	const float middleX = (st->p1.x + st->p2.x) * 0.5f;
	const float middleY = (st->p1.y + st->p2.y) * 0.5f;

	if (mBraceState & BRACELEFT)
		hdc.LineTo( middleX, middleY );

	hdc.MoveTo( middleX, middleY );

	if (mBraceState & BRACERIGHT)
		hdc.LineTo( st->p2.x, st->p2.y );

	if (mBraceState & BRACERIGHT && 
		sse->endflag == GRSystemStartEndStruct::RIGHTMOST)
	{
		hdc.LineTo( st->p2.x, st->p2.y - 0.5f * LSPACE);
	}

	GFontRef hfontold = hdc.SelectFont( gFontDefs.gFontText );
	GColor backColor = hdc.GetTextBackgroundColor();
	hdc.SetTextBackgroundColor( 255, 255, 255, 255 );

	const unsigned int ta = hdc.GetTextAlign();
	hdc.SetTextAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );
	hdc.PrintAt( st->textpos.x, st->textpos.y, mybuf, length );
	hdc.SetTextAlign(ta);

	hdc.SetTextBackgroundColor( backColor );
//	hdc.SelectFont( hfontold );			// JB test for optimisation: do not restore font context.
}
*/
