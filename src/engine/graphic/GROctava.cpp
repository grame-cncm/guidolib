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

#include "AROctava.h"
#include "ARNote.h"

#include "GROctava.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRDefine.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"	// for gFontDefs.gFontText
#include "VGDevice.h"
#include "VGFont.h"
#include "GRSpecial.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "TagParameterFloat.h"

using namespace std;

extern GRSystem * gCurSystem;

GROctava::GROctava( GRStaff * staff, const NVstring & text, const AROctava* ar, bool bassa)
	: GRARNotationElement(ar),
	fStaff(staff), fText (text), fBassa(bassa)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	addSystemStartEndStruct (sse);
	setGRStaff(staff);
	
	mNeedsSpring = 1;
	float x = 0;
	float y = 0;
	if( gGlobalSettings.gDevice )
		FontManager::gFontText->GetExtent( fText.c_str(), (int)fText.length(), &x, &y, gGlobalSettings.gDevice );

	mBoundingBox.right = (GCoord)(x);
	mBoundingBox.top = (GCoord)(y - staff->getStaffLSPACE());
	mBoundingBox.left -= (GCoord)(LSPACE/4);
	mBoundingBox.bottom = (GCoord)(4*LSPACE);
	fTextHeight = y; // - 10;
	fHidden = ar->getHidden();
	fDx = ar->getDX()->getValue();
	fDy = ar->getDY()->getValue();
	fOctava = ar->getOctava();
}

GROctava::~GROctava()
{
}

void GROctava::accept (GRVisitor& visitor) {
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

bool GROctava::DeleteStaff(GRStaff * grstaff)
{
	delete mAssociated;		// makes sure that associated notes are deleted before the GROctava
	mAssociated = 0;
	return GRPositionTag::DeleteStaff(grstaff);
}

void GROctava::setColRef(const TagParameterString *tps) {
    if (!mColRef)
        mColRef = new unsigned char[4];

    tps->getRGB(mColRef);
}

//---------------------------------------------------------------------------------
NVRect GROctava::getEltBox (const GRNotationElement* el) const
{
	NVRect outRect;
	const GRSingleNote * note = dynamic_cast<const GRSingleNote *>(el);
	if (note)
		return note->getEnclosingBox();

	const GRSingleRest * rest = dynamic_cast<const GRSingleRest *>(el);
	if (rest) {
		outRect = rest->getBoundingBox();
		outRect += rest->getPosition();
	}
	return outRect;
}

//---------------------------------------------------------------------------------
NVRect GROctava::getExtensionLine (const NEPointerList * assoc, GuidoPos start, GuidoPos end) const
{
	NVRect outRect;
	if (!assoc) return outRect;

	float space = fStaff->getStaffLSPACE();
	float staffTop		= space * -0.5f;
	float staffBottom	= staffTop + fStaff->getDredgeSize() + space * 2.5f;
	GuidoPos pos = start ? start :  assoc->GetHeadPosition();
	const GRNotationElement* first = nullptr;
//cerr << "------------- GROctava::getExtensionLine " << endl;
	while (pos != end) {
		const GRNotationElement* el = assoc->GetNext(pos);
		if (!first) first = el;
		NVRect bb = getEltBox(el);
//cerr << "... " << el << " " << bb << endl;
		if (bb.Height()) {
			if (!outRect.left)		// set the left border
				outRect.left = bb.left + space;
			outRect.right = bb.right + space /3;	// and adjust the right border
			if (fBassa) {
				float y = bb.bottom + space * 1.7f;
				float bottom = (y > staffBottom) ? y : staffBottom;
				if (bottom > outRect.bottom) outRect.top = outRect.bottom = bottom;
			}
			else {
				float y = bb.top - space * 0.2f;
				float top = (y < staffTop) ? y : staffTop;
				if (top < outRect.top) outRect.top = outRect.bottom = top - fTextHeight;
			}
		}
	}
	const GRStaff* firstStaff = first->getGRStaff();
	if (getGRStaff()->getStaffNumber() != firstStaff->getStaffNumber()) {
		outRect.top += firstStaff->getPosition().y;
		outRect.bottom += firstStaff->getPosition().y;
	}
	return outRect;
}

//---------------------------------------------------------------------------------
GROctava::TSegment GROctava::nvrect2Segment(const NVRect& r) const
{
	TSegment seg;
	seg.x1 = r.left;
	seg.x2 = r.right;
	seg.y = fBassa ? r.bottom : r.top;
	return seg;
}

//---------------------------------------------------------------------------------
void GROctava::drawText (const TSegment& seg, VGDevice & hdc ) const
{
	VGColor savedColor = hdc.GetFontColor();
   if (mColRef) {
		VGColor color = VGColor(mColRef);
		hdc.SetFontColor(color);
	}
	NVPoint pos (seg.x1 - fStaff->getStaffLSPACE() + fDx, seg.y - fDy);
	pos.y += fBassa ? -5 : fTextHeight - 5;
	OnDrawText(hdc, pos, fText.c_str(), int(fText.length()) );
    if (mColRef) {
        hdc.SetFontColor(savedColor);
	}
}

//---------------------------------------------------------------------------------
void GROctava::drawLine (const TSegment& seg, bool last, VGDevice & hdc ) const
{
    if (mColRef) {
		VGColor color = VGColor(mColRef);
		hdc.PushPenColor(color);
	}
	hdc.PushPenWidth(6);
	bool first = (seg.index == 1);
	float space = fStaff->getStaffLSPACE() / 2;
	float xStart = seg.x1 + (first ? fDx : 0);
	float xEnd   = seg.x2;
	float linespace = first ? 0 : fStaff->getStaffLSPACE() / 4;
	while (xEnd >= (xStart + linespace)) {
		float y = seg.y - fDy;
		if ((xEnd == seg.x2) && last)		// is it the last extension line ?
			hdc.Line(xEnd, y, xEnd, fBassa ? y - space : y + space);
		hdc.Line(xEnd - space, y, xEnd, y);
		xEnd -= 2.5f * space;
	}

	hdc.PopPenWidth();
    if (mColRef) hdc.PopPenColor();
}

//---------------------------------------------------------------------------------
void GROctava::OnDraw( VGDevice & hdc) const
{
	if (fText.empty()) 	return;		// nothing to draw
	if (fHidden) 		return;		// nothing to draw
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	const GRStaff* staff = getGRStaff();

	const GRSystem * sys = sse->grsystem ? sse->grsystem : gCurSystem;
	TSegment seg;
	if (fSegmentsMap.empty()) {		// assume there is a single segment
		// actually this segment might be associated to another GROctava,
		// which draw method is never called
		// this is clearly looking like a bug
		NVRect r = getExtensionLine(getAssociations(), 0, 0);
		seg = nvrect2Segment(r);
		seg.index = 1;
	}
	else {
		NVRect r = getExtensionLine(getAssociations(), sse->startpos, sse->endpos);
		if (staff->getStaffNumber() != gCurStaff->getStaffNumber()) {
			float offset = gCurStaff->getPosition().y - staff->getPosition().y;
			if (staff->getStaffNumber() > gCurStaff->getStaffNumber()) {
				r.top 	-= offset;
				r.bottom -= offset;
			}
			else if (staff->getStaffNumber() < gCurStaff->getStaffNumber()) {
				r.top 	+= offset;
				r.bottom += offset;
			}
		}

		auto i = fSegmentsMap.find(sys);
		if (i != fSegmentsMap.end()) {
			seg = i->second;
			seg.y = fBassa ? r.bottom : r.top;
		}
		else {
			// the last segment might be associated to another GROctava
			// then
			seg = nvrect2Segment(r);
			seg.index = fSegmentsMap.size();
		}
		// when the segment extends to the end of the staff
		// then use the position provided by tellPosition
		if (seg.index < fSegmentsMap.size())
			seg.x2 = seg.x3;
		if (!seg.x2) // might occur with the position form without \oct<0> (implicit extension to end)
			seg.x2 = seg.x3 - LSPACE;
	}

	if (seg.index == 1)
		drawText (seg, hdc);
	drawLine (seg, seg.index >= fSegmentsMap.size(), hdc );
}


void GROctava::tellPosition(GObject * caller, const NVPoint & position)
{
	if (!fOctava) return;		// ignore when no octava

	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( !grel ) return;

	GRStaff * staff = grel->getGRStaff();
	if( !staff ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);
	
	const GRSystem* sys = sse->grsystem ? sse->grsystem : gCurSystem;
//cerr << (void*)sys << " " << (void*)this << " " << fOctava << " GROctava::tellPosition " << grel << " pos.y: " << position.y << " on staff " << staff->getStaffNumber()  << endl;
	TSegment segment = fSegmentsMap[sys];
	if (!segment.x1) {
		segment.x1 = position.x;
		segment.index = fSegmentsMap.size();
	}
	else {
		segment.x3 = position.x;
	}
	fSegmentsMap[sys] = segment;
}
