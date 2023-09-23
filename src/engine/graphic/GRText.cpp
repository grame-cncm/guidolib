/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>
#include <algorithm>

#include "ARText.h"

#include "GRDefine.h"
#include "FontManager.h"
#include "GRPage.h"
#include "GRRod.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRText.h"
#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice

#include "TagParameterFloat.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "FontManager.h"

using namespace std;

extern GRStaff * gCurStaff;

// -----------------------------------------------------------------------------
GRText::GRText(GRStaff * staff, const ARText * ar) : GRPTagARNotationElement(ar)
{
	assert(ar);
	assert(staff);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRTextSaveStruct * st = new GRTextSaveStruct;
	sse->p = (void *) st;
	sse->grsystem = staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	mMustFollowPitch = false;
	addSystemStartEndStruct (sse);

	float curLSPACE = staff->getStaffLSPACE();
	mPosition.y = staff->getDredgeSize();
	mPosition.y -= ar->getDY()->getValue( curLSPACE );

	mTextAlign = VGDevice::kAlignLeft + VGDevice::kAlignTop;
	fFont = FontManager::GetTextFont(ar, curLSPACE, mTextAlign);
	
	st->boundingBox.left = 0;
	st->boundingBox.top  = 0;
	string text = ar->getText() ? ar->getText() : "";
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
	
	st->text = ar->getText() ? ar->getText() : "";

	const char * cp = st->text.c_str();
	float sizex = 0;
	float sizey = 0;
	if( gGlobalSettings.gDevice )
		fFont->GetExtent( cp, (int)st->text.size(), &sizex, &sizey, gGlobalSettings.gDevice );

	st->boundingBox.right = sizex;
	st->boundingBox.top = sizey;
	st->boundingBox.bottom = 4 * LSPACE;
	mStaffBottom = 0;
}


// -----------------------------------------------------------------------------
GRText::~GRText()
{
	assert(mStartEndList.empty());
	// this is important ...
	// All associaions that have been made are dealt with in GRPositionTag ...

	// this makes sure, that we don't remove associations, that are no longer there
	// after the Tag has been delete (especially, if more than one system is handled.)
	delete mAssociated;
	mAssociated = 0;
}

bool GRText::isLyrics() const			{ return getARText()->isLyric(); }

// -----------------------------------------------------------------------------
void GRText::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

// -----------------------------------------------------------------------------
float GRText::getLineHeight(VGDevice & hdc) const
{
	const VGFont* savedFont = hdc.GetTextFont();
	hdc.SetTextFont( fFont );
	float lw, lh;
	fFont->GetExtent(fSubstrings[0].c_str(), (int)fSubstrings[0].size(), &lw, &lh, &hdc);
	hdc.SetTextFont( savedFont );
	return lh;
}

// -----------------------------------------------------------------------------
FloatRect GRText::getTextMetrics(VGDevice & hdc, const GRStaff* staff ) const
{
	FloatRect r;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( staff->getGRSystem() );
	assert(sse);
	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	const ARText * arText = getARText();
	const float curLSPACE = staff ? staff->getStaffLSPACE(): LSPACE;
	// - Setup position.
	// y-reference position if the lowest line of the staff.
	NVPoint drawPos (st->position);
	if( arText->isAutoPos() && staff) {
		drawPos.y = mStaffBottom;
	}
	else if( mMustFollowPitch == false ) {
		// - Force the position to be relative to the bottom line of the staff
		if (staff) // else ?
			drawPos.y = staff->getDredgeSize();
		else
			drawPos.y = 0;
	}

	float dx = arText->getDX()->getValue( curLSPACE );
	float dy = -arText->getDY()->getValue( curLSPACE );

	float x = drawPos.x + st->boundingBox.left + dx;
	float y = drawPos.y + dy;
	float w = 0;
	float h = 0;

	const VGFont* savedFont = hdc.GetTextFont();
	hdc.SetTextFont( fFont );
	if (fSubstrings.size() > 1)
		for (string a: fSubstrings) {
			float lw, lh;
			fFont->GetExtent(a.c_str(), (int)a.size(), &lw, &lh, &hdc);
			w = std::max(w, lw);
			h += lh;
		}
	else
		fFont->GetExtent(st->text.c_str(), (int)st->text.size(), &w, &h, &hdc);

	hdc.SetTextFont( savedFont );
	if( arText->isLyric() && arText->isAutoPos() ) {
		y += curLSPACE * 0.75f;
		r.Set(x, y-h, x+w, y);
	}
	else r.Set(x, y, x+w, y+h);
	return r;
}

// -----------------------------------------------------------------------------
/**
	Here, we must handle two different cases:
		(1) The text position must be relative to the last staff-line. (i.e: Lyrics)
		(2) The text position must follow the y-position of the note. (i.e: Fingering)
*/
void GRText::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	assert(sse);
	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;

	unsigned int fontalign;
	const VGColor prevTextColor = startDraw(hdc, fontalign);

	// - Print text
    if (!st->text.empty()) {
		const char* t = st->text.c_str();
		size_t prevpos = 0 , pos = 0;
		float y = mPosition.y;
		bool align = fSubstrings.size() > 1;
		for (auto a: fSubstrings) {
			hdc.DrawString(mPosition.x, y, a.c_str(), (int)a.size());
			y += fLineHeight;
		}
	}
	endDraw(hdc, prevTextColor, fontalign);

//	DrawBoundingBox(hdc, VGColor(0,0,255));
}

const VGColor GRText::startDraw( VGDevice & hdc, unsigned int& fontalign ) const
{
	hdc.SetTextFont( fFont );

	const VGColor prevTextColor = hdc.GetFontColor();
	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));
	fontalign = hdc.GetFontAlign();
	hdc.SetFontAlign( mTextAlign );
	return prevTextColor;
}

void GRText::endDraw( VGDevice & hdc, const VGColor textcolor, unsigned int fontalign) const
{
	if( mColRef )
		hdc.SetFontColor( textcolor );
	hdc.SetFontAlign( fontalign );
}

// -----------------------------------------------------------------------------
const ARText * GRText::getARText() const
{
	return static_cast<const ARText*>(getAbstractRepresentation());
}

// -----------------------------------------------------------------------------
void GRText::addAssociation(GRNotationElement * el)
{
	GRNotationElement::addAssociation(el);
	GRPositionTag::addAssociation(el);
}

// -----------------------------------------------------------------------------
/** \brief Called directly by a spring. Then we know that we
	do not have a position tag.
*/
void GRText::setPosition(const NVPoint & inPosition)
{
	GRPTagARNotationElement::setPosition(inPosition);

	// how do I get the current sse?

	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	assert(st);
	st->position = inPosition;
}

// -----------------------------------------------------------------------------
void GRText::setHPosition( GCoord nx )
{
	GRPTagARNotationElement::setHPosition(nx);
	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	assert(st);

	const ARText * arText = getARText();
// this way to read the current lspace is not deterministic !!!
//	const float curLSPACE = gCurStaff ? gCurStaff->getStaffLSPACE(): LSPACE;
	float dx = 0;
	if (arText->getDX())
		dx = arText->getDX()->getValue( LSPACE );
	mPosition.x += dx;

	st->position.x = nx;
}

// -----------------------------------------------------------------------------
void GRText::tellPosition(GObject * caller, const NVPoint & inPosition)
{
	// this can be only called by an event, that is there ..
	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( grel == 0 ) return;

	const GRStaff * staff = grel->getGRStaff();
	if( staff == 0 ) return;

	mStaffBottom = staff->getStaffBottom();
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	GRNotationElement * startElement = sse->startElement;
	NVPoint newPos( inPosition );

	// - Check if we're left-opened
	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT) {
		if (grel != startElement) {
			if (st->position.x == 0)
			{
				newPos.x -= LSPACE * 0.5f; // this is actually notebreite!
				st->position = newPos;
				st->text = "-";
			}
		}
	}

	// - Follows the y-position of the first element of the range (if any)
	else if (grel == startElement)
	{
		newPos.y = grel->getPosition().y;
		st->position = newPos;

		FloatRect r = getTextMetrics (*gGlobalSettings.gDevice, staff);
		fLineHeight = getLineHeight (*gGlobalSettings.gDevice);
		setPosition (NVPoint(r.left, r.top));
		NVRect bb (0, 0, r.Width(), r.Height());
		mBoundingBox = bb;
		float xoffset = 0;
		if (mTextAlign & VGDevice::kAlignCenter)
			xoffset = mBoundingBox.Width()/2;
		if (mTextAlign & VGDevice::kAlignRight)
			xoffset = mBoundingBox.Width();
		mBoundingBox -= NVPoint(xoffset, 0);
	}
}

// -----------------------------------------------------------------------------
void GRText::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}

// -----------------------------------------------------------------------------
GCoord GRText::getLeftSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	if (!st) return 0;

	return -st->boundingBox.left;
}

// -----------------------------------------------------------------------------
GCoord GRText::getRightSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	if (!st) return 0;

	return st->boundingBox.right;
}
