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

#include "GRDefine.h"
#include "FontManager.h"
#include "GRPage.h"
#include "GRRod.h"
#include "GRStaff.h"
#include "GRText.h"
#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice

#include "TagParameterFloat.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "FontManager.h"


extern GRStaff * gCurStaff;

GRMark::GRMark(GRStaff * p_staff, ARMark * abstractRepresentationOfText)
  : GRText(p_staff, abstractRepresentationOfText)
{
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
		FloatRect r = getTextMetrics(hdc);
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

FloatRect GRMark::getTextMetrics(VGDevice & hdc) const
{
	FloatRect r = GRText::getTextMetrics(hdc);
	if (gCurStaff) {
		const float curLSPACE = gCurStaff->getStaffLSPACE();
		int n = gCurStaff->getNumlines() + 2;
		float offset = n * curLSPACE;
		r.ShiftY(-offset);
	}
	return r;
}

const ARMark * GRMark::getARMark() const
{
	return /*dynamic*/static_cast<const ARMark*>(getAbstractRepresentation());
}


/** \brief Called directly by a spring. Then we know that we
	do not have a position tag.
*/
//void GRMark::setPosition(const NVPoint & inPosition)
//{
//	GRPTagARNotationElement::setPosition(inPosition);
//
//	// how do I get the current sse?
//
//	// there can be only one sse! -> no overlap
//	assert(mStartEndList.size() == 1);
//	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
//
//	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
//	assert(st);
//
//	st->position = inPosition;
//}
//
//void GRMark::setHPosition( GCoord nx )
//{
//	GRPTagARNotationElement::setHPosition(nx);
//	// there can be only one sse! -> no overlap
//	assert(mStartEndList.size() == 1);
//	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
//
//	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
//	assert(st);
//
//	st->position.x = nx;
//}

void GRMark::tellPosition(GObject * caller, const NVPoint & inPosition)
{
	GRText::tellPosition (caller, inPosition);
	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( grel == 0 ) return;
	GRStaff * staff = grel->getGRStaff();
	const NVPoint & staffPos = staff->getPosition();

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);
	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	NVPoint newPos( st->position );
	newPos.y = staffPos.y - staff->getStaffLSPACE(); // Space between two lines
	st->position = newPos;

cout << "GRMark::tellPosition " << newPos.y << endl;
	
//	// this can be only called by an event, that is there ..
//	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
//	if( grel == 0 ) return;
//
//	GRStaff * staff = grel->getGRStaff();
//	if( staff == 0 ) return;
//
//	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
//	assert(sse);
//
//	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
//	GRNotationElement * startElement = sse->startElement;
//	NVPoint newPos( inPosition );
//
//	// - Check if we're left-opened
//	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
//	{
//		if (grel != startElement)
//		{
//			if (st->position.x == 0)
//			{
//				newPos.x -= LSPACE * 0.5f; // this is actually notebreite!
//				st->position = newPos;
//				st->text = "-";
//			}
//		}
//	}
//
//	// - Follows the y-position of the first element of the range (if any)
//	else if (grel == startElement)
//	{
//		newPos.y = grel->getPosition().y;
//		st->position = newPos;
//
//		const ARText * arText = getARText();
//		const char* text = arText ? arText->getText() : 0;
//		if (text) st->text = text;
//	}
}

