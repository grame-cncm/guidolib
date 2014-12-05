/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cstdlib>
#include "math.h"
#include "VGDevice.h"

#include "ARGlissando.h"
#include "ARNote.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "GRStaff.h"
#include "GRGlue.h"
#include "GRSingleNote.h"
#include "GRAccidental.h"
#include "GRGlobalStem.h"
#include "GRRest.h"
#include "GREmpty.h"
#include "GRStdNoteHead.h"
#include "GraphTools.h"

#include "GRGlissando.h"

extern GRSystem * gCurSystem;

using namespace std;


// -----------------------------------------------------------------------------
GRGlissando::GRGlissando(GRStaff * grstaff)
	: GRPTagARNotationElement( new ARGlissando, true ) // ownsAR
{
	initGRGlissando( grstaff );
	flaststartElement = NULL;
	prevGRGlissando = NULL;
	wavy = false;
	fill = false;
	hidden = false;
	filled = false;
}

// -----------------------------------------------------------------------------
GRGlissando::GRGlissando(GRStaff * grstaff, ARGlissando * abstractRepresentationOfGlissando)
				   : GRPTagARNotationElement(abstractRepresentationOfGlissando)
{
	assert(abstractRepresentationOfGlissando);
	initGRGlissando( grstaff );
	flaststartElement = NULL;
	prevGRGlissando = NULL;
	wavy = abstractRepresentationOfGlissando->isWavy();
	fill = abstractRepresentationOfGlissando->isFill();
	hidden = false;
	filled = false;
}

GRGlissando::~GRGlissando()
{
	assert(mStartEndList.GetCount() == 0);
	FreeAssociatedList();
}

GRSystemStartEndStruct * GRGlissando::initGRGlissando( GRStaff * grstaff )
{
	assert(grstaff);

	setGRStaff( grstaff );// TEST

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();

	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endflag   = GRSystemStartEndStruct::RIGHTMOST;

	mStartEndList.AddTail(sse);
		
	fglissInfos = new GRGlissandoSaveStruct;
	fglissInfos->numPoints = 4;

	sse->p = (void *)fglissInfos;
	
	return sse;
}

void GRGlissando::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw) return;
	if (error) return;

	assert( gCurSystem );

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if( sse == 0)
		return; // don't draw

	if (mColRef) 
		hdc.PushFillColor( VGColor( mColRef ) );
	
	/////////////////////  straight line
	if(!wavy && !hidden)
	{
		float coorX[4] = {fglissInfos->points[0].x, fglissInfos->points[1].x, fglissInfos->points[2].x, fglissInfos->points[3].x};
		float coorY[4] = {fglissInfos->points[0].y, fglissInfos->points[1].y, fglissInfos->points[2].y, fglissInfos->points[3].y};
		
		// if we're right open, the glissando ends with staff
		if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		{
			NVRect r = getGRStaff()->getBoundingBox();
			r += getGRStaff()->getPosition();
			coorX[2] = coorX[3] = r.right - LSPACE;
		}

		//if we're left open the glissando begins just before the last note 
		if (sse->startflag == GRSystemStartEndStruct::OPENLEFT )
		{
			coorX[0] = coorX[1] = coorX[3] - LSPACE;
		}
		
		// in any of those two cases, we have to re-calculate the thickness of our glissando line.
		// there should be a better way to do it, maybe in updateGlissando and not here..
		if(!fill && (sse->startflag == GRSystemStartEndStruct::OPENLEFT || sse->endflag == GRSystemStartEndStruct::OPENRIGHT))
		{
			float deltaX = fglissInfos->points[3].x - fglissInfos->points[0].x;
			float deltaY = fglissInfos->points[3].y - fglissInfos->points[0].y;
			float deltaX2 = coorX[3]-coorX[0];
			float deltaThickness = (fglissInfos->points[3].y-fglissInfos->points[2].y)*(1/sqrt(deltaX*deltaX + deltaY*deltaY)*deltaX*sqrt(deltaX2*deltaX2 + deltaY*deltaY)/deltaX2-1)/2;
			coorY[0] += deltaThickness;
			coorY[1] -= deltaThickness;
			coorY[3] += deltaThickness;
			coorY[2] -= deltaThickness;
		}

		hdc.Polygon(coorX, coorY, 4);
	}

	//////////////////// wavy
	else if (!hidden)
	{
		//not implemented yet
		//we have to implement new function of the device, in order to be able to rotate a symbol
		
		/*
		float width = fglissInfos->points[3].x - fglissInfos->points[0].x;
		float height = fglissInfos->points[3].y - fglissInfos->points[0].y;
		float pasX = width/10;
		float pasY = height/10;

		float X = fglissInfos->points[0].x;
		float Y = fglissInfos->points[0].y;
		const int NSEGS = 25;

		for(int i=0; i<10; i++)
		{
			NVPoint thePoints[ 2*(NSEGS+3) ];
			int index = 0;
			float x1 = X+i*pasX;
			float y1 = Y+i*pasY;
			float y2a;
			float y2b;
			float x3 = X+pasX*(i+1);
			float y3 = Y+pasY*(i+1);
			if(i%2==0)
			{
				y2a = y1 - (pasY + pasX*pasX/pasY)/2;
				y2b = y3 - (pasY + pasX*pasX/pasY)/2;
			}
			else
			{
				y2a = y1 + (pasY + pasX*pasX/pasY)/2;
				y2b = y3 + (pasY + pasX*pasX/pasY)/2;
				
				x1-=1;
				x3+=1;
			}
			makeCurve(x1, y1, x1, y2a, x3, y2b, x3, y3, NSEGS, thePoints, &index);
			
			if(i%2==0)
			{
				y2a-=5;
				y2b-=5;
				x3-=1;
				x1+=1;
			}
			else
			{
				y2a+=5;
				y2b+=5;
				x1+=1;
				x3-=1;
			}
			
			makeCurve(x3, y3, x3, y2b, x1, y2a, x1, y1, NSEGS, thePoints, &index);
			float xPoints [ 2*(NSEGS+3) ];
			float yPoints [ 2*(NSEGS+3) ];
			for( int currPt = 0; currPt < index; ++ currPt )
			{
				xPoints [ currPt ] = thePoints[ currPt ].x;
				yPoints [ currPt ] = thePoints[ currPt ].y;
			}

			hdc.Polygon( xPoints, yPoints, index );
			
		}
		*/
	}
	if (mColRef)
        hdc.PopFillColor();
}


void GRGlissando::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0)
        return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0)
        return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)
        return;

	const GRNotationElement * const endElement = sse->endElement;

	if (grel == endElement)
		updateGlissando(staff);
}


void GRGlissando::updateGlissando( GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(inStaff->getGRSystem());
	if (sse == 0)
        return;

	// Collects informations about the context

	fglissContext.staff = inStaff;
	getGlissandoBeginningContext( &fglissContext, sse );
	getGlissandoEndingContext( &fglissContext, sse );

	ARGlissando * arGliss = static_cast<ARGlissando *>(getAbstractRepresentation());
	const float staffLSpace = inStaff->getStaffLSPACE();
	assert(arGliss);
	

	// we gather the informations of parameters from the AR
	float dx1 = arGliss->getDx1()->getValue( staffLSpace );
	float dy1 = arGliss->getDy1()->getValue( staffLSpace );
	float dx2 = arGliss->getDx2()->getValue( staffLSpace );
	float dy2 = arGliss->getDy2()->getValue( staffLSpace );

	// initialization of all variables we need

	//  firstLeftHead                      firstRightHead
	//  (XLeft,Yleft)---------------------(XRight, YRight)
	//         |                                  |
	//         |                                  |
	//         |                                  |
	//         |                                  |
	//         |                                  |
	//  secondLeftHead                      secondRightHead
	//  (XLeft,Yleft2)---------------------(XRight, YRight2)

	//NB : first and second noteHeads can be inverted.

	float XLeft = 0;
	float YLeft = 0;
	float YLeft2 = 0;
	float XRight = 0;
	float YRight = 0;
	float YRight2 = 0;

	// those will be function of the noteHeads width
	float dFirstLeftx = 0;
	float dFirstLefty = 0;
	float dFirstRightx = 0;
	float dFirstRighty = 0;

	// in case of an accidental on the right note, we'll have to modify our XRight
	float acc = 0;
	
	
	if (fglissContext.firstRightHead)
	{
		// position of the noteHead + its offset
		XRight = fglissContext.firstRightHead->getPosition().x + fglissContext.rightNoteDX;
		YRight = fglissContext.firstRightHead->getPosition().y + fglissContext.rightNoteDY;
		// the basic x-offset will be 3/4 of the noteHead size.
		dFirstRightx = fglissContext.firstRightHead->getBoundingBox().Width() * 3/4 * fglissContext.sizeRight;

		if (fglissContext.accidentalRight)
			acc = fglissContext.accidentalRight->getBoundingBox().Width() * getSize() + LSPACE * 0.5f;
	}


	if (fglissContext.firstLeftHead)
	{
		// position of the noteHead + its offset
		XLeft = fglissContext.firstLeftHead->getPosition().x + fglissContext.leftNoteDX; 
		YLeft = fglissContext.firstLeftHead->getPosition().y + fglissContext.leftNoteDY;
		// the basic x-offset will be 3/4 of the noteHead size.
		dFirstLeftx = fglissContext.firstLeftHead->getBoundingBox().Width() * 3/4 * fglissContext.sizeLeft;
	}

	// the thickness entered by the user
	float thicknessWanted = arGliss->getThickness()->getValue(staffLSpace);
	
	if (fill && fglissContext.secondLeftHead && fglissContext.secondRightHead)
	{
		// now we are in the case of the "fill" option, and we have two glissandi in parallel (chords or clusters)

		YLeft2  = fglissContext.secondLeftHead->getPosition().y + fglissContext.leftNoteDY;
		YRight2 = fglissContext.secondRightHead->getPosition().y + fglissContext.rightNoteDY;

		// we add all the variables
		if(YLeft > YLeft2)
		{
			fglissInfos->points[0].y = YLeft  - dy1 + thicknessWanted/2;
			fglissInfos->points[1].y = YLeft2 - dy1 - thicknessWanted/2;
		}
		else
		{
			fglissInfos->points[0].y = YLeft  - dy1 - thicknessWanted/2;
			fglissInfos->points[1].y = YLeft2 - dy1 + thicknessWanted/2;	
		}

		if (YRight > YRight2)
		{
			fglissInfos->points[3].y = YRight  - dy2 + thicknessWanted/2;
			fglissInfos->points[2].y = YRight2 - dy2 - thicknessWanted/2;
		}
		else
		{
			fglissInfos->points[3].y = YRight  - dy2 - thicknessWanted/2;
			fglissInfos->points[2].y = YRight2 - dy2 + thicknessWanted/2;
		}
		
		fglissInfos->points[0].x = fglissInfos->points[1].x = XLeft  + dx1 + dFirstLeftx;
		fglissInfos->points[3].x = fglissInfos->points[2].x = XRight + dx2 - dFirstRightx - acc;
		fglissInfos->position = fglissInfos->points[0];
		
		// we have to remember the fact that this glissando has been filled, so it is not set "hidden" 
		// by the next one in the case of a more than 2 notes chord.
		filled = true;
	}
	else
	{
		// now we manage the case of same Y but different pitches...
		if (YRight == YLeft)
		{
			bool forceUp = false;
			bool forceDown = false;
			compareAccidentals(sse, &forceUp, &forceDown);
			if(forceUp)
			{
				YLeft  += LSPACE/4;
				YRight -= LSPACE/4;
			}
			else if (forceDown)
			{
				YLeft  -= LSPACE/4;
				YRight += LSPACE/4;
			}
		}

		// in order to calculate the orientation, the thickness, and the y-offset
		float deltaX = XRight - XLeft;
		float deltaY = YRight - YLeft;

		// basic y-offset
		dFirstLefty  = dFirstLeftx * deltaY/deltaX;
		dFirstRighty = dFirstRightx * deltaY/deltaX;

		// in order to have the same thickness, independently from the orientation of the line
		float thickness = thicknessWanted*sqrt(deltaX*deltaX + deltaY*deltaY)/deltaX*getSize();

		// we add all the variables
		fglissInfos->points[0].y = YLeft  - dy1 + dFirstLefty + thickness/2;
		fglissInfos->points[1].y = YLeft  - dy1 + dFirstLefty - thickness/2;
		fglissInfos->points[3].y = YRight - dy2 - dFirstRighty + thickness/2;
		fglissInfos->points[2].y = YRight - dy2 - dFirstRighty - thickness/2;
		fglissInfos->points[0].x = fglissInfos->points[1].x = XLeft + dx1 + dFirstLeftx;
		fglissInfos->points[3].x = fglissInfos->points[2].x = XRight + dx2 - dFirstRightx - acc;
		fglissInfos->position = fglissInfos->points[0];
	}
}

void GRGlissando::getGlissandoBeginningContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse )
{
	GRNotationElement * startElement = sse->startElement;
	if(sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		startElement = flaststartElement;
		
	GRSingleNote * note = dynamic_cast<GRSingleNote *>(startElement);

	if(note)
	{
		ioContext->firstLeftHead = note->getNoteHead();

		// fill = true  &&  a previous glissando exists ?
		if (fill && prevGRGlissando && prevGRGlissando->getContext()->firstLeftHead)
		{
			if (prevGRGlissando->getContext()->firstLeftHead->getPosition().x == fglissContext.firstLeftHead->getPosition().x)
			{
				ioContext->secondLeftHead = prevGRGlissando->getContext()->firstLeftHead;
				// if this glissando fills the space between him and the previous one, we don't need to draw the line of the previous one
				// except in the case of another filled glissando.
				if (!prevGRGlissando->isFilled())
					prevGRGlissando->setHidden();
			}
		}
		else
			ioContext->secondLeftHead = NULL;

		ioContext->sizeLeft   = note->getSize();
		ioContext->leftNoteDX = note->getOffset().x;
		ioContext->leftNoteDY = note->getOffset().y;
	}
}

void GRGlissando::getGlissandoEndingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse )
{	
	GRNotationElement * endElement = sse->endElement;

	if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		endElement = lastendElement;

	GRSingleNote * note = dynamic_cast<GRSingleNote *>(endElement);

	if(note)
	{
		//noteHead
		ioContext->firstRightHead = note->getNoteHead();
		//accidental
		GRAccidentalList noteacclist;
		note->extractAccidentals( &noteacclist );
		if(!noteacclist.empty())
			ioContext->accidentalRight = noteacclist.GetHead();

		// fill = true  &&  a previous glissando exists ?
		if (fill && prevGRGlissando && prevGRGlissando->getContext()->firstRightHead)
		{
			if(prevGRGlissando->getContext()->firstRightHead->getPosition().x == fglissContext.firstRightHead->getPosition().x)
			{
				//noteHead
				ioContext->secondRightHead = prevGRGlissando->getContext()->firstRightHead;
				//accidental
				if(!ioContext->accidentalRight)
				{
					ioContext->accidentalRight = prevGRGlissando->getContext()->accidentalRight;
				}
				// already done in the begining context..
				if(!prevGRGlissando->isFilled())
					prevGRGlissando->setHidden();
			}
		}
		else
			ioContext->secondRightHead = NULL;
		
		ioContext->sizeRight   = note->getSize();
		ioContext->rightNoteDX = note->getOffset().x;
		ioContext->rightNoteDY = note->getOffset().y;
	}
}

void GRGlissando::compareAccidentals(GRSystemStartEndStruct * sse, bool * isUp, bool * isDown)
{
	GRNotationElement * startElement = sse->startElement;
	if(sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		startElement = flaststartElement;
	GRSingleNote * startnote = dynamic_cast<GRSingleNote *>(startElement);

	GRNotationElement * endElement = sse->endElement;
	if(sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		endElement = lastendElement;
	GRSingleNote * endnote = dynamic_cast<GRSingleNote *>(endElement);

	if(startnote && endnote )
	{
		ARNote * arstartnote = startnote->getARNote();
		ARNote * arendnote = endnote->getARNote();
		if(arstartnote && arendnote )
		{
			int startnoteAcc = arstartnote->getAccidentals() + int(arstartnote->getDetune());
			int endnoteAcc = arendnote->getAccidentals() + int(arendnote->getDetune());
			if(startnoteAcc < endnoteAcc)
				*isUp = true;
			else if(endnoteAcc < startnoteAcc)
				*isDown = true;
		}
	}
}


void GRGlissando::removeAssociation(GRNotationElement * el )
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}
void GRGlissando::addAssociation(GRNotationElement * grnot)
{
	if (error) return;

		if ( GREvent::cast( grnot )  && 	// stop immediately if it's not an event.
		(dynamic_cast<GRNote *>(grnot) ||
		 dynamic_cast<GRRest *>(grnot) ||
		 dynamic_cast<GREmpty *>(grnot)))
	{
	  	GRARNotationElement::addAssociation(grnot);
	}
	else
	{
		setError(1);
	}

	if (!error)
	{
		GRPositionTag::addAssociation(grnot);
	}
}


// -----------------------------------------------------------------------------
GRNotationElement * GRGlissando::getStartElement(GRStaff * grstaff) const
{
	GuidoPos pos = mStartEndList.GetHeadPosition();
	while (pos)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetNext(pos);
		if (sse->grsystem == grstaff->getGRSystem())
			return sse->startElement;
	}
	return 0;
}

// -----------------------------------------------------------------------------
GRNotationElement * GRGlissando::getEndElement(GRStaff * grstaff) const
{
	GuidoPos pos = mStartEndList.GetHeadPosition();
	while (pos)
	{
		GRSystemStartEndStruct * sse = mStartEndList.GetNext(pos);
		if (sse->grsystem == grstaff->getGRSystem())
			return sse->endElement;
	}
	return 0;
}

//reimplemented to keep the last startelement in memory..
void GRGlissando::BreakTag(GRStaff * grstaff, GuidoPos & assocpos)
{
	if (grstaff == 0) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	sse->grsystem = grstaff->getGRSystem();

	assert(sse);
	
	NEPointerList * associated = 0;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(this);
	if (el)
		associated = el->getAssociations();

	if (!associated) return;
	

	lastendElement = sse->endElement;
	lastendflag = sse->endflag;
	lastendpos = sse->endpos;
	
	sse->endflag = GRSystemStartEndStruct::OPENRIGHT;

	flaststartElement = sse->startElement;
	sse->endElement = NULL;

	setEndElement(grstaff,grstaff->getEndGlue());
	sse->endpos = assocpos;

	if (associated && assocpos)
		associated->GetNext(assocpos);
}

void GRGlissando::setPrevGlissando( GRGlissando * prev)
{
	prevGRGlissando = prev;
}
