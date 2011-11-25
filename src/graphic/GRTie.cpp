/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "ARTie.h"
#include "GRTie.h"
#include "GRGlue.h"
#include "GRRest.h"
#include "GREmpty.h"
#include "GRDefine.h"
#include "GRStdNoteHead.h"
#include "GRStaff.h"

const int kGuidoLongTieDistance = 12; // unit: dent. // 30 is too much

/** \brief Constructor without REAL abstract element.
*/
GRTie::GRTie(GRStaff * grstaff) : GRBowing(grstaff)
{
}


GRTie::GRTie(GRStaff * grstaff, ARTie * abstractRepresentationOfBowing)
		: GRBowing(grstaff, abstractRepresentationOfBowing)
{
}

GRTie::~GRTie()
{
}

GRTie::GRTie(GRStaff * grstaff,GRNotationElement * start, GRNotationElement * end)
	: GRBowing(grstaff, start, end)
{
}

bool GRTie::IsFull()
{
	assert(false);
	return false;
}

void GRTie::addAssociation(GRNotationElement * el)
{
	if (dynamic_cast<GRRest *>(el))
	{
		setError(1);
		return;
	}
	GRBowing::addAssociation(el);
}

// -----------------------------------------------------------------------------
/** \brief Calculate the placement of the bow.

	Changes the parameter-names and just handle positions correctly.

	calculation the position of y for dy3:
	(eposy-sposy)*dx3 + sposy + dy3 * hspace;
	-> (endElement->getTieEnd().y - 
	-> startElement->getTieEnd().y ) * dx3 +  ...

*/
void 
GRTie::updateBow( GRStaff * inStaff )
{
	GRBowing::updateBow( inStaff );
}

// -----------------------------------------------------------------------------
/**
*/
void
GRTie::automaticAnchorPoints( GRBowingContext * bowContext, ARBowing * arBow, 
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
	arBow->setCurve( bowContext->curveDir, posLeft, posRight ); // (JB) useless ?
}

// -----------------------------------------------------------------------------
/** \brief Try to minimize collisions.
*/
void
GRTie::automaticControlPoints( GRBowingContext * bowContext, ARBowing * arBow, 
												GRSystemStartEndStruct * sse )
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
