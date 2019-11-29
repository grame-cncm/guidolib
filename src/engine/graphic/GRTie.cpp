/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARTie.h"
#include "ARNote.h"
#include "GRAccidental.h"
#include "GRTie.h"
#include "GRRest.h"
#include "GREmpty.h"
#include "GRDefine.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"
#include "GRStaff.h"
#include "TagParameterString.h"

using namespace std;

const int kGuidoLongTieDistance = 12; // unit: dent. // 30 is too much

void GRTie::addAssociation(GRNotationElement * el)
{
	if (dynamic_cast<GRRest *>(el)) {
		setError(1);
		return;
	}
	GRBowing::addAssociation(el);
	if (!fHideAccidentals) return;

	GRSingleNote* note = el->isSingleNote();
	if (!note) return;
	if (fAssoc) {
		if (fPitch >=0) {
			if (note->getARNote()->getPitch() == fPitch)
				note->hideAccidentals ();
		}
		else  note->hideAccidentals ();
	}
	else {
		if (!note->getDuration()) {		// a note with a null duration denotes a chord
			fPitch = note->getARNote()->getPitch();
			fAssoc = true;
		}
		if (!el->isEmpty()) fAssoc = true;
	}
}

// -----------------------------------------------------------------------------
/**
*/
void GRTie::automaticAnchorPoints( const GRBowingContext * bowContext, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	// Careful, we have to deal with chords! what about getStemStartPosition()?
	NVPoint posLeft;
	NVPoint posRight;

	GRNotationElement * startElement = sse->startElement;
	GRNotationElement * endElement = sse->endElement;
	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;

	GRStdNoteHead * head;
	
	const bool upward = bowContext->curveDir == 1;
	
	const float xMargin = float(0.15) * LSPACE;
	
	// - Find left position
	if (bowContext->bottomLeftHead && bowContext->topLeftHead) // && !noteleft // useless additional test ? don't exists for rightPos !
	{
		if( upward ) 
			head = bowContext->topLeftHead;
		else 
			head = bowContext->bottomLeftHead;		// down ... we take the lowest notehead
		
		posLeft = head->getNoteHeadPosition() + NVPoint( head->getRightSpace() + xMargin, 0 );
	}
	else
		posLeft = startElement->getPosition() + NVPoint( startElement->getRightSpace() + xMargin, 0 );

	// - Find right position
	if (bowContext->bottomRightHead && bowContext->topRightHead)
	{
		if( upward )			
			head = bowContext->topRightHead;
		else 
			head = bowContext->bottomRightHead;		// down ... we take the lowest notehead
		
		posRight = head->getNoteHeadPosition() - NVPoint( head->getLeftSpace() + xMargin, 0 );
	}
	else
		posRight = endElement->getPosition() - NVPoint( endElement->getLeftSpace() + xMargin, 0 );

	// - Handle broken ties and minimal width
	const float minWidth = float(1.5) * LSPACE; // arbitrary miminal width of a tie
	if (bowContext->openLeft)	
	{
		posLeft.y = posRight.y;
		if( posLeft.x > (posRight.x - minWidth))
			posLeft.x = (posRight.x - minWidth);
	}

	if (bowContext->openRight)	
	{
		posRight.y = posLeft.y;
		if( posRight.x < (posLeft.x + minWidth))
			posRight.x = (posLeft.x + minWidth);
	}

	// - Avoid staff line collisions.
//	const float distx = posRight.x - posLeft.x;
//	const float longDistance = kGuidoLongTieDistance * LSPACE; 
//	const bool isLong = (distx >= longDistance);
//  const bool crossing = (isLong || ( ))
	GRStaff * staff = bowContext->staff;
	const float staffLSpace = staff->getStaffLSPACE();
	
	float yMargin = 0;
	if( positionIsOnStaffLine( posLeft.y, staffLSpace )) // at this point, posLeft.y == posRight.y
	{
		const float fact = float(0.25); // isLong ? float(0.5) : float(0.2);
		yMargin = fact * (upward ? -LSPACE : LSPACE);
	}
	else
	{
		const float fact = float(0.75); // isLong ? float(0) : float(0.7);
		yMargin = fact * (upward ? -LSPACE : LSPACE);
	}

	posLeft.y += yMargin;
	posRight.y += yMargin;

	// - Store
	bowInfos->position = posLeft;
	bowInfos->offsets[2] = posRight - posLeft; // control points are stored as offsets to the position.

	//	if( manualSettings == false )
//	arBow->setCurve( bowContext->curveDir, posLeft, posRight ); // (JB) useless ?
}

// -----------------------------------------------------------------------------
/** \brief Try to minimize collisions.
*/
void GRTie::automaticControlPoints( const GRBowingContext * bowContext, const ARBowing * arBow, GRSystemStartEndStruct * sse )
{
	// Do not cross staff lines, of clearly cross staff lines if the 
	// distance between the two tied note is large.

	GRBowingSaveStruct * bowInfos = (GRBowingSaveStruct *)sse->p;
	GRStaff * staff = bowContext->staff;
	const int dir = bowContext->curveDir;

	const float distx = bowInfos->offsets[2].x - bowInfos->offsets[0].x;
	const float longDistance = kGuidoLongTieDistance * LSPACE; 
	const bool isLong = (distx >= longDistance);

	const float r3 = float(0.5);	
	
	bowInfos->offsets[1].x = ( distx * r3 );

	const float staffLSpace = staff->getStaffLSPACE(); 
	const float valueH = staffLSpace * (isLong ? float(1.25) : float(0.5));

	bowInfos->offsets[1].y += -dir * valueH;

	// - Store
	bowInfos->offsets[1] += bowInfos->offsets[0];
}
