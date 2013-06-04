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

#include "ARSymbol.h"

#include "GRDefine.h"
#include "FontManager.h"
#include "GRPage.h"
#include "GRRod.h"
#include "GRStaff.h"
#include "GRSymbol.h"
#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice

#include "TagParameterFloat.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "FontManager.h"


extern GRStaff * gCurStaff;

GRSymbol::GRSymbol(GRStaff * p_staff, ARSymbol * abstractRepresentationOfSymbol)
  : GRPTagARNotationElement(abstractRepresentationOfSymbol)
{
	assert(abstractRepresentationOfSymbol);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	GRSymbolSaveStruct * st = new GRSymbolSaveStruct;
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
	const ARSymbol * myar = getARSymbol();
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

	if (myar && myar->getSymbol())
		st->filename = myar->getSymbol();
	else
		st->filename = "";

//	if (st->text)
//	{
		const char * cp = st->filename.c_str();
		float sizex = 0;
		float sizey = 0;
		if( gGlobalSettings.gDevice )
			hmyfont->GetExtent( cp, (int)st->filename.size(), &sizex, &sizey, gGlobalSettings.gDevice );

		st->boundingBox.right = sizex;
		st->boundingBox.top = sizey;
//	}

	st->boundingBox.bottom = 4 * LSPACE;
}


GRSymbol::~GRSymbol()
{
	assert(mStartEndList.empty());
	// this is important ...
	// All associaions that have been made
	// are dealt with in GRPositionTag ...

	// this makes sure, that we don't remove
	// associations, that are no longer there
	// after the Tag has been delete
	// (especially, if more than one system
	//  is handled.)
	delete mAssociated;
	mAssociated = 0;
}

/**
	Here, we must handle two different cases:
		(1) The text position must be relative to the last staff-line. (i.e: Lyrics)
		(2) The text position must follow the y-position of the note. (i.e: Fingering)
*/
void GRSymbol::OnDraw( VGDevice & hdc ) const
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	assert(sse);
	GRSymbolSaveStruct * st = (GRSymbolSaveStruct *) sse->p;
//	if (st->text == 0) return;

	const ARSymbol * arSymbol = getARSymbol();
	const float curLSPACE = gCurStaff ? gCurStaff->getStaffLSPACE(): LSPACE;

	// - Setup position.
	// y-reference position if the lowest line of the staff.
	NVPoint drawPos (st->position);

	// - Force the position to be relative to the bottom line of the staff
	if( mMustFollowPitch == false )
	{
		if (gCurStaff) // else ?
			drawPos.y = gCurStaff->getDredgeSize();
		else
			drawPos.y = 0;
	}

	float dx = 0;
	float dy = 0;

	if( arSymbol->getYPos()) // && arText->getYPos()->TagIsSet())
		drawPos.y -= (arSymbol->getYPos()->getValue(curLSPACE));
	if (arSymbol->getDY())
		dy = -arSymbol->getDY()->getValue( curLSPACE );
	if (arSymbol->getDX())
		dx = arSymbol->getDX()->getValue( curLSPACE );

	// - Setup font ....
	const VGFont* hmyfont;
	if (font && font->length() > 0)
		hmyfont = FontManager::FindOrCreateFont( mFontSize, font, fontAttrib );
	else
		hmyfont = FontManager::gFontText;

	hdc.SetTextFont( hmyfont );

	//	int nBackmode = hdc.GetBackgroundMode();
	//	hdc.SetBackgroundMode( VGDevice::kModeTransparent );

//	const unsigned int ta = hdc.GetTextAlign();
	const VGColor prevTextColor = hdc.GetFontColor();

	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));

// DEBUG
//	if( arText->getRange())
//		hdc.SetTextColor( GColor( 0, 200, 0 ));	// green if tag has a range

	hdc.SetFontAlign( mTextAlign );

	// - Print text
	const char * theText = st->filename.c_str();
	const int charCount = (int)st->filename.size();

	hdc.DrawString( drawPos.x + st->boundingBox.left + dx,
				 drawPos.y + dy, theText, charCount );

//hdc.SetTextColor( GColor( 0, 0, 0 ));	// DEBUG

	// - Restore context
	// hdc.SetTextAlign( ta );
	if( mColRef )
		hdc.SetFontColor( prevTextColor );
	// hdc.SetBackgroundMode( nBackmode );
}

void GRSymbol::print() const
{
}

const ARSymbol * GRSymbol::getARSymbol() const
{
	return /*dynamic*/static_cast<const ARSymbol*>(getAbstractRepresentation());
}

void GRSymbol::addAssociation(GRNotationElement * el)
{
	GRNotationElement::addAssociation(el);
	GRPositionTag::addAssociation(el);
}
/** \brief Called directly by a spring. Then we know that we
	do not have a position tag.
*/
void GRSymbol::setPosition(const NVPoint & inPosition)
{
	GRPTagARNotationElement::setPosition(inPosition);

	// how do I get the current sse?

	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRSymbolSaveStruct * st = (GRSymbolSaveStruct *) sse->p;
	assert(st);

	st->position = inPosition;
}

void GRSymbol::setHPosition( GCoord nx )
{
	GRPTagARNotationElement::setHPosition(nx);
	// there can be only one sse! -> no overlap
	assert(mStartEndList.size() == 1);
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();

	GRSymbolSaveStruct * st = (GRSymbolSaveStruct *) sse->p;
	assert(st);

	st->position.x = nx;
}

void GRSymbol::tellPosition(GObject * caller, const NVPoint & inPosition)
{
	// this can be only called by an event, that is there ..
	GRNotationElement * grel =  dynamic_cast<GRNotationElement *>(caller);
	if( grel == 0 ) return;

	GRStaff * staff = grel->getGRStaff();
	if( staff == 0 ) return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	assert(sse);

	GRSymbolSaveStruct * st = (GRSymbolSaveStruct *)sse->p;
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
				st->filename = "-";
			}
		}
	}

	// - Follows the y-position of the first element of the range (if any)
	else if (grel == startElement)
	{
		newPos.y = grel->getPosition().y;
		st->position = newPos;
	}
}

void GRSymbol::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}

GCoord GRSymbol::getLeftSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;

	GRSymbolSaveStruct * st = (GRSymbolSaveStruct *)sse->p;
	if (!st) return 0;

	return -st->boundingBox.left;
}

GCoord GRSymbol::getRightSpace() const
{
	GRSystemStartEndStruct * sse = mStartEndList.GetHead();
	if (!sse) return 0;

	GRSymbolSaveStruct * st = (GRSymbolSaveStruct *)sse->p;
	if (!st) return 0;

	return st->boundingBox.right;
}
