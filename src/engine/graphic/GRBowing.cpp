/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003,2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include <cstdlib>
#include "VGDevice.h"

#include "ARBowing.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

#include "GRBowing.h"
#include "GREmpty.h"
#include "GRStaff.h"
#include "GRRest.h"
#include "GRGlue.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"
#include "GRGlobalStem.h"
#include "GRGlobalLocation.h"
#include "GraphTools.h"
// #include "NEPointerList.h"

extern GRSystem * gCurSystem;

using namespace std;

// -----------------------------------------------------------------------------
GRBowing::GRBowing(GRStaff * grstaff, GRNotationElement * startEl,
					GRNotationElement * endEl)
			: GRPTagARNotationElement(new ARBowing, 1)
{
	GRSystemStartEndStruct * sse = initGRBowing( grstaff );

	// The timeposition is set. Because only timeordered
	// Graphicalelements can be added into a staff,
	// the end-time-position is chosen as a default
	// .....

	// a new entry for the SSEList (SystemStartEnd)
	// as defined in GRPositionTag

	// - First, setup the start/end informations, check if we're
	// opened to the left or to the right.

	if (startEl)
	{
		setStartElement(grstaff, startEl);
	}
	else // no start element: we're left-opened
	{
		setStartElement(grstaff, /*dynamic cast<GRNotationElement *>*/(grstaff->getSecondGlue()));
		sse->startflag = GRSystemStartEndStruct::OPENLEFT;
	}

	if (endEl)
	{
		setEndElement(grstaff,endEl);
		sse->endflag = GRSystemStartEndStruct::RIGHTMOST;
	}
	else  // no end element: we're righ-opened
	{
		setEndElement(grstaff, /*dynamic cast<GRNotationElement *>*/(grstaff->getEndGlue()));
		sse->endflag = GRSystemStartEndStruct::OPENRIGHT;
	}

	// - Get the time position.
	GRNotationElement * endElement;
	GRNotationElement * startElement;
	if(( endElement = getEndElement(grstaff)) != 0 )
	{
		mRelativeTimePositionOfGR = endElement->getRelativeTimePosition();
	}
	else if((startElement = getStartElement(grstaff)) != 0 )
	{
		mRelativeTimePositionOfGR = startElement->getRelativeTimePosition();
	}


	mBoundingBox.Set( 0, 0, 0, 0 );

//	updateBow(grstaff);
}

// -----------------------------------------------------------------------------
GRBowing::GRBowing(GRStaff * grstaff)
	: GRPTagARNotationElement( new ARBowing, true ) // ownsAR
{
	initGRBowing( grstaff );
}

// -----------------------------------------------------------------------------
GRBowing::GRBowing(GRStaff * grstaff,
				   ARBowing * abstractRepresentationOfBowing)
				   : GRPTagARNotationElement(abstractRepresentationOfBowing)
{
	assert(abstractRepresentationOfBowing);
	initGRBowing( grstaff );
}

/*
// -----------------------------------------------------------------------------
GRBowing::GRBowing( const GRBowing & inCopy ) : GRPTagARNotationElement( inCopy )
{
	initGRBowing( inCopy.getGRStaff());	// TEST
}*/

// -----------------------------------------------------------------------------
GRSystemStartEndStruct *
GRBowing::initGRBowing( GRStaff * grstaff )
{
	assert(grstaff);

setGRStaff( grstaff );// TEST

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	mStartEndList.AddTail(sse);

	GRBowingSaveStruct * st = new GRBowingSaveStruct;
	st->numoffsets = 3;

	// set Standard-Offsets
	st->offsets[0].x = 0;
	st->offsets[0].y = 0;

	sse->p = (void *)st;

	// mBoundingBox.Set( 0, 0, 0, 0 );
	return sse;
}

// -----------------------------------------------------------------------------
GRBowing::~GRBowing()
{
	assert(mStartEndList.GetCount() == 0);
	FreeAssociatedList();
}

// -----------------------------------------------------------------------------
GRPositionTag::GRSaveStruct * GRBowing::getNewGRSaveStruct()
{
	return new GRBowingSaveStruct;
}

// -----------------------------------------------------------------------------
/**
*/
GRSystemStartEndStruct *
GRBowing::prepareSSEStructForBow( const GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( inStaff->getGRSystem());
	if( sse == 0 ) return 0;

/*	if (!sse)
	{
		assert(false);
		sse = new GRSystemStartEndStruct();
		sse->grsystem = inStaff->getGRSystem();
		sse->startflag = GRSystemStartEndStruct::OPENLEFT;
		sse->endflag = GRSystemStartEndStruct::OPENRIGHT;
		sse->p = (void *) new GRBowingSaveStruct;
		mStartEndList.AddTail(sse);
	}*/

	// I am not sure of this; I am not sure of the position
	// and I am not sure, how to test it ....
	if (sse->endflag == GRSystemStartEndStruct::NOTKNOWN)
	{
		// this is not good ...
		setError(1);
		setStartElement(inStaff,NULL);
		setEndElement(inStaff,NULL);
		return 0 ;
	}
	return sse;
}

// -----------------------------------------------------------------------------
// Figure out if the first element is a part of a chord.
// Find the highest and lowest note heads.
// Get the stem directions (or the possible stem directions)
void
GRBowing::getBowBeginingContext( GRBowingContext * ioContext, GRSystemStartEndStruct * sse )
{
	GRNotationElement * startElement = sse->startElement;

	GRSingleNote * note = dynamic_cast<GRSingleNote *>(startElement);
	if( note )
	{
		ioContext->bottomLeftHead = note->getNoteHead();
		ioContext->topLeftHead = ioContext->bottomLeftHead;	// the same as bottom head.

		ioContext->stemDirLeft = note->getThroatDirection();
	}
	else
	{
		GRGlobalStem * stem = findGlobalStem( sse, startElement );
		if( stem )
		{
			stem->getHighestAndLowestNoteHead( &ioContext->topLeftHead, &ioContext->bottomLeftHead );
			ioContext->stemDirLeft = stem->getStemDir();
		}
		ioContext->leftChordStem = stem;
	}
}

// -----------------------------------------------------------------------------
// (JB) It is similar to the beginning one, but has some additionnal code to
// try to get the right stem direction from a global location. Why not
// for the beginning context ?
void
GRBowing::getBowEndingContext( GRBowingContext * ioContext, GRSystemStartEndStruct * sse )
{
	GRNotationElement * endElement = sse->endElement;

	GRSingleNote * note = dynamic_cast<GRSingleNote *>(endElement);
	if( note )
	{
		ioContext->bottomRightHead = note->getNoteHead();
		ioContext->topRightHead = ioContext->bottomRightHead; // the same as bottom head.

		ioContext->stemDirRight = note->getThroatDirection();
	}
	else
	{
		GRGlobalStem * stem = findGlobalStem( sse, endElement );
		if( stem )
		{
			stem->getHighestAndLowestNoteHead( &ioContext->topRightHead, &ioContext->bottomRightHead );
			ioContext->stemDirRight = stem->getStemDir();
		}
		else		// we have not found one ....
		{
			const NEPointerList * ptlist2 = endElement->getAssociations();
			GuidoPos nepos = ptlist2->GetHeadPosition();
			while (nepos)
			{
				GRGlobalLocation * gloc = dynamic_cast<GRGlobalLocation *>(ptlist2->GetNext(nepos));
				if (gloc)
				{
					ioContext->stemDirRight = (GDirection)gloc->getHighestAndLowestNoteHead(
											&ioContext->topRightHead, &ioContext->bottomRightHead );
					break;
				}
			}
		}
		ioContext->rightChordStem = stem;
	}
}

// -----------------------------------------------------------------------------
GRGlobalStem *
GRBowing::findGlobalStem( GRSystemStartEndStruct * sse, GRNotationElement * stemOwner )
{
	// - It was an optimisation, but is now already tested before the call ->
	//	if ( dynamic cast<GRSingleNote *>(stemOwner) != 0 ) return 0;
	// <-

	const NEPointerList * ptlist1 = stemOwner->getAssociations();
	if (ptlist1)
	{
		GuidoPos nepos = ptlist1->GetHeadPosition();
		while (nepos)
		{
			GRGlobalStem * stem = dynamic_cast<GRGlobalStem *>(ptlist1->GetNext(nepos));
			if (stem)
				return stem;
		}
	}
	return 0;
}

// -----------------------------------------------------------------------------
void
GRBowing::updateBoundingBox()
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( getGRStaff()->getGRSystem());
	if ( sse == 0 ) return;

	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	if( bowInfos == 0 ) return;

	// - Update bounding box
	mBoundingBox.left = bowInfos->offsets[0].x + bowInfos->position.x;		// middle point might be smaller.
	mBoundingBox.right = bowInfos->offsets[2].x + bowInfos->position.x;	    // middle point might be larger.

	const float y0 = bowInfos->offsets[0].y + bowInfos->position.y;
	const float y1 = bowInfos->offsets[1].y + bowInfos->position.y;
	const float y2 = bowInfos->offsets[2].y + bowInfos->position.y;

	if( y1 < y0 )	// upward
	{
		mBoundingBox.top = y1;
		mBoundingBox.bottom = y2 > y0 ? y2 : y0;
	}
	else
	{
		mBoundingBox.top = y2 < y0 ? y2 : y0;
		mBoundingBox.bottom = y1;
	}
}

// -----------------------------------------------------------------------------
/** \brief Calculates the placement of the bow: direction, anchor and control points.

	Changes the parameter-names and just handle positions correctly.

	calculation the position of y for dy3:
	(eposy-sposy)*dx3 + sposy + dy3 * hspace;
	-> (endElement->getTieEnd().y -
	-> startElement->getTieEnd().y ) * dx3 +  ...

*/
void GRBowing::updateBow( GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = prepareSSEStructForBow( inStaff );
	if ( sse == 0 ) return;

	// --- Collects informations about the context ---

	GRBowingContext bowContext;
	bowContext.staff = inStaff;
	getBowBeginingContext( &bowContext, sse );
	getBowEndingContext( &bowContext, sse );

	// --- Handles the cases where the bow is opened to the left or to the right ---
	// find out, if the elements contain shareStem associations!?
	// this shows, wether any parameter for the bow was set or not.

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT || !startElement)
	{
		bowContext.openLeft = true;
		if (!startElement && inStaff)
		{
			setStartElement( inStaff, /*dynamic cast<GRNotationElement *>*/(inStaff->getSecondGlue()));

			sse->startflag = GRSystemStartEndStruct::OPENLEFT;

			sse = getSystemStartEndStruct( inStaff->getGRSystem()); //(JB) useless ?
			bowInfos = (GRBowingSaveStruct *) sse->p; // useless ? already set to sse->p
			startElement = sse->startElement;
		}

		if (!startElement)
		{
			assert(false);
			bowInfos->position.x = 0;
			bowInfos->position.y = (float(6.5) * LSPACE);
		}
	}

	if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT || !endElement)
	{
		bowContext.openRight = true;
		if (!endElement && inStaff)
		{
			setEndElement(inStaff, /*dynamic cast<GRNotationElement *>*/(inStaff->getEndGlue()));

			sse->endflag = GRSystemStartEndStruct::OPENRIGHT;

			sse = getSystemStartEndStruct( inStaff->getGRSystem());	// (JB) useless ?
			bowInfos = (GRBowingSaveStruct *) sse->p; // useless ?
			endElement = sse->endElement;
		}

		if (!endElement)
		{
			assert(false);
			bowInfos->offsets[bowInfos->numoffsets-1].x = LSPACE;
		}

		bowInfos->offsets[bowInfos->numoffsets-1].y = 0;
	}

	// --- Chooses the best curve direction ---

	ARBowing * arBow = /*dynamic*/static_cast<ARBowing *>(getAbstractRepresentation());
	assert(arBow);

	// - Here, we look if the curve direction has been explicitly
	// (with the tag "curve") or implicitly (with the tag "h") specified.
	const TagParameterFloat * paramH = arBow->getH();
	const TagParameterString * curveParam = arBow->getCurve();
	if (curveParam && curveParam->TagIsSet())
	{
		if( string(curveParam->getValue()) == "down" )
			bowContext.curveDir = -1;
		else
			bowContext.curveDir = 1;
	}
	else if( paramH && paramH->TagIsSet())
	{
		bowContext.curveDir = (paramH->getValue() > 0) ? 1 : -1;
	}
	else
		automaticCurveDirection( &bowContext, arBow, sse );

	// --- Calculate the start and end anchor points positions --
	// NOTE: in the futur, offsets could also be applied after automatic
	// positionning. (tag parameter list for 'curve' would be completed)

	if( (curveParam && curveParam->TagIsSet()) || arBow->getParSet() == false )
	{
		automaticAnchorPoints( &bowContext, arBow, sse );
	}
	else
	{
		manualAnchorPoints( &bowContext, arBow, sse );
		applyAnchorPointsOffsets( &bowContext, arBow, sse );
	}

	// WARNING: THE PRECEDING CODE MODIFIED THE VALUE OF paramH WHILE
	// PROCESSING ANCHOR CALCULATION: THEN PTR BECAME UNVALID (DELETED
	// AND REBUILT) ! SO WE HAD TO RE-CHECK ITS VALUE BEFORE REUSING IT !
	// see ARBowing::setCurve(). Code modified to avoid the pb. YC-oct2007

	// --- Calculate the control Points
	const TagParameterFloat * paramR3 = arBow->getR3();
	if(( paramH && paramH->TagIsSet()) || (paramR3 && paramR3->TagIsSet()))
	{
		// A tag for control points has been specified in the guido script.
		manualControlPoints( &bowContext, arBow, sse );
	}
	else
	{
		// Automatic placement of control points, it will try to
		// avoid collisions.
		automaticControlPoints( &bowContext, arBow, sse );
	}

	updateBoundingBox();
}

// -----------------------------------------------------------------------------
void
GRBowing::automaticAnchorPoints( GRBowingContext * bowContext, ARBowing * arBow,
														GRSystemStartEndStruct * sse )
{
	manualAnchorPoints( bowContext, arBow, sse );
}

// -----------------------------------------------------------------------------
/** \brief Manual anchor points positionning, when dx and dy tags are specified.

	This calculates the reference points on which dx and dy offset will be applied.

	Following the guido specification of tie ans slur tags, those reference
	points are the center of the noteheads.
*/
void
GRBowing::manualAnchorPoints( GRBowingContext * bowContext, ARBowing * arBow,
														GRSystemStartEndStruct * sse )
{
	// Careful, we have to deal with chords! what about getStemStartPosition()?
	NVPoint posLeft;
	NVPoint posRight;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

	GRStdNoteHead * head;

	const bool upward = bowContext->curveDir == 1;

	// - Find left position
	if (bowContext->bottomLeftHead && bowContext->topLeftHead) // && !noteleft // useless additional test ? don't exists for rightPos !
	{
		if( upward )
			head = bowContext->topLeftHead;
		else
			head = bowContext->bottomLeftHead;		// down ... we take the lowest notehead

		posLeft = head->getNoteHeadPosition();
	}
	else
		posLeft = startElement->getPosition();

	// - Find right position
	if (bowContext->bottomRightHead && bowContext->topRightHead)
	{
		if( upward )
			head = bowContext->topRightHead;
		else
			head = bowContext->bottomRightHead;		// down ... we take the lowest notehead

		posRight = head->getNoteHeadPosition();
	}
	else
		posRight = endElement->getPosition();

	if (bowContext->openLeft)	posLeft.y = posRight.y;
	if (bowContext->openRight)	posRight.y = posLeft.y;

	bowInfos->position = posLeft;
	bowInfos->offsets[2] = posRight - posLeft; // control points are stored as offsets to the position.

	//	if( manualSettings == false )
	arBow->setCurve( bowContext->curveDir, posLeft, posRight ); // (JB) useless ?

}

// -----------------------------------------------------------------------------
void
GRBowing::applyAnchorPointsOffsets( GRBowingContext * bowContext, ARBowing * arBow,
														GRSystemStartEndStruct * sse )
{
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	GRStaff * staff = bowContext->staff;

	// -- Applies the offset settings to the start anchor point --
	const float staffLSpace = staff->getStaffLSPACE();
	if( arBow->getDX1())
		bowInfos->offsets[0].x += (arBow->getDX1()->getValue( staffLSpace ));

	if( arBow->getDY1())
		bowInfos->offsets[0].y -= (arBow->getDY1()->getValue( staffLSpace ));

	// -- Applies the offset settings to the end anchor point --
	if( arBow->getDX2())
		bowInfos->offsets[2].x += (arBow->getDX2()->getValue( staffLSpace ));

	if( arBow->getDY2())
		bowInfos->offsets[2].y -= (arBow->getDY2()->getValue( staffLSpace ));
}

// -----------------------------------------------------------------------------
/** \brief Use tag parameters to calculate the control points position.
*/
void
GRBowing::manualControlPoints( GRBowingContext * bowContext, ARBowing * arBow,
														GRSystemStartEndStruct * sse )
{
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	GRStaff * staff = bowContext->staff;

	const float distx = bowInfos->offsets[2].x - bowInfos->offsets[0].x;
	const float disty = bowInfos->offsets[2].y - bowInfos->offsets[0].y;
	const int dir = bowContext->curveDir;

	// -- Get the initial middle control point position.
	// If the R3 factor is near zero, the control point will be near the first anchor point
	// if the R3 factor is near 1, the control point will be near the second anchor point.

	const TagParameterFloat * tagR3 =  arBow->getR3();
	const TYPE_FLOATPARAMETER valueR3 = tagR3 ? tagR3->getValue() : float(0.5);

	bowInfos->offsets[1].x = (GCoord)(distx * valueR3 );
	bowInfos->offsets[1].y = (GCoord)(disty * valueR3 * dir);

	// -- Apply the middle control point y-offset.
	const float staffLSpace = staff->getStaffLSPACE();
	const TagParameterFloat * tagH = arBow->getH();
	const TYPE_FLOATPARAMETER valueH = tagH ? tagH->getValue( staffLSpace ) : float(2);
	bowInfos->offsets[1].y += -dir * ((valueH > 0) ? valueH : -valueH);

	// - Store
	bowInfos->offsets[1] += bowInfos->offsets[0];
}

// -----------------------------------------------------------------------------
/** \brief Defines the bowing direction, depending on specified tags or music notation rules.

	This method only implement the basic rule: based on stem directions. Subclasses
	can override to add their own rules;
*/
void
GRBowing::automaticCurveDirection( GRBowingContext * bowContext, ARBowing * arBow,
														GRSystemStartEndStruct * sse )
{
	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;

	// - Determines the direction of the first stem.
	int firstDir = bowContext->stemDirLeft;
	if( firstDir == 0 )
	{
		GRNote * firstNote = dynamic_cast<GRNote *>(startElement);
		if( firstNote )
			firstDir = firstNote->getThroatDirection();

		bowContext->stemDirLeft = firstDir;
	}

	// - Determines the direction of the last stem.
	int lastDir = bowContext->stemDirRight;
	if( lastDir == 0 )
	{
		GRNote * lastNote = dynamic_cast<GRNote *>(endElement);
		if( lastNote )
			lastDir = lastNote->getThroatDirection();

		bowContext->stemDirRight = lastDir;
	}

	// - If a stem direction is unknown, copy the other stem's direction.
	if( firstDir == 0 ) firstDir = lastDir;
	if( lastDir == 0 ) lastDir = firstDir;

	// - Here, we only look at first and last note to determine if stems are
	// mixed up/down. This is not perfect, we should look at all notes covered by the bow.
	const bool mixed = ( firstDir == -lastDir ); // also true if both are equal to 0

	// - For mixed stem directions, choose upward (ties and slurs)
	if( mixed )
		bowContext->curveDir = 1;	// upward.
	else
		bowContext->curveDir = - firstDir;
}

// -----------------------------------------------------------------------------
/** \brief Tries to avoid collisions.

	Called after anchor points positionning and after curve direction has been decided.
*/
void
GRBowing::automaticControlPoints( GRBowingContext * bowContext, ARBowing * arBow,
														GRSystemStartEndStruct * sse )
{
	manualControlPoints( bowContext, arBow, sse );
}

// -----------------------------------------------------------------------------
void GRBowing::setOffset(int n_point,const NVPoint & p)
{
	assert(false);
	// here an array must be used (need lists and arrays)
}

// -----------------------------------------------------------------------------
void GRBowing::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)	return;

	const GRNotationElement * const startElement = sse->startElement;
	const GRNotationElement * const endElement = sse->endElement;

	// if ( openLeftRange && openRightRange ) return;
		// updateBow();

	if( grel == endElement || ( endElement == 0 && grel == startElement))
	{
		updateBow( staff );
	}
}

// -----------------------------------------------------------------------------
/** \brief Called by the notes inbetween the range
*/
void GRBowing::addAssociation(GRNotationElement * grnot)
{
	if (error) return;

	// here, we need to test, whether Staff-Numbers
	// have changed ... than this tag goes inbetween
	// staves on the same system.

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

	// maybe we do somethings else here? ->
	// like setting flags ...
}

// -----------------------------------------------------------------------------
void GRBowing::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}

// -----------------------------------------------------------------------------
GRNotationElement * GRBowing::getStartElement(GRStaff * grstaff) const
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
GRNotationElement * GRBowing::getEndElement(GRStaff * grstaff) const
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

// -----------------------------------------------------------------------------
void GRBowing::OnDraw( VGDevice & hdc) const
{
// DrawBoundingBox( hdc, GColor( 255, 120, 150, 120 )); // DEBUG
	if(!mDraw)
		return;

	if (error) return;
	
	assert( gCurSystem );

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if( sse == 0)
		return; // don't draw

	// now we need to get the position and offset for the current bowing ...

	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	assert(bowInfos);

/*
	NVPoint pstart, pmid, pend, poffsetUp, poffsetDown;
	pstart.x=bowInfos->position.x+bowInfos->offsets[0].x;
	pstart.y=bowInfos->position.y+bowInfos->offsets[0].y;
	pmid.x=bowInfos->position.x+bowInfos->offsets[1].x;
	pmid.y=bowInfos->position.y+bowInfos->offsets[1].y;
	pend.x=bowInfos->position.x+bowInfos->offsets[2].x;
	pend.y=bowInfos->position.y+bowInfos->offsets[2].y;
*/
//	GColor backColor = hdc.GetTextBackgroundColor();
//	hdc.SetTextBackgroundColor( 255,0,0 ); // why red?

	// now we do it with correct colors:
//	VGColor color ( mColRef ); 	// custom or black
	if (mColRef) hdc.PushFillColor( VGColor( mColRef ) );
//	hdc.PushPen( color, 1 );

	const float x = bowInfos->position.x;
	const float y = bowInfos->position.y;
// keith hamel
	::drawSlur( hdc,
				x + bowInfos->offsets[0].x,
				y + bowInfos->offsets[0].y,
				x + bowInfos->offsets[1].x,
				y + bowInfos->offsets[1].y,
				x + bowInfos->offsets[2].x,
				y + bowInfos->offsets[2].y,
				bowInfos->inflexion );

	// no floodfill until point-issue is eliminated
	///ExtFloodFill(hdc,pmid.x,pmid.y,fg,FLOODFILLSURFACE);
	// FloodFill(hdc,pmid.x,pmid.y,fg);

	// restore old pen and brush
//	hdc.PopPen();
	if (mColRef) hdc.PopFillColor();

	// overdraw borderlines with black color
	/* drawslur(hdc, pstart,poffsetUp,pend);
	drawslur(hdc, pstart,poffsetDown,pend); */

	// restore old backgound color
	// hdc.SetTextBackgroundColor( backColor );
}

// -----------------------------------------------------------------------------
/* SLUR THICKNESS IS THE RELATIVE THICKNESS OF THE SLUR 1 is very thin, 10 is
	very thick, 3 or 4 is a normal thickness.

   NSEGS sets the number of line segments used to draw the curves -
   should be a variable so it can be increased during printing
   5 can be used for quick drawing and 100 or more for higher
   resolution
*/

const float SLUR_THICKNESS = (LSPACE/5); //(JB) was 8
const int NSEGS = 25;

// -----------------------------------------------------------------------------
/** \brief Draws a slur.

	(JB) experimental modifications to original code

	It takes 3 control points x1,y1,x2,y2,x3,y3 and draws a curv from x1, y1
	through x2,y2 (approximately) to x3, y3.  The thickness of the slur is
	set by SLUR_THICKNESS (1 - 10)
*/
void drawSlur(VGDevice & hdc, float x1, float y1, float x2, float y2,
								float x3, float y3, float inflexion )
{
	float delx1, delx2, ratio;
	float addY2, addX, addY, x2a, y2a, x2b, y2b;
	float maxD;//, h1, h2;

	// if start and endpoint is the same, just don't do anything.
	// this is a hack, whatsoever, because this should not really happen!
	if (x1==x3) return;

	const float oneOverDeltaX = (1.0f / (x3 - x1));

	maxD = (SLUR_THICKNESS * 1.25f);
	ratio = (y3 - y1) * oneOverDeltaX;
	addY2 = (y2 - y1) / 3;
	addX = (x3 - x1) / (2 + inflexion);	// defines the attack of the curve.
	addY = addX * ratio;
	x2a = x2 - addX;
	y2a = y2 - addY + addY2;
	x2b = x2 + addX;
	y2b = y2 - addY + addY2;
//	h1 = (y2a > y1) ? (y2a - y1) : (y1 - y2a);
//	h2 = (y2b > y3) ? (y2b - y3) : (y3 - y2b);
//	if (x2a - x1 > h1) x2a = x1 + h1;
//	if (x3 - x2b > h2) x2b = x3 - h2;
	delx1 = ((y3 - y1) * SLUR_THICKNESS) * oneOverDeltaX;
	delx2 = ((y3 - y1) * SLUR_THICKNESS) * oneOverDeltaX;
	if (delx1 > maxD) delx1 = maxD;
	if (delx2 > maxD) delx2 = maxD;
	if (delx1 < -maxD) delx1 = -maxD;
	if (delx2 < -maxD) delx2 = -maxD;

	const int ptCount = (2 * ( NSEGS + 3 ));

	NVPoint thePoints[ ptCount ]; //ptCount ];

	// CALCULATE THE FIRST CURVE
	// PROBABLY YOU WANT TO START A POLYGON NOW
	int index = 0;
	makeCurve( x1, y1, x2a, y2a, x2b, y2b, x3, y3, NSEGS, thePoints, &index );

	y2a = (y2a < y1 ? y2a + SLUR_THICKNESS : y2a - SLUR_THICKNESS);
	y2b = (y2b < y3 ? y2b + SLUR_THICKNESS : y2b - SLUR_THICKNESS);
	x2a += delx1;
	x2b += delx2;

	// CALCULATE THE SECOND CURVE
	makeCurve( x3, y3, x2b, y2b, x2a, y2a, x1, y1, NSEGS, thePoints, &index );

	//PROBABLY YOU WANT TO CLOSE THE POLYGON NOW AND FILL IT
	float xPoints [ ptCount ];
	float yPoints [ ptCount ];
	for( int currPt = 0; currPt < index; ++ currPt )
	{
		xPoints [ currPt ] = thePoints[ currPt ].x;
		yPoints [ currPt ] = thePoints[ currPt ].y;
	}

	hdc.Polygon( xPoints, yPoints, index );

	/* - DEBUG ->
	hdc.PushPen( GColor( 200, 0, 0 ), 5 );

	hdc.Line( x1 - 20, y1 - 20, x1 + 20, y1 + 20);
	hdc.Line( x1 - 20, y1 + 20, x1 + 20, y1 - 20);

	hdc.PushPen( GColor( 200, 0, 200 ), 5 );
	hdc.Line( x2 - 20, y2 - 20, x2 + 20, y2 + 20);
	hdc.Line( x2 - 20, y2 + 20, x2 + 20, y2 - 20);
	hdc.PopPen();

	hdc.Line( x2a - 20, y2a - 20, x2a + 20, y2a + 20);
	hdc.Line( x2a - 20, y2a + 20, x2a + 20, y2a - 20);

	hdc.Line( x2b - 20, y2b - 20, x2b + 20, y2b + 20);
	hdc.Line( x2b - 20, y2b + 20, x2b + 20, y2b - 20);

	hdc.Line( x3 - 20, y3 - 20, x3 + 20, y3 + 20);
	hdc.Line( x3 - 20, y3 + 20, x3 + 20, y3 - 20);


	hdc.PopPen();
	// <- */
}

/* Original curve code ->
void drawSlur(VGDevice & hdc, float x1, float y1, float x2, float y2, float x3, float y3)
{
	float delx1, delx2, ratio;
	float addY2, addX, addY, x2a, y2a, x2b, y2b;
	float maxD, h1, h2;

	// if start and endpoint is the same, just don't do anything.
	// this is a hack, whatsoever, because this should not really happen!
	if (x1==x3) return;

	const float oneOverDeltaX = (1.0f / (x3 - x1));

	maxD = (SLUR_THICKNESS * 1.25f);
	ratio = (y3 - y1) * oneOverDeltaX;
	addY2 = (y2 - y1) / 3;
	addX = (x3 - x1) / 5;	// defines the attack of the curve.
	addY = addX * ratio;
	x2a = x2 - addX;
	y2a = y2 - addY + addY2;
	x2b = x2 + addX;
	y2b = y2 - addY + addY2;
	h1 = abs(y2a - y1);
	h2 = abs(y2b - y3);
	if (x2a - x1 > h1) x2a = x1 + h1;
	if (x3 - x2b > h2) x2b = x3 - h2;
	delx1 = ((y3 - y1) * SLUR_THICKNESS) * oneOverDeltaX;
	delx2 = ((y3 - y1) * SLUR_THICKNESS) * oneOverDeltaX;
	if (delx1 > maxD) delx1 = maxD;
	if (delx2 > maxD) delx2 = maxD;
	if (delx1 < -maxD) delx1 = -maxD;
	if (delx2 < -maxD) delx2 = -maxD;

	const int ptCount = (2 * ( NSEGS + 3 ));

	NVPoint thePoints[ ptCount ]; //ptCount ];

	// DRAW THE FIRST CURVE
	// PROBABLY YOU WANT TO START A POLYGON NOW
	int index = 0;
	makeCurve( hdc, x1, y1, x2a, y2a, x2b, y2b, x3, y3, NSEGS, thePoints, &index );

	y2a = (y2a < y1 ? y2a + SLUR_THICKNESS : y2a - SLUR_THICKNESS);
	y2b = (y2b < y3 ? y2b + SLUR_THICKNESS : y2b - SLUR_THICKNESS);
	x2a += delx1;
	x2b += delx2;

	//CALCULATE THE SECOND CURVE
	makeCurve( hdc, x3, y3, x2b, y2b, x2a, y2a, x1, y1, NSEGS, thePoints, &index );

	//PROBABLY YOU WANT TO CLOSE THE POLYGON NOW AND FILL IT
	float xPoints [ ptCount ];
	float yPoints [ ptCount ];
	for( int currPt = 0; currPt < index; currPt ++ )
	{
		xPoints [ currPt ] = thePoints[ currPt ].x;
		yPoints [ currPt ] = thePoints[ currPt ].y;
	}

	hdc.Polygon( xPoints, yPoints, index );
}
*/
// -----------------------------------------------------------------------------
/** \brief Rudis Slur-routine (currently unused)
*/
void GRBowing::drawSlur( VGDevice & hdc, NVPoint pstart, NVPoint pmid, NVPoint pend) const
{
	NVRect rectangle ; // rectangle in which the arc is to be drawn.

	rectangle.left = pstart.x;

	if (pstart.y<pmid.y)	rectangle.top=pmid.y;
	else					rectangle.top= (pstart.y-pmid.y)*2 + pmid.y;

	rectangle.right =  (pmid.x-pstart.x)*2 + pstart.x;

	if(pstart.y<pmid.y) rectangle.bottom= pmid.y - (pmid.y-pstart.y)*2;
	else				rectangle.bottom= pmid.y;

	hdc.Arc( rectangle.left, rectangle.top, rectangle.right, rectangle.bottom,
				pstart.x,pstart.y,pmid.x,pmid.y);

	//second arc
	/// pmid.y-=10;

	rectangle.left =  pmid.x - (pend.x - pmid.x);
	if (pmid.y < pend.y)
		rectangle.top =  (pend.y-pmid.y)*2 - pmid.y;
	else
		rectangle.top= pmid.y;

	rectangle.right=pend.x;

	if(pmid.y < pend.y)	rectangle.bottom= pmid.y;
	else				rectangle.bottom= pmid.y - (pmid.y-pend.y)*2;

	hdc.Arc( rectangle.left, rectangle.top, rectangle.right, rectangle.bottom,
				pmid.x,pmid.y,pend.x,pend.y);

	// - DEBUG ->
/*	hdc.PushPen( GColor( 0, 0, 200 ), 5 );

	hdc.Line( pstart.x - 20, pstart.y - 20, pstart.x + 20, pstart.y + 20);
	hdc.Line( pstart.x - 10, pstart.y + 20, pstart.x + 20, pstart.y - 20);

	hdc.Line( pmid.x - 20, pmid.y - 20, pmid.x + 20, pmid.y + 20);
	hdc.Line( pmid.x - 10, pmid.y + 20, pmid.x + 20, pmid.y - 20);

	hdc.Line( pend.x - 20, pend.y - 20, pend.x + 20, pend.y + 20);
	hdc.Line( pend.x - 20, pend.y + 20, pend.x + 20, pend.y - 20);

	hdc.PopPen();*/
	// <-
}


