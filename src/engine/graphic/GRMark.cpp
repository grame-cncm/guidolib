/*
  GUIDO Library
  Copyright (C) 2016 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "GRMark.h"
#include "ARMark.h"
#include "GRStaff.h"
#include "VGDevice.h"

using namespace std;

extern GRStaff * gCurStaff;

GRMark::GRMark(GRStaff * staff, ARMark * ar) : GRText(staff, ar)
{
	mPosition.y -= yoffset(staff);
}


GRMark::~GRMark()
{
}

//----------------------------------------------------------------
void GRMark::toEllipse (const FloatRect& r, VGDevice & hdc) const
{
	cerr << "\\mark tag: ellipse is not yet supported" << endl;
}

//----------------------------------------------------------------
void GRMark::toTriangle (const FloatRect& r, VGDevice & hdc) const
{
	FloatRect d = toSquare(r);
	d.Expand(d.Width()/4);
	d.ShiftY(-d.Width()/5);
	d.left -= d.Width()/8;
	d.right += d.Width()/8;
	float xtop = d.left + d.Width()/2;

	hdc.MoveTo (d.left, d.bottom);
	hdc.LineTo (xtop, d.top);
	hdc.LineTo (d.right, d.bottom);
	hdc.LineTo (d.left, d.bottom);
}

//----------------------------------------------------------------
void GRMark::toDiamond (const FloatRect& r, VGDevice & hdc) const
{
	FloatRect d = toSquare(r);
	d.Expand(d.Width()/4);
	d.ShiftY(-8);
	float w = d.Width()/2;
	float xtop	= d.left + w;
	float yleft = d.top + w;

	hdc.MoveTo (d.left, yleft);
	hdc.LineTo (xtop, d.top);
	hdc.LineTo (d.right, yleft);
	hdc.LineTo (xtop, d.bottom);
	hdc.LineTo (d.left, yleft);
}


//----------------------------------------------------------------
void GRMark::toBracket (const FloatRect& r, VGDevice & hdc) const
{
	float h = r.Height() / 5;
	hdc.MoveTo (r.left, r.top+h);
	hdc.LineTo (r.left, r.top);
	hdc.LineTo (r.right, r.top);
	hdc.LineTo (r.right, r.top+h);
}

//----------------------------------------------------------------
FloatRect GRMark::toSquare (const FloatRect& r) const
{
	FloatRect out (r);
	float d = r.Width() - r.Height();
	if (d > 0) {		// rect width is greater than the height
		float dy = d/2;
		out.top -= dy;
		out.bottom += dy;
	}
	else {
		float dx = -d/2;
		out.left -= dx;
		out.right += dx;
	}
	return out;
}

//----------------------------------------------------------------
/**
	Here, we must handle two different cases:
		(1) The text position must be relative to the last staff-line. (i.e: Lyrics)
		(2) The text position must follow the y-position of the note. (i.e: Fingering)
*/
void GRMark::OnDraw( VGDevice & hdc ) const
{
	GRText::OnDraw (hdc);
	const ARMark* mark = getARMark();
	int enclosure = mark->getEnclosure();
	if (enclosure) {
		FloatRect r = getTextMetrics(hdc, gCurStaff);
		r.Expand (3.f);
		r.left -= 2;
		r.right += 2;
		const VGColor color = mColRef ? VGColor( mColRef ) : hdc.GetFontColor();
		hdc.PushPen( color, 5);
		switch (enclosure) {
			case ARMark::kSquare:
				r = toSquare(r);
			case ARMark::kRectangle:
				r.ShiftY(-2);
				hdc.Frame(r.left, r.top, r.right, r.bottom);
				break;
			case ARMark::kCircle:
				r = toSquare(r);
			case ARMark::kOval:
				toEllipse(r, hdc);
				break;
			case ARMark::kBracket:
				toBracket (r, hdc);
				break;
			case ARMark::kTriangle:
				toTriangle (r, hdc);
				break;
			case ARMark::kDiamond:
				toDiamond (r, hdc);
				break;
		}
		hdc.PopPen();
	}
}

float GRMark::yoffset (const GRStaff* staff) const
{
	const float curLSPACE = staff->getStaffLSPACE();
	int n = staff->getNumlines() + 2;
	return n * curLSPACE;
}

FloatRect GRMark::getTextMetrics(VGDevice & hdc, const GRStaff* staff) const
{
	FloatRect  r = GRText::getTextMetrics(hdc, staff);
	if (staff) r.ShiftY (-yoffset (staff));
	return r;
}

const ARMark * GRMark::getARMark() const
{
	return static_cast<const ARMark*>(getAbstractRepresentation());
}

