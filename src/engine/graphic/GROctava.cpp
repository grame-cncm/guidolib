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
	fStaff(staff), fText (text), fBassa(bassa), fSegmentsCount(0) //, fElement(0)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	addSystemStartEndStruct (sse);

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
	fDy = ar->getDY()->getValue();
	fOctava = ar->getOctava();
}

GROctava::~GROctava()
{
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
// assumes that there is a single segment and no line break
NVRect GROctava::getExtensionLine (const NEPointerList * assoc) const
{
	NVRect outRect;
	if (!assoc) return outRect;

	float space = fStaff->getStaffLSPACE();
	float staffTop		= 0 - space * 0.5f; // fStaff->getPosition().y - space * 0.5f;
	float staffBottom	= staffTop + fStaff->getDredgeSize() + space * 2.5f;
	GuidoPos pos = assoc->GetHeadPosition();
cerr << "------------- GROctava::getExtensionLine " << endl;
	while (pos) {
		const GRNotationElement* el = assoc->GetNext(pos);
		NVRect bb = getEltBox(el);
cerr << "... " << el << " " << bb << endl;
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
	return outRect;
}

//---------------------------------------------------------------------------------
NVRect GROctava::getExtensionLine (const NEPointerList * assoc, GuidoPos start, GuidoPos end) const
{
	NVRect outRect;
	if (!assoc) return outRect;

	float space = fStaff->getStaffLSPACE();
	float staffTop		= 0 - space * 0.5f; // fStaff->getPosition().y - space * 0.5f;
	float staffBottom	= staffTop + fStaff->getDredgeSize() + space * 2.5f;
	GuidoPos pos = start ? start :  assoc->GetHeadPosition();
//cerr << "------------- GROctava::getExtensionLine " << endl;
	while (pos != end) {
		const GRNotationElement* el = assoc->GetNext(pos);
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
	return outRect;
}

//---------------------------------------------------------------------------------
NVRect GROctava::getExtensionLine (const NEPointerList * assoc, int num) const
{
	NVRect outRect;
	if (!assoc) return outRect;

	float space = fStaff->getStaffLSPACE();
	float staffTop		= 0 - space * 0.5f; // fStaff->getPosition().y - space * 0.5f;
	float staffBottom	= staffTop + fStaff->getDredgeSize() + space * 2.5f;
	GuidoPos pos = assoc->GetHeadPosition();
	float currentXPos = 0;
	while (pos) {
		const GRNotationElement* el = assoc->GetNext(pos);
		NVRect bb = getEltBox(el);
		if (bb.Height()) {
			if (bb.right < currentXPos) {		// there is a line break
				if (!num) {						// and it is the required segment
					outRect.right = /*gCurSystem->getPosition().x +*/ gCurSystem->getBoundingBox().Width();
					return outRect;
				}
				num--;
			}
			if (!num) {					// this is the required segment
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
			currentXPos = bb.left;
		}
	}
	return outRect;
}

//---------------------------------------------------------------------------------
int GROctava::countSegments()
{
	if (fSegmentsCount) return fSegmentsCount;
	float currentXPos = 0;
	int fSegmentsCount = 1;
	if (!getAssociations()) return 0;
	GuidoPos pos = getAssociations()->GetHeadPosition();
	while (pos) {
		const GRNotationElement* el = getAssociations()->GetNext(pos);
		const GRSingleNote * note = dynamic_cast<const GRSingleNote *>(el);
		if (note) {
			float nx = note->getPosition().x;
			if (nx < currentXPos) fSegmentsCount++;
			currentXPos = nx;
		}
	}
	return fSegmentsCount;
}

//---------------------------------------------------------------------------------
void GROctava::showAssoc(const GRSystemStartEndStruct * sse) const
{
	if (!getAssociations()) return 0;
	if (sse->startpos) {
		GRNotationElement* el = getAssociations()->GetAt(sse->startpos);
		if (el) cerr << "GROctava::showAssoc startpos: " << el << " " << el->getPosition().x << endl;
		else cerr << "GROctava::showAssoc startpos is null" << endl;
	}
	else cerr << "GROctava::showAssoc: no startpos" << endl;
	if (sse->endpos) {
		GRNotationElement* el = getAssociations()->GetAt(sse->endpos);
		if (el) cerr << "GROctava::showAssoc endpos: " << el << " " << el->getPosition().x << endl;
		else cerr << "GROctava::showAssoc endpos is null" << endl;
	}
	else cerr << "GROctava::showAssoc: no endpos" << endl;
}

//---------------------------------------------------------------------------------
float GROctava::endPos(const GRSystemStartEndStruct * sse) const
{
	if (!getAssociations()) return 0;
	GRNotationElement* el = sse->endpos ? getAssociations()->GetAt(sse->endpos) : 0;
	return el ? el->getPosition().x : 0;
}

//---------------------------------------------------------------------------------
void GROctava::oldOnDraw( VGDevice & hdc) const
{
	if (fText.empty()) 	return;		// nothing to draw
	if (fHidden) 		return;		// nothing to draw
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);

	const GRSystem * sys = sse->grsystem ? sse->grsystem : gCurSystem;
	
	TSegment seg;
	if (fSegmentsMap.empty()) {		// assume there is a single segment
		// actually this segment might be associated to another GROctava,
		// which draw method is never called
		// this is clearly looking like a bug
		NVRect r = getExtensionLine(getAssociations(), 0, 0);
		seg.x1 = r.left;
		seg.x2 = r.right;
		seg.y = fBassa ? r.bottom : r.top;
	}
	else {
//cerr  << "GROctava::OnDraw map size " << fSegmentsMap.size() << endl;
//cerr  << "GROctava::OnDraw map: ";
//for (auto a: fSegmentsMap) cerr << (void*)a.first << " " << a.second.index << " " << a.second.x1 << " " << a.second.x2 << " " << a.second.x3 << " - " ;
//cerr << endl;

		auto i = fSegmentsMap.find(sys);
		if (i != fSegmentsMap.end())
			seg = i->second;
		else {
			NVRect r = getExtensionLine(getAssociations(), sse->startpos, sse->endpos);
			seg.x1 = r.left;
			seg.x2 = r.right;
			seg.y = fBassa ? r.bottom : r.top;
			seg.index = fSegmentsMap.size() + 1;
		}
	}
	cerr  << (void*)sys << " " << (void*)this << " GROctava::OnDraw " << fOctava << ": " << sse->startflag << "-" << sse->endflag << " [" << seg.x1 << ", " << seg.x2 << "] " << seg.index << endl;
	if (seg.x1) {
		float y = -200;
		float x2 = (seg.index < fSegmentsMap.size()) ? seg.x3 : seg.x2;
		hdc.PushPenWidth(5);
		hdc.Line (seg.x1, y, x2, y);
		hdc.PopPenWidth();
	}

	static int drawingstate = 0;
	int nsegments = const_cast<GROctava*>(this)->countSegments();
	
//	if (!drawingstate) {
//cerr << "GROctava::OnDraw " << fSegments.size() << " segments"  << endl;
//for (auto a: fSegments) cerr << "   " << a.x1 << " - " << a.x2 << endl;
//}
	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT && (nsegments == 1))
		return;		// nothing to do : this is due to the bar before the octava change
	
	bool endSegment =   (sse->endflag == GRSystemStartEndStruct::NOTKNOWN) || (nsegments == 1);
	float space = fStaff->getStaffLSPACE() / 2;

//cerr << "GROctava::OnDraw seg: " << drawingstate << "/" << nsegments << endl;
//if (drawingstate == 1)
//cerr << "GROctava::OnDraw seg 1" << endl;
//	NVRect r = getExtensionLine (getAssociations(), sse);
	NVRect r = getExtensionLine (getAssociations(), drawingstate);
	VGColor savedColor = hdc.GetFontColor();
	VGColor color;	// custom or black
   if (mColRef) {
		color = VGColor(mColRef);
		hdc.SetFontColor(color);
	}
	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
		NVPoint pos (r.left - fStaff->getStaffLSPACE(), r.top - fDy);
		pos.y += fBassa ? -5 : fTextHeight - 5;
		OnDrawText(hdc, pos, fText.c_str(), int(fText.length()) );
	}
	
	// next, draw the extension lines
    if (mColRef) hdc.PushPenColor(color);
	hdc.PushPenWidth(6);
	float xStart = r.left;
	float xEnd = r.right;
	float linespace = drawingstate ? 0 : fStaff->getStaffLSPACE() / 4;
	while (xEnd >= (xStart + linespace)) {
		float y = r.top - fDy;
		if ((xEnd == r.right) && endSegment)		// is it the last extension line ?
			hdc.Line(xEnd, y, xEnd, fBassa ? y - space : y + space);
		hdc.Line(xEnd - space, y, xEnd, y);
		xEnd -= 2.5f * space;
	}
	hdc.PopPenWidth();
    if (mColRef) {
		hdc.PopPenColor();
        hdc.SetFontColor(savedColor);
	}
	if (endSegment) drawingstate = 0;
	else drawingstate++;
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
	NVPoint pos (seg.x1 - fStaff->getStaffLSPACE(), seg.y - fDy);
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
	float space = fStaff->getStaffLSPACE() / 2;
	float xStart = seg.x1;
	float xEnd   = seg.x2;
	float linespace = (seg.index == 1) ? 0 : fStaff->getStaffLSPACE() / 4;
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
//cerr << (void*)sys << " " << (void*)this << " " << fOctava << " GROctava::OnDraw [" << seg.x1 << ", " << seg.x2 << "] " << seg.index << endl;
	drawLine (seg, seg.index >= fSegmentsMap.size(), hdc );
}


void GROctava::tellPosition(GObject * caller, const NVPoint & position)
{
	if (!fOctava) return;		// ignore when no octava

	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if( staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);
	
	const GRSystem* sys = sse->grsystem ? sse->grsystem : gCurSystem;
//cerr << (void*)sys << " " << (void*)this << " " << fOctava << " GROctava::tellPosition " << grel << " " << sse->startflag << "-" << sse->endflag << " pos: " << position << endl;
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
