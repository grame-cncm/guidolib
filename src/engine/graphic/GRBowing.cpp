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
#include <cmath>

#include "VGDevice.h"

#include "ARBowing.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

#include "GRGlue.h"
#include "GRBowing.h"
#include "GRStaff.h"
#include "GRRest.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"
#include "GRGlobalStem.h"
#include "GRGlobalLocation.h"
#include "GraphTools.h"

extern GRSystem * gCurSystem;

#define FIX_SLOPE

using namespace std;


// -----------------------------------------------------------------------------
GRBowing::GRBowing(GRStaff * grstaff, GRNotationElement * startEl, GRNotationElement * endEl)
			: GRPTagARNotationElement(new ARBowing, 1)
{
	GRSystemStartEndStruct * sse = initGRBowing( grstaff );

	// The timeposition is set. Because only timeordered
	// Graphical elements can be added into a staff, the end-time-position is chosen as a default

	// a new entry for the SSEList (SystemStartEnd) as defined in GRPositionTag

	// - First, setup the start/end informations, check if we're opened to the left or to the right.
	if (startEl)
		setStartElement(grstaff, startEl);
	else // no start element: we're left-opened
	{
		setStartElement(grstaff, grstaff->getSecondGlue());
		sse->startflag = GRSystemStartEndStruct::OPENLEFT;
	}

	if (endEl) {
		setEndElement(grstaff,endEl);
		sse->endflag = GRSystemStartEndStruct::RIGHTMOST;
	}
	else  // no end element: we're righ-opened
	{
		setEndElement(grstaff, grstaff->getEndGlue());
		sse->endflag = GRSystemStartEndStruct::OPENRIGHT;
	}

	// - Get the time position.
	GRNotationElement * endElement = getEndElement(grstaff);
	GRNotationElement * startElement = getStartElement(grstaff);
	if ( endElement )
		setRelativeTimePosition (endElement->getRelativeTimePosition());
	else if ( startElement )
		setRelativeTimePosition (startElement->getRelativeTimePosition());
	mBoundingBox.Set( 0, 0, 0, 0 );
}

// -----------------------------------------------------------------------------
GRBowing::GRBowing(GRStaff * grstaff)
	: GRPTagARNotationElement( new ARBowing, true ) // ownsAR
{
	initGRBowing( grstaff );
}

// -----------------------------------------------------------------------------
GRBowing::GRBowing(GRStaff * grstaff, const ARBowing * ar)
				   : GRPTagARNotationElement(ar)
{
	assert(ar);
	initGRBowing( grstaff );
}

// -----------------------------------------------------------------------------
GRSystemStartEndStruct * GRBowing::initGRBowing( GRStaff * grstaff )
{
	assert(grstaff);

	setGRStaff( grstaff );// TEST

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	addSystemStartEndStruct (sse);

	GRBowingSaveStruct * st = new GRBowingSaveStruct;
	st->numoffsets = 3;

	// set Standard-Offsets
	st->offsets[0].x = 0;
	st->offsets[0].y = 0;

	sse->p = (void *)st;
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
GRSystemStartEndStruct * GRBowing::prepareSSEStructForBow( const GRStaff * inStaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( inStaff->getGRSystem());
	if( sse == 0 ) return 0;

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
void GRBowing::getBowBeginingContext( GRBowingContext * ioContext, GRSystemStartEndStruct * sse ) const
{
	const GRNotationElement * startElement = sse->startElement;
	const GRSingleNote * note = startElement->isSingleNote();
	if( note ) {
		ioContext->bottomLeftHead = note->getNoteHead();
		ioContext->topLeftHead = ioContext->bottomLeftHead;	// the same as bottom head.
		ioContext->stemDirLeft = note->getThroatDirection();
	}
	else {
		GRGlobalStem * stem = findGlobalStem( startElement );
		if( stem ) {
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
void GRBowing::getBowEndingContext( GRBowingContext * ioContext, GRSystemStartEndStruct * sse ) const
{
	const GRNotationElement * endElement = sse->endElement;
	const GRSingleNote * note = endElement->isSingleNote();
	if( note ) {
		ioContext->bottomRightHead = note->getNoteHead();
		ioContext->topRightHead = ioContext->bottomRightHead; // the same as bottom head.
		ioContext->stemDirRight = note->getThroatDirection();
	}
	else {
		GRGlobalStem * stem = findGlobalStem( endElement );
		if( stem ) {
			stem->getHighestAndLowestNoteHead( &ioContext->topRightHead, &ioContext->bottomRightHead );
			ioContext->stemDirRight = stem->getStemDir();
		}
		else		// we have not found one ....
		{
			const NEPointerList * ptlist2 = endElement->getAssociations();
			GuidoPos pos = ptlist2->GetHeadPosition();
			while (pos) {
				GRGlobalLocation * gloc = dynamic_cast<GRGlobalLocation *>(ptlist2->GetNext(pos));
				if (gloc) {
					ioContext->stemDirRight = (GDirection)gloc->getHighestAndLowestNoteHead(&ioContext->topRightHead, &ioContext->bottomRightHead );
					stem = findGlobalStem( gloc->getFirstEl() );
					break;
				}
			}
		}
		ioContext->rightChordStem = stem;
	}
}

// -----------------------------------------------------------------------------
GRGlobalStem * GRBowing::findGlobalStem( const GRNotationElement * stemOwner ) const
{
	const NEPointerList * ptlist1 = stemOwner->getAssociations();
	if (ptlist1) {
		GuidoPos pos = ptlist1->GetHeadPosition();
		while (pos) {
			GRGlobalStem * stem = dynamic_cast<GRGlobalStem *>(ptlist1->GetNext(pos));
			if (stem) return stem;
		}
	}
	return 0;
}

// -----------------------------------------------------------------------------
void GRBowing::updateBoundingBox()
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
void GRBowing::updateBow( GRStaff * inStaff, bool grace )
{
	GRSystemStartEndStruct * sse = prepareSSEStructForBow( inStaff );
	if ( sse == 0 ) return;


	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

	// --- Collects informations about the context ---
	GRBowingContext* context = &bowInfos->context;
	context->staff = inStaff;
	getBowBeginingContext( context, sse );
	getBowEndingContext( context, sse );

	// --- Handles the cases where the bow is opened to the left or to the right ---
	// find out, if the elements contain shareStem associations!?
	// this shows, wether any parameter for the bow was set or not.

	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT || !startElement)
	{
		context->openLeft = true;
		if (!startElement && inStaff) {
			setStartElement( inStaff, inStaff->getSecondGlue());
			sse->startflag = GRSystemStartEndStruct::OPENLEFT;
			startElement = sse->startElement;
		}
	}

	if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT || !endElement)
	{
		context->openRight = true;
		if (!endElement && inStaff) {
			setEndElement(inStaff, inStaff->getEndGlue());
			sse->endflag = GRSystemStartEndStruct::OPENRIGHT;
			endElement = sse->endElement;
		}
		bowInfos->offsets[bowInfos->numoffsets-1].y = 0;
	}

	// --- Chooses the best curve direction ---
	const ARBowing * arBow = static_cast<const ARBowing *>(getAbstractRepresentation());
	assert(arBow);

	// - Here, we look if the curve direction has been explicitly
	// (with the tag "curve") or implicitly (with the tag "h") specified.
	float paramH = arBow->getH();
	ARBowing::CurveDirection dir = arBow->getCurve();
	if (dir != ARBowing::kUndefined) {
		context->curveDir = (dir == ARBowing::kDown) ? -1 : 1;
	}
	else if( paramH != ARBowing::undefined()) {
		context->curveDir = (paramH > 0) ? 1 : -1;
	}
	else automaticCurveDirection( context, arBow, sse );

	// --- Calculate the start and end anchor points positions --
	// NOTE: in the futur, offsets could also be applied after automatic
	// positionning. (tag parameter list for 'curve' would be completed)
	if (grace)
		graceAnchorPoints( context, arBow, sse, inStaff );
	else if( (dir != ARBowing::kUndefined) || !arBow->getParSet() )
		automaticAnchorPoints( context, arBow, sse );
	else
		manualAnchorPoints( context, arBow, sse );
	applyAnchorPointsOffsets( context, arBow, sse );

	// WARNING: THE PRECEDING CODE MODIFIED THE VALUE OF paramH WHILE
	// PROCESSING ANCHOR CALCULATION: THEN PTR BECAME UNVALID (DELETED
	// AND REBUILT) ! SO WE HAD TO RE-CHECK ITS VALUE BEFORE REUSING IT !
	// see ARBowing::setCurve(). Code modified to avoid the pb. YC-oct2007

	// --- Calculate the control Points
	float paramR3 = arBow->getR3();
	if ( (paramH != ARBowing::undefined()) || (paramR3 != ARBowing::undefined()))
	{
		// A tag for control points has been specified in the guido script.
		manualControlPoints( context, arBow, sse );
	}
	else { // Automatic placement of control points, it will try to avoid collisions.
#ifdef NONE //FIX_SLOPE
		NVRect rect = getAssociatedBoundingBox();
		float limit = context.staff->getStaffLSPACE() * 6;
		int count = 0;
		float prevypos = bowInfos->position.y;
		float prevgap = 10000;

//cerr << "GRBowing::updateBow position 1: " << bowInfos->position << endl;
		do {
			automaticControlPoints( &context, arBow, sse );
			updateBoundingBox();
			const bool upward = (context.curveDir == 1);
			float gap = upward ? rect.top - mBoundingBox.top : mBoundingBox.bottom - rect.bottom;
			if (gap > limit) {
				if (gap > prevgap) {				// situation is getting worse
					bowInfos->position.y -= prevypos;	// revert to previous pos
					automaticControlPoints( &context, arBow, sse );
					updateBoundingBox();
					break;
				}
				prevgap = gap;
				float yAdjust  = context.staff->getStaffLSPACE() / (upward ? 2.f : -2.f);
				bowInfos->position.y -= yAdjust;
				count++;
//cerr << "GRBowing::updateBow position 2: " << bowInfos->position << endl;
			}
			else break;
		} while (count < 3);
		float width = bowInfos->offsets[2].x - bowInfos->offsets[0].x;
		if (context.openLeft && (width < 110)) {
			bowInfos->offsets[0].x -= 110 - width;
			bowInfos->offsets[1].y = bowInfos->offsets[2].y - ((context.curveDir > 0) ? LSPACE : -LSPACE);
			bowInfos->offsets[0].y = bowInfos->offsets[1].y;
		}
//cerr << "GRBowing::updateBow position 3: " << bowInfos->position << endl;
		return;
#else
		automaticControlPoints( context, arBow, sse );
		float corr = LSPACE ;
		float hcorr = LSPACE / 2;
		if (context->openLeft) {
			bowInfos->offsets[0].x -= corr;
			bowInfos->offsets[0].y = bowInfos->offsets[1].y - ((context->curveDir > 0) ? hcorr : -hcorr);
		}
		if (context->openRight) {
			bowInfos->offsets[2].x += corr;
		}
#endif
	}
	updateBoundingBox();
}

// -----------------------------------------------------------------------------
void GRBowing::automaticAnchorPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	manualAnchorPoints( context, arBow, sse );
}

// -----------------------------------------------------------------------------
/** \brief Anchor points positionning whith grace notes
*/
void GRBowing::graceAnchorPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse, GRStaff * staff )
{
	NVPoint posLeft;
	NVPoint posRight;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	float lspace = staff->getStaffLSPACE();

	const bool upward = context->curveDir == 1;
	GRStdNoteHead * lefthead = upward ? context->topLeftHead : context->bottomLeftHead;
	posLeft = lefthead ? lefthead->getNoteHeadPosition() : startElement->getPosition();

	GRStdNoteHead * righthead = upward ? context->topRightHead : context->bottomRightHead;
	posRight = righthead ? righthead->getNoteHeadPosition() : endElement->getPosition();
	float graceoffset = lspace/2 * (upward ? -1 : 1);
	float noteoffset  = lspace * 0.8f * (upward ? -1 : 1);
	posLeft.y  += graceoffset;
	posRight.y += noteoffset;
	posRight.x -= lspace/4;

	if (context->openLeft)	posLeft.y = posRight.y;
	if (context->openRight)	posRight.y = posLeft.y;

	bowInfos->position = posLeft;
	bowInfos->offsets[2] = posRight - posLeft; // control points are stored as offsets to the position.
}

// -----------------------------------------------------------------------------
/** \brief Manual anchor points positionning, when dx and dy tags are specified.

	This calculates the reference points on which dx and dy offset will be applied.

	Following the guido specification of tie ans slur tags, those reference
	points are the center of the noteheads.
*/
void GRBowing::manualAnchorPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	// Careful, we have to deal with chords! what about getStemStartPosition()?
	NVPoint posLeft;
	NVPoint posRight;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

	const GRStaff * startStaff = startElement->getGRStaff();
	const GRStaff * endStaff   = endElement->getGRStaff();
	bool spanStaves = (startStaff != endStaff);

	GRStdNoteHead * head;

	const bool upward = context->curveDir == 1;

	// - Find left position
	if (context->bottomLeftHead && context->topLeftHead) // && !noteleft // useless additional test ? don't exists for rightPos !
	{
		head = upward ? context->topLeftHead : context->bottomLeftHead;
		posLeft = head->getNoteHeadPosition();
	}
	else posLeft = startElement->getPosition();

	// - Find right position
	if (context->bottomRightHead && context->topRightHead)
	{
		head = upward ? context->topRightHead : context->bottomRightHead;
		posRight = head->getNoteHeadPosition();
	}
	else posRight = endElement->getPosition();

	if (context->openLeft)	posLeft.y = posRight.y;
	if (context->openRight)	posRight.y = posLeft.y;

	if (spanStaves) posRight.y += endStaff->getPosition().y - startStaff->getPosition().y;

	bowInfos->position = posLeft;
	bowInfos->offsets[2] = posRight - posLeft; // control points are stored as offsets to the position.

	//	if( manualSettings == false )
//	arBow->setCurve( context->curveDir, posLeft, posRight ); // (JB) useless ?
}

// -----------------------------------------------------------------------------
void GRBowing::applyAnchorPointsOffsets( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	GRStaff * staff = context->staff;

	// -- Applies the offset settings to the start anchor point --
	const float spaceRatio = staff->getStaffLSPACE() / LSPACE;
	bowInfos->offsets[0].x += arBow->getDX1() * spaceRatio;
	bowInfos->offsets[0].y -= arBow->getDY1() * spaceRatio;
	bowInfos->offsets[2].x += arBow->getDX2() * spaceRatio;
	bowInfos->offsets[2].y -= arBow->getDY2() * spaceRatio;
}

// -----------------------------------------------------------------------------
/** \brief Use tag parameters to calculate the control points position.
*/
void GRBowing::manualControlPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	GRStaff * staff = context->staff;

	const float distx = bowInfos->offsets[2].x - bowInfos->offsets[0].x;
	const float disty = bowInfos->offsets[2].y - bowInfos->offsets[0].y;
	const int dir = context->curveDir;

	// -- Get the initial middle control point position.
	// If the R3 factor is near zero, the control point will be near the first anchor point
	// if the R3 factor is near 1, the control point will be near the second anchor point.

	float tagR3 =  arBow->getR3();
	const TYPE_FLOATPARAMETER valueR3 = (tagR3 == ARBowing::undefined()) ? 0.5f : tagR3;

	bowInfos->offsets[1].x = (GCoord)(distx * valueR3 );
	bowInfos->offsets[1].y = (GCoord)(disty * valueR3 * dir);

	// -- Apply the middle control point y-offset.
	const float lspaceRatio = staff->getStaffLSPACE() / LSPACE;
	float tagH = arBow->getH();
	const TYPE_FLOATPARAMETER valueH = (tagH == ARBowing::undefined() ? LSPACE/2 : tagH) * lspaceRatio;
	bowInfos->offsets[1].y += -dir * ((valueH > 0) ? valueH : -valueH);

	// - Store
	bowInfos->offsets[1] += bowInfos->offsets[0];
}

// -----------------------------------------------------------------------------
/** \brief Defines the bowing direction, depending on specified tags or music notation rules.

	This method only implement the basic rule: based on stem directions. Subclasses
	can override to add their own rules;
*/
void GRBowing::automaticCurveDirection( GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;

	// - Determines the direction of the first stem.
	int firstDir = context->stemDirLeft;
	if( firstDir == 0 )
	{
		const GRNote * firstNote = startElement->isGRNote();
		if( firstNote )
			firstDir = firstNote->getThroatDirection();
		context->stemDirLeft = firstDir;
	}

	// - Determines the direction of the last stem.
	int lastDir = context->stemDirRight;
	if( lastDir == 0 )
	{
		const GRNote * lastNote = endElement->isGRNote();
		if( lastNote )
			lastDir = lastNote->getThroatDirection();
		context->stemDirRight = lastDir;
	}

	// - If a stem direction is unknown, copy the other stem's direction.
	if( firstDir == 0 ) firstDir = lastDir;
	if( lastDir == 0 ) lastDir = firstDir;

	// - Here, we only look at first and last note to determine if stems are
	// mixed up/down. This is not perfect, we should look at all notes covered by the bow.
	const bool mixed = ( firstDir == -lastDir ); // also true if both are equal to 0

	// - For mixed stem directions, choose upward (ties and slurs)
	if( mixed )
		context->curveDir = 1;	// upward.
	else
		context->curveDir = - firstDir;
}

// -----------------------------------------------------------------------------
/** \brief Tries to avoid collisions.

	Called after anchor points positionning and after curve direction has been decided.
*/
void GRBowing::automaticControlPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	manualControlPoints( context, arBow, sse );
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
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(caller);
	if (el == 0 ) return;

	if (isGRSlur()) {
		fDeferredTellPostion.push(el);
		return;   // slurs are handled at the end
	}
	tellPositionEnd (el);
}

// -----------------------------------------------------------------------------
void GRBowing::tellPositionEnd (GRNotationElement * el)
{
	if (el == 0 ) return;

	GRStaff * staff = el->getGRStaff();
	if (staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)	return;

	const GRNotationElement * const startElement = sse->startElement;
	const GRNotationElement * const endElement = sse->endElement;
	if( el == endElement || ( endElement == 0 && el == startElement)) {
		NEPointerList* al = getAssociations();
		const GRSingleNote * start = startElement->isSingleNote();
		const GRSingleNote * end = endElement ? endElement->isSingleNote() : 0;
		int n = al ? al->size() : 0;
		bool grace = ((n == 2) && start && end && start->isGraceNote() && !end->isGraceNote());
		updateBow( staff, grace );
	}
}

// -----------------------------------------------------------------------------
/** \brief Called by the notes inbetween the range
*/
void GRBowing::addAssociation(GRNotationElement * grnot)
{
	if (getError()) return;

	if ( grnot->isGRNote() || grnot->isRest() || grnot->isEmpty() )
	  	GRARNotationElement::addAssociation(grnot);
	else setError(1);

	if (!getError())
		GRPositionTag::addAssociation(grnot);

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
	if(!mDraw || !mShow) return;
	if (getError()) return;
	assert( gCurSystem );

//	NVRect r = getAssociatedBoundingBox();
//	hdc.Frame(r.left, r.top, r.right, r.bottom);
//
//	DrawBoundingBox( hdc, VGColor( 255, 120, 150, 120 )); // DEBUG
//	hdc.Frame(mBoundingBox.left, mBoundingBox.top, mBoundingBox.right, mBoundingBox.bottom);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if( sse == 0) return; // don't draw

	// now we need to get the position and offset for the current bowing ...
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	assert(bowInfos);

	if (mColRef) hdc.PushFillColor( VGColor( mColRef ) );

	const float x = bowInfos->position.x;
	const float y = bowInfos->position.y;

//cerr << __LINE__  << " GRBowing::OnDraw " << bowInfos->position << " : " << bowInfos->offsets[0].x << " " << bowInfos->offsets[0].y << " | "
//		<< bowInfos->offsets[1].x << " " << bowInfos->offsets[1].y << " | "
//		<< bowInfos->offsets[2].x << " " << bowInfos->offsets[2].y << endl;
	::drawSlur( hdc,
				x + bowInfos->offsets[0].x,
				y + bowInfos->offsets[0].y,
				x + bowInfos->offsets[1].x,
				y + bowInfos->offsets[1].y,
				x + bowInfos->offsets[2].x,
				y + bowInfos->offsets[2].y,
				bowInfos->inflexion );

	// restore old pen and brush
	if (mColRef) hdc.PopFillColor();
	
//	hdc.Frame(fStartBox.left, fStartBox.top, fStartBox.right, fStartBox.bottom);
//	hdc.Frame(fEndBox.left, fEndBox.top, fEndBox.right, fEndBox.bottom);
//	hdc.Frame(fMidBox.left, fMidBox.top, fMidBox.right, fMidBox.bottom);
//cerr << "GRBowing::OnDraw high : " << fMidBox.TopLeft() << " low: " << fMidBox.BottomRight() << endl;
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
void drawSlur(VGDevice & hdc, float x1, float y1, float x2, float y2, float x3, float y3, float inflexion )
{
    float ratio;
    float x2a, y2a, x2b, y2b;

	// if start and endpoint is the same, just don't do anything.
	// this is a hack, whatsoever, because this should not really happen!
	if (x1==x3) return;

	const float oneOverDeltaX = (1.0f / (x3 - x1));

	ratio = (y3 - y1) * oneOverDeltaX;
    const float phi = atan(ratio);
    const float sinPhi = sin(phi);
    const float cosPhi = cos(phi);

    const bool upward = (y1 + x2*ratio)>y2;

    const float arcHalfWidth = (float)sqrt(pow(x1-x3,2)+pow(y1-y3,2))/2;
    const float arcHeight = (y2-y1)*cosPhi - (x2-x1)*sinPhi;
    const float inflexionCurveControl = 4; // Arbitrary, control the minimum inflexion under which the curve flattens
    const float inflexionH = arcHalfWidth * exp(-inflexion/8 ) * 4/5;
    const float inflexionV = arcHeight * (1 - 5* exp(-inflexion*inflexionCurveControl/8) ) / 4; //The bezier curve smooth out 1/4 of the arc's heigth

    // CALCULATE THE FIRST CURVE1
    x2a = x2 - inflexionH*cosPhi - inflexionV*sinPhi;
    y2a = y2 - inflexionH*sinPhi + inflexionV*cosPhi;
    x2b = x2 + inflexionH*cosPhi - inflexionV*sinPhi;
    y2b = y2 + inflexionH*sinPhi + inflexionV*cosPhi;

    // PROBABLY YOU WANT TO START A POLYGON NOW
    const int ptCount = (2 * ( NSEGS + 3 ));
	NVPoint thePoints[ ptCount ]; //ptCount ];
	int index = 0;
	makeCurve( x1, y1, x2a, y2a, x2b, y2b, x3, y3, NSEGS, thePoints, &index );

    // CALCULATE THE SECOND CURVE
    x2a -= (upward ? -1: 1) * SLUR_THICKNESS * sinPhi;
    y2a += (upward ? -1: 1) * SLUR_THICKNESS * cosPhi;
    x2b -= (upward ? -1: 1) * SLUR_THICKNESS * sinPhi;
    y2b += (upward ? -1: 1) * SLUR_THICKNESS * cosPhi;

    const float smoothHCorrection = inflexionH*(-inflexionV/arcHeight) / 4;
    x2a += smoothHCorrection * cosPhi;
    y2a += smoothHCorrection * sinPhi;
    x2b -= smoothHCorrection * cosPhi;
    y2b -= smoothHCorrection * sinPhi;

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
	hdc.PushPen( VGColor( 200, 0, 0 ), 5 );

	hdc.Line( x1 - 20, y1 - 20, x1 + 20, y1 + 20);
	hdc.Line( x1 - 20, y1 + 20, x1 + 20, y1 - 20);

	hdc.PushPen( VGColor( 200, 0, 200 ), 5 );
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


