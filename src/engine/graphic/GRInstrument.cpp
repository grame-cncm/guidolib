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

#include <string>

#include "GRInstrument.h"
#include "GRDefine.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "ARMusicalObject.h"
#include "ARInstrument.h"
#include "FontManager.h"
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

extern GRSystem * gCurSystem;

GRInstrument::GRInstrument(const ARInstrument * ar, GRStaff* staff)
		: GRTagARNotationElement(ar, LSPACE)
{
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;
	mNeedsSpring = 1;

	// to do: read real text ...
	const GCoord extent = 100;

	mBoundingBox.right = extent;
	mBoundingBox.bottom = (GCoord)(3*LSPACE);

	// no Space needed !
	mLeftSpace = 0;
	mRightSpace = 0;

	fTextAlign = VGDevice::kAlignLeft + VGDevice::kAlignBase;
	fFont = FontManager::GetTextFont(ar, staff->getStaffLSPACE(), fTextAlign);

	// when autopos is set, text alignment is ignored
	if (!ar->autoPos()) fTextAlign = VGDevice::kAlignLeft + VGDevice::kAlignBase;

	setGRStaff(staff);
	if (ar->autoPos())
		fRefPos = NVPoint(0, (staff->getDredgeSize() + LSPACE) / 2);
	else
		fRefPos = NVPoint(0,0);

	string text = ar->getName();
	string sub;
	size_t len = text.size();
	for(size_t i = 0; i < len; i++)  {
		char c = text[i];
		if (c == '\n') {
			fSubstrings.push_back ( sub);
			sub = "";
		}
		else sub += c;
	}
	fSubstrings.push_back ( sub);
}


const NVPoint& GRInstrument::getReferencePosition() const { return fRefPos; }

const ARInstrument* GRInstrument::getARInstrument() const
		{ return static_cast<const ARInstrument*>(getAbstractRepresentation()); };

// -----------------------------------------------------------------------------
void GRInstrument::getMetrics(VGDevice & hdc, float& h, float& w) const
{
	const VGFont* savedFont = hdc.GetTextFont();
	hdc.SetTextFont( fFont );
	float lw, lh, maxw=0;
	for (auto elt: fSubstrings) {
		fFont->GetExtent(elt.c_str(), (int)elt.size(), &lw, &lh, &hdc);
		if (lw > maxw) maxw = lw;
	}
	hdc.SetTextFont( savedFont );
	h = lh;
	w = maxw;
}

// -----------------------------------------------------------------------------
void GRInstrument::OnDraw(VGDevice & hdc) const
{
	// repeated instr are drawn by GRStaff
	if(!mDraw || !mShow || getARInstrument()->repeat()) return;
	OnDraw(hdc, mPosition.y);
}


// -----------------------------------------------------------------------------
void GRInstrument::OnDraw(VGDevice & hdc, float ypos) const
{
	if(!mDraw || !mShow) return;

	const string name = getARInstrument()->getName();
	if (name.empty()) return;

	hdc.SetTextFont( fFont );
	const VGColor prevTextColor = hdc.GetFontColor();

	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));
	hdc.SetFontAlign( fTextAlign );

	NVPoint refpos = getReferencePosition();
	const NVPoint & offset = getOffset();
	const NVPoint pos = mPosition;
	float w, h;
	getMetrics(hdc, h, w);
	if (getARInstrument()->autoPos())
		refpos = NVPoint(gCurSystem->getBoundingBox().left - LSPACE*2, fRefPos.y);

    float x = pos.x + offset.x + refpos.x;
    float y = ypos + offset.y + refpos.y;
    if (fSubstrings.size() > 1) {
		for (auto a: fSubstrings) {
			hdc.DrawString(x, y, a.c_str(), (int)a.size());
			y += h;
		}
	}
	else hdc.DrawString (x, y, name.c_str(), int(name.size()) );
	if( mColRef )
		hdc.SetFontColor( prevTextColor );
}
