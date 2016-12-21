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

#include "ARText.h"

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

GRText::GRText(GRStaff * p_staff, ARText * abstractRepresentationOfText)
  : GRPTagARNotationElement(abstractRepresentationOfText)
{
	assert(abstractRepresentationOfText);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRTextSaveStruct * st = new GRTextSaveStruct;
	sse->p = (void *) st;
	sse->grsystem = p_staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	mMustFollowPitch = false;
	mStartEndList.AddTail(sse);

	float curLSPACE = LSPACE;
	if (p_staff)
	{
		curLSPACE = p_staff->getStaffLSPACE();
	}

	const VGFont* hmyfont = FontManager::gFontText;
	const ARText * myar = getARText();
	if (myar)
	{
		mFontSize = myar->getFSize(curLSPACE);
		if (mFontSize == 0)
			mFontSize = (int)(1.5f * LSPACE);
		// we do not want that (do we?)
		font = new NVstring(myar->getFont());
		fontAttrib = new NVstring(myar->getFAttrib());
	}

	if (font && font->length() > 0)
	{
		// handle font-attributes ...
		hmyfont = FontManager::FindOrCreateFont( mFontSize, font, fontAttrib );
	}

	// depending on the textformat ...

	unsigned int xdir = VGDevice::kAlignLeft;
	unsigned int ydir = VGDevice::kAlignTop;
	const char* tf = myar->getTextformat();
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

	if (myar && myar->getText())
		st->text = myar->getText();
	else
		st->text = "";

//	if (st->text)
//	{
		const char * cp = st->text.c_str();
		float sizex = 0;
		float sizey = 0;
		if( gGlobalSettings.gDevice )
			hmyfont->GetExtent( cp, (int)st->text.size(), &sizex, &sizey, gGlobalSettings.gDevice );

		st->boundingBox.right = sizex;
		st->boundingBox.top = sizey;
//	}

	st->boundingBox.bottom = 4 * LSPACE;
	mStaffBottom = 0;
}


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

FloatRect GRText::getTextMetrics(VGDevice & hdc) const
{
	FloatRect r;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	assert(sse);
	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	const ARText * arText = getARText();
	const float curLSPACE = gCurStaff ? gCurStaff->getStaffLSPACE(): LSPACE;
	// - Setup position.
	// y-reference position if the lowest line of the staff.
	NVPoint drawPos (st->position);
	if( arText->isAutoPos() && gCurStaff) {
		drawPos.y = mStaffBottom;
	}
	else if( mMustFollowPitch == false )
	{
		// - Force the position to be relative to the bottom line of the staff
		if (gCurStaff) // else ?
			drawPos.y = gCurStaff->getDredgeSize();
		else
			drawPos.y = 0;
	}

	float dx = 0;
	float dy = 0;
	if( arText->getYPos())
		drawPos.y -= (arText->getYPos()->getValue(curLSPACE));
	if (arText->getDY())
		dy = -arText->getDY()->getValue( curLSPACE );
	if (arText->getDX())
		dx = arText->getDX()->getValue( curLSPACE );

	const char * theText = st->text.c_str();
	const int charCount = (int)st->text.size();
	float x = drawPos.x + st->boundingBox.left + dx;
	float y = drawPos.y + dy;
	float w = 0;
	float h = 0;

	const VGFont* hmyfont;
	if (font && font->length() > 0)
		hmyfont = FontManager::FindOrCreateFont( mFontSize, font, fontAttrib );
	else
		hmyfont = FontManager::gFontText;

	const VGFont* savedFont = hdc.GetTextFont();
	hdc.SetTextFont( hmyfont );
	hmyfont->GetExtent(theText, charCount, &w, &h, &hdc);
	hdc.SetTextFont( savedFont );
	if( arText->isLyric() && arText->isAutoPos() ) {
		y += curLSPACE * 0.75f;
		r.Set(x, y-h, x+w, y);
	}
	else
		r.Set(x, y, x+w, y+h);
	return r;
}

/**
	Here, we must handle two different cases:
		(1) The text position must be relative to the last staff-line. (i.e: Lyrics)
		(2) The text position must follow the y-position of the note. (i.e: Fingering)
*/
void GRText::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw) return;

//cerr << "GRText::OnDraw pos: " << getPosition() << " bb: " << getBoundingBox() << endl;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	assert(sse);
	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;

	// - Setup font ....
	const VGFont* hmyfont;
	if (font && font->length() > 0)
		hmyfont = FontManager::FindOrCreateFont( mFontSize, font, fontAttrib );
	else
		hmyfont = FontManager::gFontText;

	hdc.SetTextFont( hmyfont );
	const VGColor prevTextColor = hdc.GetFontColor();

	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));

	hdc.SetFontAlign( mTextAlign );

	// - Print text
	const char * theText = st->text.c_str();
	const int charCount = (int)st->text.size();
	FloatRect r = getTextMetrics (hdc);

    if (charCount > 0)
	    hdc.DrawString( r.left, r.top, theText, charCount);

	if( mColRef )
		hdc.SetFontColor( prevTextColor );
}

const ARText * GRText::getARText() const
{
	return /*dynamic*/static_cast<const ARText*>(getAbstractRepresentation());
}

void GRText::addAssociation(GRNotationElement * el)
{
	GRNotationElement::addAssociation(el);
	GRPositionTag::addAssociation(el);
}
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

void GRText::setHPosition( GCoord nx )
{
	GRPTagARNotationElement::setHPosition(nx);
	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRTextSaveStruct * st = (GRTextSaveStruct *) sse->p;
	assert(st);

	st->position.x = nx;
}

void GRText::tellPosition(GObject * caller, const NVPoint & inPosition)
{
	// this can be only called by an event, that is there ..
	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if( staff == 0 ) return;

	mStaffBottom = staff->getBoundingBox().bottom;
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

		const ARText * arText = getARText();
		const char* text = arText ? arText->getText() : 0;
		if (text) st->text = text;
	}
}

void GRText::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}

GCoord GRText::getLeftSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	if (!st) return 0;

	return -st->boundingBox.left;
}

GCoord GRText::getRightSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;

	GRTextSaveStruct * st = (GRTextSaveStruct *)sse->p;
	if (!st) return 0;

	return st->boundingBox.right;
}
