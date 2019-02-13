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

#include "ARArticulation.h"
#include "ARStaccato.h"
#include "ARMarcato.h"
#include "ARAccent.h"
#include "ARTenuto.h"
#include "ARFermata.h"
#include "ARBreathMark.h"
#include "ARPizzicato.h"
#include "ARHarmonic.h"

#include "GRArticulation.h"
#include "GRSingleNote.h"
#include "GRDefine.h"
#include "GRStaff.h"

#include "VGDevice.h" // debug only

using namespace std;

NVPoint GRArticulation::sRefposStacc;
NVPoint GRArticulation::sRefposStaccmoUp;
NVPoint GRArticulation::sRefposStaccmoDown;
NVPoint GRArticulation::sRefposLeftHPizz;
NVPoint GRArticulation::sRefposSnapPizz;
NVPoint GRArticulation::sRefposBuzzPizz;
NVPoint GRArticulation::sRefposFingernailPizz;
NVPoint GRArticulation::sRefposAccent;
NVPoint GRArticulation::sRefposMarcatoUp;
NVPoint GRArticulation::sRefposMarcatoDown;
NVPoint GRArticulation::sRefposTenuto;
NVPoint GRArticulation::sRefposFermataUp;
NVPoint	GRArticulation::sRefposFermataDown;
NVPoint GRArticulation::sRefposShortFermataUp;
NVPoint GRArticulation::sRefposShortFermataDown;
NVPoint GRArticulation::sRefposLongFermataUp;
NVPoint GRArticulation::sRefposLongFermataDown;
NVPoint GRArticulation::sRefposHarmonic;
NVPoint GRArticulation::sRefposBreathMark;

map<int,int> GRArticulation::sOrdering;

GRArticulation::GRArticulation(const ARMusicalTag * inTag, float curLSPACE, bool ownsar)
				: GRTagARNotationElement(inTag, curLSPACE, ownsar)	// , tinfo(typeid(*mtag))
{
	// assert(inTag);
	const std::type_info & tinfo = typeid(*inTag);
	if (tinfo == typeid(ARStaccato))	
	{
		if ( ((ARStaccato *) inTag)->getType() == ARStaccato::HEAVY )
			setupStaccmo();
		else setupStaccato();
	}
	else if (tinfo == typeid(ARAccent))			setupAccent();
	else if (tinfo == typeid(ARMarcato))
    {
        if ( ((ARMarcato *) inTag)->getMPosition() == ARMarcato::BELOW )
			setupMarcatoBelow();
		else if ( ((ARMarcato *) inTag)->getMPosition() == ARMarcato::ABOVE )
			setupMarcatoAbove();
        else
            setupMarcato();
    }
	else if (tinfo == typeid(ARTenuto))			setupTenuto();
	else if (tinfo == typeid(ARFermata))		
	{
		if ( ((ARFermata *) inTag)->getArticulationPosition() == ARFermata::kBelow )
		{
			if ( ((ARFermata *) inTag)->getType() == ARFermata::REGULAR )
				setupFermataDown();
			else if ( ((ARFermata *) inTag)->getType() == ARFermata::SHORT)
				setupShortFermataDown(); 
			else setupLongFermataDown(); 
		}
		else // above the note
		{
			if ( ((ARFermata *) inTag)->getType() == ARFermata::REGULAR )
				setupFermataUp();
			else if ( ((ARFermata *) inTag)->getType() == ARFermata::SHORT)	
				setupShortFermataUp();
			else setupLongFermataUp();
		}
	}
	else if (tinfo == typeid(ARBreathMark))	 	setupBreathMark();
	else if (tinfo == typeid(ARHarmonic))		setupHarmonic();
	else if (tinfo == typeid(ARPizzicato))			
	{
		if ( ((ARPizzicato *) inTag)->getType() == ARPizzicato::SNAP )
			setupSnapPizz();
		else if ( ((ARPizzicato *) inTag)->getType() == ARPizzicato::BUZZ)
			setupBuzzPizz();
		else if ( ((ARPizzicato *) inTag)->getType() == ARPizzicato::FINGERNAIL)
			setupFingernailPizz();
		else setupLeftHPizz();
	}
	else setupStaccato();
}

void GRArticulation::print(ostream& os) const
{
	os << "articulation " << mArticulationFlag << endl;
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
//  kFlagBreathMark ???
}

/*----------------Setup fonctions-------------------------------------------------*/
void GRArticulation::setupStaccato()
{
	setArticulationSymbol( kStaccatoSymbol );
	const float height = LSPACE * float(0.38);		
	sRefposStacc = NVPoint(-mLeftSpace, 0 );		
	mBoundingBox.Set( -mLeftSpace, -height*0.5f, mRightSpace, 0.5f*height );
	mArticulationFlag = kFlagStaccato;
}

void GRArticulation::setupStaccmo()
{
	setArticulationSymbol( kStaccmoUpSymbol);
	mArticulationFlag = kFlagStaccmo;
}

void GRArticulation::setupLeftHPizz()
{
	setArticulationSymbol(kLeftHPizzSymbol);
	const float height = LSPACE*float(1.2);		
	sRefposLeftHPizz = NVPoint(-mLeftSpace, 0);	
	mBoundingBox.Set(-mLeftSpace, height*float(0.5), mRightSpace, -height*float(0.5)); 
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupSnapPizz()
{
	setArticulationSymbol(kSnapPizzSymbol);
	const float height = LSPACE * float(1.17);
	sRefposSnapPizz = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, height*float(0.29), mRightSpace, -height*float(0.71));
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupBuzzPizz()
{
	setArticulationSymbol(kBuzzPizzSymbol);
	const float height = LSPACE * float(0.68);
	sRefposBuzzPizz = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace - float(0.72)*height, height*float(0.5), mRightSpace, -height*float(0.5));
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupFingernailPizz()
{
	setArticulationSymbol(kFingernailPizzSymbol);
	const float height = LSPACE * float (0.9);
	sRefposFingernailPizz = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, 0, mRightSpace, -height);
	mArticulationFlag = kFlagPizz;
}

void GRArticulation::setupAccent()
{
	setArticulationSymbol( kAccentSymbol );
	const float height = LSPACE * float(1);
	sRefposAccent = NVPoint(-mLeftSpace, height * float(0.5));		
	mBoundingBox.Set( -mLeftSpace, -height * float(0.5), mRightSpace, height * float(0.5));
	mArticulationFlag = kFlagAccent;
}

void GRArticulation::setupMarcato()
{
	mArticulationFlag = kFlagMarcato;
	setArticulationSymbol( kMarcatoUpSymbol );

	setMarcatoDirection( true );
}

void GRArticulation::setupMarcatoAbove()
{
    mArticulationFlag = kFlagMarcatoUp;
	setArticulationSymbol( kMarcatoUpSymbol );

	setMarcatoDirection( true );
}

void GRArticulation::setupMarcatoBelow()
{
	mArticulationFlag = kFlagMarcatoDown;
	setArticulationSymbol( kMarcatoDownSymbol );

	setMarcatoDirection( false );
}

void GRArticulation::setupTenuto()
{
	setArticulationSymbol( kTenutoSymbol );
	const float height = LSPACE * float(0.15);
	sRefposTenuto = NVPoint(-mLeftSpace,0);		
	mBoundingBox.Set( -mLeftSpace, -height*float(0.5), mRightSpace, height*float(0.5) );
	mArticulationFlag = kFlagTenuto;
}

void GRArticulation::setupFermataUp()
{
	setArticulationSymbol( kFermataUpSymbol );
	const float height = LSPACE * float(1.33);
	sRefposFermataUp = NVPoint(-mLeftSpace, -0.3f*LSPACE);	//reference = center of the dot
	mBoundingBox.Set( -mLeftSpace, -height-0.3f*LSPACE, mRightSpace, -0.3f*LSPACE );
	mArticulationFlag = kFlagFermataUp;
}

void GRArticulation::setupFermataDown()
{
	setArticulationSymbol(kFermataDownSymbol);	
	const float height = LSPACE * float(1.33);
	sRefposFermataDown = NVPoint (-mLeftSpace,-0.2f*LSPACE);		//reference = center of dot
	mBoundingBox.Set(-mLeftSpace, height-0.2f*LSPACE, mRightSpace, -0.2f*LSPACE);
	mArticulationFlag = kFlagFermataDown;
}

void GRArticulation::setupShortFermataUp()
{
	setArticulationSymbol( kShortFermataUpSymbol );
	const float height = LSPACE * 1.66f;
	sRefposShortFermataUp = NVPoint(-mLeftSpace, -0.34f*LSPACE);	//reference = center of dot
	mBoundingBox.Set(-mLeftSpace, -height-0.34f*LSPACE, mRightSpace, -0.34f*LSPACE);
	mArticulationFlag = kFlagFermataUp;
}

void GRArticulation::setupShortFermataDown()
{
	setArticulationSymbol(kShortFermataDownSymbol);
	const float height = LSPACE * 1.66f;
	sRefposShortFermataDown = NVPoint(-mLeftSpace, float(1.5)*LSPACE); //reference = center of dot
	mBoundingBox.Set(-mLeftSpace, 1.5f*LSPACE-height, mRightSpace, 1.5f*LSPACE);
	mArticulationFlag = kFlagFermataDown;
}

void GRArticulation::setupLongFermataUp()
{
	setArticulationSymbol(kLongFermataUpSymbol);
	const float height = LSPACE * 1.33f;
	sRefposLongFermataUp = NVPoint(-mLeftSpace, -0.2f*LSPACE);
	mBoundingBox.Set(-mLeftSpace, -0.2f*LSPACE-height, mRightSpace, -0.2f*LSPACE);
	mArticulationFlag = kFlagFermataUp;
}

void GRArticulation::setupLongFermataDown()
{
	setArticulationSymbol(kLongFermataDownSymbol);
	const float height = LSPACE * float(1.33);
	sRefposLongFermataDown = NVPoint(-mLeftSpace, float(1)*LSPACE);
	mBoundingBox.Set(-mLeftSpace, 1*LSPACE-height, mRightSpace, 1*LSPACE);
	mArticulationFlag = kFlagFermataDown;
}

void GRArticulation::setupHarmonic()
{
	setArticulationSymbol (kHarmonicSymbol);
	const float height = LSPACE * float(0.5);
	sRefposHarmonic = NVPoint(-mLeftSpace, -LSPACE*float(0.5));
	mBoundingBox.Set(-mLeftSpace, -1.5f*height, mRightSpace, -height/2); //TODO !
	mArticulationFlag = kFlagHarmonic;
}

void GRArticulation::setupBreathMark()
{
	setArticulationSymbol( kBreathMarkSymbol );
	const float height = LSPACE * 1.33f;
	sRefposBreathMark = NVPoint(-mLeftSpace, -height * float(0.75));	
	mBoundingBox.Set( -mLeftSpace, -height, mRightSpace, 0 );
	mArticulationFlag = kFlagBreathMark;
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

void GRArticulation::setMarcatoDirection( bool above )
{
	const float height = LSPACE * float(1.66);
	if( above )
	{
		mSymbol = kMarcatoUpSymbol;
		sRefposMarcatoUp = NVPoint(-mLeftSpace, height * float(0.33));	// up		
		mBoundingBox.Set( -mLeftSpace, - height * float(0.66), mRightSpace, height * float(0.33)); // up
	}
	else
	{
		mSymbol = kMarcatoDownSymbol;
		sRefposMarcatoDown = NVPoint(-mLeftSpace, height * float(0.70));	// down
		mBoundingBox.Set( -mLeftSpace, - height * float(0.30), mRightSpace, height * float(0.70)); // down
	}
}

void GRArticulation::setStaccmoDirection( bool above )
{
	const float height = LSPACE * float(0.64);
	if( above )
	{
		mSymbol = kStaccmoUpSymbol;
		sRefposStaccmoUp = NVPoint(-mLeftSpace, -LSPACE*0.5f);	// up		
		mBoundingBox.Set( -mLeftSpace, - height-0.5f*LSPACE, mRightSpace, -LSPACE*0.5f); // up
	}
	else
	{
		mSymbol = kStaccmoDownSymbol;
		sRefposStaccmoDown = NVPoint(-mLeftSpace, -LSPACE*0.25f);	// down
		mBoundingBox.Set( -mLeftSpace, 0, mRightSpace, height); // down
	}
}

// ----------------------------------------------------------------------------
void GRArticulation::tellPosition(GObject * caller, const NVPoint & inPos)	// Called by GREvent when event position
{																			// is set
	GREvent * ev = GREvent::cast(caller);
	if( ev == 0 )
		return;

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
		case kFlagBreathMark:	placeBreathMark( ev, newPoint );	break;
		case kFlagPizz :		placePizz (ev, newPoint);			break;
		case kFlagHarmonic :	placeHarmonic(ev, newPoint);		break;
	}

	setPosition( newPoint );
	// - TEST, Update bounding box, but only if we're not a breath-mark
	if( mArticulationFlag != kFlagBreathMark )
	{
		ev->addToBoundingBox( this );
		ev->updateBoundingBox();
	}
}

// ----------------------------------------------------------------------------
// The staccato has the highest priority. It is placed first, close to the note.
void GRArticulation::placeStaccato( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	float hspace = space * 0.5f;

	if (getPlacement( inParent ) == ARArticulation::kAbove) {
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
	setStaccmoDirection( getPlacement( inParent ) == ARArticulation::kAbove );

	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float hspace = space * 0.5f;

	if (getPlacement( inParent ) == ARArticulation::kAbove) {
		float topMax = min(inParent->getStemEndPos().y, inParent->getPosition().y) - hspace;
		// ensure the position is outside the staff
		if (topMax > -space) topMax = -space;
		ioPos.y = onStaffLine (staff, topMax) ? topMax - hspace : topMax;
	}
	else {
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
		topMax = resolveCollisionAbove(inParent, topMax, space*1.2f, kFlagMarcato | kFlagMarcatoUp | kFlagAccent | kFlagFermataUp);
		ioPos.y = (float)topMax;
	}
	else {
		const double bottom = staffBottom(staff) + space * 1.1f;
		double bottomMin = max(inParent->getStemEndPos().y, inParent->getPosition().y) + minSpace;
		// ensure the position is outside the staff
		if (bottomMin <= bottom) bottomMin = bottom;
		bottomMin = resolveCollisionBelow(inParent, bottomMin, space*1.1f, kFlagMarcato | kFlagMarcatoDown | kFlagAccent | kFlagFermataDown);
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

		double y = (*i)->getPosition().y;
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
	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, minSpace, kFlagFermataUp | kFlagMarcato | kFlagMarcatoUp);
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
	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, minSpace, kFlagFermataDown | kFlagMarcato | kFlagMarcatoDown);
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
void GRArticulation::placeMarcatoAbove( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	const float space = staff->getStaffLSPACE();
	const bool above = true;
	const float minSpace = space * 1.5f;

	setMarcatoDirection( above );
	// check the minimum y position regarding note position and stems
	double topMax = min(-space, inParent->getStemEndPos().y - space);
	topMax = min(topMax, double(inParent->getPosition().y - minSpace));

	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, minSpace, kFlagFermataUp | kFlagMarcato | kFlagMarcatoUp);
}

// ----------------------------------------------------------------------------
// outside the staff prefered
void GRArticulation::placeMarcatoBelow( const GREvent * inParent, NVPoint & ioPos )
{
	GRStaff * staff = inParent->getGRStaff();
	float space = staff->getStaffLSPACE();
	const float minSpace = space * 1.5f;
	const double bottom = staffBottom(staff) + space;
	const bool above = false;

	setMarcatoDirection( above );
	// check the minimum y position regarding note position and stems
	double bottomMin = max((float)bottom, inParent->getStemEndPos().y + space);
	bottomMin = max(bottomMin, double(inParent->getPosition().y + minSpace));

	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, minSpace, kFlagFermataDown | kFlagMarcato | kFlagMarcatoDown);
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
		bottomMin = resolveCollisionBelow(inParent, bottomMin, (float)minSpace, kFlagPizz | kFlagMarcato | kFlagMarcatoDown | kFlagAccent | kFlagFermataDown);
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
	ioPos.y = (float)resolveCollisionAbove(inParent, topMax, space, kFlagFermataUp);
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
	ioPos.y = (float)resolveCollisionBelow(inParent, bottomMin, space*1.5f, kFlagFermataDown);
}

// ----------------------------------------------------------------------------
// - Obsolete, breath-mark is no longer attached to any note.
void GRArticulation::placeBreathMark( const GREvent * inParent, NVPoint & ioPos )
{
	const GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	const float halfSpace = float(0.5) * currLSpace;

	// TODO: place just before next element.
	ioPos.y = - halfSpace;
	ioPos.x += float(1.5) * LSPACE;
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

const NVPoint & GRArticulation::getReferencePosition() const
{
	switch ( mSymbol )	// this depends on the type ...
	{
		case kStaccatoSymbol:			return sRefposStacc;			break;
		case kStaccmoUpSymbol :			return sRefposStaccmoUp;		break;
		case kStaccmoDownSymbol :		return sRefposStaccmoDown;		break;
		case kLeftHPizzSymbol:			return sRefposLeftHPizz;		break;
		case kSnapPizzSymbol :			return sRefposSnapPizz;			break;
		case kBuzzPizzSymbol :			return sRefposBuzzPizz;			break;
		case kFingernailPizzSymbol :	return sRefposFingernailPizz;	break;
		case kAccentSymbol:				return sRefposAccent;			break;
		case kMarcatoUpSymbol:			return sRefposMarcatoUp;		break;
		case kMarcatoDownSymbol:		return sRefposMarcatoDown;		break;
		case kTenutoSymbol:				return sRefposTenuto;			break;
		case kFermataUpSymbol:			return sRefposFermataUp;		break;
		case kFermataDownSymbol:		return sRefposFermataDown;		break;
		case kShortFermataUpSymbol:		return sRefposShortFermataUp;	break;
		case kShortFermataDownSymbol:	return sRefposShortFermataDown; break;
		case kLongFermataUpSymbol:		return sRefposLongFermataUp;	break;
		case kLongFermataDownSymbol :	return sRefposLongFermataDown;  break;
		case kBreathMarkSymbol:			return sRefposBreathMark;		break;
		case kHarmonicSymbol:			return sRefposHarmonic;			break;
		default:						return GObject::sRefposNone;
	}
}
