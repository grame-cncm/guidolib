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
#include "ARMusicalObject.h"
#include "ARInstrument.h"
#include "FontManager.h"
#include "VGDevice.h"

using namespace std;


GRInstrument::GRInstrument(const ARInstrument * ar, GRStaff* staff)
		: GRTagARNotationElement(ar, LSPACE)
{
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;

	// No!
	mNeedsSpring = 1;

	// to do: read real text ...
	const GCoord extent = 100;

	mBoundingBox.right = extent;
	mBoundingBox.bottom = (GCoord)(3*LSPACE);

	// no Space needed !
	mLeftSpace = 0;
	mRightSpace = 0;

	// no reference position ...
	fFont = ar->getFont();
	string attr = ar->getTextFormat();
	unsigned int xdir = VGDevice::kAlignLeft;
	unsigned int ydir = VGDevice::kAlignBase;
	if (ar->autoPos()) {
		if( attr.size() == 2 )
		{
			switch (attr[0]) {
				case 'l':	xdir = VGDevice::kAlignLeft; break;
				case 'c':	xdir = VGDevice::kAlignCenter; break;
				case 'r':	xdir = VGDevice::kAlignRight; break;
			}	  
			switch (attr[1]) {
				case 't':	ydir = VGDevice::kAlignTop; break;
				case 'c':	ydir = VGDevice::kAlignBase; break;
				case 'b':	ydir = VGDevice::kAlignBottom; break;
			}
		}
		fTextFormat = xdir | ydir;
	}
	else fTextFormat = xdir | ydir;

    fFontAttributes = ar->getTextAttributes();
	fSize = ar->getFSize();
	setGRStaff(staff);
	if (ar->autoPos())
		fRefPos = NVPoint(-LSPACE*2, (staff->getDredgeSize() + LSPACE) / 2);
	else
		fRefPos = NVPoint(0,0);
}

GRInstrument::~GRInstrument()
{
}

const NVPoint& GRInstrument::getReferencePosition() const { return fRefPos; }

const ARInstrument* GRInstrument::getARInstrument() const
		{ return static_cast<const ARInstrument*>(getAbstractRepresentation()); };


void GRInstrument::OnDraw(VGDevice & hdc) const
{
	if(!mDraw || !mShow) return;

	const string name = getARInstrument()->getName();
	if (name.empty()) return;

	const VGFont* font = FontManager::FindOrCreateFont( int(fSize), fFont.c_str(), fFontAttributes.c_str() );
	hdc.SetTextFont( font );
	const VGColor prevTextColor = hdc.GetFontColor();

	if( mColRef )
		hdc.SetFontColor( VGColor( mColRef ));
	hdc.SetFontAlign( fTextFormat );

	const NVPoint & refpos = getReferencePosition();
	const NVPoint & offset = getOffset();
	const NVPoint pos = mPosition;
	hdc.DrawString(	pos.x + offset.x + refpos.x, pos.y + offset.y + refpos.y, name.c_str(), int(name.size()) );
	if( mColRef )
		hdc.SetFontColor( prevTextColor );
}

