/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "ARDynamic.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "GRStaff.h"
//#include "TagParameterFloat.h"
//#include "GRStdNoteHead.h"
//#include "GRSingleNote.h"
#include "GRSystem.h"
//#include "GRRest.h"
//#include "GREmpty.h"
#include "MusicalSymbols.h"

#include "GRDynamics.h"

using namespace std;

extern GRSystem *gCurSystem;

map<const string, unsigned int>	GRDynamics::fDynamic2Symbol;

//----------------------------------------------------------------------
GRDynamics::GRDynamics(GRStaff * grstaff, ARDynamic* ar) : GRPTagARNotationElement(ar, false)
{
	assert(grstaff);
	setGRStaff(grstaff);

	if (fDynamic2Symbol.empty()) initDynamicsMap();

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();

	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endflag = GRSystemStartEndStruct::RIGHTMOST;
	mStartEndList.AddTail(sse);
		
	fThickness = 1.f;
	fMarkingSymbol = 0;
}

GRDynamics::~GRDynamics()	{}

//----------------------------------------------------------------------
void GRDynamics::initDynamicsMap()
{
	fDynamic2Symbol["p"]	= kIntensPSymbol;
	fDynamic2Symbol["f"]	= kIntensFSymbol;
	fDynamic2Symbol["ff"]	= kIntensFFSymbol;
	fDynamic2Symbol["fff"]	= kIntensFFFSymbol;
	fDynamic2Symbol["ffff"]	= kIntensFFFFSymbol;
	fDynamic2Symbol["mf"]	= kIntensMFSymbol;
	fDynamic2Symbol["mp"]	= kIntensMPSymbol;
	fDynamic2Symbol["sf"]	= kIntensSFSymbol;
	fDynamic2Symbol["pp"]	= kIntensPPSymbol;
	fDynamic2Symbol["ppp"]	= kIntensPPPSymbol;
	fDynamic2Symbol["pppp"]	= kIntensPPPPSymbol;
}

//---------------------------------------------------------------------------------
void GRDynamics::tellPosition(GObject *caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0) return;
	GRStaff * staff = grel->getGRStaff();
	if (staff == 0) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if (sse == 0) return;
	const ARDynamic *arDyn = dynamic_cast<const ARDynamic *>(getAbstractRepresentation());
	assert(arDyn);

	float dx1    = arDyn->getDx1();
	float dx2    = arDyn->getDx2();
    float dy     = arDyn->getDy();
	bool autopos = arDyn->autoPos();

	if (empty(fCurrentSegment)) 	// this is the first continuation call
		fCurrentSegment.fx1 = newPosition.x + (sse->startflag == GRSystemStartEndStruct::LEFTMOST ? dx1 : 0);
	else {
		fCurrentSegment.fx2 = newPosition.x;
		if (sse->endflag == GRSystemStartEndStruct::RIGHTMOST)	fCurrentSegment.fx2 += dx2 ;
		fCurrentSegment.fx2 = newPosition.x + (sse->endflag == GRSystemStartEndStruct::RIGHTMOST ? dx2 : 0);
		const float staffLSpace = staff->getStaffLSPACE();
		const float y = autopos ? staff->getStaffBottom() + staffLSpace : staff->getDredgeSize() + 2 * staffLSpace;
		fCurrentSegment.fy = y + dy;
		fXPoints[sse->grsystem] = fCurrentSegment;
		clear (fCurrentSegment);
	}
	if ((sse->endflag == GRSystemStartEndStruct::RIGHTMOST) && empty(fCurrentSegment)) {	// this is the last segment
		fWidth = arDyn->getDeltaY();
		fMarkingSymbol = fDynamic2Symbol[arDyn->getDynamicMarking()];
		fThickness = arDyn->getThickness();
#if 0
	cerr << "GRDynamics::tellPosition finalize" << endl;
	for (map<const GRSystem*, TXSegment>::const_iterator i = fXPoints.begin(); i != fXPoints.end(); i++) {
		cerr << "  GRDynamics::tellPosition " << i->second.fx1 << " " << i->second.fx2 << endl;
	}
#endif
	}
}

//---------------------------------------------------------------------------------
void GRDynamics::addAssociation(GRNotationElement * grnot)
{
	if (error) return;

	if ( GREvent::cast(grnot)  && (grnot->isGRNote() || grnot->isRest() || grnot->isEmpty()))
	  	GRARNotationElement::addAssociation(grnot);
	else setError(1);

	if (!error)
		GRPositionTag::addAssociation(grnot);
}


//---------------------------------------------------------------------------------
const GRDynamics::TXSegment* GRDynamics::getSegment(const GRSystem* system) const
{
	map<const GRSystem*, TXSegment>::const_iterator i = fXPoints.find(system);
	return i == fXPoints.end() ? 0 : &i->second;
}


//---------------------------------------------------------------------------------
void GRDynamics::DrawDynamic( VGDevice & hdc, bool cresc) const
{
	if (!mDraw) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0) return;

    const VGColor prevTextColor = hdc.GetFontColor();
    if (mColRef) {
        hdc.PushPenColor(VGColor(mColRef));
        hdc.PushFillColor(VGColor(mColRef));
        hdc.SetFontColor(VGColor(mColRef));
    }

    hdc.PushPenWidth(fThickness);

	const TXSegment* segment = getSegment (sse->grsystem);
	if (segment) {
		float w1 = 0, w2 = 0;
		if (cresc) {
			w2 = fWidth;
			if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) w1 = fWidth/4;
			if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)  w2 = fWidth/1.7;
		}
		else {
			w1 = fWidth;
			if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) w1 = fWidth/1.7 ;
			if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)  w2 = fWidth/4;
		}
		float y1 = segment->fy - w1/2;
		float y2 = segment->fy - w2/2;
		float x2 = segment->fx2;
		if (segment->fx1 == x2)	{
			// this is to catch end staff crescendo on a single note
			// system bounding box is not available at tellPosition time
			x2 = gCurSystem->getBoundingBox().right - LSPACE;
		}
		hdc.Line ( segment->fx1, y1, x2, y2);
		hdc.Line ( segment->fx1, y1+w1, x2, y2+w2);
		if ((sse->endflag == GRSystemStartEndStruct::RIGHTMOST) && fMarkingSymbol) {
			const VGFont* font = hdc.GetMusicFont();
			float fw, fh=0;
			if (font) font->GetExtent( fMarkingSymbol, &fw, &fh, &hdc);
			OnDrawSymbol(hdc, fMarkingSymbol, x2 + 30, segment->fy + 24 * mTagSize, mTagSize);
		}
	}
	
    hdc.PopPenWidth();
    if (mColRef) {
        hdc.SetFontColor(prevTextColor);
        hdc.PopFillColor();
        hdc.PopPenColor();
    }
}

