/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2018 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cstring>

#include "ARArpeggio.h"
#include "ARNote.h"
#include "GRStaff.h"
#include "GRArpeggio.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "GRSingleNote.h"
#include "TagParameterFloat.h"

using namespace std;

extern GRSystem * gCurSystem;

//-------------------------------------------------------------------------
GRArpeggio::GRArpeggio( GRStaff * inStaff, const ARArpeggio * ar )
  : GRPTagARNotationElement(ar), fDx(0), fDy(0)
{
	assert(inStaff);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);

	const TagParameterFloat * dx = ar->getDX();
	const TagParameterFloat * dy = ar->getDY();
	fLSpace = inStaff ? inStaff->getStaffLSPACE() : LSPACE;
	fDx = dx ? dx->getValue (fLSpace) : 0.f;
	fDy = dy ? dy->getValue (fLSpace) : 0.f;
}

//-------------------------------------------------------------------------
void GRArpeggio::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow)
        return;

	assert(gCurSystem);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0)
		return; // don't draw

	 const ARArpeggio * ar = static_cast<const ARArpeggio *>(getAbstractRepresentation());
	 ARArpeggio::dir dir = ar->getDirection();

	// set up color
	if (mColRef) {
		VGColor color (mColRef); 	// custom or black
		hdc.PushFillColor(color);
		hdc.PushPen(color, 1);
		hdc.SetFontColor(color);
	}

	hdc.PushPenWidth(7);
	for (auto r: fPos) {		// for each chord, retrive the chord bounding box
		float x = r.left - (fLSpace * 1.1) + fDx;
		float y = r.top + fLSpace - fDy;
		float bottom = r.bottom + fLSpace - fDy - 5;
		if (dir == ARArpeggio::kUp) DrawUpArrow (hdc, x+17, r.top - fLSpace*0.4 - fDy);
		while (y < bottom) {
			hdc.DrawMusicSymbol (x, y, kArpeggioSymbol);
			y += fLSpace;
		}
		if (dir == ARArpeggio::kDown) DrawDownArrow (hdc, x+13, y - fLSpace*0.7);
	}
	hdc.PopPenWidth();

	if (mColRef) {
		hdc.PopPen();
		hdc.PopFillColor();
		hdc.SetFontColor(VGColor()); //black
	}
}

//-------------------------------------------------------------------------
#define kArrowHeigth	0.6f
#define kArrowWidth		0.5f
#define kArrowXCross	0.1f
void GRArpeggio::DrawUpArrow (VGDevice & hdc, float x, float y) const
{
	float y2 = y + fLSpace * kArrowHeigth;
	float x1 = x - fLSpace * kArrowWidth;
	float x2 = x + fLSpace * kArrowWidth;
	hdc.Line (x+kArrowXCross, y, x1, y2);
	hdc.Line (x-kArrowXCross, y, x2, y2);
}

void GRArpeggio::DrawDownArrow (VGDevice & hdc, float x, float y) const
{
	float y2 = y - fLSpace * kArrowHeigth;
	float x1 = x - fLSpace * kArrowWidth;
	float x2 = x + fLSpace * kArrowWidth;
	hdc.Line (x+kArrowXCross, y, x1, y2);
	hdc.Line (x-kArrowXCross , y, x2, y2);
}

#define kUndefinedOffset 10000
//-------------------------------------------------------------------------
void GRArpeggio::tellPosition(GObject * caller, const NVPoint & np)
{
	if (caller != getAssociations()->GetTail())
		return;

	const NEPointerList * nelist = getAssociations();
	bool inChord = false;
	GuidoPos pos = nelist->GetHeadPosition();
	NVRect rect; float yoffset = kUndefinedOffset;
	while (pos) {
		GRNotationElement * e = nelist->GetNext(pos);
		const ARNote* note = e->getAbstractRepresentation()->isARNote();
		const GRNote * grn = e->isGRNote();

		if (note && note->isEmptyNote()) {
			if (note->getOctave() == 0) {
				inChord = true;
				rect = NVRect ();
			}
			else if (note->getOctave() == 1) {
				inChord = false;
				rect -= NVPoint(0.f, yoffset);
				fPos.push_back (rect);
			}
		}
		else if (grn && inChord) {
			NVRect r = grn->getBoundingBox() + grn->getPosition();
			const GRStaff * staff = e->getGRStaff();
//cerr <<  "GRArpeggio::tellPosition " << staff->getPosition() << " " << e << endl;
			if (staff) {
				float staffy = staff->getPosition().y;
				if (yoffset == kUndefinedOffset) yoffset = staffy;
				r += NVPoint(0.f, staffy);
			}
			rect.Merge (r);
		}
	}
}
