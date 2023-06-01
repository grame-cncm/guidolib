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
#include <sstream>
#include <algorithm>

#include "GRBeam.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRStaff.h"
#include "GRStem.h"
#include "GRPage.h"
#include "GRMusic.h"
#include "GRSystemTag.h"
#include "GRAutoBeam.h"
#include "GREmpty.h"
#include "GRSystemSlice.h"
#include "GRSimpleBeam.h"
#include "VGDevice.h"
#include "secureio.h"
#include "FontManager.h"
#include "GRTremolo.h"

#include "TagParameterFloat.h"

using namespace std;


GRBeamSaveStruct::~GRBeamSaveStruct()
{
	for (GRSimpleBeam* b: simpleBeams) {
		delete b;
	}
}

pair<float, float> GRBeam::fLastPositionOfBarDuration = make_pair(0.f, 0.f);

GRBeam::GRBeam(GRStaff * grstaf,const ARBeam * arbeam) : GRPTagARNotationElement(arbeam)
{
	fHasRestInMiddle = false;
	fIsGraceBeaming = false;
	fIsFeathered = arbeam->isFeatheredBeam();
	if(fIsFeathered) {
        const ARFeatheredBeam * ar = static_cast<const ARFeatheredBeam *>(arbeam->isARFeatheredBeam());
		fDrawDur = ar->drawDuration();
	}
	else fDrawDur = false;

	fLastPositionOfBarDuration.first = 0;
	fLastPositionOfBarDuration.second = 0;
	setGRStaff(grstaf);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct();
	
	GRBeamSaveStruct * st = new GRBeamSaveStruct();
	sse->grsystem = grstaf->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) st;

	addSystemStartEndStruct (sse);
	fLevel = 0;
}

GRBeam::~GRBeam()
{
	// associations are removed within
	// the GRPositionTag-Removal ...
	FreeAssociatedList();
}

// -----------------------------------------------------------------------------
void GRBeam::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

#define BEAMDBG 0
void GRBeam::OnDraw( VGDevice & hdc) const
{
	if (getError()) return;
	if(!mDraw || !mShow) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if (sse == 0) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	assert(st);

	if (mColRef) {
		VGColor color ( mColRef ); 	// custom or black
		hdc.PushFillColor( color );
		hdc.PushPen( color, 1);
	}
		
	const BeamRect& rect = mainRect();
	float ax [4], ay[4];
	rect.xList(ax);
	rect.yList(ay);

	// This does the drawing!
#if BEAMDBG
	VGColor dbgcolor(255,0,0,100);
	VGColor dbg2color(0,0,0,100);
	VGColor dbg3color(0,0,255,100);
if (fParent) {
	hdc.PushFillColor( dbgcolor );
	hdc.PushPen( dbgcolor, 1);
}
else {
	hdc.PushFillColor( dbg2color );
	hdc.PushPen( dbg2color, 1);
}
	hdc.Polygon(ax, ay, 4);

	hdc.PopPen();
	hdc.PopFillColor();
	
	hdc.PushFillColor( dbg3color );
	hdc.PushPen( dbg3color, 1);
	for (GRSimpleBeam* b: st->simpleBeams) {
		b->OnDraw (hdc);
	}
	hdc.PopPen();
	hdc.PopFillColor();
#else
	hdc.Polygon(ax, ay, 4);
	for (GRSimpleBeam* b: st->simpleBeams) {
		b->OnDraw (hdc);
	}

#endif
	if(fDrawDur)
	{
		const char * fraction = st->duration.c_str();
		size_t n = st->duration.length();

        hdc.PushPenWidth(4);

		if(sse->startflag != GRSystemStartEndStruct::OPENLEFT)
		{	
			hdc.Line(st->DurationLine[0].x, st->DurationLine[0].y, st->DurationLine[1].x, st->DurationLine[1].y);
			hdc.Line(st->DurationLine[1].x, st->DurationLine[1].y, st->DurationLine[2].x, st->DurationLine[2].y);
			hdc.Line(st->DurationLine[3].x, st->DurationLine[3].y, st->DurationLine[4].x, st->DurationLine[4].y);
		}
		else
			hdc.Line(st->DurationLine[1].x, st->DurationLine[1].y, st->DurationLine[4].x, st->DurationLine[4].y);
		if(sse->endflag != GRSystemStartEndStruct::OPENRIGHT)
			hdc.Line(st->DurationLine[4].x, st->DurationLine[4].y, st->DurationLine[5].x, st->DurationLine[5].y);
		
		const VGFont* hmyfont;
		hmyfont = FontManager::gFontText;
		hdc.SetTextFont( hmyfont );

		if (sse->startflag != GRSystemStartEndStruct::OPENLEFT)
			hdc.DrawString(st->DurationLine[2].x + LSPACE/4, st->DurationLine[2].y + LSPACE / 2, fraction, int(n));

        hdc.PopPenWidth();
	}

	if (mColRef)
    {
		hdc.PopPen();
		hdc.PopFillColor();
	}

}

string GRBeam::beamed() const
{
	stringstream list;
	const NEPointerList * p = getAssociations();
	if (p) list << p->reduce_print();
	return list.str();
}


void GRBeam::addAssociation(GRNotationElement * grnot)
{
	if (getError() || !grnot) return ;

	const GRStaff * staff = grnot->getGRStaff();
	if (staff == 0) return;

    GRNote * grnote = dynamic_cast<GRNote*>(grnot);
    bool isGrace = grnote ? grnote->isGraceNote() : false;
	if (isGrace) {
		if (!mAssociated || (mAssociated->size() == 0))
			fIsGraceBeaming = true;		// beam start on a grace note
		if (!fIsGraceBeaming) return;	// otherwise skip grace notes
	}
	else if (fIsGraceBeaming) return;	// skip regular notes when the beaming starts on grace notes
 
	if (grnot->getDuration() == DURATION_0  && !isGrace) {
		const GREvent * grn = grnot->isGREvent();
		if (!grn || grn->getGlobalStem() == NULL)
			return;
	}

	// if we are an autobeam we do not beam rests ....
	const bool isautobeam = isAutoBeam(); // (dynamic cast<GRAutoBeam *>(this) != 0);

	GREvent * grn = GREvent::cast(grnot);
	if (grn == 0) {
		setError(staff,1);
		return ;
	}

	const bool isRest = (grnot->isRest() != 0);
	if (isautobeam && isRest) {
		if (mAssociated && mAssociated->GetCount() > 0)
			fHasRestInMiddle = true;
	}

	if (fHasRestInMiddle && !isRest  && !getARBeam()->isFullBeaming()) {
		// then we have a real Error
		setError(staff,1);
		return;
	}

	// ignore all elements with duration 0 but only, if I already have the stem
	if ((grnot->getDuration() == DURATION_0) && !isGrace)
	{
		if (mAssociated) {
			GRNotationElement * el = mAssociated->GetTail();
			if (el) {
				const GREvent * ev = el->isGREvent();
				if (ev->getGlobalStem() == NULL ||
					ev->getGlobalStem() == grn->getGlobalStem())
					return;
			}
		}
	}

	if (dynamic_cast<const GRSingleRest *>(grn))
		return;

	const ARMusicalObject* o = grn->getAbstractRepresentation();
	bool allowBeam = isautobeam ? false : o && o->isARNote() && ((o->getDuration() == DURATION_2) || o->isEmptyNote());
	if ((grn->getNumFaehnchen() == 0) && !allowBeam) {
		setError(staff,1);
		return ;
	} 

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if (!sse) return ;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (grn->getStemDirSet())
		st->dirset = 1;
	else {
		if (grn->getStemDirection() == dirUP)			st->direction++;
		else if (grn->getStemDirection() == dirDOWN)	st->direction--;
	}
	GRPTagARNotationElement::addAssociation(grnot);
		
	// this is a test ... and cannot be done that way.
	// otherwise we do not get the flags on again ...
	grn->setFlagOnOff(false);
}

void GRBeam::RangeEnd(GRStaff * grstaff)
{
	assert(grstaff);

	GRPositionTag::RangeEnd(grstaff);

	if (getError()) return;
	if (!mAssociated) return;
	
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);

	if (mAssociated && (mAssociated->GetCount() == 1) && isAutoBeam())
	{
		GREvent * ev = GREvent::cast(mAssociated->GetHead());
		if (ev)
		{
			ev->setFlagOnOff(true);
			ev->decBeamCount();
			if (sse->startElement)	// something did not work, if the starElement is a glue ... and not an event
				ev->removeAssociation(this);
			if (sse->endElement)
				ev->removeAssociation(this);
		}
		return;
	}

	GREvent * grn;
	GuidoPos syststpos = sse->startpos;
	if (syststpos)
	{
		// this is all done so that I really get a correct first staff to test my stuff ...
		while (syststpos && !(mAssociated->GetAt(syststpos)))
		{
			mAssociated->GetNext(syststpos);
		}
		int tststaffnum = mAssociated->GetNext(syststpos)->getStaffNumber(); 
		while (syststpos)
		{
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el)
			{
				if (el->getStaffNumber() != tststaffnum)
				{
					setTagType (GRTag::SYSTEMTAG);
					GRSystemTag * mysystag = new GRSystemTag(this);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	GuidoPos pos = sse->startpos;
	GRBeamSaveStruct * st = (GRBeamSaveStruct *) sse->p;
	bool first = true;

	GDirection mytmpdir = dirOFF;
	while (pos)
	{
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
		{
			if (!st->dirset)
			{
				if (first)
				{
					// get the stemdir of the first !? maybe it now has been set?
					mytmpdir = grn->getStemDirection();
				}
				if (mytmpdir != dirOFF)
					grn->setStemDirection(mytmpdir);
				else if (st->direction>=0)
					grn->setStemDirection(dirUP);
				else
					grn->setStemDirection(dirDOWN);
			}
			grn->setFlagOnOff(false);
			first = false;
		}
	}
}

void GRBeam::StaffFinished(GRStaff * grstaff)
{
	assert(false);
	assert(grstaff);

	// first, all the BASIC stuff is handled ...
	GRPositionTag::StaffFinished(grstaff);
	if (getError()) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);

	GuidoPos syststpos = sse->startpos;
	if (syststpos) {
		// this is all done so that I really get a correct first staff to test my stuff ...
		while (syststpos && !(mAssociated->GetAt(syststpos)))
			mAssociated->GetNext(syststpos);

		const GRStaff *tststaff = mAssociated->GetNext(syststpos)->getGRStaff();
		while (syststpos) {
			GRNotationElement * el = mAssociated->GetNext(syststpos);
			if (el) {
				if (el->getGRStaff() != tststaff) {
					setTagType (GRTag::SYSTEMTAG);
					GRSystemTag * mysystag = new GRSystemTag(this);
					el->getGRSystemSlice()->addSystemTag(mysystag);
					break;
				}
			}		
		}
	}

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	if (!checkNotes(grstaff)) return;
//	if (error) return;

	GuidoPos pos = sse->startpos;
	while (pos) {
		GREvent * grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn) {
			if (!st->dirset) {
				if (st->direction > 0)	grn->setStemDirection(dirUP);
				else					grn->setStemDirection(dirDOWN);
			}
			grn->setFlagOnOff(false);
		}
	}
}

void GRBeam::StaffBegin(GRStaff * grstaff)
{
	// now we need to build a new staff 
	assert(grstaff);
	GRPositionTag::StaffBegin(grstaff);
}

//--------------------------------------------------------------------
void GRBeam::initRect (const GREvent * startElt, const GREvent * endElt, const GRBeam* parent, const PosInfos& infos)
{
	const BeamRect& r = parent->mainRect();
	const GREvent* pstart = parent->startElt();
	double slope = double(r.height()) / double(r.width());
	const float c = 1.75f;		// use to reach the border of the note (actually the stem position)
	if (pstart == startElt) {
		fRect.topLeft.x  	= fRect.bottomLeft.x = r.topLeft.x;
		fRect.topLeft.y  	= r.topLeft.y;
		fRect.bottomLeft.y  = r.bottomLeft.y;
	}
	else {
		float offset = ((startElt->getStemDirection() == dirUP) ? infos.currentLSPACE : -infos.currentLSPACE) * infos.currentSize / c;
		fRect.topLeft.x  = fRect.bottomLeft.x  = startElt->getPosition().x + offset;
		float x1 = (pstart) ? fRect.topLeft.x - pstart->getPosition().x : 0;
		fRect.topLeft.y  	= float(slope * x1 + r.topLeft.y);
		fRect.bottomLeft.y  = fRect.topLeft.y + (r.bottomLeft.y - r.topLeft.y);
	}
	if (parent->endElt() == endElt) {
		fRect.topRight.x 	= fRect.bottomRight.x = r.topRight.x;
		fRect.topRight.y  	= r.topRight.y;
		fRect.bottomRight.y = r.bottomRight.y;
	}
	else {
		float offset = ((endElt->getStemDirection() == dirUP) ? infos.currentLSPACE : -infos.currentLSPACE) * infos.currentSize / c;
		fRect.topRight.x = fRect.bottomRight.x = endElt->getPosition().x + offset;
		float x2 = pstart	? fRect.topRight.x - pstart->getPosition().x : r.width();
		fRect.topRight.y  	= float(slope * x2 + r.topLeft.y);
		fRect.bottomRight.y = fRect.topRight.y + (r.bottomRight.y - r.topRight.y);
	}
	if ((getTagType() != SYSTEMTAG) && (parent->getTagType() == SYSTEMTAG)) {
		GRStaff * staff = endElt ? endElt->getGRStaff() : getGRStaff();
		fRect.yOffset(-staff->getPosition().y);
	}
}

//--------------------------------------------------------------------
void GRBeam::initTopLeft (GRSystemStartEndStruct * sse, const GREvent * startEl, PosInfos& infos)
{
	// -- Init point 0 (top left)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const ARBeam * arBeam = getARBeam();
	const GREvent * refEvt = startEl;
	bool setref = (getTagType() == SYSTEMTAG);
	BeamRect& rect = mainRect ();

	if (setref && !infos.stemsReverse && infos.stavesStartEnd && !startEl->getStemLengthSet())
		refEvt = (infos.stemdir == dirUP) ? infos.highNote : infos.lowNote;

	GRStaff * refStaff = refEvt ? refEvt->getGRStaff() : getGRStaff();
	infos.stemdir = refEvt ? refEvt->getStemDirection() : GDirection(st->direction);
	infos.currentSize = refEvt ? refEvt->getSize() : 1;
	infos.currentLSPACE = refStaff->getStaffLSPACE();
	if (startEl) {
		rect.topLeft = startEl->getStemStartPos();
		rect.topLeft.y = refEvt ? refEvt->getStemStartPos().y : refStaff->getPosition().y;
	}
	else rect.topLeft = refEvt ? refEvt->getStemStartPos() : refStaff->getPosition();

	if (arBeam && arBeam->isGuidoSpecBeam() && refEvt)
		rect.topLeft.y = startEl->getPosition().y;

	if (setref) {
		rect.topLeft.x += infos.startStaff.x;
		float yoffset = 0;
		if (infos.stavesStartEnd && !infos.stemsReverse && !startEl->getStemLengthSet()) {
			if (infos.stemdir == dirUP) {
				yoffset = std::min(infos.endStaff.y, infos.startStaff.y);
			}
			else yoffset = std::max(infos.startStaff.y, infos.endStaff.y);
		}
		else yoffset = infos.startStaff.y;
		rect.topLeft.y += yoffset;
	}
	rect.bottomLeft = rect.topLeft;
	
	// -- Adjust point 0
	const TagParameterFloat * p = arBeam->getDx1();
	if (p && p->TagIsSet())
		rect.topLeft.x += p->getValue(infos.currentLSPACE);
	else {
		// This depends on the direction, we do not know this yet (do we?)
		if (infos.oneNote) {
            double result;
            bool conversionOk = TagParameterFloat::convertValue(2.0f, result, "hs", infos.currentLSPACE);

            if (conversionOk)
                rect.topLeft.x -= (float)result * infos.currentSize;
		}
	}

	p = arBeam->getDy1();
	if (p && p->TagIsSet()) rect.topLeft.y -= p->getValue(infos.currentLSPACE);
}

//--------------------------------------------------------------------
void GRBeam::initBottomLeft (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 1
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	BeamRect& rect = mainRect();
	const TagParameterFloat * p = arBeam->getDx2();
	if (p && p->TagIsSet())
		rect.bottomLeft.x += p->getValue(infos.currentLSPACE);
	else {
		if (infos.oneNote) {
            double result;
            if (TagParameterFloat::convertValue(2.0f, result, "hs", infos.currentLSPACE))
                rect.bottomLeft.x -= (float)result * infos.currentSize;
        }
	}

	p = arBeam->getDy2();
	if (p && p->TagIsSet()) rect.bottomLeft.y -= p->getValue(infos.currentLSPACE);
	else {
		double result;
		if (!TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE)) return;
		if (infos.stemdir == dirUP) {
			rect.bottomLeft.y = rect.topLeft.y + (float)result * infos.currentSize;
        }
		else if (infos.stemdir == dirDOWN) {
			rect.bottomLeft.y = rect.topLeft.y - (float)result * infos.currentSize;
        }
	}
}

//--------------------------------------------------------------------
void GRBeam::initTopRight (GRSystemStartEndStruct * sse, const GREvent * endEl, PosInfos& infos)
{
	// -- Init point 2 (top right)
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	BeamRect& rect = mainRect();
	const ARBeam * arBeam = getARBeam();

	rect.topRight = endEl ? endEl->getStemEndPos() : rect.topLeft;
	// beam length adjustment - DF sept 15 2009
	rect.topRight.x += infos.currentLSPACE/10;
	if (arBeam && arBeam->isGuidoSpecBeam() && endEl)
		rect.topRight.y = endEl->getPosition().y;
	GRStaff * refStaff = endEl ? endEl->getGRStaff() : getGRStaff();
	infos.currentLSPACE = refStaff->getStaffLSPACE();

	if (getTagType() == SYSTEMTAG) {
		rect.topRight += refStaff->getPosition();
		if (infos.stavesStartEnd && !infos.stemsReverse &&  !endEl->getStemLengthSet()) {
			rect.topRight.y = rect.topLeft.y;
			float slopeOffset = infos.currentLSPACE/2;
			if (infos.endStaff.y < infos.startStaff.y) rect.topRight.y -= slopeOffset;
			else rect.topRight.y += slopeOffset;
		}
	}

	rect.bottomRight = rect.topRight;

	// -- Adjust point 2
	const TagParameterFloat * p = arBeam->getDx3();
	if (p)					rect.topRight.x += p->getValue(infos.currentLSPACE);

	p = arBeam->getDy3();
	if (p && p->TagIsSet()) rect.topRight.y -= p->getValue(infos.currentLSPACE);
	else {
		GCoord val = 0;
		const TagParameterFloat * p = arBeam->getDy1();
		if (p && p->TagIsSet())	val = p->getValue(infos.currentLSPACE);
		rect.topRight.y -= val;
	}
}

//--------------------------------------------------------------------
void GRBeam::initBottomRight (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	// -- Adjust point 3 (bottom right)
	const ARBeam * arBeam = getARBeam();
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	BeamRect& rect = mainRect();

	const TagParameterFloat * p = arBeam->getDx4();
	if (p)	rect.bottomRight.x += (p->getValue(infos.currentLSPACE));

	p = arBeam->getDy4();
	if (p && p->TagIsSet())	rect.bottomRight.y -= (p->getValue(infos.currentLSPACE));
	else {
		double result;
        if (!TagParameterFloat::convertValue(0.9f, result, "hs", infos.currentLSPACE)) return;
		if (infos.stemdir == dirUP)
			rect.bottomRight.y = rect.topRight.y + ((float)result * infos.currentSize);
		else if (infos.stemdir == dirDOWN)
			rect.bottomRight.y = rect.topRight.y - ((float)result * infos.currentSize);
	}
}

//--------------------------------------------------------------------
void GRBeam::slopeAdjust (GRSystemStartEndStruct * sse, const GREvent * startEl, const GREvent * endEl, float slope, PosInfos& infos)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	BeamRect& rect = mainRect();
	
	if (endEl == 0)
	{
		// then we have an empty-event make the slope even ....
		// or make it dependant on generell direction of beam (how ...)
		rect.topRight.y = rect.topLeft.y;
		rect.bottomRight.y = rect.bottomLeft.y;
	}
	else if (startEl == 0)
	{
		rect.topLeft.y = rect.topRight.y;
		rect.bottomLeft.y = rect.bottomRight.y;
	}

	else if (infos.stemdir != endEl->getStemDirection())
	{
//cerr << "GRBeam::slopeAdjust not implemented now" << endl;
		// then we should try an optimizing strategy which is not implemented now ....
	}
	else if (slope > 0.1f )
	{
		float width = rect.width();
		// adjust the length of the stem for second st->p ...
		if( infos.stemdir == dirDOWN) {
			// change the first one ...
			const float newy = rect.topRight.y - 0.1f * width;
			const float diff = newy - rect.topLeft.y;
			rect.topLeft.y  	= newy;
			rect.bottomLeft.y += diff;
		}
		else {
			// change the second one ...
			const float newy = rect.topLeft.y + 0.1f * width;
			const float diff = newy - rect.topRight.y;
			rect.topRight.y 	= newy;
			rect.bottomRight.y += diff;
		}
	}
	else if (slope < -0.1f)
	{
		float width = rect.width();
		if (infos.stemdir == dirDOWN)
		{
			// change the last one 
			const float newy = rect.topLeft.y - 0.1f * width;
			const float diff =  newy - rect.topRight.y;
			rect.topRight.y 	 = newy;
			rect.bottomRight.y += diff;
		}
		else
		{
			// change the first
			const float newy = rect.topRight.y + 0.1f * width;
			const float diff =  newy - rect.topLeft.y;
			rect.topLeft.y 	= newy;
			rect.bottomLeft.y += diff;
		}
	}
}

//--------------------------------------------------------------------
void GRBeam::refreshBeams (const GRSystemStartEndStruct * sse, float currentLSPACE, int dir)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	BeamRect r = mainRect();
	const float space = getBeamSpace (currentLSPACE) * dir;
	for (GRSimpleBeam* b: st->simpleBeams) {
		b->setPoints(r);
		r.yOffset(space);
	}
}

//--------------------------------------------------------------------
bool GRBeam::checkPartialBeaming (GuidoPos pos, GuidoPos endpos, GREvent*& next, int curFaehnchen)
{
	while (pos) {
		GuidoPos prev = pos;
		GREvent * cur = GREvent::cast(mAssociated->GetAt(pos));
		GREvent * elt = GREvent::cast(mAssociated->GetNext(pos));
		
		if (elt) {
			if (curFaehnchen < elt->getNumFaehnchen()) {
				next = elt;
				return false;
			}
			if (elt->getBeamCount() < elt->getNumFaehnchen()) {
				next = elt;
				next->incBeamCount();
				continue;
			}
			else return true;
		}
		if (prev == endpos) break;
	}
	return false;
}

//--------------------------------------------------------------------
void GRBeam::setLeft (BeamRect& r, const NVPoint& pos, const GRStaff* staff) const
{
	r.topLeft = pos;
	if (staff) r.topLeft += staff->getPosition();
	r.bottomLeft = r.topLeft;
}

//--------------------------------------------------------------------
void GRBeam::setRight (BeamRect& r, const NVPoint& pos, const GRStaff* staff, float xadjust) const
{
	r.topRight = pos;
	r.topRight.x += xadjust;
	if (staff) r.topRight += staff->getPosition();
	r.bottomRight = r.topRight;
}

//--------------------------------------------------------------------
void GRBeam::setBeams (GRSystemStartEndStruct * sse, PosInfos& infos, float beamSpace, float beamSize, int dir)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const float xadjust = infos.currentLSPACE/10; // for beam length adjustment - DF sept 15 2009
	const bool systemBeam = (getTagType() == SYSTEMTAG);
	const BeamRect& rect = mainRect();
	if (fParent) dir = fParent->getStemsDir ();

	GuidoPos pos = sse->startpos;
	int previousCount = 0;
	while (pos) {
		GREvent * stemNote = GREvent::cast(mAssociated->GetNext(pos));
		if (stemNote) {
			GDirection localDir = stemNote->getStemDirection();
			bool dirchange = (localDir != dir);
			float localSpace = beamSpace * localDir * infos.currentSize;
			float localSize = beamSize * localDir * infos.currentSize;

			// check the number of beams ...
			BeamRect r = rect;
			int previousdir = 0;
			while (stemNote->getBeamCount() < stemNote->getNumFaehnchen())
			{
//cerr << "GRBeam::setBeams " << stemNote << " " << stemNote->getBeamCount() << endl;

				int beamCount = stemNote->getBeamCount();
				stemNote->incBeamCount();
				GREvent * endElt = nullptr;
				// look for the endposition and for partial beams
				// partialbeam is set if the new SimpleBeam only covers part of the masterBeam.
				bool partialbeam = checkPartialBeaming(pos, sse->endpos, endElt, stemNote->getNumFaehnchen());
				if (fParent && !beamCount) {	// first beam is actually the main rect
					GRSimpleBeam* sb = new GRSimpleBeam(this,r);
					st->simpleBeams.push_back(new GRSimpleBeam(this,r));
					continue;
				}
				
				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
					// the additional beam starts at the startElement (glue), we have more beams to draw
					setLeft (r, sse->startElement->getPosition(), (systemBeam ? stemNote->getGRStaff() : nullptr));
				}
				else {
					// the additional beam starts at stemNote. We have more beams to draw
					setLeft (r, stemNote->getStemStartPos(), (systemBeam ? stemNote->getGRStaff() : nullptr));
					r.topLeft.y += beamCount * localSpace;
					if (dirchange) r.topLeft.y -= localSize;
					r.bottomLeft.y = r.topLeft.y + localSize;
				}
				
				if (endElt) {
					if (!partialbeam && sse->endflag == GRSystemStartEndStruct::OPENRIGHT) {
						// then the position is different ...
						setRight (r, sse->endElement->getPosition(), (systemBeam ? endElt->getGRStaff() : nullptr), xadjust);
						r.topRight.y = r.topLeft.y;
					}
					else {
						// we have an End-Position ...
						setRight (r, endElt->getStemEndPos(), (systemBeam ? endElt->getGRStaff() : nullptr), xadjust);
						r.topRight.y += beamCount * localSpace;
						// hack to fix incorrect end position with nested beams
						if (fParent && (std::abs(r.topRight.x - rect.topRight.x) < 5))
							r.topRight.x = r.bottomRight.x = rect.topRight.x;
					}
					if (dirchange) r.topRight.y -= localSize;
					r.bottomRight.y = r.topRight.y + localSize;
				}
				else {
					// we do not have an End-Positon single beam ... (meaning a single straight flag)
					// but only, if it is not open on the left or the right hand side.
					const float slope = rect.slope();
					if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
						// then we have to deal with the startposition of the glue-element ....
						// BUT, you can only set this, if the previous beam had this beamcount at the end ....
						// how do I know that? not now.

						// sn is the only element .... and we are open on the left ...
						if (systemBeam) r.topLeft += stemNote->getGRStaff()->getPosition();
						r.bottomLeft = r.topLeft;
						setRight(r, stemNote->getStemEndPos(), (systemBeam ? stemNote->getGRStaff() : nullptr), xadjust);
						r.topRight.y += beamCount * localSpace;
						if (dirchange) r.topRight.y -= localSize;
						r.bottomRight.y = r.topRight.y + localSize;
					}
					else if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT) {						
						setLeft (r, stemNote->getStemEndPos(), (systemBeam ? stemNote->getGRStaff() : nullptr));
						r.topLeft.y += beamCount * localSpace;
						if (dirchange) r.topLeft.y -= localSize;
						r.bottomLeft.y = r.topLeft.y + localSize;

						setRight(r, sse->endElement->getPosition(), nullptr, xadjust);
						r.topRight.y = r.topLeft.y;
						r.bottomRight.y = r.bottomLeft.y;
					}
					else if( partialbeam && ((stemNote == sse->startElement) || ((beamCount > 1) && (previousCount != beamCount) && (previousdir > 0))) ) {
//cerr << "GRBeam::setBeams right " << stemNote << " prevdir "<< previousdir << " bc: " << beamCount << " pc: " << previousCount << endl;
						getRightPartialBeam(r, localSize, infos.currentLSPACE, slope);
					}
					else if( pos == sse->endpos || pos == NULL || ((!stemNote->isSyncopated()) && (pos != sse->startpos))) {
						r = getLeftPartialBeam(stemNote, localSpace, localSize, infos.currentLSPACE * infos.currentSize, slope, dirchange, beamCount);
					}
					else
						getRightPartialBeam(r, localSize, infos.currentLSPACE * infos.currentSize, slope);
				}

				// we now have to "undo" the systemTag-stuff
				if (systemBeam) {
					r -= sse->startElement->getGRStaff()->getPosition();
				}

				if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {	// this is a beam open to the left (due a new line)
					r.topLeft.y = r.topRight.y;
					r.bottomLeft.y = r.bottomRight.y;
				}
				GRSimpleBeam* sb = new GRSimpleBeam(this,r);
				st->simpleBeams.push_back(new GRSimpleBeam(this,r));

				float curx = stemNote->getStemStartPos().x;
				bool toRight = (r.topRight.x - curx) > beamSpace;
				bool toLeft  = (curx - r.topLeft.x) > beamSpace;
				if (toRight && toLeft) previousdir = 0;
				else if (toRight) previousdir = 1;
				else if (toLeft) previousdir = -1;

//cerr << "GRBeam::setBeams previous " << stemNote << " --- "<< r.topLeft.x << " - " << stemNote->getStemStartPos().x << " - " << r.topRight.x << "sp: " << beamSpace << " bc: " << stemNote->getBeamCount() << " dir: " << previousdir << endl;
			}
			previousCount = stemNote->getBeamCount();
		}
	}
}

//--------------------------------------------------------------------
// Partial beams rightward ( using slope)
void GRBeam::getRightPartialBeam (BeamRect& r, float size, float lspace, float slope) const
{
	r.topRight = r.topLeft;
	r.topRight.x += lspace;
	r.topRight.y += slope * lspace;
	r.bottomRight = r.topRight;
	r.bottomRight.y += size;
}

//--------------------------------------------------------------------
// Partial beams leftward ( using slope)
BeamRect GRBeam::getLeftPartialBeam (GREvent* elt, float space, float size, float lspace, float slope, bool dirchange, int num) const
{
	BeamRect r;
	r.topRight = elt->getStemEndPos();
	r.topRight.x += lspace / 10;
	if (getTagType() == SYSTEMTAG)
		r.topRight += elt->getGRStaff()->getPosition();
	
	if (dirchange)
		r.topRight.y -= size;

	r.topRight.y += num * space;
	r.bottomRight = r.topRight;
	r.bottomRight.y += size;

	r.topLeft = r.topRight;
	r.topLeft.x -= lspace;
	r.topLeft.y -= slope * lspace;
	r.bottomLeft = r.topLeft;
	r.bottomLeft.y += size;
	return r;
}

//--------------------------------------------------------------------
// compute the stem length of the beamed notes
void GRBeam::setStemEndPos (GRSystemStartEndStruct * sse, PosInfos& infos, bool needsadjust)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const BeamRect& rect = mainRect();
	const GREvent * startEl = sse->startElement->isGREvent();

	GuidoPos pos = sse->startpos;
	while (pos)
	{
		// now we calculate the stem-end-positions ...
		GuidoPos oldpos = pos;
		GREvent * sn = GREvent::cast(mAssociated->GetNext(pos));
		NVPoint staffPos = sn->getGRStaff()->getPosition();
		if (sn) {
			bool empty = false;
			if (sn->isEmpty()) {
				const GREmpty* gr = static_cast<const GREmpty*>(sn);
				empty = !gr->isInChord();
			}
			float x = sn->getStemStartPos().x - rect.topLeft.x; // the current x position relative to the beam
			if (getTagType() == SYSTEMTAG) x += staffPos.x;

			float y = rect.slope() * x + rect.topLeft.y;	 // the target y position relative to the beam
			float cury = sn->getStemStartPos().y; // + (getTagType() == SYSTEMTAG ? staffPos.y : 0);
			if (sn->getStemDirection() == dirUP)
				cury += sn->getStemLength();
			else if (sn->getStemDirection() == dirDOWN)
				cury -= sn->getStemLength();
			
			float diffy = y - cury;		// the difference between the target and the current y position
//cerr << "GRBeam::setStemEndPos " << sn << " diffy " << diffy << " stem : " << sn->getStemLength() << endl;
			if (getTagType() == SYSTEMTAG) diffy -= staffPos.y;
			
			// if we have a beam between grace notes, we don't want an offbase that whould make the stems too long
			const GRNote * gnote = startEl ? startEl->isGRNote() : 0;
			bool isGrace = gnote ? gnote->isGraceNote() : false;
			
			float offbase = isGrace ? 0 : 3.5f * infos.currentLSPACE;
//			if (fParent) offbase = 0;
			if (diffy < 0)
			{
				if (needsadjust && (sn->getStemDirection() == dirDOWN))
					diffy = -offbase;
				diffy = -diffy;
			}
			else if (needsadjust) {
				if (!empty && (sn->getStemDirection() == dirUP)) {
					diffy = offbase;
				}
			}
			// adjusted - DF sept 15 2009
//			if (fParent)
//				sn->changeStemLength( diffy, true );
//			else
				sn->changeStemLength( diffy - infos.currentLSPACE/20, true );
			// so that the possible next featherd beam knows that he is chained (and dont change its slope)
			sn->setStemChanged();
		}
		if (oldpos == sse->endpos) break;
	}
}

//--------------------------------------------------------------------
// gives the highest or lowest beam at a given x position
static const GRSimpleBeam* findAtPos (const SimpleBeamList& list, float x, bool highest)
{
	const GRSimpleBeam * found = list.back();
	float y = highest ? 100000 : 0;
	for (const GRSimpleBeam* beam : list) {
		if (beam->fRect.includes (x)) {
			float cury = beam->fRect.topLeft.y;
			if (highest) {
				if (cury <= y) {
					y = cury;
					found = beam;
				}
			}
			else if (cury >= y) {
				y = cury;
				found = beam;
			}
		}
	}
	return found;
}

//--------------------------------------------------------------------
// adjust the stem end pos when when sub beams have been computed
// should be called only with stem reverse
void GRBeam::adjustStemEndPos (GRSystemStartEndStruct * sse, PosInfos& infos)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const GREvent * startEl = sse->startElement->isGREvent();
	if (st->simpleBeams.empty() || (st->simpleBeams.size() == 1)) return;

	const BeamRect& rect = mainRect();
	GuidoPos pos = sse->startpos;
	while (pos)
	{
		GuidoPos oldpos = pos;
		GREvent * sn = GREvent::cast(mAssociated->GetNext(pos));
		NVPoint staffPos = sn->getGRStaff()->getPosition();
		if (sn) {
			if (sn->isEmpty()) continue;
			if (sn->getNumFaehnchen() == 1) continue;				// nothing to do : the beam length should be correctly set
			float slope = rect.slope();
			float x = sn->getStemStartPos().x;  					// the stem x position
			if (sn->getStemDirection() == dirUP) {
				const GRSimpleBeam* b = findAtPos (st->simpleBeams, x, true);
				float y1 = slope * x + rect.topLeft.y;
				float y2 = slope * x + b->fRect.topLeft.y;
				if (y2 < y1) {
					sn->changeStemLength( sn->getStemLength() + (y1 - y2), true );
				}
			}
			else {
				const GRSimpleBeam* b = findAtPos (st->simpleBeams, x, false);
				float y1 = slope * x + rect.topLeft.y;
				float y2 = slope * x + b->fRect.topLeft.y;
				if (y2 > y1) {
					sn->changeStemLength( sn->getStemLength() + (y2 - y1), true );
				}
			}
		}
		if (oldpos == sse->endpos) break;
	}
}


//--------------------------------------------------------------------
// computes the offset due to the stem length of notes in the middle might be too short
// then the offset is used to change the main beam position
float GRBeam::getStemsOffset (GRSystemStartEndStruct * sse, PosInfos& infos, bool needsadjust) const
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	const GREvent * startEl = sse->startElement->isGREvent();

	float offsetbeam = 0.f;
	GuidoPos pos = sse->startpos;
	while (pos)
	{
		// now we calculate the stem-end-positions ...
		GuidoPos oldpos = pos;
		GREvent * sn = GREvent::cast(mAssociated->GetNext(pos));
		GRSingleNote* note = dynamic_cast<GRSingleNote*>(sn);
		NVPoint staffPos = sn->getGRStaff()->getPosition();
		const BeamRect& rect = mainRect();
		if (sn) {
			
			bool empty = false;
			if (sn->isEmpty()) {
				const GREmpty* gr = static_cast<const GREmpty*>(sn);
				empty = !gr->isInChord();
			}
			float x = sn->getStemStartPos().x - rect.topLeft.x; // the current x position relative to the beam
			if (getTagType() == SYSTEMTAG) x += staffPos.x;
			float y = rect.slope() * x + rect.topLeft.y;	 // the target y position relative to the beam
			
			float cury = sn->getStemStartPos().y;
			if (sn->getStemDirection() == dirUP)
				cury += sn->getStemLength();
			else if (sn->getStemDirection() == dirDOWN)
				cury -= sn->getStemLength();
			
			float diffy = y - cury;		// the difference between the target and the current y position
			int subBeams = sn->getNumFaehnchen();
			if (getTagType() == SYSTEMTAG) diffy -= staffPos.y;
			const GRNote * gnote = startEl ? startEl->isGRNote() : 0;
			bool isGrace = gnote ? gnote->isGraceNote() : false;
			// if we have a beam between grace notes, we don't want an offbase that whould make the stems too long
			float offbase = (isGrace ? 0.9f * subBeams : 3.5f) * infos.currentLSPACE;
			if (diffy < 0)
			{
				if (needsadjust) {
					if (sn->getStemDirection() == dirDOWN) {
						const float newoffs = diffy - offbase;
						if (newoffs < offsetbeam)
							offsetbeam = (offsetbeam > 0) ? 0 : newoffs;
						diffy = -offbase;
					}
					else if (diffy > -offbase) {
						const float newoffs = diffy + offbase;
						if (newoffs > offsetbeam)
							offsetbeam = (offsetbeam < 0) ? 0 : newoffs;
					}
				}
				diffy = -diffy;
			}
			else if (needsadjust) {
				if (empty) {
					offsetbeam = 0;
				}
				else if (sn->getStemDirection() == dirUP) {
					const float newoffs = diffy + offbase;
					if (newoffs > offsetbeam)
						offsetbeam = (offsetbeam < 0) ? 0 : newoffs;
					diffy = offbase;
				}
				else if (diffy < offbase) {
					const float newoffs = diffy - offbase;
					if (newoffs < offsetbeam)
						offsetbeam = (offsetbeam > 0) ? 0 : newoffs;
				}
			}
		}
		if (oldpos == sse->endpos) break;
	}
	return offsetbeam;
}

//--------------------------------------------------------------------
// if we have a feathered beam, we just have to draw the main beam (already done)
// and the other simple beams will only depend on the begining and ending 
// points, regardless of the durations of the inner notes.
void GRBeam::adjustFeathered (float beamSpace, float beamSize, PosInfos& infos, GRSystemStartEndStruct * sse)
{
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;

	const ARFeatheredBeam * ar = static_cast<const ARFeatheredBeam *>(getARBeam()->isARFeatheredBeam());
	const GREvent * stemNoteBegin = mAssociated->GetHead()->isGREvent();
	
	GDirection localDir = stemNoteBegin->getStemDirection();
	float yLocalFact1 = beamSpace * localDir * infos.currentSize;
	float yLocalFact2 = beamSize * localDir * infos.currentSize;
	
	
	// if the user hasn't set the durations as parameters, 
	// we will take the first and last notes'durations
	int end = ar->getLastBeaming();
	int begin = ar->getFirstBeaming();

	BeamRect r;
	const BeamRect& rect = mainRect();
	for(int i=1; i <= begin; i++) {
		r.topLeft = rect.topLeft;
		r.topLeft.y += (i-1) * yLocalFact1;
		r.bottomLeft.Set (r.topLeft.x, r.topLeft.y + yLocalFact2);
			
		r.topRight = rect.topRight;
		if(end > i || (end == i && i != 1)) 	// no need to draw the main beam again.
			r.topRight.y += (i-1) * yLocalFact1;
		else
			r.topRight.y += (end-1) * yLocalFact1;
		r.bottomRight.Set (r.topRight.x, r.topRight.y + yLocalFact2);
		st->simpleBeams.push_back(new GRSimpleBeam(this,r));
	}
	// if end > begin
	for(int i = begin; i < end; i++)
	{
		r.topLeft = rect.topLeft;
		r.topLeft.y += (begin-1) * yLocalFact1;
		r.bottomLeft.Set (r.topLeft.x, r.topLeft.y + yLocalFact2);

		r.topRight = rect.topRight;
		r.topRight.y += i * yLocalFact1;
		r.bottomRight.Set (r.topRight.x, r.topRight.y + yLocalFact2);
		st->simpleBeams.push_back(new GRSimpleBeam(this,r));
	}
	

	// in order to draw the total duration of the beam
	if(fDrawDur)
	{
		TYPE_TIMEPOSITION begin = ar->getBeginTimePosition();
		TYPE_TIMEPOSITION end = ar->getEndTimePosition();
		TYPE_DURATION dur = end - begin;
		int num = dur.getNumerator();
		int den = dur.getDenominator();
		stringstream out;
		out << num << '/' << den;
		st->duration = out.str();
		size_t n = st->duration.length();
				
		GREvent * ev = dynamic_cast<GREvent *>(mAssociated->GetHead());
		const NVPoint p1 = ev->getStemEndPos();
		float xBegin = ev->getPosition().x;
	
		ev = dynamic_cast<GREvent *>(mAssociated->GetTail());
		const NVPoint p2 = ev->getStemEndPos();
		float xEnd = ev->getPosition().x + ev->getBoundingBox().Width()/2;
	
		int dir = ev->getStemDirection();
		float Y1;
		float Y2;
		if(dir>0)
		{
			Y1 = min(p1.y, p2.y) - LSPACE;
			if(Y1>=fLastPositionOfBarDuration.first && Y1<fLastPositionOfBarDuration.second+LSPACE/2)
				Y1 -= LSPACE;
			Y2 = Y1 - LSPACE/2;
			fLastPositionOfBarDuration.first = Y2;
			fLastPositionOfBarDuration.second = Y1;
		}
		else
		{
			Y1 = max(p1.y, p2.y) + LSPACE;
			if(Y1>=fLastPositionOfBarDuration.first-LSPACE/2 && Y1<fLastPositionOfBarDuration.second)
				Y1 += LSPACE;
			Y2 = Y1 + LSPACE/2;
			fLastPositionOfBarDuration.first = Y1;
			fLastPositionOfBarDuration.second = Y2;
		}
		if (xBegin > xEnd)
		{
			if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
				xEnd = sse->endElement->getPosition().x;
			if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
				xBegin = sse->startElement->getPosition().x;
		}
		float x = xBegin + (xEnd - xBegin) / 2;
		float X1 = x - (n - 0.5f) / 2 * LSPACE;
		float X2 = x + (n - 0.5f) / 2 * LSPACE;

		st->DurationLine[0] = NVPoint(xBegin, Y1);
		st->DurationLine[1] = NVPoint(xBegin, Y2);
		st->DurationLine[2] = NVPoint(X1, Y2);
		st->DurationLine[3] = NVPoint(X2, Y2);
		st->DurationLine[4] = NVPoint(xEnd, Y2);
		st->DurationLine[5] = NVPoint(xEnd, Y1);
	}
}

//--------------------------------------------------------------------
// scan associated elements to detect reverse beaming
bool GRBeam::reverseStems 	(const NEPointerList* assoc) const
{
	GDirection dir = dirOFF;
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GRNotationElement* elt = assoc->GetNext(pos);
		const GREvent* ev = elt->isGREvent();
		if (ev) {
			GDirection nd = ev->getStemDirection();
			if (dir == dirOFF) dir = nd;
			else if ((dir == dirUP) && (nd == dirDOWN)) return true;
			else if ((dir == dirDOWN) && (nd == dirUP)) return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------
// collect highest and lowest staves
void GRBeam::scanStaves (const NEPointerList* assoc, float& highStaff, float& lowStaff) const
{
	float high = 10000.f;
	float low = 0.f;
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GREvent* ev = assoc->GetNext(pos)->isGREvent();
		if (ev) {
			const GRStaff* currentStaff = ev->getGRStaff();
			float ypos = currentStaff->getPosition().y ;
			if (ypos > low) low = ypos;
			if (ypos < high) high = ypos;
		}
	}
	highStaff = high;
	lowStaff = low;
}

//--------------------------------------------------------------------
// retrieve the lowest and highest notes regarding their y position
void GRBeam::yRange (const NEPointerList* assoc, const GREvent*& high, const GREvent*& low) const
{
	const GREvent* ymin  = 0;
	const GREvent* ymax = 0;
	float min = 10000000.f;
	float max = 0;
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GREvent* ev = assoc->GetNext(pos)->isGREvent();
		if (ev) {
			NVPoint p = ev->getPosition() + ev->getGRStaff()->getPosition();
			if (p.y > max) {
				max = p.y;
				ymax = ev;
			}
			if (p.y < min) {
				min = p.y;
				ymin = ev;
			}
		}
	}
	high = ymin;
	low = ymax;
}

//--------------------------------------------------------------------
const GREvent* GRBeam::previousEvent (GREvent* ev) const
{
	const GREvent* previous = nullptr;
	GuidoPos pos = mAssociated->GetHeadPosition();
	while (pos && !previous) {
		GREvent * elt = GREvent::cast (mAssociated->GetNext(pos));
		if (elt == ev) return previous;
		previous = elt;
	}
	return previous;
}

//--------------------------------------------------------------------
// this method has been introduced to trig the position computation in case of
// shared stem with chords spanning different staves
// (the global stem layout is changed after tellPosition has been called for the beam)
void GRBeam::refreshPosition()
{
	GRNotationElement * el = mAssociated->GetTail();
	tellPosition (el, el->getPosition());
	const GRStaff* staff = el->getGRStaff();
	const GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	const GREvent * ev = sse->startElement->isGREvent();
	refreshBeams(sse, staff->getStaffLSPACE(), (ev->getStemDirection() == dirUP) ? 1 : -1);
}

//--------------------------------------------------------------------
void GRBeam::adjustTremolos (GuidoPos pos) {
    while(pos) {
        GREvent * stemNote = GREvent::cast(mAssociated->GetNext(pos));
        if(stemNote) {
            GuidoPos tagpos = NULL;
            if (stemNote->getAssociations())
                tagpos = stemNote->getAssociations()->GetHeadPosition();

            while(tagpos) {
                GRNotationElement * tag = stemNote->getAssociations()->GetNext(tagpos);
                GRTremolo * trem = dynamic_cast<GRTremolo*>(tag);
                if(trem) {
                    trem->tellPosition(stemNote,stemNote->getPosition());
                    if(trem->isTwoNotesTremolo()) // in order to force the second pitch (especially the chords) to update
                    {
                        GREvent * secondPitch = dynamic_cast<GREvent*>(mAssociated->GetNext(pos));
                        if(secondPitch) trem->tellPosition(secondPitch, secondPitch->getPosition());
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------------
// then we have to set the length ... if it is not set otherwise ...
void GRBeam::setUserLengths(GRNotationElement* from, GRNotationElement* to, const ARBeam * arBeam)
{
	GRSingleNote * nt = dynamic_cast<GRSingleNote *>(from);
	if(nt && !nt->getStemLengthSet()) {
		const float myval = arBeam->getDy1()->getValue();
		 nt->setStemLength( myval);
	}

	nt = dynamic_cast<GRSingleNote *>(to);
	if(nt && !nt->getStemLengthSet()) {
		float myval;
		const TagParameterFloat* p = arBeam->getDy3();
		if (p && p->TagIsSet())		myval = p->getValue();
		else						myval = arBeam->getDy1()->getValue();
		nt->setStemLength( myval);
	}
}

//--------------------------------------------------------------------
float GRBeam::slopeAdjust (BeamRect& rect)
{
	float stemWidth = rect.width();
	float slope = rect.slope();
	// another hack to control the slope when events are on different staves - DF sept 15 2009
	float shift = LSPACE/4;
	while (slope < -0.20) {
		rect.tilt (shift);
		slope = rect.slope();
	}
	while (slope > 0.20) {
		rect.tilt (-shift);
		slope =rect.slope();
	}
	return slope;
}

//--------------------------------------------------------------------
float GRBeam::getSlope (const GRSystem * system) const
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(system);
	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	return mainRect().slope();
}

//--------------------------------------------------------------------
GDirection GRBeam::getStemsDir () const  {
	if (fStartElt)		return fStartElt->getStemDirection();
	else if(fEndElt)	return fEndElt->getStemDirection();
	return dirOFF;
}

//--------------------------------------------------------------------
/** \brief Called from the LAST-Note of the Beam ...
*/
// (JB) \bug There is probably a bug somewhere: with map mode
// different than "ISOTROPIC" (proportionnal), beamings polygons between "UP" stems
// draw at an incorrect x position, while "DOWN" beams are always ok.
//
void GRBeam::tellPosition( GObject * gobj, const NVPoint & p_pos)
{
	/* Beams are polygons made of four points:
		
		0                  2		
		 ------------------
		 |                |
		 ------------------
		1                  3
	*/

	// a new test is performed: if it is a systemTag and
	// it is not a systemcall (checkpos), than we can just do nothing.
	if (getError() || !mAssociated || ( mAssociated->GetCount() == 0 ) || ( getTagType() == GRTag::SYSTEMTAG && !mIsSystemCall && !fParent))
		return;
	GRNotationElement * el = dynamic_cast<GRNotationElement *>(gobj);
	if (!el || !el->getGRStaff()) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(el->getGRStaff()->getGRSystem());
	assert(sse);

	if (el != sse->endElement) return;
	if(fLevel != 0) return;

	GRBeamSaveStruct * st = (GRBeamSaveStruct *)sse->p;
	fStartElt = sse->startElement->isGREvent();
	fEndElt	  = sse->endElement->isGREvent();

	GuidoPos startpos = sse->startpos;
	GREvent * stemNote = GREvent::cast(mAssociated->GetNext(startpos));

//cerr << "GRBeam::tellPosition " << el << " START elt " << sse->startElement << " END elt " << sse->endElement << " level: " << fLevel  << " smaller: " << fSmallerBeams.size() << endl; // << " beamed: " << beamed() << endl;

	if (fStartElt && fStartElt->getGlobalStem()) 	fStartElt->getGlobalStem()->setBeam (this);
	if (fEndElt   && fEndElt->getGlobalStem()) 		fEndElt->getGlobalStem()->setBeam (this);
	bool differentStaves = (fStartElt && fEndElt) ? (fEndElt->getGRStaff() != fStartElt->getGRStaff()) : false;

	// this is the staff to which the beam belongs and who draws it.
	PosInfos infos = { dirUP, LSPACE, 1.0f, (fEndElt == fStartElt), reverseStems(mAssociated), differentStaves, 0, 0 };
	if (getTagType() == SYSTEMTAG) {
		infos.startStaff = fStartElt->getGRStaff()->getPosition();
		infos.endStaff	 = fEndElt->getGRStaff()->getPosition();
		scanStaves(mAssociated, infos.highStaff, infos.lowStaff); 	// set the y position of the staves
		yRange(mAssociated, infos.highNote, infos.lowNote);			// retrieve the lowest and highest notes (regarding their y position)
	}

	infos.stemdir = getStemsDir();
	const ARBeam * arBeam = getARBeam();
	const bool isSpecBeam = arBeam->isGuidoSpecBeam();
	bool needsadjust = true;
	// we have to adjust the slope ONLY if the stemlength of the first and last element has not been set automatically!
	// and if we are note in the case of a chained feather beam
	if ( (fStartElt && fStartElt->getStemLengthSet() && fEndElt && fEndElt->getStemLengthSet())
			|| (arBeam && isSpecBeam) || (fIsFeathered && fStartElt && fStartElt->stemHasBeenChanged()))
			needsadjust = false;
	BeamRect& rect = mainRect();
	if (!fParent) {
		initTopLeft 	(sse, fStartElt, infos);
		initBottomLeft 	(sse, infos);
		initTopRight 	(sse, fEndElt, infos);
		initBottomRight (sse, infos);

		// in case of system tag, check and minize the current slope
		float slope = (getTagType() == SYSTEMTAG) ? slopeAdjust(rect) : rect.slope();
		if (needsadjust) slopeAdjust (sse, fStartElt, fEndElt, slope, infos);
	}
	// in case of nested beams, the parent beam is used as e reference rect
	else initRect(fStartElt, fEndElt, topParent(fParent), infos);

	if (arBeam && isSpecBeam)
		setUserLengths (sse->startElement, sse->endElement, arBeam);
	
	float offsetbeam = getStemsOffset(sse, infos, needsadjust); 	// computes a possible offset due to inner notes
	if (offsetbeam ) rect.yOffset(-offsetbeam);
	// and finally adjust the stems length - skip nested beams since that's already done by the parent beam
	if (!fParent) setStemEndPos(sse, infos, needsadjust);

	if(!fSmallerBeams.empty()) {
		bool feathered = fIsFeathered;
		for(GRBeam* sb: fSmallerBeams) {
			sb->decLevel();
			sb->tellPosition(sb->getEndElement(), sb->getEndElement()->getPosition());
//			sb->tellPosition(fEndElt, fEndElt->getPosition());
			sb->incLevel();
			if (sb->fIsFeathered) feathered = true;
		}
		if (feathered) return; // || !mIsSystemCall) return;
	}

	// -- Now we need to add the simplebeams as simplebeamgroups ...
	int dir = st->direction;
	if (st->dirset) {
		// then the direction was set explicitly by the user ...
		const GREvent * sn = mAssociated->GetHead()->isGREvent();
		if (sn) dir = sn->getStemDirection();
	}
	else dir = infos.stemdir;

	if (getTagType() == SYSTEMTAG) {
		rect -= sse->startElement->getGRStaff()->getPosition();
	}
	// - These constants define the space and the size of additionnal beams.
	const float beamSpace = getBeamSpace (infos.currentLSPACE);
	const float beamSize  = getBeamSize (infos.currentLSPACE);

	// if we have a feathered beam, we just have to draw the main beam (already done)
	// and the other simple beams will only depend on the begining and ending
	// points, regardless of the durations of the inner notes.
	if (fIsFeathered) adjustFeathered (beamSpace, beamSize, infos, sse);
	else setBeams(sse, infos, beamSpace, beamSize, dir);	// sets the additionals beams
	if (infos.stemsReverse) adjustStemEndPos (sse, infos);

	adjustTremolos(sse->startpos);
}


void GRBeam::setError(const GRStaff * grstaff, int p_error)
{
	GRTag::setError(p_error);
	if (getError() && mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		while (pos) {
			GREvent * grnot = GREvent::cast (mAssociated->GetNext(pos));
			if (grnot)
				grnot->setFlagOnOff(1);
		}
	}

	// this just removes the associations from all active SSE's. The SSE's themselves
	// stay, where they are, so that they are delete properly later ....
	DeleteAllSSEs();

	delete mAssociated;
	mAssociated = NULL;
}

bool GRBeam::checkNotes( GRStaff * grstaff )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grstaff->getGRSystem());
	if (!sse) return false;

	if (mAssociated) {
		GuidoPos pos = sse->startpos;
		bool last = false;
		while (pos) {
			const GREvent * grnot = mAssociated->GetNext(pos)->isGREvent();
			if (!grnot)  {
				setError( grstaff, 1 );
				return false;
			}

			if (last) break;
			if (pos == sse->endpos) last = true;
		}
	}
	return true;
}

/** \brief Called after the new Staff has been created by the Staffmanager after a pbreak-Break.

	(It is ALWAYS called after a call to BreakTag)
	It is somewhat analogous to StaffBegin but uses the assocpos ...
*/
void GRBeam::ResumeTag(GRStaff *grstaff,GuidoPos assocpos)
{
	if (!grstaff) return;
	GRPositionTag::ResumeTag(grstaff,assocpos);
}

/** \brief Break a tag at a previously saved PBreak-Location.

	It sets the sse-Positions ...
	It is somewhat equivalent to StaffFinished with the difference, that assocpos is
	used). assocpos is the tail-position of the mAssociated list at the time of the 
	pbreak situation.
*/
void GRBeam::BreakTag( GRStaff * grstaff, GuidoPos & assocpos)
{
	if (!grstaff) return;

	// do the "basic" stuff.
	GRPositionTag::BreakTag(grstaff,assocpos);

	if (getError()) return;

	GRSystemStartEndStruct *sse = getSystemStartEndStruct(grstaff->getGRSystem());
	assert(sse);
	GREvent * grn;

	if (!checkNotes(grstaff)) return;
//	if (error) return;

	GuidoPos pos = sse->startpos;
	while (pos)
	{
		GuidoPos prevpos = pos;
		grn = GREvent::cast(mAssociated->GetNext(pos));
		if (grn)
			grn->setFlagOnOff(0);

		if (prevpos == sse->endpos)
			break;
	}
}

void GRBeam::checkPosition( const GRSystem * grsys)
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(grsys);
	assert(sse);

	mIsSystemCall = true;
	tellPosition( sse->endElement, sse->endElement->getPosition());
	mIsSystemCall = false;
}

GRNotationElement * GRBeam::getEndElement()
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	return sse->endElement;
}

void GRBeam::addSmallerBeam(GRBeam * beam)
{
	beam->setLevel(fLevel+1);
	fSmallerBeams.push_back(beam);
}
