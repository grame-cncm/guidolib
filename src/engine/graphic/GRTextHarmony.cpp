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
#include <cctype>

#include "ARTextHarmony.h"
#include "GRTextHarmony.h"

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

using namespace std;

extern GRStaff * gCurStaff;

GRTextHarmony::GRTextHarmony(GRStaff * p_staff, const ARTextHarmony * ar)
  : GRPTagARNotationElement(ar)
{
	assert(ar);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRTextSaveStruct * st = new GRTextSaveStruct;
	sse->p = (void *) st;
	sse->grsystem = p_staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	mMustFollowPitch = false;
	mStartEndList.AddTail(sse);

	float curLSPACE = LSPACE;
	if (p_staff)
		curLSPACE = p_staff->getStaffLSPACE();

	fFont = FontManager::gFontText;
	if (ar) {
		mFontSize = int(ar->getFSize() * curLSPACE / LSPACE);
		if (mFontSize == 0)
			mFontSize = (int)(1.5f * LSPACE);
		font = new NVstring(ar->getFont());
		fontAttrib = new NVstring(ar->getTextAttributes());

		st->text = ar->getText() ? ar->getText() : "";
	}

	if (!font->empty())
		fFont = FontManager::FindOrCreateFont( mFontSize, font->c_str(), fontAttrib->c_str() );

	// depending on the textformat ...
	unsigned int xdir = VGDevice::kAlignLeft;
	unsigned int ydir = VGDevice::kAlignTop;
	const char* tf = ar->getTextFormat();
	if (tf && (strlen(tf) == 2))
	{
		switch (tf[0]) {
			case 'l':	xdir = VGDevice::kAlignLeft; break;
			case 'c':	xdir = VGDevice::kAlignCenter; break;
			case 'r':	xdir = VGDevice::kAlignRight; break;
		}

		switch (tf[1]) {
			case 't':	ydir = VGDevice::kAlignTop; break;
			case 'c':	ydir = VGDevice::kAlignBase; break;
			case 'b':	ydir = VGDevice::kAlignBottom; break;
		}
	}

	mTextAlign = xdir | ydir;
	st->boundingBox.left = 0;
	st->boundingBox.top  = 0;

	float sizex = 0;
	float sizey = 0;
	if( gGlobalSettings.gDevice)
		fFont->GetExtent( st->text.c_str(), (int)st->text.size(), &sizex, &sizey, gGlobalSettings.gDevice );

    st->boundingBox.right = 0; //sizex;
	st->boundingBox.top = sizey;
	st->boundingBox.bottom = 4 * LSPACE;
}


GRTextHarmony::~GRTextHarmony()
{
	assert(mStartEndList.empty());
	// this is important ...
	// All associaions that have been made are dealt with in GRPositionTag ...

	// this makes sure, that we don't remove associations, that are no longer there
	// after the Tag has been delete (especially, if more than one system is handled.)
	delete mAssociated;
	mAssociated = 0;
}

/**
	Here, we must handle two different cases:
		(1) The text position must be relative to the last staff-line. (i.e: Lyrics)
		(2) The text position must follow the y-position of the note. (i.e: Fingering)
*/
void GRTextHarmony::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow)
		return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	assert(sse);
	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;

	const ARTextHarmony * arText = getARTextHarmony();
	const float curLSPACE = gCurStaff ? gCurStaff->getStaffLSPACE(): LSPACE;

	// - Setup position.
	// y-reference position if the lowest line of the staff.
	NVPoint drawPos (st->position);

	// - Force the position to be relative to the bottom line of the staff
	if( mMustFollowPitch == false ) {
		int pos = arText->position();
		if (gCurStaff) {
			switch (pos) {
				case ARTextHarmony::kUndefined:
				case ARTextHarmony::kBelow:
					drawPos.y = gCurStaff->getDredgeSize(); // + curLSPACE;
					break;
				case ARTextHarmony::kAbove:
					drawPos.y = -gCurStaff->getDredgeSize() - curLSPACE/2;
					break;
			}
		}
		else drawPos.y = -curLSPACE;
	}
	float dx = arText->getDX()->getValue( curLSPACE );
	float dy = -arText->getDY()->getValue( curLSPACE );;

	hdc.SetTextFont( fFont);
	const VGColor prevTextColor = hdc.GetFontColor();
	if( mColRef ) hdc.SetFontColor( VGColor( mColRef ));
	hdc.SetFontAlign( mTextAlign );
	DrawHarmonyString (hdc, fFont, st->text, drawPos.x + st->boundingBox.left + dx, drawPos.y + dy);

	if( mColRef ) hdc.SetFontColor( prevTextColor );
}

float GRTextHarmony::CharExtend (const char* c, const VGFont* font, VGDevice* hdc) const
{
	float w, h;
	font->GetExtent (c, 1, &w, &h, hdc);
	return w;
}

void GRTextHarmony::DrawHarmonyString (VGDevice & hdc, const VGFont* font, const string& str, float x, float y) const
{
	if (str.empty()) return;

	const VGFont* mfont = hdc.GetMusicFont();
	float ratio = font->GetSize() / 150.f; // 150 is the default font size for harmony (20 pt)

	const VGFont* mBigFont = FontManager::FindOrCreateFont( mfont->GetSize() * 1.3 * ratio, mfont->GetName(), "");
	const VGFont* mSmallFont = FontManager::FindOrCreateFont( mfont->GetSize() * 0.8 * ratio, mfont->GetName(), "");
	const VGFont* tSmallFont = FontManager::FindOrCreateFont( font->GetSize() * 0.8, font->GetName(), "");
	const VGFont* curmfont = mBigFont;
	const VGFont* curtfont = font;
	const char * ptr = str.c_str();
	float moffset = 4 * LSPACE * ratio;
	float yoffset = 0;
	float xoffset = LSPACE/2;
	bool inSecPart = false;

	hdc.SetMusicFont (mBigFont);
	while (*ptr) {
		char c = *ptr++;
		char buff[2] = { c, 0 };
		if (!inSecPart) {
			inSecPart = std::isdigit(c);
			if (inSecPart) {
				hdc.SetMusicFont (mSmallFont);
				moffset = 2 * LSPACE * ratio;
				yoffset = -2;
				xoffset /= 2;
			}
		}

		if (c == '#') {
			hdc.DrawMusicSymbol(x + 10, y - moffset - LSPACE, kSharpSymbol);
			buff[0] = kSharpSymbol;
			x += CharExtend (buff, curmfont, &hdc) + xoffset;
		}
		else if (c == '&') {
			hdc.DrawMusicSymbol(x + 10, y - moffset - (inSecPart ? LSPACE/2: 0), kFlatSymbol);
			buff[0] = kFlatSymbol;
			x += CharExtend (buff, curmfont, &hdc) + xoffset;
		}
		else {
			hdc.DrawString( x, y + yoffset, buff, 1);
			x += CharExtend (buff, curtfont, &hdc);
			hdc.SetTextFont (tSmallFont);
			if (!inSecPart) {
				curtfont = tSmallFont;
				yoffset = font->GetSize() - curtfont->GetSize();
			}
		}
	}
	hdc.SetMusicFont (mfont);
}

const ARTextHarmony * GRTextHarmony::getARTextHarmony() const
{
	return static_cast<const ARTextHarmony*>(getAbstractRepresentation());
}

void GRTextHarmony::addAssociation(GRNotationElement * el)
{
	GRNotationElement::addAssociation(el);
	GRPositionTag::addAssociation(el);
}
/** \brief Called directly by a spring. Then we know that we
	do not have a position tag.
*/
void GRTextHarmony::setPosition(const NVPoint & inPosition)
{
	GRPTagARNotationElement::setPosition(inPosition);

	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	assert(st);
	st->position = inPosition;
}

void GRTextHarmony::setHPosition( GCoord nx )
{
	GRPTagARNotationElement::setHPosition(nx);
	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	assert(st);
	st->position.x = nx;
}

void GRTextHarmony::tellPosition(GObject * caller, const NVPoint & inPosition)
{
	// this can be only called by an event, that is there ..
	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if( staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	GRNotationElement * startElement = sse->startElement;
	NVPoint newPos( inPosition );

	// - Check if we're left-opened
	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
	{
		if (grel != startElement)
		{
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

		const ARTextHarmony* arText = getARTextHarmony();
		const char* text = arText ? arText->getText() : 0;
		if (text) st->text = text;
	}
}

void GRTextHarmony::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}

GCoord GRTextHarmony::getLeftSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;
	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	if (!st) return 0;

	return -st->boundingBox.left;
}

GCoord GRTextHarmony::getRightSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;
	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	if (!st) return 0;

	return st->boundingBox.right;
}
