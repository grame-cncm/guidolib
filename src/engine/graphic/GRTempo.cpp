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
#include "secureio.h"

#include "ARTempo.h"
#include "FontManager.h"
#include "FontManager.h"
#include "GRFlag.h"
#include "GRStaff.h"
#include "GRTempo.h"
#include "GuidoDefs.h"
#include "GUIDOInternal.h"
#include "MusicalSymbols.h"
#include "NoteDrawer.h"
#include "TagParameterFloat.h"
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

// ----------------------------------------------------------------------------
GRTempo::GRTempo( GRStaff * staff, const ARTempo * inAR ) : GRTagARNotationElement( inAR, LSPACE )
{
	assert(inAR);

	// Sets the Type -> This tag is a SystemTag.
	setTagType(GRTag::SYSTEMTAG);
	mPosition.y -= 2 * LSPACE; // Tempo is usually placed two linespaces above the staff.

	VGDevice * hdc = gGlobalSettings.gDevice;
	mBoundingBox.Set (0, -2*LSPACE, 0, 0);

	fTextAlign = VGDevice::kAlignLeft + VGDevice::kAlignBase;
	fFont = FontManager::GetTextFont(inAR, staff->getStaffLSPACE(), fTextAlign);
	fFormat = inAR->getTextFormat();

	float fsize = inAR->getFSize();
	fNoteScale = NoteDrawer::GetScaling (fsize);
	fMusicFont = NoteDrawer::GetMusicFont(fNoteScale);

	for (auto l : inAR->getTempoMark()) {
		if (l.second == FormatStringParser::kSpecial) {
			TYPE_DURATION duration = inAR->string2Duration(l.first.c_str());
			mBoundingBox.right += GetSymbolExtent( kFullHeadSymbol );
			if (duration.getNumerator() == 3) mBoundingBox.right += LSPACE;
		}
		else if( hdc ) {
			float w, h;
			const char * str = l.first.c_str();
			fFont->GetExtent( str, int(l.first.size()), &w, &h, hdc );
			mBoundingBox.right += w;
		}
	}
	setGRStaff(staff);
	fDate = inAR->getRelativeTimePosition();
	fYAlign = getYAlign(fsize);
	fXAlign = getXAlign();
}

// ----------------------------------------------------------------------------
float GRTempo::getXPos() const
{
	float x = getOffset().x;
	if (fDate) {
		NEPointerList * elts = getGRStaff()->getElements();
		GuidoPos pos = elts->GetHeadPosition();
		while (pos) {
			GRNotationElement * e = elts->GetNext(pos);
			if ((e->getRelativeTimePosition() >= fDate) && e->isGREvent()) {
				x += e->getPosition().x - mPosition.x;
				if (e->isGREvent()) x -= e->getBoundingBox().Width()/2;
				break;
			}
		}
	}
	return x;
}

// ----------------------------------------------------------------------------
void GRTempo::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow) return;

    const ARTempo *ar = static_cast<const ARTempo *>(mAbstractRepresentation);
    if (!ar) return;

    VGColor prevFontColor = hdc.GetFontColor();
    if (mColRef) hdc.SetFontColor(VGColor(mColRef));

	float space = getGRStaff()->getStaffLSPACE() / 2 * fNoteScale;
	float currX = getXPos() + fXAlign;
//	float dy = ar->getDY() ? - ar->getDY()->getValue(LSPACE) : 0.f;
	float dy = - ar->getDY()->getValue(LSPACE);
	for (auto l : ar->getTempoMark()) {
		if (l.second == FormatStringParser::kSpecial) {
			NoteDrawer nd (fMusicFont, mPosition, fYAlign);
			currX += nd.DrawNote( hdc, ar->string2Duration(l.first.c_str()), currX, dy ) + space;
		}
		else {
			currX += DrawText( hdc, l.first.c_str(), currX, dy ) + space;
		}
	}
    if (mColRef) hdc.SetFontColor(prevFontColor);
}

// ----------------------------------------------------------------------------
float GRTempo::DrawText( VGDevice & hdc, const char * cp, float xOffset, float yOffset) const
{
	hdc.SetTextFont( fFont );
    hdc.SetFontAlign (getTextAlign());
	hdc.DrawString ( xOffset + mPosition.x, yOffset + mPosition.y, cp, (int)strlen(cp) );

	float h, width;
	fFont->GetExtent(cp, (int)strlen(cp), &width, &h, &hdc);
	return width;
}

// ----------------------------------------------------------------------------
unsigned int GRTempo::getTextAlign() const
{
	// horizontal alignment is ignored - achieved manually using xoffset
	return fTextAlign & 0x07 + VGDevice::kAlignLeft;
}

// ----------------------------------------------------------------------------
float GRTempo::getYAlign(float fsize) const
{
	if (fFormat.size() == 2) {
		switch (fFormat[1]) {
			case 't':	return fsize * 0.8f;
			case 'c':	return 0.f;
			case 'b':	return -fsize/3.f;
		}
	}
	return 0.f;
}

// ----------------------------------------------------------------------------
float GRTempo::getXAlign() const
{
	if (fFormat.size() == 2) {
		switch (fFormat[0]) {
			case 'l':	return 0.f;
			case 'c':	return -mBoundingBox.Width() / 2;
			case 'r':	return -mBoundingBox.Width();
		}
	}
	return 0.f;
}
