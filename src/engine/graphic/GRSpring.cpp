/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

// - Std c++
#include <cmath>
#include <typeinfo>
#include <algorithm>

// - Guido GR
#include "GRAccidental.h"
#include "GRSpring.h"
#include "GRNotationElement.h"
#include "GRGlue.h"
#include "GRRest.h"
#include "GREmpty.h"
#include "GRBar.h"
#include "GRMeter.h"
#include "GRStaff.h"
#include "GRKey.h"
#include "GRSpringCollider.h"
#include "GRSingleNote.h"
#include "GRVoice.h"
#include "GRRepeatEnd.h"
#include "GRStaffManager.h"

/* For proportionnal rendering */
#include "GRClef.h"
#include "GRFinishBar.h"
#include "GRRepeatBegin.h"
#include "GRRepeatEnd.h"

// - Guido misc
#include "kf_vect.h"
#include "VGDevice.h"
#include "GUIDOInternal.h"

using namespace std;

GRSpring::GRSpring(GRNotationElement *grn, GRVoice *vce, float spring, float propRender) :
	fSprcol(NULL), fFuncpar(spring), fProportionalRendering(propRender), fGrolst(0),fGrvlst(0)
{
	fIsfrozen = 0;
	fId = -1;
	fX  = 0;
	fForce = 0;

	fDur = grn->getDuration();
	fTp  = grn->getRelativeTimePosition();

	fHasDurElement = true;

	// add the grn to the List
	fGrolst.AddTail(grn);
	fGrvlst.AddTail(vce);

	fSconst = calcconst(grn);

    fIsProportionalElement = false;

	assert(fSconst != 0);
}

GRSpring::GRSpring(const TYPE_TIMEPOSITION & vtp,
				   const TYPE_DURATION  &vdur, float spring, float propRender) :
	fSprcol(NULL), fFuncpar(spring), fProportionalRendering(propRender), fGrolst(0),fGrvlst(0)
{
	fIsfrozen = 0;
	fId = -1;
	fX  = 0;
	fForce = 0;

	fDur = vdur;
	fTp  = vtp;

	fHasDurElement         = false;
    fIsProportionalElement = false;
	fSconst = defconst(fDur, fFuncpar);
	assert(fSconst != 0);
}

GRSpring::~GRSpring()
{
	delete fSprcol;
}

/** \brief Calculates the default spring-constant
*/ 
float GRSpring::defconst(const TYPE_DURATION &dur, float spring) // REM: merger avec la fonction de dessous
{
    float retval;

    if (dur == DURATION_0)
        return 20.0f;
    else {
        // This needs to be externally controlled!
		retval = (float)(1.0 / log( spring *( (double)dur + 1.0 )));
        long l = (long)floor( retval * 1000.0 + 0.5 );
        retval = (float)l  / 1000.0f;
        return retval;
    }
}

float GRSpring::defconst(float dur, float spring)
{
	float retval;

	if (dur == 0)
		return 20.0f;
	else {
		// This needs to be externally controlled!
		retval = (float)(1.0/log( spring *( dur + 1.0 )));
		long l = (long)floor( retval * 1000.0 + 0.5 );
		retval = (float)l / 1000.0f;
		return retval;
	}
}

/** \brief spring-constant remains constant. force is adjusted.
*/
float GRSpring::change_x(float dx)
{
	assert(dx >= 0.0);
	if (fIsfrozen)
	{
		// GuidoTrace("Warning, chaning length of a frozen spring");
	}
	long l = long(floor(dx * 1000.0+0.5));
	dx = l / 1000.0f;
	fX = dx;

	fForce = fX * fSconst;

	l = long(floor(fForce*1000.0+0.5));
	fForce = l / 1000.0f;

	return fForce;
}

/** \brief returns new Constant
*/
float GRSpring::change_dur(const TYPE_DURATION & ndur)
{
	if (ndur != fDur && ndur >= DURATION_0)
	{
		fDur = ndur;

		fHasDurElement = false;
		// we have to check the hasDurElement-flag
		GuidoPos pos = fGrolst.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = fGrolst.GetNext(pos);
			if (el->getDuration() == fDur)
			{
				fHasDurElement = true;
				break;
			}
		}

		return calcconst( 0 );
	}

	// if the duration didn't change, we just have
	// the same hasDurElement value
	return fSconst;
}

/** \brief fSconst remains const
*/
float GRSpring::change_force(float df)
{
	assert(df >= 0.0);
	long l = long(floor(df * 1000.0 + 0.5));
	df = l/1000.0f;
	fForce = df;

	fX = fForce / fSconst;

	l = long(floor(fX * 1000.0 +0.5));
	fX = l / 1000.0f;
	return fX;
}


/** \brief Sets the spring-constant. x remains constant, force is adjusted.
	This is important for prestretching!
*/
float GRSpring::set_const(float dc)
{
    if (fIsProportionalElement)
        dc = 1;

    assert(dc>0);
    fSconst = dc;
    fForce = fX*dc;

    return fForce;
}

/** \brief force remains const, x is adjusted
*/
float GRSpring::change_const(float dc)
{
   if (fIsProportionalElement)
        dc = 1;
	
    assert(dc > 0);
    long l = long(floor(dc * 100000.0+0.5));
    dc = l/100000.0f;
    fSconst = dc;

    fX = fForce / fSconst;

    l = long(floor(fX * 100000.0+0.5));
    fX = l / 100000.0f;

    return fX;
}

/** \brief Sets the length of the spring (from the rods)
	this is done, even if the spring is	frozen!
*/
float GRSpring::setlength(float dx)
{
	if (fX < dx)
		return change_x(dx);
	else
		return fForce;
}

/** \brief the spring-comparison-function for putting the
	springs into a sorted list.
*/
int GRSpring::operator<(const GRSpring & spr) const
{
	if (fTp < spr.fTp)
		return -1;
	if (spr.fTp > fTp)
		return 1;
	// timepositions are equal ....

	// this depends on the different graphical elements ...
	// make it changeable ... 
	return GRNotationElement::ComparePos(getFirstGRO(),spr.getFirstGRO());
	// return grcomp(*this,spr) ...
}

/** \brief Returns the length that would result, if the given force is applied
	to the spring with the current constant
*/
float GRSpring::apply_force(float df)
{
	if (!fIsfrozen)
		return df/fSconst;
	else
		return fX;
}

const GRNotationElement * GRSpring::getFirstGRO() const
{
	const GRNotationElement *el = fGrolst.GetHead();
	if (!el) 
		return NULL;
	else
		return el;
}


GuidoPos GRSpring::getFirstGROPosition()
{
	return fGrolst.GetHeadPosition();
}

const GRNotationElement * GRSpring::getNextGRO(GuidoPos &pos) const
{
	assert(pos);
	return fGrolst.GetNext(pos);
}

int GRSpring::getID() const			{ return fId; }
void GRSpring::setID(int id)		{ fId = id; }

void GRSpring::setGRSpringID()
{
	GuidoPos pos = fGrolst.GetHeadPosition();
    while (pos)
		fGrolst.GetNext(pos)->tellSpringID(fId);
}

int sprcomp(const GRSpring &gr1, const GRSpring &gr2)
{
	return gr1 < gr2;
}

int sprpcomp(const GRSpring *gr1,const GRSpring *gr2)
{
	return *gr1 < *gr2;
}

int GRSpring::setGRPositionX(GCoord p_posx)
{
	fPosx = p_posx;
	GuidoPos pos = fGrolst.GetHeadPosition();

	while (pos) {
		GRNotationElement * el = fGrolst.GetNext(pos);
		el->setHPosition(fPosx);
	}

	return 0;
}

int GRSpring::removeElement(GRNotationElement *el)
{
	GuidoPos pos = fGrolst.GetHeadPosition();
	GuidoPos pos2 = fGrvlst.GetHeadPosition();

	while (pos) {
		GRNotationElement *tmpel = fGrolst.GetAt(pos);

		if (tmpel == el) {
			fSprcol->RemoveElement(el,pos);
			fGrolst.RemoveElementAt(pos);
			fGrvlst.RemoveElementAt(pos2);
			break;
		}

		fGrolst.GetNext(pos);
		fGrvlst.GetNext(pos2);
	}

	return (pos != NULL);
}

/** \brief Sets the right extent of the elements to
	 zero... this is needed for bar/glue
*/
void GRSpring::setNoExtent()
{

	GuidoPos pos = fGrolst.GetHeadPosition();

	while (pos) {
		GRNotationElement * el = fGrolst.GetNext(pos);
		if (typeid(*el) == typeid(GRGlue)) {
			el->getReferenceBoundingBox().left = 0;
			el->getReferenceBoundingBox().right = 0;
		}
		else if (typeid(*el) == typeid(GRBar))
			el->getReferenceBoundingBox().right = 0; // change this!
	}
}

/** \brief Returns true if the type is present.
*/
bool GRSpring::hasType(const std::type_info & ti)
{
	GuidoPos pos = fGrolst.GetHeadPosition();
	
    while (pos) {
		GRNotationElement * el = fGrolst.GetNext(pos);

		if (typeid(*el) == ti)
			return true;
	}

	return false;
}

/** \brief Returns true if there is grace note.
*/
bool GRSpring::hasGraceNote()
{
	GuidoPos pos = fGrolst.GetHeadPosition();

	while (pos) {
		GRNotationElement *el   = fGrolst.GetNext(pos);
		GRSingleNote      *note = dynamic_cast<GRSingleNote*>(el);

        if (note && note->isGraceNote())
            return true;
	}

	return false;
}

bool GRSpring::containsBar() const
{
	GuidoPos pos = fGrolst.GetHeadPosition();
	
    while (pos) {
		GRNotationElement * el = fGrolst.GetNext(pos);

		if (dynamic_cast<GRBar *>(el)) // || dynamic_cast<GRRepeatEnd *>(el))
			return true;
	}

	return false;
}

float GRSpring::setProportionalForce()
{
	return change_force((float) ((double)fDur * fProportionalRendering * 3));
}

void GRSpring::addElement(GRNotationElement * el, GRVoice * vce)
{
	if (fProportionalRendering != 0) {
        if (!el->isInHeader()) {
            fIsProportionalElement = true;

            change_const(1);
            setProportionalForce();
        }
    }

	if (el->getRelativeTimePosition() != fTp) {
		if (el->getNeedsSpring() == 1) {
			fTp = el->getRelativeTimePosition();
		}
	}

	GuidoPos tmppos = fGrolst.AddTail(el);
	fGrvlst.AddTail(vce);

	const TYPE_DURATION & durel = el->getDuration();

	if (durel == fDur)
		fHasDurElement = true;

	if (fSprcol == 0)
		fSprcol = new GRSpringCollider();

	fSprcol->AddElement(el,tmppos);
}

/** \brief Called when the spring-constant needs to be recalculated.
*/
float GRSpring::recalcConstant()
{
	float oldconst = fSconst;
	
//	x  = 0;
//	fForce = 0;

	GRNotationElement * grn = fGrolst.GetHead();

	fSconst = (fIsProportionalElement ? 1 : calcconst(grn));

	if (oldconst != fSconst && (fX != 0 || fForce != 0)) {
		// then we have to recaluate the
		// force and all that ...
		// what happens with SPF?
		GuidoTrace("Error with springconstants");

		// let x be constant!
		fForce = fX / fSconst;
	}

	return fSconst;
}

float GRSpring::calcconst(GRNotationElement * grn)
{
	GRGlue * glue;
	GRTag * tmp;
	if (grn &&
		(glue = dynamic_cast<GRGlue *>(grn)) != NULL)
		fSconst = glue->getSConst();
	else if (grn && 
		(tmp = dynamic_cast<GRTag *>(grn)) != NULL)
		fSconst = tmp->getSConst();
	else if (fDur == DURATION_0) {
		if (dynamic_cast<GREmpty *>(grn))
            fSconst = (fIsProportionalElement ? 1 : 200.0f);
		else
			fSconst = (fIsProportionalElement ? 1 : 20.0f);
	}
	else if (grn && (dynamic_cast<GRRest *>(grn))) {
		// rests are treated as if they were
		// 8 times shorter -> this results
		// in a nicer spacing when rests occur ...
		// this must be adjusted for optimal output!
		TYPE_DURATION mydur;
		if ((float) fDur > 0.125)
			mydur.set( fDur.getNumerator(), fDur.getDenominator() * 2);
		else
			mydur = fDur;

		fSconst = (fIsProportionalElement ? 1 : defconst(mydur, fFuncpar));
	}
	else
		fSconst = (fIsProportionalElement ? 1 : defconst(fDur, fFuncpar));

	assert(fSconst != 0);

	return fSconst;
}

bool GRSpring::hasStaffAndType(const GRStaff * grstaff,const std::type_info & ti)
{
	GuidoPos pos = fGrolst.GetHeadPosition();

	while (pos) {
		GRNotationElement *el = fGrolst.GetNext(pos);

		if (typeid(*el) == ti) {
			// Staff only defined for Key right now...
			// maybe needs to be changed ...?
			GRKey *key = dynamic_cast<GRKey *>(el);

			if (key) {
				if (key->getGRStaff() == grstaff)
					return true;
			}
		}
	}

	return false;
}

/** \brief Finally stretches the spring.

	if the saved force is smaller then the given
	force is used. Also checks, whether the
	spring is frozen (no stretch if frozen)
*/
float GRSpring::stretchWithForce(float newforce)
{
    if (!fIsProportionalElement) {
        if (fForce >= newforce || fIsfrozen)
            return fX;

        return change_force(newforce);
    }
    else
        return setProportionalForce();
}


/** \brief Gets another spring and then figures out, how much distance
	should be put inbetween this spring and nextspring. 
	
	It looks at GRSpringCollider to find those areas, where a collision
	might (or does) occur.
*/
float GRSpring::GetMaxDistance(GRSpring * nextspring)
{
	if (this->hasType(typeid(GRGlue)))
	{
		// the first spring is a glue-spring.
		// then we just need the maximum leftspace
		// of the second spring and thats it ...
		float max = 0;
		GuidoPos pos = nextspring->fGrolst.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = nextspring->fGrolst.GetNext(pos);
			if (el->getLeftSpace() > max)
				max = el->getLeftSpace();
		}
		return max;
	}
	if (nextspring->hasType(typeid(GRGlue)))
	{
		// then the next is a glue, then we just need the maxrightspace
		// of this spring ....
		float max = 0;
		GuidoPos pos = fGrolst.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = fGrolst.GetNext(pos);
			if (el->getRightSpace() > max)
				max = el->getRightSpace();
		}
		return max;
	}

	float maxright = 0;
	float maxleft = 0;
	float max = 0;
	int count = KF_VECTOR_MININDEX;
	if (fSprcol && nextspring && nextspring->fSprcol)
	{
		poslist * pl1 = NULL;
		poslist * pl2 = NULL;
		bool cont = true;
		do
		{
			maxright = maxleft = 0;
			fSprcol->GetMatchingPoslists(count,nextspring->fSprcol, &pl1, &pl2);
			if (pl1 && pl2)
			{
				// I have matching poslists ....
				GuidoPos pospl1 = pl1->GetHeadPosition();
				while (pospl1)
				{
					GuidoPos evpos = pl1->GetNext(pospl1);
					GRNotationElement * el = fGrolst.GetAt(evpos);
					if (el->getRightSpace() > maxright)
						maxright = el->getRightSpace();

				}
				GuidoPos pospl2 = pl2->GetHeadPosition();
				while (pospl2)
				{
					GuidoPos evpos = pl2->GetNext(pospl2);
					GRNotationElement * el = nextspring->fGrolst.GetAt(evpos);
					if (el->getLeftSpace() > maxleft)
						maxleft = el->getLeftSpace();
				}
			}
			if (count==KF_VECTOR_MININDEX) 
				cont = false;
			if (maxright + maxleft > max)
				max = maxright + maxleft;
		} 
		while (cont);
	}
	return max;
}

/** \brief Checks for collisions at the current location. 

	It makes use of the GRSpringCollider
	because this already has a collection
	of elements that lie at the same y-position ....
	the idea is as follows:
	try to find elements that collide and relocate them.
*/
void GRSpring::checkLocalCollisions()
{
	checkAccidentalCollisions();
}

//--------------------------------------------------------------------
// static comparison functions, used to sort accidentals lists
static bool sortbyypos (const GRAccidental* a1, const GRAccidental* a2)
{
	return a1->getPosition().y < a2->getPosition().y;
}
static bool sortbystaff (const GRAccidental* a1, const GRAccidental* a2)
{
	return a1->getGRStaff() < a2->getGRStaff();
}

//------------------------------------------------------------
/** \brief Goes through the elements and finds notes and then checkes,
	 wether the accidentals collide.
*/
void GRSpring::checkAccidentalCollisions()
{
	vector<GRAccidental *> accidents;
	GuidoPos pos = fGrolst.GetHeadPosition();
	while (pos)
	{
		GRNotationElement * el = fGrolst.GetNext(pos);
		GRSingleNote * note = dynamic_cast<GRSingleNote *>(el);
		if (note)
		{
			GRAccidentalList noteacclist;
			note->extractAccidentals( &noteacclist );
			GuidoPos tmppos = noteacclist.GetHeadPosition();
			while (tmppos)
				accidents.push_back (noteacclist.GetNext(tmppos));
		}
	}

	vector<NVRect> accbbs;
	size_t n = accidents.size();
	if (n) {
		sort (accidents.begin(), accidents.end(), sortbyypos);
		sort (accidents.begin(), accidents.end(), sortbystaff);
		GRStaff * previousStaff = 0;
		NVPoint ystaffOffset;
		for (size_t i=0; i < n; i++) {				// start to collect the bonding boxes
			GRAccidental * acc = accidents[i];
			GRStaff * staff = acc->getGRStaff();	// check if we're on the smae staff
			ystaffOffset.y += (staff != previousStaff) ? LSPACE * 36 : 0;
			NVRect bb = acc->getBoundingBox();
			bb += acc->getPosition();
			bb += ystaffOffset;						// on next staff, add a y offset
			accbbs.push_back(bb);
			previousStaff = staff;
		}
		
		vector<float> accoffsets;					// next computes the offsets
		accoffsets.push_back(0);					// no offset for the first accidental
		for (size_t i=1; i<accbbs.size(); i++) {
			NVPoint offset (0,0);
			bool collides = accbbs[i].Collides(accbbs[i-1]);		// look for collision with the preceding accidental first
			int collindex = 0;
			for (int j=1; j<accbbs.size() && !collides; j++) {		// and then check the next accidentals
				if ((i > j) && accoffsets[i-j]) {
					collides = accbbs[i].Collides(accbbs[i-j-1]);
					if (collides) {
						collindex = int(i-j-1);
						break;
					}
				}
			}
			if (collides) {							// collision detected
				float offset = accoffsets[i-1] + accbbs[collindex].Width() + LSPACE/7;
				accoffsets.push_back(offset);		// put an offset to the left of the preceding accidental
				accbbs[i] += NVPoint(-offset,0);	// and update the accidental bounding box
			}
			else
				accoffsets.push_back(0);
		}

		for (size_t i=0; i < n; i++) {				// finally report the offsets to the notes
			GRAccidental * acc = accidents[i];
//			if (!acc->getOffsetSet()) {
				acc->addToOffset(NVPoint(-accoffsets[i],0));
//			}
		}
		
// this was the previous version
//		GuidoPos pos = myacclist->GetHeadPosition();
//		NVPoint pt;
//		GRAccidental * prevacc = 0;
//		while (pos)
//		{
//			GRAccidental * acc = myacclist->GetNext(pos);
//			GCoord cury = acc->getPosition().y;
//			if (prevacc)
//			{
//				if (acc->getGRStaff() != prevacc->getGRStaff()  || (cury - prevacc->getPosition().y) > (3 * LSPACE))
//					pt.x = 0;
//			}
//			if (!acc->getOffsetSet()) acc->addToOffset(pt);
//			pt.x -= 2 * (acc->getRightSpace());
//			prevacc = acc;
//		}

		// now I have to update bounding boxes of notes...
		pos = fGrolst.GetHeadPosition();
		while (pos)
		{
			GRSingleNote *sngnot = dynamic_cast<GRSingleNote *>(fGrolst.GetNext(pos));
			if (sngnot)
				sngnot->updateBoundingBox();
		}
	}
}

int GRSpring::isCommonSpring(int numvoices) const
{
	if (numvoices==1) return 1;

	// a hack!
	if (fGrolst.GetCount() >= 3) return 1;

	GuidoPos pos = fGrvlst.GetHeadPosition();
	GRVoice * savevoice = NULL;
	while (pos)
	{
		GRVoice *tmpvce = fGrvlst.GetNext(pos);
		if (tmpvce == NULL)
			return 1;
		if (savevoice == NULL)
			savevoice = tmpvce;
		else if (tmpvce != savevoice)
			return 1;
	}
	return 0;
//	if (fGrolst.GetCount() >= 2)  return 1;
}

/** \brief This tries to draw the spring.
*/
void GRSpring::OnDraw( VGDevice & hdc ) const
{
	const VGColor springColor ( 0, 0, 255, 50 ); 		//  semi-transparent blue
	const float springThickness = 5;

	hdc.PushPen( springColor, springThickness );

	const float offsety = -100;

	float x = fPosx, y = offsety;
	const int numturns = (int)((x-60.0f) / 40.0f + 0.5f );
	if (numturns < 1)
	{
		hdc.Line( x, y, (fPosx + x), offsety );
	}
	else
	{
		const float offset = ((x - numturns * 40) * 0.5f);
		hdc.Line( x, y, fPosx + offset, offsety );
		x = fPosx + offset;

		for( float i = 0; i < numturns; i++ )
		{
			hdc.Line( x, y, fPosx + offset + 40 * i + 10, -20 + offsety );
			x = fPosx + offset + 40 * i + 10;
			y = -20 + offsety;
			
			hdc.Line( x, y, fPosx + offset + 40 * i + 30, 20 + offsety );
			x = fPosx + offset + 40 * i + 30;
			y = 20 + offsety;

			hdc.Line( x, y, fPosx + offset + 40 * i + 40, offsety );
			x = fPosx + offset + 40 * i + 40;
			y = offsety;
		}
		hdc.Line( x, y, (fPosx + x), 0 + offsety );
	}
	hdc.PopPen();
}
