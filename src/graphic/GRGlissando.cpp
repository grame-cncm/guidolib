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
//#include "GRChord.h"
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
	laststartElement = NULL;
	laststartpos = NULL;
}

// -----------------------------------------------------------------------------
GRGlissando::GRGlissando(GRStaff * grstaff,
				   ARGlissando * abstractRepresentationOfGlissando)
				   : GRPTagARNotationElement(abstractRepresentationOfGlissando)
{
	assert(abstractRepresentationOfGlissando);
	initGRGlissando( grstaff );
	laststartElement = NULL;
	laststartpos = NULL;
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
	sse->endflag = GRSystemStartEndStruct::RIGHTMOST;

	mStartEndList.AddTail(sse);
		
	GRGlissandoSaveStruct * st = new GRGlissandoSaveStruct;
	st->numPoints = 4;

	sse->p = (void *)st;
	
	return sse;
}

void GRGlissando::print() const
{
}

void GRGlissando::OnDraw( VGDevice & hdc ) const
{
	if (error) return;

	assert( gCurSystem );

//	GRSystem * sys = gCurSystem;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if( sse == 0)
		return; // don't draw

	GRGlissandoSaveStruct * glissInfos = (GRGlissandoSaveStruct *)sse->p;
	assert(glissInfos);

	
	if (mColRef) 
		hdc.PushFillColor( VGColor( mColRef ) );
	
	//straight line
	if(!wavy)
	{
		float coorX[4] = {glissInfos->points[0].x, glissInfos->points[1].x, glissInfos->points[2].x, glissInfos->points[3].x};
		if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		{
			NVRect r = this->getGRStaff()->getBoundingBox();
			r += this->getGRStaff()->getPosition();
			coorX[2] = coorX[3] = r.right - LSPACE;
		}
		else if (sse->startflag == GRSystemStartEndStruct::OPENLEFT )
		{
			coorX[0] = coorX[1] = glissInfos->points[3].x - LSPACE;
		}
			
		float coorY[4] = {glissInfos->points[0].y, glissInfos->points[1].y, glissInfos->points[2].y, glissInfos->points[3].y};
		hdc.Polygon(coorX, coorY, 4);
	}
	//wavy
	else
	{
		float width = glissInfos->points[3].x - glissInfos->points[0].x;
		float height = glissInfos->points[3].y - glissInfos->points[0].y;
		float pasX = width/10;
		float pasY = height/10;

		float X = glissInfos->points[0].x;
		float Y = glissInfos->points[0].y;
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
	}
	if (mColRef) hdc.PopFillColor();
}


void GRGlissando::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)	return;

	const GRNotationElement * const startElement = sse->startElement;
	const GRNotationElement * const endElement = sse->endElement;


	if( grel == endElement || ( endElement == 0 && grel == startElement))
	{
		updateGlissando( staff );
	}
}


void GRGlissando::updateGlissando( GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( inStaff->getGRSystem());
	//GRSystemStartEndStruct * sse = prepareSSEStructForGlissando( inStaff );
	if ( sse == 0 ) return;

	// --- Collects informations about the context ---

	GRGlissandoContext glissContext;
	glissContext.staff = inStaff;
	getGlissandoBeginingContext( &glissContext, sse );
	getGlissandoEndingContext( &glissContext, sse );

	GRGlissandoSaveStruct * glissInfos = (GRGlissandoSaveStruct *)sse->p;

	ARGlissando * arGliss = static_cast<ARGlissando *>(getAbstractRepresentation());
	const float staffLSpace = inStaff->getStaffLSPACE();
	assert(arGliss);

	float dx1 = arGliss->getDx1()->getValue( staffLSpace );
	float dy1 = arGliss->getDy1()->getValue( staffLSpace );
	float dx2 = arGliss->getDx2()->getValue( staffLSpace );
	float dy2 = arGliss->getDy2()->getValue( staffLSpace );

	float XLeft = 0;
	float YLeft = 0;
	float XRight = 0;
	float YRight = 0;

	float dBottomLeftx = 0;
	float dBottomLefty = 0;
	float dBottomRightx = 0;
	float dBottomRighty = 0;

	float acc = 0;

	if(glissContext.bottomLeftHead)
	{
		XLeft = glissContext.bottomLeftHead->getPosition().x + glissContext.leftNoteDX; 
		dBottomLeftx = glissContext.bottomLeftHead->getBoundingBox().Width()*3/4*glissContext.sizeLeft;
		YLeft = glissContext.bottomLeftHead->getPosition().y + glissContext.leftNoteDY;
	}
	if(glissContext.bottomRightHead)
	{
		XRight = glissContext.bottomRightHead->getPosition().x + glissContext.rightNoteDX;
		dBottomRightx = glissContext.bottomRightHead->getBoundingBox().Width()*3/4*glissContext.sizeRight;
		YRight = glissContext.bottomRightHead->getPosition().y + glissContext.rightNoteDY;
		if(glissContext.accidentalRight)
		{
			acc = glissContext.accidentalRight->getBoundingBox().Width()*getSize();
		}
	}
	float deltaX = XRight - XLeft;
	float deltaY = YRight - YLeft;

	dBottomLefty = dBottomLeftx*deltaY/deltaX;
	dBottomRighty = dBottomRightx*deltaY/deltaX;

	float thickness = arGliss->getThickness()->getValue( staffLSpace )*sqrt(deltaX*deltaX + deltaY*deltaY)/deltaX;

	glissInfos->points[0].y = YLeft - dy1 + dBottomLefty + thickness/2;
	glissInfos->points[1].y = YLeft - dy1 + dBottomLefty - thickness/2;
	glissInfos->points[3].y = YRight - dy2 - dBottomRighty + thickness/2;
	glissInfos->points[2].y = YRight - dy2 - dBottomRighty - thickness/2;
	glissInfos->points[0].x = glissInfos->points[1].x = XLeft + dx1 + dBottomLeftx;
	glissInfos->points[3].x = glissInfos->points[2].x = XRight + dx2 - dBottomRightx - acc;
	glissInfos->position = glissInfos->points[0];

	wavy = arGliss->isWavy();
}


void GRGlissando::getGlissandoBeginingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse )
{
		GRNotationElement * startElement = sse->startElement;
		
		GRSingleNote * note = dynamic_cast<GRSingleNote *>(startElement);
		if( note )
		{
			ioContext->bottomLeftHead = note->getNoteHead();
			ioContext->topLeftHead = NULL;
			ioContext->sizeLeft = note->getSize();
			ioContext->leftNoteDX = note->getOffset().x;
			ioContext->leftNoteDY = note->getOffset().y;
		}
	/*	else
		{
			//here, we should deal with clusters...
		}
	*/
}

void GRGlissando::getGlissandoEndingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse )
{	
		GRNotationElement * endElement = sse->endElement;

		GRSingleNote * note = dynamic_cast<GRSingleNote *>(endElement);
		if( note )
		{
			GRAccidentalList noteacclist;
			note->extractAccidentals( &noteacclist );
			if(!noteacclist.empty())
				ioContext->accidentalRight = noteacclist.GetHead();
			ioContext->bottomRightHead = note->getNoteHead();
			ioContext->topRightHead = NULL;
			ioContext->sizeRight = note->getSize();
			ioContext->rightNoteDX = note->getOffset().x;
			ioContext->rightNoteDY = note->getOffset().y;
		}
	/*	else
		{
			//here, we should deal with clusters...
		}
	*/
	
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
//		 dynamic_cast<GRChord *>(grnot) ||
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



void GRGlissando::ResumeTag(GRStaff * grstaff,GuidoPos assocpos)
{
	//assert( gCurSystem );
	if (!grstaff) return;

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	//sse->grsystem = grstaff->getGRSystem();
	//sse->grsystem = gCurSystem;
	sse->startflag = GRSystemStartEndStruct::OPENLEFT;
	sse->startpos = assocpos;
	sse->startElement = laststartElement;

	sse->endflag = lastendflag;
	sse->endElement = lastendElement;
	sse->endpos = lastendpos;
	
	GRSaveStruct * st = getNewGRSaveStruct();
	sse->p = (void *) st;

	mStartEndList.AddTail(sse);

	if (error)
	{
		setStartElement(grstaff,NULL);
		setEndElement(grstaff,NULL);
		return;
	}

	setStartElement(grstaff, laststartElement);
}

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

	laststartElement = sse->startElement;
	laststartpos = sse->startpos;

	setEndElement(grstaff,sse->endElement);
	sse->endpos = assocpos;

	if (associated && assocpos)
		associated->GetNext(assocpos);
}
