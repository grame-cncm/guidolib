/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

	-- Articulations of duration ---

		Never combined, with a rare exception for the tenuto.

		- (TODO) Staccatissimo:( v ) aka Dash (no musical symbol in font?)
			Placement: next free space from the notehead.
					can be inside the staff.
			Height: less than a dent. 
			Refpoint: TODO.
			Tag:  \staccmo (advanced guido draft)

		- Staccato: ( . ) 
			Placement: next free space from the notehead.
					can be inside the staff. Must be placed before
					other articulations.
			Height: a quarter of a dent. 
			Refpoint: centered on the symbol. 
			Tag: \stacc
			
		- Pizzicato: (left-hand, snap, pizz, buzz)
			Placement: above the staff, except when parts share a staff.
			Tag : \pizz
						

		- Tenuto:  ( - ) aka Sostenuto or Stress
			Placement: next free space from the notehead.
					can be inside the staff.
			Height: thin: about 1/8 of a dent.
			Refpoint: centered. 
			Tag: \ten

	-- Articulations of force  ---

		Never combined;

		- Accent:   ( > ) 
			Height: one dent
			Placement: outside the staff prefered.
			Refpoint:  centered. 
			Tag: \accent

		- Marcato:  ( ^ ) aka Sforzando
			Height: one dent and half
			Placement: head side prefered. outside the staff prefered.
					based on a line (then it looks like a pyramide)
			Refpoint: centered, 1/4 of the height 
			Tag: \marcato

    -- Others sign handled as articulations --

		- Fermata: (dot in arc)			 
			Placement: above the staff, except when parts share a staff.
						Must be placed outside (after) articulations ans slurs
			Height: a dent and a quarter.
			Refpoint: dot.
			Tag: \fermata
			
		- Short fermata : (dot in ^)
			Placement: above the staff, except when parts share a staff.
						Must be placed outside (after) articulations ans slurs
			Height: a dent and a quarter.
			Refpoint: dot.
			Tag: \shortFermata

		( no longer used here: Breath-mark ( , ) 
			Placement: above the staff, just before the next note or barline.
			Refpoint: middle bottom.
			Tag: \breathMark	)		


	--- Rules:

		(1) Symbols must not cross staff lines (excepts the marcato)
		(2) Symbols are always centered on the note head. A staccato alone on 
			the stem side is centered on this stem.
		(3) Combined articulations must be kept on the same side of the note.
		(4) When combined, the staccato is always centered on the note head, 
			even if it's on the stem size.

	-----

	implementation revised on Sept. 2016 DF
	there is room for improvements:
		check refpos for all symbols
		possibly check also for the font implementation
		with homogeneous refpos, the code could be more factorized
		there also possible useless functions and settings
*/

#include <typeinfo>
#include <cassert>
#include <algorithm>

#include "ARAccent.h"
#include "ARArticulation.h"
#include "ARBow.h"
#include "ARFermata.h"
#include "ARHarmonic.h"
#include "ARMarcato.h"
#include "ARPizzicato.h"
#include "ARStaccato.h"
#include "ARTenuto.h"

#include "GRArticulation.h"
#include "GRDefine.h"
#include "GRSingleNote.h"
#include "GRStaff.h"

#include "VGDevice.h"

using namespace std;

map<int,int> GRArticulation::sOrdering;

GRArticulation::GRArticulation(const ARMusicalTag * inTag, float curLSPACE, bool ownsar)
				: GRTagARNotationElement(inTag, curLSPACE, ownsar)
{
	// assert(inTag);
	setArticulationSymbol (getSymbol(inTag));
	fRefPos = getReferencePosition (mSymbol);
	mArticulationFlag = getArticulationFlag (inTag);
	float height = getSymbolHeight(mSymbol);
	const std::type_info & tinfo = typeid(*inTag);
	if ((tinfo == typeid(ARFermata)) || (tinfo == typeid(ARHarmonic)))
		mBoundingBox.Set( -mLeftSpace, height * -1, mRightSpace, 0);
	else
		mBoundingBox.Set( -mLeftSpace, height * -0.5f, mRightSpace, height * 0.5f);
}

void GRArticulation::print(ostream& os) const
{
	os << "articulation " << mArticulationFlag << endl;
}


// -----------------------------------------------------------------------------
int GRArticulation::getArticulationFlag		( const ARMusicalTag * ar ) const
{
	const ARArticulation* art = dynamic_cast<const ARArticulation*>(ar);
	if (!art) return 0;

	bool below = art->getArticulationPosition() == ARArticulation::kBelow;
	const std::type_info & tinfo = typeid(*ar);
	if (tinfo == typeid(ARStaccato))	return ( ((ARStaccato *)ar)->getType() == ARStaccato::HEAVY ) ?  kFlagStaccmo : kFlagStaccato;
	if (tinfo == typeid(ARAccent))		return kFlagAccent;
	if (tinfo == typeid(ARMarcato))		return below ? kFlagMarcatoDown : kFlagMarcatoUp;
	if (tinfo == typeid(ARTenuto))		return kFlagTenuto;
	if (tinfo == typeid(ARFermata)) 	return below ? kFlagFermataDown : kFlagFermataUp;
	if (tinfo == typeid(ARHarmonic))	return kFlagHarmonic;
	if (tinfo == typeid(ARPizzicato))	return kFlagPizz;
	if (tinfo == typeid(ARBow))	 		return kFlagBow;
	return 0;
}

// -----------------------------------------------------------------------------
void GRArticulation::OnDraw( VGDevice & hdc ) const
{
	GRTagARNotationElement::OnDraw (hdc);
//	DrawBoundingBox(hdc, VGColor(250,0,0));
}

// -----------------------------------------------------------------------------
void GRArticulation::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

/*----------------initialize ordering map ----------------------------------------*/
void GRArticulation::initOrder()
{
	int i = 1;
	sOrdering[kFlagStaccato]	= i++;
	sOrdering[kFlagTenuto]		= i++;
	sOrdering[kFlagStaccmo]		= i++;
	sOrdering[kFlagHarmonic]	= i++;
	sOrdering[kFlagPizz]		= i++;
	sOrdering[kFlagAccent]		= i++;
	sOrdering[kFlagMarcato]		= i;
	sOrdering[kFlagMarcatoUp]	= i;
	sOrdering[kFlagMarcatoDown]	= i++;
	sOrdering[kFlagFermataUp]	= i;
	sOrdering[kFlagFermataDown]	= i++;
	sOrdering[kFlagBow]			= i++;
}

// -----------------------------------------------------------------------------
float GRArticulation::getSymbolHeight(unsigned int symbol) const
{
	float hspace = LSPACE;
	switch (symbol) {
		case kBowDownASymbol :
		case kBowDownBSymbol :		return hspace * 1.3f;
		case kBowUpASymbol :
		case kBowUpBSymbol :		return hspace * 2.1f;
		
		case kStaccmoDownSymbol:
		case kStaccmoUpSymbol:		return hspace * 0.55f;

		case kSnapPizzSymbol :
		case kLeftHPizzSymbol:		return hspace * 1.25f;
		case kBuzzPizzSymbol :		return hspace * 0.5f;
		case kFingernailPizzSymbol: return hspace;

		case kTenutoSymbol:			return hspace * 0.15f;
		case kStaccatoSymbol:		return hspace * 0.33f;
		case kMarcatoUpSymbol:
		case kMarcatoDownSymbol:	return hspace * 1.6f;

		case kFermataUpSymbol:
		case kFermataDownSymbol:
		case kLongFermataUpSymbol:
		case kLongFermataDownSymbol :	return hspace * 1.33f;
		case kShortFermataUpSymbol:
		case kShortFermataDownSymbol:	return hspace * 1.6f;

		case kAccentSymbol:			return hspace;
		case kHarmonicSymbol:		return hspace * 0.5f;

		default:	return 0.f;
	}
}

unsigned int GRArticulation::getSymbol( const ARMusicalTag * ar ) const
{
	const ARArticulation* art = dynamic_cast<const ARArticulation*>(ar);
	return art ? art->getSymbol() : 0;
}

void GRArticulation::setArticulationSymbol( unsigned int inSymbolID )
{
	mSymbol = inSymbolID;
	const float extent = GetSymbolExtent( inSymbolID );
	mLeftSpace = (extent * 0.5f);
	mRightSpace = (extent * 0.5f);
}

void GRArticulation::setPosition(const NVPoint & point)
{
	GRARNotationElement::setPosition(point);
}

// ----------------------------------------------------------------------------
void GRArticulation::tellPosition(GObject * caller, const NVPoint & inPos)	// Called by GREvent when event position
{																			// is set
	GREvent * ev = GREvent::cast(caller);
	if( ev == 0 ) return;
	
	NVPoint newPoint (inPos);
	switch( mArticulationFlag )
	{
		case kFlagStaccato:		placeStaccato( ev, newPoint );		break;
		case kFlagStaccmo:		placeStaccmo( ev, newPoint);		break;
		case kFlagAccent:		placeAccent( ev, newPoint );		break;
		case kFlagMarcato:		placeMarcato( ev, newPoint );		break;
        case kFlagMarcatoUp:	placeMarcatoAbove( ev, newPoint );	break;
        case kFlagMarcatoDown:	placeMarcatoBelow( ev, newPoint );	break;
		case kFlagTenuto:		placeTenuto( ev, newPoint );		break;
		case kFlagFermataUp:	placeFermataAbove( ev, newPoint );	break;
		case kFlagFermataDown:	placeFermataBelow(ev, newPoint );	break;
		case kFlagPizz :		placePizz (ev, newPoint);			break;
		case kFlagHarmonic :	placeHarmonic(ev, newPoint);		break;

		case kFlagBow :			placeBow(ev, newPoint);			break;
	}

	setPosition( newPoint );
	ev->addToBoundingBox( this );
	ev->updateBoundingBox();
}

// ----------------------------------------------------------------------------
// The staccato has the highest priority. It is placed first, close to the note.
void GRArticulation::placeStaccato( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	float hspace = space * 0.5f;
	int placement = getPlacement( inParent );

	if ((placement == ARArticulation::kAbove) || (placement == ARArticulation::kDefaultPosition)) {
		float topMax = min(inParent->getStemEndPos().y, inParent->getPosition().y) - space;
		ioPos.y = onStaffLine (staff, topMax) ? topMax - hspace : topMax;
	}
	else {
		float bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + space;
		ioPos.y = onStaffLine (staff, bottomMin) ? bottomMin + hspace : bottomMin;
	}
}

// ----------------------------------------------------------------------------
// to be checked !!
void GRArticulation::placeStaccmo( const GREvent * inParent, NVPoint & ioPos )
{
//	setStaccmoDirection( getPlacement( inParent ) == ARArticulation::kAbove );

	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float hspace = space * 0.5f;
	int placement = getPlacement( inParent );

	if ((placement == ARArticulation::kAbove) || (placement == ARArticulation::kDefaultPosition)) {
		float topMax = min(inParent->getStemEndPos().y, inParent->getPosition().y) - hspace;
		// ensure the position is outside the staff
		if (topMax > -space) topMax = -space;
		ioPos.y = onStaffLine (staff, topMax) ? topMax - hspace : topMax;
	}
	else {
		setArticulationSymbol (kStaccmoDownSymbol);
		const float bottom = (float)staffBottom(staff) + space*0.65f;
		float bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + space;
		// ensure the position is outside the staff
		if (bottomMin <= bottom) bottomMin = bottom;
		ioPos.y = onStaffLine (staff, bottomMin) ? bottomMin + hspace : bottomMin;
	}
}

// ----------------------------------------------------------------------------
void GRArticulation::placePizz( const GREvent * inParent, NVPoint & ioPos)
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float minSpace = space * 1.5f;

	if (getPlacement( inParent ) == ARArticulation::kAbove) {
		double topMax = min(-space, inParent->getStemEndPos().y - space);
		topMax = min(topMax, double(inParent->getPosition().y - minSpace));
		if (topMax > -space) topMax = -space;
		topMax = resolveCollisionAbove(inParent, topMax, space*1.2f, kFlagMarcato | kFlagMarcatoUp | kFlagAccent | kFlagFermataUp | kFlagBow);
		ioPos.y = (float)topMax;
	}
	else {
		const double bottom = staffBottom(staff) + space * 1.1f;
		double bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + minSpace;
		// ensure the position is outside the staff
		if (bottomMin <= bottom) bottomMin = bottom;
		bottomMin = resolveCollisionBelow(inParent, bottomMin, space*1.1f, kFlagMarcato | kFlagMarcatoDown | kFlagAccent | kFlagFermataDown | kFlagBow);
		ioPos.y = (float)bottomMin;
	}
}

//-----------------------------------------------------------------------------
// avoid collisions with other articulations when above the staff
double GRArticulation::resolveCollisionAbove ( const GREvent * inParent, double currentpos, float minSpace, int skiptypes) const
{
	const GRNEList& articulations = inParent->getArticulations();
	for (GRNEList::const_iterator i = articulations.begin(); i != articulations.end(); i++) {
		if (*i == this) continue;

		GRArticulation* grart = dynamic_cast<GRArticulation*>(*i);
		int type = grart ? grart->getArticulationType() : 0;
		if (type & skiptypes) continue;

		double y = (*i)->getPosition().y + (*i)->getBoundingBox().top;
		if (y <= (currentpos + minSpace)) currentpos = y - minSpace;
	}
	return currentpos;
}

//-----------------------------------------------------------------------------
// avoid collisions with other articulations when above the staff
double GRArticulation::resolveCollisionBelow ( const GREvent * inParent, double currentpos, float minSpace, int skiptypes) const
{
	const GRNEList& articulations = inParent->getArticulations();
	for (GRNEList::const_iterator i = articulations.begin(); i != articulations.end(); i++) {
		if (*i == this) continue;

		GRArticulation* grart = dynamic_cast<GRArticulation*>(*i);
		int type = grart ? grart->getArticulationType() : 0;
		if (type & skiptypes) continue;

		double y = (*i)->getPosition().y;
		if (y >= (currentpos - minSpace)) currentpos = y + minSpace;
	}
	return currentpos;
}

//-----------------------------------------------------------------------------
bool GRArticulation::onStaffLine (const GRStaff * staff, double pos) const
{
	if (pos <  0) return false;
	int linesCount = staff->getNumlines();
	float lspace = staff->getStaffLSPACE();
	if (pos > (linesCount - 1) * lspace) return false;
	
	double curpos = 0;
	for (int i=0; i < linesCount; i++, curpos+=lspace ) {
		if (pos == curpos) return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
double GRArticulation::staffBottom (const GRStaff * staff) const
{
	int linesCount = staff->getNumlines();
	float lspace = staff->getStaffLSPACE();
	return (linesCount - 1) * lspace;
}

//-----------------------------------------------------------------------------
void GRArticulation::placeAccent( const GREvent * inParent, NVPoint & ioPos )
{
	const int placement = getPlacement( inParent );
	if (placement == ARArticulation::kAbove)
		placeAccentAbove(inParent, ioPos);
	else
		placeAccentBelow(inParent, ioPos);
}

// ----------------------------------------------------------------------------
void GRArticulation::placeAccentAbove( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	const float space = staff->getStaffLSPACE();
	const float topspace = space;
	const float minSpace = space * 1.5f;

	// check the minimum y position regarding note position and stems
	double topMax = min(inParent->getStemEndPos().y, inParent->getPosition().y) - minSpace;
	// ensure the position is outside the staff
	if (topMax > -topspace) topMax = -topspace;
	// avoid collisions with other articulations
	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, space, kFlagFermataUp | kFlagMarcato | kFlagMarcatoUp | kFlagBow);
}

// ----------------------------------------------------------------------------
void GRArticulation::placeAccentBelow( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float minSpace = space * 1.5f;
	const double bottom = staffBottom(staff) + space;

	// check the minimum y position regarding note position and stems
	double bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + minSpace;
	// ensure the position is outside the staff
	if (bottomMin <= bottom) bottomMin = bottom;
	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, minSpace, kFlagFermataDown | kFlagMarcato | kFlagMarcatoDown | kFlagBow);
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeMarcato( const GREvent * inParent, NVPoint & ioPos )
{
	// default position for strong accent is above the stave, regardless of stems direction
	// see Gould - Behind bars - p.117
	placeMarcatoAbove (inParent, ioPos);
}


// ----------------------------------------------------------------------------
void GRArticulation::placeBowAbove( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	const float space = staff->getStaffLSPACE() * 1.4f;
	const float minSpace = space * 1.5f;

	// check the minimum y position regarding note position and stems
	double topMax = min(-space, inParent->getStemEndPos().y - space);
	topMax = min(topMax, double(inParent->getPosition().y - minSpace));
	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, space, 0); //kFlagFermataUp | kFlagMarcato | kFlagMarcatoUp);
}

// ----------------------------------------------------------------------------
void GRArticulation::placeBowBelow( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float minSpace = space *  (mSymbol == kBowUpBSymbol ?  1.5 : 1.0f);;

	const double bottom = staffBottom(staff) + space * (mSymbol == kBowUpBSymbol ?  1.5 : 0.6f);
	double bottomMin = max((float)bottom, inParent->getStemEndPos().y + space);
	bottomMin = max(bottomMin, double(inParent->getPosition().y + minSpace));
	// + space * (mSymbol == kBowUpBSymbol ?  1.5 : 0.6f);
	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, minSpace, 0); //kFlagFermataDown | kFlagMarcato | kFlagMarcatoDown);
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeBow( const GREvent * inParent, NVPoint & ioPos )
{
	const ARBow* bow = dynamic_cast<const ARBow*>(getAbstractRepresentation());
	if (! bow) return;
	if (bow->getArticulationPosition() == ARArticulation::kBelow) placeBowBelow (inParent, ioPos);
	else placeBowAbove (inParent, ioPos);
}

// ----------------------------------------------------------------------------
void GRArticulation::placeMarcatoAbove( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	const float space = staff->getStaffLSPACE();
	const float minSpace = space * 1.5f;

	// check the minimum y position regarding note position and stems
	double topMax = min(-space, inParent->getStemEndPos().y - space);
	topMax = min(topMax, double(inParent->getPosition().y - minSpace));

	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, space, kFlagFermataUp | kFlagMarcato | kFlagMarcatoUp | kFlagBow);
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeMarcatoBelow( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float minSpace = space * 1.5f;
	const double bottom = staffBottom(staff) + space;

	// check the minimum y position regarding note position and stems
	double bottomMin = max((float)bottom, inParent->getStemEndPos().y + space);
	bottomMin = max(bottomMin, double(inParent->getPosition().y + minSpace));

	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, minSpace, kFlagFermataDown | kFlagMarcato | kFlagMarcatoDown | kFlagBow);
}

// ----------------------------------------------------------------------------
// The tenuto is placed just after the staccato, if any
void GRArticulation::placeTenuto( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float hspace = space * 0.5f;

	if (getPlacement( inParent ) == ARArticulation::kAbove) {
		double topMax = min(inParent->getStemEndPos().y, inParent->getPosition().y) - space;
		topMax = resolveCollisionAbove(inParent, topMax, hspace, ~kFlagStaccato);
		if (onStaffLine (staff, topMax))	topMax -= hspace;
		ioPos.y = (float)topMax;
	}
	else {
		double bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + space;
		bottomMin = resolveCollisionBelow(inParent, bottomMin, hspace, ~kFlagStaccato);
		if (onStaffLine (staff, bottomMin)) bottomMin += hspace;
		ioPos.y = (float)bottomMin;
	}
}

// ----------------------------------------------------------------------------
// harmonic
void GRArticulation::placeHarmonic( const GREvent * inParent, NVPoint & ioPos)
{
	GRStaff * staff = inParent->getGRStaff();
	double space = staff->getStaffLSPACE();
	const double minSpace = space * 1.5f;
	const double hspace = space * 0.5f;
	const double hhspace = hspace * 0.5f;

	if (getPlacement( inParent ) == ARArticulation::kAbove) {
		double topMax = min(-space*0.15, inParent->getStemEndPos().y - hhspace);
		topMax = min(topMax, inParent->getPosition().y - space * 0.7f);
		topMax = resolveCollisionAbove(inParent, topMax, (float)hspace, kFlagPizz | kFlagMarcato | kFlagMarcatoUp | kFlagAccent | kFlagFermataUp);
		ioPos.y = (float)topMax;
	}
	else {
		const double bottom = staffBottom(staff) + minSpace;
		double bottomMin = max(bottom, inParent->getStemEndPos().y + space * 1.2f);
		bottomMin = max(bottomMin, inParent->getPosition().y + space * 1.6f);
		bottomMin = resolveCollisionBelow(inParent, bottomMin, (float)minSpace, kFlagPizz | kFlagMarcato | kFlagMarcatoDown | kFlagAccent | kFlagFermataDown | kFlagBow);
		ioPos.y = (float)bottomMin;
	}

}

// ----------------------------------------------------------------------------
void GRArticulation::placeFermataAbove( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	const float space = staff->getStaffLSPACE();
	const float hspace = space * 0.5f;

	// check the minimum y position regarding note position and stems
	double topMax = min(0.f, inParent->getStemEndPos().y);
	topMax = min(topMax, double(inParent->getPosition().y - hspace));
	// avoid collisions with other articulations
	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, hspace, kFlagFermataUp | kFlagBow);
}

void GRArticulation::placeFermataBelow( const GREvent * inParent, NVPoint & ioPos)
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float bottom = (float)staffBottom(staff) + space * 0.5f;

	// check the minimum y position regarding note position and stems
	float bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + space;
	// ensure the position is outside the staff
	if (bottomMin <= bottom) bottomMin = bottom;
	// avoid collisions with other articulations
	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, space*1.5f, kFlagFermataDown | kFlagBow);
}

// ----------------------------------------------------------------------------
// gives the ARArticulation setting
int GRArticulation::getARPlacement() const {
	const ARArticulation* art = dynamic_cast<const ARArticulation*>(getAbstractRepresentation());
	return art ? art->getArticulationPosition() : ARArticulation::kDefaultPosition;
}

// ----------------------------------------------------------------------------
// gives an ARArticulation placement
int GRArticulation::getPlacement( const GREvent * inParent ) const
{
	int arplacement = getARPlacement();
	if (arplacement) return arplacement;

	const GRSingleNote * sng = inParent->isSingleNote();
	if( sng && sng->getDirection() == dirUP )
		return ARArticulation::kBelow;
	else 
		return ARArticulation::kAbove;
}

const NVPoint & GRArticulation::getReferencePosition() const	{ return fRefPos; }

NVPoint GRArticulation::getReferencePosition (unsigned int symbol) const
{
	float height = LSPACE;
//	return NVPoint(-mLeftSpace, height );
	switch ( symbol )	// this depends on the type ...
	{
		case kStaccatoSymbol:
		case kLeftHPizzSymbol:
		case kSnapPizzSymbol :
		case kBuzzPizzSymbol :
		case kFingernailPizzSymbol :
		case kTenutoSymbol:				return NVPoint(-mLeftSpace, 0 );

		case kAccentSymbol:				return NVPoint(-mLeftSpace, height * 0.5f);
		case kStaccmoUpSymbol :			return NVPoint(-mLeftSpace, height * -0.5f);
		case kStaccmoDownSymbol :		return NVPoint(-mLeftSpace, height * -0.25f);

		case kBowUpASymbol:
		case kBowDownASymbol:			return NVPoint(-mLeftSpace, height * 1.0f);
		case kMarcatoUpSymbol:			return NVPoint(-mLeftSpace, height * 0.5478f);

		case kBowUpBSymbol:
		case kBowDownBSymbol:			return NVPoint(-mLeftSpace, height * 1.2);
		case kMarcatoDownSymbol:		return NVPoint(-mLeftSpace, height * 1.162f);

		case kFermataUpSymbol:			return NVPoint(-mLeftSpace, height * -0.3f);
		case kFermataDownSymbol:		return NVPoint(-mLeftSpace, height * -0.2f);

		case kShortFermataUpSymbol:		return NVPoint(-mLeftSpace, height * -0.34f);
		case kShortFermataDownSymbol:	return NVPoint(-mLeftSpace, height * 1.5);

		case kLongFermataUpSymbol:		return NVPoint(-mLeftSpace, height * -0.2f);
		case kLongFermataDownSymbol :	return NVPoint(-mLeftSpace, height);

		case kHarmonicSymbol:			return NVPoint(-mLeftSpace, height * -0.5f);

		default:						return GObject::sRefposNone;
	}
}
