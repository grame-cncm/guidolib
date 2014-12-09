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
#include "GuidoDefs.h"		 // For kLayoutSettingDefaultSpring
#include "kf_vect.h"
#include "VGDevice.h"
#include "GUIDOInternal.h"

using namespace std;

float GRSpring::funcpar = kSettingDefaultSpring;

GRSpring::GRSpring(GRNotationElement *grn, GRVoice *vce) :
	grolst(0),grvlst(0),sprcol(NULL)
{
	isfrozen = 0;
	id = -1;
	x  = 0;
	force = 0;

	dur = grn->getDuration();
	tp  = grn->getRelativeTimePosition();

	hasDurElement = true;

	// add the grn to the List
	grolst.AddTail(grn);
	grvlst.AddTail(vce);

	sconst = calcconst(grn);

    isProportionalElement = false;

	assert(sconst != 0);
}

GRSpring::GRSpring(const TYPE_TIMEPOSITION & vtp,
				   const TYPE_DURATION  &vdur) :
	grolst(0),grvlst(0),sprcol(NULL)
{
	isfrozen = 0;
	id = -1;
	x  = 0;
	force = 0;

	dur = vdur;
	tp  = vtp;

	hasDurElement         = false;
    isProportionalElement = false;

    sconst = defconst(dur);

	assert(sconst != 0);
}

GRSpring::~GRSpring()
{
	delete sprcol;
}

/** \brief Calculates the default spring-constant
*/ 
float GRSpring::defconst(const TYPE_DURATION &dur) // REM: merger avec la fonction de dessous
{
    float retval;

    if (dur == DURATION_0)
        return 20.0f;
    else {
        // This needs to be externally controlled!
        retval = (float)(1.0 / log( funcpar *( (double)dur + 1.0 ))); 
        long l = (long)floor( retval * 1000.0 + 0.5 );
        retval = (float)l  / 1000.0f;
        return retval;
    }
}

float GRSpring::defconst(float dur) 
{
	float retval;

	if (dur == 0)
		return 20.0f;
	else {
		// This needs to be externally controlled!
		retval = (float)(1.0/log( funcpar *( dur + 1.0 ))); 
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
	if (isfrozen)
	{
		// GuidoTrace("Warning, chaning length of a frozen spring");
	}
	long l = long(floor(dx * 1000.0+0.5));
	dx = l / 1000.0f;
	x = dx;

	force = x * sconst;

	l = long(floor(force*1000.0+0.5));
	force = l / 1000.0f;

	return force;
}

/** \brief returns new Constant
*/
float GRSpring::change_dur(const TYPE_DURATION & ndur)
{
	if (ndur != dur && ndur >= DURATION_0)
	{
		dur = ndur;

		hasDurElement = false;
		// we have to check the hasDurElement-flag
		GuidoPos pos = grolst.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = grolst.GetNext(pos);
			if (el->getDuration() == dur)
			{
				hasDurElement = true;
				break;
			}
		}

		return calcconst( 0 );
	}

	// if the duration didn't change, we just have
	// the same hasDurElement value
	return sconst;
}

/** \brief sconst remains const
*/
float GRSpring::change_force(float df)
{
	assert(df >= 0.0);
	long l = long(floor(df * 1000.0 + 0.5));
	df = l/1000.0f;
	force = df;

	x = force / sconst;

	l = long(floor(x * 1000.0 +0.5));
	x = l / 1000.0f;
	return x;
}


/** \brief Sets the spring-constant. x remains constant, force is adjusted.
	This is important for prestretching!
*/
float GRSpring::set_const(float dc)
{
    if (isProportionalElement)
        dc = 1;

    assert(dc>0);
    sconst = dc;
    force = x*dc;

    return force;
}

/** \brief force remains const, x is adjusted
*/
float GRSpring::change_const(float dc)
{
    if (isProportionalElement)
        dc = 1;

    assert(dc > 0);
    long l = long(floor(dc * 100000.0+0.5));
    dc = l/100000.0f;
    sconst = dc;

    x = force / sconst;

    l = long(floor(x * 100000.0+0.5));
    x = l / 100000.0f;

    return x;
}

/** \brief Sets the length of the spring (from the rods)
	this is done, even if the spring is	frozen!
*/
float GRSpring::setlength(float dx)
{
	if (x < dx)
		return change_x(dx);
	else
		return force;
}

/** \brief Adds a new graphical-element at to the spring. First, it is checked, wether
	the duration is shorter; if this is so, the spring-constant is changed. 
*/
void GRSpring::addSameTP( GRNotationElement * el, GRVoice * vce )
{
	if (el->getDuration()<dur)
	{
		hasDurElement = true;
		change_const(defconst(dur));
	}
	
	dur = el->getDuration();

	addElement(el,vce);

}

/** \brief the spring-comparison-function for putting the
	springs into a sorted list.
*/
int GRSpring::operator<(const GRSpring & spr) const
{
	if (tp<spr.tp)
		return -1;
	if (spr.tp>tp)
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
	if (!isfrozen)
		return df/sconst;
	else
		return x;
}

const GRNotationElement * GRSpring::getFirstGRO() const
{
	const GRNotationElement *el = grolst.GetHead();
	if (!el) 
		return NULL;
	else
		return el;
}


GuidoPos GRSpring::getFirstGROPosition()
{
	return grolst.GetHeadPosition();
}

const GRNotationElement * GRSpring::getNextGRO(GuidoPos &pos) const
{
	assert(pos);
	return grolst.GetNext(pos);
}

int GRSpring::getID()
{
	return id;
}

void GRSpring::setID(int _id)
{
	id = _id;
}

void GRSpring::setGRSpringID()
{
	GuidoPos pos = grolst.GetHeadPosition();
	
    while (pos)
		grolst.GetNext(pos)->tellSpringID(id);
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
	posx = p_posx;
	GuidoPos pos = grolst.GetHeadPosition();

	while (pos) {
		GRNotationElement * el = grolst.GetNext(pos);
		el->setHPosition(posx);
	}

	return 0;
}

int GRSpring::removeElement(GRNotationElement *el)
{
	GuidoPos pos = grolst.GetHeadPosition();
	GuidoPos pos2 = grvlst.GetHeadPosition();

	while (pos) {
		GRNotationElement *tmpel = grolst.GetAt(pos);

		if (tmpel == el) {
			sprcol->RemoveElement(el,pos);
			grolst.RemoveElementAt(pos);
			grvlst.RemoveElementAt(pos2);
			break;
		}

		grolst.GetNext(pos);
		grvlst.GetNext(pos2);
	}

	return (pos != NULL);
}

/** \brief Sets the right extent of the elements to
	 zero... this is needed for bar/glue
*/
void GRSpring::setNoExtent()
{

	GuidoPos pos = grolst.GetHeadPosition();

	while (pos) {
		GRNotationElement * el = grolst.GetNext(pos);
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
	GuidoPos pos = grolst.GetHeadPosition();
	
    while (pos) {
		GRNotationElement * el = grolst.GetNext(pos);

		if (typeid(*el) == ti)
			return true;
	}

	return false;
}

/** \brief Returns true if there is grace note.
*/
bool GRSpring::hasGraceNote()
{
	GuidoPos pos = grolst.GetHeadPosition();

	while (pos) {
		GRNotationElement *el   = grolst.GetNext(pos);
		GRSingleNote      *note = dynamic_cast<GRSingleNote*>(el);

        if (note && note->isGraceNote())
            return true;
	}

	return false;
}

bool GRSpring::containsBar() const
{
	GuidoPos pos = grolst.GetHeadPosition();
	
    while (pos) {
		GRNotationElement * el = grolst.GetNext(pos);

		if (dynamic_cast<GRBar *>(el)) // || dynamic_cast<GRRepeatEnd *>(el))
			return true;
	}

	return false;
}

float GRSpring::setProportionalForce()
{
    return change_force((float) ((double)dur * GRStaffManager::sPropRender * 3));
}

void GRSpring::addElement(GRNotationElement * el, GRVoice * vce)
{
    if (GRStaffManager::sPropRender != 0) {
        if (!el->isInHeader()) {
            isProportionalElement = true;

            change_const(1);
            setProportionalForce();
        }
    }

	if (el->getRelativeTimePosition() != tp) {
		if (el->getNeedsSpring() == 1)
			tp = el->getRelativeTimePosition();
	}

	GuidoPos tmppos = grolst.AddTail(el);
	grvlst.AddTail(vce);

	const TYPE_DURATION & durel = el->getDuration();

	if (durel == dur)
		hasDurElement = true;

	if (sprcol == 0)
		sprcol = new GRSpringCollider();

	sprcol->AddElement(el,tmppos);
}

void GRSpring::setFunctionParameter(float npar)
{
	funcpar = npar;
}

/** \brief Called when the spring-constant needs to be recalculated.
*/
float GRSpring::recalcConstant()
{
	float oldconst = sconst;
	
//	x  = 0;
//	force = 0;

	GRNotationElement * grn = grolst.GetHead();

	sconst = (isProportionalElement ? 1 : calcconst(grn));

	if (oldconst != sconst && (x != 0 || force != 0)) {
		// then we have to recaluate the
		// force and all that ...
		// what happens with SPF?
		GuidoTrace("Error with springconstants");

		// let x be constant!
		force = x / sconst;
	}

	return sconst;
}

float GRSpring::onlycalcconst(const GRNotationElement * grn)
{
	float mysconst;
	const TYPE_DURATION & dur = grn->getDuration();
	if (grn && (dynamic_cast<const GRGlue *>(grn)) != 0)
	{
		const GRGlue * glue = dynamic_cast<const GRGlue *>(grn);
		mysconst = glue->getSConst();
	}
	else if (grn && (dynamic_cast<const GRTag *>(grn)) != 0)
	{
		const GRTag * tmp = dynamic_cast<const GRTag *>(grn);
		mysconst = tmp->getSConst();
	}
	/*else if (grn && dynamic cast<GRBar *>(grn))
	{
		sconst = 7.0;
	}
	else if (grn && dynamic cast<GRClef *>(grn))
	{
		sconst = 100.0;
	}
	else if (grn && dynamic cast<GRIntens *>(grn))
	{
		sconst = 100.0;
	} */
	else if (dur == DURATION_0)
	{
		if (dynamic_cast<const GREmpty *>(grn))
			mysconst = 200;
		else
			mysconst = 20.0f;
	}
	else if (grn && (dynamic_cast<const GRRest *>(grn)))
	{
		// rests are treated as if they were
		// 8 times shorter -> this results
		// in a nicer spacing when rests occur ...
		// this must be adjusted for optimal output!
		TYPE_DURATION mydur;
		if ((float) dur> float(0.125))
			mydur.set( dur.getNumerator(), dur.getDenominator() * 2);
		else
			mydur = dur;

		mysconst = defconst(mydur);
	}
	else
		mysconst = defconst(dur);

	assert(mysconst != 0);

	return mysconst;
}

float GRSpring::calcconst(GRNotationElement * grn)
{
	GRGlue * glue;
	GRTag * tmp;
	if (grn &&
		(glue = dynamic_cast<GRGlue *>(grn)) != NULL)
		sconst = glue->getSConst();
	else if (grn && 
		(tmp = dynamic_cast<GRTag *>(grn)) != NULL)
		sconst = tmp->getSConst();
	else if (dur == DURATION_0) {
		if (dynamic_cast<GREmpty *>(grn))
            sconst = (isProportionalElement ? 1 : 200.0f);
		else
			sconst = (isProportionalElement ? 1 : 20.0f);
	}
	else if (grn && (dynamic_cast<GRRest *>(grn))) {
		// rests are treated as if they were
		// 8 times shorter -> this results
		// in a nicer spacing when rests occur ...
		// this must be adjusted for optimal output!
		TYPE_DURATION mydur;
		if ((float) dur>0.125)
			mydur.set( dur.getNumerator(), dur.getDenominator() * 2);
		else
			mydur = dur;

        sconst = (isProportionalElement ? 1 : defconst(mydur));
	}
	else
		sconst = (isProportionalElement ? 1 : defconst(dur));

	assert(sconst != 0);

	return sconst;
}

float GRSpring::getFunctionParameter()
{
	return funcpar;
}

bool GRSpring::hasStaffAndType(const GRStaff * grstaff,const std::type_info & ti)
{
	GuidoPos pos = grolst.GetHeadPosition();

	while (pos) {
		GRNotationElement *el = grolst.GetNext(pos);

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
    if (!isProportionalElement) {
        if (force >= newforce || isfrozen)
            return x;

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
		GuidoPos pos = nextspring->grolst.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = nextspring->grolst.GetNext(pos);
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
		GuidoPos pos = grolst.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * el = grolst.GetNext(pos);
			if (el->getRightSpace() > max)
				max = el->getRightSpace();
		}
		return max;
	}

	float maxright = 0;
	float maxleft = 0;
	float max = 0;
	int count = KF_VECTOR_MININDEX;
	if (sprcol && nextspring && nextspring->sprcol)
	{
		poslist * pl1 = NULL;
		poslist * pl2 = NULL;
		bool cont = true;
		do
		{
			maxright = maxleft = 0;
			sprcol->GetMatchingPoslists(count,nextspring->sprcol, &pl1, &pl2);
			if (pl1 && pl2)
			{
				// I have matching poslists ....
				GuidoPos pospl1 = pl1->GetHeadPosition();
				while (pospl1)
				{
					GuidoPos evpos = pl1->GetNext(pospl1);
					GRNotationElement * el = grolst.GetAt(evpos);
					if (el->getRightSpace() > maxright)
						maxright = el->getRightSpace();

				}
				GuidoPos pospl2 = pl2->GetHeadPosition();
				while (pospl2)
				{
					GuidoPos evpos = pl2->GetNext(pospl2);
					GRNotationElement * el = nextspring->grolst.GetAt(evpos);
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

/** \brief Goes through the elements and finds notes and then checkes,
	 wether the accidentals collide.
*/
void GRSpring::checkAccidentalCollisions()
{
	GRAccidentalList * myacclist = 0;
	GuidoPos pos = grolst.GetHeadPosition();
	while (pos)
	{
		GRNotationElement * el = grolst.GetNext(pos);
		GRSingleNote * note = dynamic_cast<GRSingleNote *>(el);
		if (note)
		{
			GRAccidentalList noteacclist;
			note->extractAccidentals( &noteacclist );
			if (myacclist == 0)
				myacclist = new GRAccidentalList(0);
			
			myacclist->DumpListAtTail( &noteacclist );
		}
	}

	if (myacclist && myacclist->GetCount() > 1)
	{
	// now I have the accidentals at this Spring-location ....
	
		myacclist->sort( &compaccposy ); 

		GuidoPos pos = myacclist->GetHeadPosition();
		NVPoint pt;
		GRAccidental * prevacc = 0;
		while (pos)
		{
			GRAccidental * acc = myacclist->GetNext(pos);
			GCoord cury = acc->getPosition().y;
			if (prevacc)
			{
				if (acc->getGRStaff() != prevacc->getGRStaff() 
					|| (cury - prevacc->getPosition().y) > (3 * LSPACE))
					
					pt.x = 0;
			}
			prevacc = acc;
			if (!acc->getOffsetSet())
				acc->addToOffset(pt);
			
			pt.x -= 2 * (acc->getRightSpace());
		}

		// now I have to update bounding boxes of notes...
		pos = grolst.GetHeadPosition();
		while (pos)
		{
			GRSingleNote *sngnot = dynamic_cast<GRSingleNote *>(grolst.GetNext(pos));

			if (sngnot)
				sngnot->updateBoundingBox();
		}
	}
	delete myacclist;
}

int GRSpring::isCommonSpring(int numvoices) const
{
	if (numvoices==1) return 1;

	// a hack!
	if (grolst.GetCount() >= 3) return 1;

	GuidoPos pos = grvlst.GetHeadPosition();
	GRVoice * savevoice = NULL;
	while (pos)
	{
		GRVoice *tmpvce = grvlst.GetNext(pos);
		if (tmpvce == NULL)
			return 1;
		if (savevoice == NULL)
			savevoice = tmpvce;
		else if (tmpvce != savevoice)
			return 1;
	}
	return 0;
//	if (grolst.GetCount() >= 2)  return 1;
}

/** \brief This tries to draw the spring.
*/
void GRSpring::OnDraw( VGDevice & hdc ) const
{
	const VGColor springColor ( 0, 0, 255, 50 ); 		//  semi-transparent blue
	const float springThickness = 5;

	hdc.PushPen( springColor, springThickness );

	const float offsety = -100;

	float x = posx, y = offsety;
	const int numturns = (int)((x-60.0f) / 40.0f + 0.5f );
	if (numturns < 1)
	{
		hdc.Line( x, y, (posx + x), offsety );
	}
	else
	{
		const float offset = ((x - numturns * 40) * 0.5f);
		hdc.Line( x, y, posx + offset, offsety );
		x = posx + offset;

		for( float i = 0; i < numturns; i++ )
		{
			hdc.Line( x, y, posx + offset + 40 * i + 10, -20 + offsety );
			x = posx + offset + 40 * i + 10;
			y = -20 + offsety;
			
			hdc.Line( x, y, posx + offset + 40 * i + 30, 20 + offsety );
			x = posx + offset + 40 * i + 30;
			y = 20 + offsety;

			hdc.Line( x, y, posx + offset + 40 * i + 40, offsety );
			x = posx + offset + 40 * i + 40;
			y = offsety;
		}
		hdc.Line( x, y, (posx + x), 0 + offsety );
	}
	hdc.PopPen();
}
