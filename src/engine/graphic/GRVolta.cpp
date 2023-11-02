/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#ifdef VOLTADEBUG
#include <fstream>
ofstream vdebug("voltadebug.txt");
#endif

#include "ARMusicalObject.h"
#include "ARMusicalTag.h"
#include "ARVolta.h"
#include "GRBar.h"
#include "GRDefine.h"
#include "GREvent.h"
#include "GRGlue.h"
#include "GRNote.h"
#include "GRRest.h"
#include "GRStem.h"
#include "GRFlag.h"
#include "GRRepeatBegin.h"
#include "GRSingleNote.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRVolta.h"
#include "TagParameterFloat.h"

#include "kf_ivect.h"
#include "VGDevice.h"
#include "FontManager.h"

using namespace std;


NVPoint GRVolta::refpos;

GRVolta::GRVolta( GRStaff * inStaff, const ARVolta * ar )
		: GRPTagARNotationElement( ar )
{
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	sse->p = (void *) getNewGRSaveStruct();
	addSystemStartEndStruct (sse);
    mEnd = mBeg = 0;
    mShape = kDefault;

    if (ar->getFormat()) {
        if (!strcmp(ar->getFormat(), "|-"))
            mShape = kRightOpened;
        else if (!strcmp(ar->getFormat(), "-|"))
            mShape = kLeftOpened;
        else if (!strcmp(ar->getFormat(), "-"))
            mShape = kOpened;
    }
    mString = ar->getMark();
    mStringSize = (int)strlen(mString);
}

GRVolta::~GRVolta()
{
	FreeAssociatedList();
}

// -----------------------------------------------------------------------------
/*!
    This method makes final adjustments to the volta bounding boxes.
	This final adjustement occurs here because start and end elements
	do not carry graphic properties when tellPosition is called
*/
void GRVolta::FinishPTag (GRStaff * staff)
{
    if (!staff) return;

	if (mBeg) adjustLeft (mBeg);
	if (mEnd) {
		adjustRight(mEnd);
		GRVolta* next = getNextVolta(mEnd->getGRStaff());
		if (next) adjustToNext (next);
	}
	float sy = staff->getPosition().y;
}

// -----------------------------------------------------------------------------
void GRVolta::adjustToNext (GRVolta * next)
{
	size_t n = fSegments.size();
	if (n && next->getSegmentsSize()) {
		NVRect first = next->getFirstSegment();
		NVRect last = fSegments[n-1];
		last.right -= LSPACE*0.7;
		if (last.top > first.top) {
			last.top = first.top;
			last.bottom = first.bottom;
		}
		else if (last.top < first.top) {
			first.top = last.top;
			first.bottom = last.bottom;
			next->setFirstSegment (first);
		}
		fSegments[n-1] = last;
	}
}

// -----------------------------------------------------------------------------
void GRVolta::adjustLeft (const GRNotationElement * startElt)
{
	NVRect r = startElt->getBoundingBox() + startElt->getPosition();
	NVRect seg = fSegments[0];
	float offset = startElt->isSingleNote()	? 0 : LSPACE/3;
	seg.left = r.left; // + offset; // + dx;
	fSegments[0] = seg;
	mBoundingBox = seg;
}

// -----------------------------------------------------------------------------
void GRVolta::adjustRight(const GRNotationElement * endElt)
{
    NVRect r = endElt->getBoundingBox();
    r += endElt->getPosition();

    NVRect tmp = mBoundingBox;
    tmp += mPosition;

    mBoundingBox.right += r.left - tmp.right;
    mBoundingBox.right -= LSPACE / 4;

	size_t n = fSegments.size();
	if (n) {
		NVRect seg = fSegments[n-1];
		seg.right = r.left; // - LSPACE / 3;
		fSegments[n-1] = seg;
	}
}

// -----------------------------------------------------------------------------
void GRVolta::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grne = dynamic_cast<GRNotationElement *>(caller);
	if (grne == 0 ) return;
    NEPointerList * assoc = getAssociations();
	if (assoc == 0 ) return;
	GRStaff * staff = grne->getGRStaff();
	if (staff == 0 ) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if (sse == 0 ) return;

	float lspace = staff->getStaffLSPACE();
	if (fStartTell) {
		fCurrentSegment.left = grne->getBoundingBox().left + grne->getPosition().x;
		fCurrentSegment.top = lspace * -3;
		fCurrentSegment.bottom = fCurrentSegment.top + lspace * 1.5f;
		if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) mBeg = getBegElt(sse->startElement);
	}
	else {
		const GRNotationElement* start = mAssociated->GetAt(sse->startpos);
		const GRNotationElement* end = mAssociated->GetAt(sse->endpos);
		GuidoPos pos = sse->startpos;
		while (pos) {			// adjust the y position according to the enclosed notes
			GRNotationElement * elt = assoc->GetNext(pos);
			const GRSingleNote* note = elt->isSingleNote();
			NVRect r = note ? note->getEnclosingBox() : (elt->getBoundingBox() + elt->getPosition());
			if (r.top <= fCurrentSegment.top)
				fCurrentSegment.top = r.top - lspace;
			if (pos == sse->endpos) break;
		}
		fCurrentSegment.right = grne->getPosition().x + grne->getBoundingBox().Width();
		fCurrentSegment.bottom = fCurrentSegment.top + lspace * 1.5f;
		fSegments.push_back (fCurrentSegment);
		
		if (sse->endflag == GRSystemStartEndStruct::RIGHTMOST) mEnd = getEndElt(sse->endElement);
	}
	fStartTell = !fStartTell;
}

// -----------------------------------------------------------------------------
GRVolta * GRVolta::getNextVolta(GRStaff * staff) const
{
    if (!staff) return 0;
    NEPointerList * staffEvs = staff->getElements();
    if (!staffEvs) return 0;

    GuidoPos pos = staffEvs->GetElementPos(mEnd);
    while (pos) {
        GRNotationElement * elt = staffEvs->GetNext(pos);
        if (elt == this) continue;
        if (dynamic_cast<GRVolta *>(elt)) return dynamic_cast<GRVolta *>(elt);
        if (dynamic_cast<GRNote *>(elt) || dynamic_cast<GRRest *>(elt))
            return 0;   // range beg is not at the beginning of the measure: do nothing
    }

    // did not find anything on first staff, try on the next one
    staff = staff->getNextStaff();
    if (!staff) return 0;
    staffEvs = staff->getElements();
    if (!staffEvs) return 0;

    pos = staffEvs->GetHeadPosition();
    while (pos) {
        GRNotationElement * elt = staffEvs->GetNext(pos);
        if (dynamic_cast<GRVolta *>(elt)) return dynamic_cast<GRVolta *>(elt);
        if (dynamic_cast<GRNote *>(elt) || dynamic_cast<GRRest *>(elt))
            return 0;   // range beg is not at the beginning of the measure: do nothing
    }
	return 0;
}

// -----------------------------------------------------------------------------
GRNotationElement * GRVolta::getBegElt(GRNotationElement *before) {
    if (!before) return 0;

    GRStaff * staff = before->getGRStaff();
    if (!staff) return 0;
    NEPointerList * staffEvs = staff->getElements();
    if (!staffEvs) return 0;

    // computes now the end range next element
    GuidoPos pos = staffEvs->GetElementPos(before);
    while (pos) {
        GRNotationElement * elt = staffEvs->GetPrev(pos);
        if (elt == before) continue;
        if (dynamic_cast<GRBar *>(elt)) return elt;
        if (dynamic_cast<GRNote *>(elt) || dynamic_cast<GRRest *>(elt))
            return 0;   // range beg is not at the beginning of the measure: do nothing
    }

    // here we'll look for the previous staff (measure)
    staff = staff->getPreviousStaff();
    return staff ? staff->getElements()->GetTail() : 0;        // and returns the last staff element
}

// -----------------------------------------------------------------------------
GRNotationElement * GRVolta::getEndElt(GRNotationElement *after) {
    if (!after) return 0;

    GRStaff * staff = after->getGRStaff();
    if (!staff) return 0;
    NEPointerList * staffEvs = staff->getElements();
    if (!staffEvs) return 0;

    // computes now the end range next element
	TYPE_TIMEPOSITION date = after->getRelativeTimePosition();
    GuidoPos pos = staffEvs->GetElementPos(after);
    while (pos) {
        GRNotationElement * elt = staffEvs->GetNext(pos);
        if (elt == after) continue;
        if (elt->getRelativeTimePosition() < date) continue;
        if (dynamic_cast<GRBar *>(elt) || dynamic_cast<GRRepeatBegin *>(elt) || dynamic_cast<GRNote *>(elt) || dynamic_cast<GRRest *>(elt)) {
            return elt;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
void GRVolta::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow || fSegments.empty())
		return;

	static size_t index = 0;
	const ARVolta * ar = getARVolta();
	float dx = (index == 0) ? ar->getDX()->getValue() : 0;
	float dy = ar->getDY()->getValue();
	NVRect seg = fSegments[index++];

    hdc.PushPenWidth(4.0f);
	if (mColRef) hdc.PushPenColor(VGColor(mColRef));
	hdc.Line (seg.left+dx, seg.top+dy, seg.right, seg.top+dy);
	drawEndings (hdc, index-1, dx, dy);

	if (index == 1)		// first (and maybe last) segment
		drawText (hdc, seg.left+dx + LSPACE/2, seg.bottom+dy);
	if (index == fSegments.size())
		index = 0;

	if (mColRef) hdc.PopPenColor();
    hdc.PopPenWidth();
//    DrawBoundingBox(hdc, VGColor(0,0,0));
}

// -----------------------------------------------------------------------------
void GRVolta::drawEndings (VGDevice & hdc, size_t index, float dx, float dy) const
{
	size_t n = fSegments.size()-1;
	NVRect r = fSegments[index];
	r.left	+= dx;
	r.top 	+= dy;
	r.bottom+= dy;
    switch (mShape) {
        case kDefault:
            if (!index) hdc.Line (r.left, r.top, r.left, r.bottom);
            if (index==n) hdc.Line (r.right, r.top, r.right, r.bottom);
            break;
        case kLeftOpened:
            if (index==n) hdc.Line (r.right, r.top, r.right, r.bottom);
            break;
        case kRightOpened:
            if (!index) hdc.Line (r.left, r.top, r.left, r.bottom);
            break;
    }
}

// -----------------------------------------------------------------------------
void GRVolta::drawText ( VGDevice & hdc, float x, float y ) const
{
	const VGColor prevTextColor = hdc.GetFontColor();
	hdc.SetTextFont(FontManager::gFontText);

	if (mColRef)
		hdc.SetFontColor(VGColor(mColRef));

	hdc.SetFontAlign (VGDevice::kAlignBaseLeft);
	hdc.DrawString(x, y, mString, mStringSize);

	if (mColRef)
		hdc.SetFontColor(prevTextColor);
	
}

