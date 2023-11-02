/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath> // for abs
#include <algorithm>

#include "GRGlobalStem.h"
#include "GRPage.h"
#include "GRSingleNote.h"
#include "GRSlur.h"
#include "GRStaff.h"
#include "GRStem.h"
#include "GRStdNoteHead.h"
#include "GRSystem.h"
#include "VGDevice.h"

using namespace std;


// -----------------------------------------------------------------------------
void GRSlur::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
void GRSlur::automaticCurveDirection( GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	// -- Long phrase positionning rule: choose upward if there is a lot of notes.
	// in the range.
	// We consider that this rule has the higer priority, so we return as soon as
	// it applies.

	const int aLotOfEvents = 16;
	const int evCount = mAssociated->GetCount();
	if( evCount > aLotOfEvents )
	{
		context->curveDir = 1;	// upward
		return;	
	}

	// -- Equal pitch for start and end: place on the stems. 
	// this rule avoids confusion with ties.
	// TODO: take care of accidentals.
	GRNote * firstNote = dynamic_cast<GRNote *>(sse->startElement);
	GRNote * lastNote = dynamic_cast<GRNote *>(sse->endElement);
	if( firstNote && lastNote )
	{
		int octave1, octave2;
		int pitch1, pitch2;
		int acc1, acc2;

		firstNote->getPitchAndOctave( &pitch1, &octave1, &acc1 );
		lastNote->getPitchAndOctave( &pitch2, &octave2, &acc2 );
		if(( pitch1 == pitch2 ) && (octave1 == octave2 ) && (acc1 == acc2))
		{
			context->curveDir = firstNote->getThroatDirection();
			return;
		}
	}

	// - Todo: the two voices rule (upward for upper voice, downward for lower voice)
	// - Todo: direction conservation if the slur covers many systems.
	// - Todo: "opposite to the tie" rule.
	
	// -- Call inherited rules.
	GRBowing::automaticCurveDirection( context, arBow, sse );
}

// -----------------------------------------------------------------------------
float GRSlur::getEltOffset (const GRNotationElement* el ) const
{
	const GRSingleNote* note = el->isSingleNote();
	if (note) {
		if (note->isTab())
			return 0;
		else
			return note->getNoteHead()->getOffset().x;
	}
	return 0;
}

// -----------------------------------------------------------------------------
NVRect	GRSlur::getElementBox 	(const GRBowingContext * context, const GRNotationElement* el ) const
{
	NVRect r = el->getBoundingBox() + el->getPosition();

	float leftlength = 0;
	float leftoffset = 0;
	float lefty = 0;
	if (context->leftChordStem) {
		leftlength = context->leftChordStem->getStemLength();
		lefty = context->leftChordStem->getGRStem()->getPosition().y;
		leftoffset = lefty - el->getPosition().y;
	}
	float rightlength = 0;
	float rightoffset = 0;
	float righty = 0;
	if (context->rightChordStem) {
		rightlength = context->rightChordStem->getStemLength();
		righty = context->rightChordStem->getGRStem()->getPosition().y;
		rightoffset = righty - el->getPosition().y;
	}
	if (context->stemDirLeft == 1) {
		if (el == context->topLeftHead)
			r.top -= leftlength - leftoffset;
		else if (el == context->topRightHead)
			r.top -= rightlength - rightoffset;
	}
	else {
		if (el == context->bottomLeftHead)
			r.bottom += leftlength + leftoffset;
		else if (el == context->bottomRightHead)
			r.bottom += rightlength + rightoffset;
	}
	return r;
}


// -----------------------------------------------------------------------------
void GRSlur::tellPositionEnd(GRNotationElement * el)
{
	if (el == 0 ) return;

	GRStaff * staff = el->getGRStaff();
	if (staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem());
	if (sse == 0)	return;

	GRBowing::tellPositionEnd( el );


//	if( el == sse->endElement || ( sse->endElement == 0 && el == sse->startElement)) {
//		if ((sse->startflag == GRSystemStartEndStruct::OPENLEFT) || (sse->endflag == GRSystemStartEndStruct::OPENRIGHT) ) {

	bool doit = ( el == sse->endElement || ( sse->endElement == 0 && el == sse->startElement))
				&& ((sse->startflag == GRSystemStartEndStruct::OPENLEFT) || (sse->endflag == GRSystemStartEndStruct::OPENRIGHT) );
	if (!doit) return;

	// special case of slurs spanning multiple staves
//cerr << (void*)this << " GRSlur::tellPositionEnd assoc " << sse->startflag << " " << sse->endflag << " sse: " << (void*)sse << endl;
	NEPointerList* al = getAssociations();
	GuidoPos pos = al->GetHeadPosition();
	NVRect r, max(1000,1000,1000,1000), min;
	int count = 0;
	while (pos) {
		const GRSystem* system = staff->getGRSystem();
		const GRNotationElement * elt = al->GetNext(pos);
		if (elt->getGRStaff()->getGRSystem() == system) {
			NVRect bb = elt->getBoundingBox() + elt->getPosition();
			if (bb.bottom > min.bottom) min = bb;
			if (bb.top < max.top) max = bb;
			r.Merge (elt->getBoundingBox() + elt->getPosition());
			if (elt->isGRNote()) count++;
		}
	}
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	bowInfos->box = r;
	bowInfos->max = max;
	bowInfos->min = min;

	const GRBowingContext* context = &bowInfos->context;
	const bool upward = (context->curveDir == 1);
	float shift = (upward ? -LSPACE : LSPACE) / 2;
	if (context->openRight) {
		if (count == 1) {
			bowInfos->offsets[2].y = bowInfos->offsets[0].y + shift;
			bowInfos->offsets[1].y = bowInfos->offsets[2].y + shift;
			bowInfos->inflexion = bowInfos->offsets[2].x / 100;
		}
		else if (upward) {
			if (bowInfos->offsets[2].y > 0) bowInfos->offsets[2].y = 0;
			if (bowInfos->offsets[2].y > bowInfos->offsets[0].y) bowInfos->offsets[2].y = bowInfos->offsets[0].y - LSPACE;
		}
	}
	if (context->openLeft) {
		bowInfos->offsets[0].y = (upward ? bowInfos->box.top - LSPACE : bowInfos->box.bottom + LSPACE) - bowInfos->position.y;
		if (count == 1) {
			bowInfos->offsets[0].y = bowInfos->offsets[2].y + shift;
			bowInfos->offsets[1].y = bowInfos->offsets[0].y + shift;
			bowInfos->inflexion = bowInfos->offsets[2].x / 100;
		}
		else if (upward) {
			if (bowInfos->offsets[0].y > 0) bowInfos->offsets[0].y = -LSPACE;
			float dist = bowInfos->offsets[1].y - r.top;
			if (dist < -100) bowInfos->offsets[1].y = -100; // force an arbitrary limit to the distance of the inflexion point
			else if (bowInfos->offsets[0].y > bowInfos->offsets[2].y) bowInfos->offsets[0].y = bowInfos->offsets[2].y - LSPACE;
		}
		else {
			float dist = r.bottom - bowInfos->offsets[1].y;
			if (dist > 100) bowInfos->offsets[1].y = 100; 	// force an arbitrary limit to the distance of the inflexion point
		}
	}
}

// -----------------------------------------------------------------------------
void GRSlur::OnDraw( VGDevice & hdc ) const
{
	GRBowing::OnDraw (hdc);
	
//	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
//	if( sse == 0) return; // don't draw
//
//	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

//	NVRect r = bowInfos->box;
//	hdc.Frame (r.left, r.top, r.right, r.bottom);
//	r = bowInfos->max;
//	hdc.PushPen(VGColor(255,0,0), 5);
//	hdc.Frame (r.left, r.top, r.right, r.bottom);
//	hdc.PopPen();
//	r = bowInfos->min;
//	hdc.PushPen(VGColor(0,0,255), 5);
//	hdc.Frame (r.left, r.top, r.right, r.bottom);
//	hdc.PopPen();

//	float d = 30;
//	NVPoint p = bowInfos->position;
//	hdc.Ellipse(bowInfos->offsets[0].x + p.x, bowInfos->offsets[0].y + p.y, d, d, VGColor(0,255,0));
//	hdc.Ellipse(bowInfos->offsets[1].x + p.x, bowInfos->offsets[1].y + p.y, d, d, VGColor(255,0,0));
//	hdc.Ellipse(bowInfos->offsets[2].x + p.x, bowInfos->offsets[2].y + p.y, d, d, VGColor(0,0,255));
}

// -----------------------------------------------------------------------------
void GRSlur::automaticAnchorPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	const bool upward = (context->curveDir == 1);

	// Careful, we have to deal with chords! what about getStemStartPosition()?
	NVPoint posLeft;
	NVPoint posRight;

	const GRNotationElement * startElement = sse->startElement;
	const GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

	const GRStaff * startStaff = startElement->getGRStaff();
	const GRStaff * endStaff   = endElement->getGRStaff();
	bool spanStaves = (startStaff != endStaff);

	// We try to fix the following problem here: with chord, the start and end
	// elements are GREmpty objects, with a zero bounding box. So we substitute
	// them by adequate noteheads.
	if( context->topLeftHead != context->bottomLeftHead ) // test if chord
		startElement = upward ? context->topLeftHead : context->bottomLeftHead;
	if( (context->topRightHead != context->bottomRightHead) || endElement->isEmpty()) // test if chord
		endElement = upward ? context->topRightHead : context->bottomRightHead;

	// -- Get the bounding box of the left and right elements.
	const NVRect leftBox  = getElementBox ( context, startElement );
	const NVRect rightBox = getElementBox ( context, endElement );

	const float yMargin = float(0.75) * LSPACE;
	// -- Calculates the start and end coordinates of the slur.
	if( upward ) {
		// Left x-pos
		posLeft.x = context->stemDirLeft == 1 ? leftBox.right : (leftBox.left + leftBox.right) * float(0.5);
		// Right x-pos
		posRight.x = context->stemDirRight == 1 ? rightBox.right : (rightBox.left + rightBox.right) * float(0.5);
		// y-pos
		posLeft.y = leftBox.top - yMargin;
		posRight.y = rightBox.top - yMargin;
	}
	else // downward
	{
		// Left x-pos
		posLeft.x = context->stemDirLeft == 1 ? (leftBox.left + leftBox.right) * float(0.5) : leftBox.left;
		// Right x-pos
		posRight.x = context->stemDirRight == 1 ? (rightBox.left + rightBox.right) * float(0.5) : rightBox.left;
		// y-pos
		posLeft.y = leftBox.bottom + yMargin;
		posRight.y = rightBox.bottom + yMargin;
	}

	if (spanStaves) posRight.y += endStaff->getPosition().y - startStaff->getPosition().y;

	// - Handle broken slurs and minimal width
	const float minWidth = 3 * LSPACE; // arbitrary miminal width of a tie
	const float openYOffset = upward ? - LSPACE : LSPACE;
	if (context->openLeft) {
		posLeft.y = posRight.y + openYOffset;
		if( posLeft.x > (posRight.x - minWidth))
			posLeft.x = (posRight.x - minWidth);
	}

	if (context->openRight) {
		const GRStaff* curStaff = endElement->getGRStaff();
		NVRect rs = curStaff->getBoundingBox();
		posRight.y = posLeft.y + openYOffset;
		posRight.y = upward ? rs.top : rs.bottom;
		if( posRight.x < (posLeft.x + minWidth))
			posRight.x = (posLeft.x + minWidth);
	}

	posLeft.x += getEltOffset(startElement);
	posRight.x += getEltOffset(endElement);
	// - Store results.
	bowInfos->position = posLeft;
	bowInfos->offsets[2] = posRight - posLeft; // control points are stored as offsets to the position.
}

// -----------------------------------------------------------------------------
/** \brief Modifies the shape of the curve to avoid collisions.

	  At this point, the curve direction (up or down) and the anchor points
	positions have already been decided. 
	
	  Here, we're only trying to choose a good position for the control points.
	  (We could also decide to adjust anchor points, according to the music 
	  notation rules) 
	  
*/
void GRSlur::automaticControlPoints( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	if (mAssociated == 0 ) return;

//cerr << "GRSlur::automaticControlPoints " << endl;
//if (1) {
//	GuidoPos pos = mAssociated->GetHeadPosition();
//	while( pos) {
//		GRNotationElement * el = mAssociated->GetNext(pos);
//cerr << " assoc: " << el << endl;
//	}
//}


	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	const bool upward = (context->curveDir == 1);

	// We don't need to adjust the curve if there is only two elements
	// (That is: only the first and the last event).
	
	// -- Calculates a bounding triangle that contains all objects between the
	// futur bow and the line joining the first and the last element.
	// Important: note that the screen y-axis is inverted.

	const float startX = bowInfos->position.x + bowInfos->offsets[0].x;
	const float startY = bowInfos->position.y + bowInfos->offsets[0].y;
	const float endX = bowInfos->position.x + bowInfos->offsets[2].x;
	const float endY = bowInfos->position.y + bowInfos->offsets[2].y;

    // Minimum angles
    const float minBaseAngle = float(12.f * M_PI/180.f);
	const float minHorizontalAngle = float(-8.f * M_PI / 180.f);


    // Slope of the base segment of the triangle
    float baseSlope = (endY-startY) / (endX-startX);
    const float baseAngle = atan(baseSlope);

	// Initial slopes of the left and right segments of the triangle.
	// (segment equation: y = Ax + B )

    //const float minSlope = float(0.03);	// force slurs to start with an angle.
//    float startA = (upward ? -minSlope : minSlope)+baseSlope;
//    float endA = (upward ? minSlope : -minSlope)+baseSlope;

    float startA = upward ? tan(-minBaseAngle + baseAngle) : tan(minBaseAngle + baseAngle);
    float endA =   upward ? tan( minBaseAngle + float(M_PI) + baseAngle) : tan( - minBaseAngle + float(M_PI) + baseAngle);
	
    if(upward){
        startA = min(startA, -tan(minHorizontalAngle));
        endA =   max(endA,   +tan(minHorizontalAngle));
    }else{
        startA = max(startA, +tan(minHorizontalAngle));
        endA =   min(endA,   -tan(minHorizontalAngle));
    }

	float x1, x2, y, a;	
	float extremeY;

	if( upward )	extremeY = (startY < endY ) ? startY : endY;
	else			extremeY = (startY > endY ) ? startY : endY;

	float refyStaff = context->staff->getPosition().y;

	// - Find the best slopes for the left and right segments of the triangle, such
	// that no object is above them (or below, if the bow goes downward).
	GuidoPos pos = mAssociated->GetHeadPosition();
	int size = mAssociated->size() - 2;
	mAssociated->GetNext(pos);		// skip the first element
	while( pos && size--)			// and the last element
	{
		GREvent * el = mAssociated->GetNext(pos)->isGREvent();
		if( el && !el->isEmpty()) {
			// - Get this element box
			NVRect elBox ( el->getBoundingBox());
			if( elBox.Width() == 0 && elBox.Height() == 0 ) continue; // useless ?
			NVPoint elPos = el->getPosition();
			elBox += elPos;

			// - Get the two corners we must deal with (depends of curve direction up/down)
			x1 = elBox.left;
			x2 = elBox.right;
			y = upward ? elBox.top : elBox.bottom;
			
			const GRStaff* staff = el->getGRStaff();
			float offset = staff ? staff->getPosition().y : refyStaff;
			y += offset - refyStaff;

			// - Ignore it, if it's outside the start/end range.
			//		this also avoid divisions by zero.
			if(( x1 <= startX ) || ( x2 >= endX )) continue;

			// - Catch the biggest encoutered Y, we'll need it later.
			if(( upward && ( y < extremeY )) || ( !upward && ( y > extremeY )))
				extremeY = y;

			// - Check for the left max slope.
			a =	(y - startY) / (x1 - startX);		// slope = (yb - ya) / (xb - xa)
			if(( upward && ( a < startA )) || ( !upward && ( a > startA )))
				startA = a;

			// - Check for the right max slope.
			a = (endY - y ) / (endX - x2);
			if(( upward && ( a > endA )) || ( !upward && ( a < endA )))
				endA = a;
		}
	}
    // -- Tune the slopes so the triangle is isocele
    float startAngle = atan(startA) - baseAngle;
    float endAngle   = atan(endA)   - baseAngle;

    if(abs(startAngle) < abs(endAngle))     startAngle = -endAngle  ;
    else                                    endAngle   = -startAngle;

    startA = tan(startAngle + baseAngle);
    endA   = tan(endAngle   + baseAngle);
	
	// -- Now, find the intersection point of the left and right 
	//	segments of the bounding triangle.

	float crossX = 0;	// the intersection point.
	float crossY = 0;	
	
	const float b1 = startY - (startA * startX); // calculate b from: y = ax + b
	const float b2 = endY - (endA * endX);		 // <=> b = y - (ax)



	// -- Force lines to intersect within the [startX, endX] range.
	// This is done by recalculating slopes that are too small.
	const float yForEndX = startA * endX + b1;
	if( upward && ( yForEndX > ( endY - 20 )))
		startA = ((endY - 20) - b1) / endX;
	else if( !upward && ( yForEndX < ( endY + 20 )))
		startA = ((endY + 20) - b1) / endX;

	const float yForStartX = endA * startX + b2;
	if( upward && ( yForStartX > ( startY - 20 ))) endA = ((startY - 20) - b2) / startX;
	else if( !upward && ( yForStartX < ( startY + 20 ))) endA = ((startY + 20) - b2) / startX;

	// -- Avoid too high curves.
	const float maxSlope = float(1.2);
	if(( abs(startA) > maxSlope ) && ( abs(endA) > maxSlope ))
	{
		startA = 0;	// abandon previous slopes, we'll use the 'middle' point.
		endA = 0;
	}
	
	// -- Calculate the intersection point by resolving the equation:
	// a1x + b1 = a2x + b2 <=> x = (b2 - b1) / (a1 - a2)
	if( startA != endA )
	{
		crossX = (b2 - b1) / (startA - endA);
		crossY = (startA * crossX) + b1;
	}

	// If we failed to find a good intersection point, just pick the middle point.
	// this should never happen.
	if( crossX <= (startX + 5) || crossX >= (endX - 5))
	{
		crossX = (startX + endX) * float(0.5);
		crossY = extremeY;
	}


	// -- Now, we have an intersection point. We're going to tune 
	//	it to get a nice curve.
    const float minDelta = 10;			// arbitrary value.
	const float maxDelta = 160;			
    const float factor = float(0.35);	// arbitrary value. old
	
	const float deltaY = abs(crossY - extremeY);
	float newDeltaY = factor * deltaY;	

	if( newDeltaY < minDelta ) newDeltaY = minDelta;
	else if( newDeltaY > maxDelta ) newDeltaY = maxDelta;	// avoid too high curves.

    crossY = upward ? (crossY - newDeltaY ) : (crossY + newDeltaY);

	
	// -- Here we change the sharpness of the curve inflexion. 
    const float maxInflexion = 7;  // Max inflexion (for small arc)
    const float power = 600;        // Arbitrary, scale the arc size
    const float arcWidth = (float)sqrt(pow(startX-endX,2)+ pow(startY-endY,2));
	bowInfos->inflexion = (maxInflexion-2) * exp( - arcWidth/power) + 2;

	// -- Apply the new control point.
	bowInfos->offsets[1].y = crossY - bowInfos->position.y;
    bowInfos->offsets[1].x = crossX - bowInfos->position.x;

{
	float limit = 250;
	float h = bowInfos->offsets[1].y;
	float y1 = bowInfos->offsets[0].y;
	float y2 = bowInfos->offsets[2].y;
	if ((h >= 0) && (h - y1 > limit) && (h - y2 > limit))
		bowInfos->offsets[1].y = std::max(y1, y2) + limit;
	else if ((h < 0) && (h - y1 < -limit) && (h - y2 < -limit))
		bowInfos->offsets[1].y = std::min(y1, y2) - limit;
}
}


