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

#include <algorithm>
#include <string.h>

#include "ARTempo.h"
#include "FontManager.h"
#include "GRSingleNote.h"
#include "GRStaff.h"
#include "GRTempoChange.h"
#include "GUIDOInternal.h"
#include "NoteDrawer.h"
#include "TagParameterFloat.h"
#include "TempoChange.h"
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

extern GRSystem * gCurSystem;

//----------------------------------------------------------------------------------------
GRTempoChange::GRTempoChange( GRStaff * inStaff, const TempoChange * ar, const char* text )
  : GRPTagARNotationElement(ar), fText (text), fTempoChge(ar)
{
	assert(inStaff);

	setGRStaff(inStaff);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);

	mdx = ar->getDX()->getValue();
	mdy = ar->getDY()->getValue();
	float curLSPACE = inStaff ? inStaff->getStaffLSPACE() : LSPACE;

	fTextAlign = VGDevice::kAlignLeft | VGDevice::kAlignTop;
	fFont = FontManager::GetTextFont(ar, curLSPACE, fTextAlign);

	float fsize = ar->getFSize();
	fNoteScale = NoteDrawer::GetScaling (fsize);
	fMusicFont = NoteDrawer::GetMusicFont(fNoteScale);
	fYAlign = getYAlign (fsize);

	VGDevice * hdc = gGlobalSettings.gDevice;
	fBeforeWidth = getFormatLength (hdc, curLSPACE, fTempoChge->getBefore());
	fAfterWidth = getFormatLength (hdc, curLSPACE, fTempoChge->getAfter());
}

// ----------------------------------------------------------------------------
float GRTempoChange::getFormatLength (VGDevice * hdc, float lspace, const FormatStringParserResult& list) const
{
	float w, h, result = 0;
	for (auto l : list) {
		if (l.second == FormatStringParser::kSpecial)
			result += lspace;
		else {
			fFont->GetExtent( l.first.c_str(), int(l.first.size()), &w, &h, hdc);
			result += w;
		}
	}
	return result;
}

// ----------------------------------------------------------------------------
float GRTempoChange::getYAlign(float fsize) const
{
	string format = fTempoChge->getTextFormat();
	if (format.size() == 2) {
		switch (format[1]) {
			case 't':	return fsize * 0.8f;
			case 'c':	return 0.f;
			case 'b':	return -fsize/3.f;
		}
	}
	return 0.f;
}

//----------------------------------------------------------------------------------------
float GRTempoChange::getXAlign(VGDevice & hdc) const
{
	float tlen, h;
	fFont->GetExtent(fText.c_str(), int(fText.size()), &tlen, &h, &hdc);
	string format = fTempoChge->getTextFormat();
	if (format.size() == 2) {
		switch (format[0]) {
			case 'l':	return 0;
			case 'c':	return -tlen / 2;
			case 'r':	return -tlen;
		}
	}
	return 0.f;
}

//----------------------------------------------------------------------------------------
float GRTempoChange::DrawText( VGDevice & hdc, const char * cp, float xOffset, float yOffset, bool alignleft) const
{
	hdc.SetTextFont( fFont );
    hdc.SetFontAlign (alignleft ? VGDevice::kAlignLeft + fTextAlign & 0x07 : fTextAlign);
	hdc.DrawString ( xOffset + mPosition.x, yOffset + mPosition.y, cp, (int)strlen(cp) );

	float h, width;
	fFont->GetExtent(cp, (int)strlen(cp), &width, &h, &hdc);
	return width;
}

//----------------------------------------------------------------------------------------
float GRTempoChange::DrawFormatString(VGDevice& hdc, float offset, float dy, const FormatStringParserResult& list) const
{
	const float space = 10;
	for (auto l : list) {
		if (l.second == FormatStringParser::kSpecial) {
			NoteDrawer nd (fMusicFont, mPosition, fYAlign);
			offset += nd.DrawNote( hdc, ARTempo::string2Duration(l.first.c_str()), offset, dy ) + space;
		}
		else {
			offset += DrawText( hdc, l.first.c_str(), offset, dy, true ) + space;
		}
	}
	return offset;
}

//----------------------------------------------------------------------------------------
void GRTempoChange::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow) return;
	assert(gCurSystem);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0) return;

	// set up color
	if (mColRef) {
		VGColor color (mColRef); 	// custom or black
		hdc.PushFillColor(color);
		hdc.PushPen(color, 1);
		hdc.SetFontColor(color);
	}

	float currX = mPosition.x;
	float xEnd   = endPos.x + fTempoChge->getDx2();
	float curLSpace = getGRStaff()->getStaffLSPACE();
	float dy = - fTempoChge->getDY()->getValue(curLSpace);
	float space = curLSpace / 2 * fNoteScale;
	float xoffset =  getXAlign(hdc);

	const FormatStringParserResult& before = fTempoChge->getBefore();
	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
		if (before.size()) {
			float offset = xoffset - fBeforeWidth - space;
			if (fTextAlign & VGDevice::kAlignRight) offset -= fYAlign;
			else if (fTextAlign & VGDevice::kAlignCenter) offset -= fYAlign/2;
			DrawFormatString(hdc, offset, dy, before);
		}
		currX += DrawText( hdc, fText.c_str(), 0, dy ) + xoffset + space * 2;
	}

	const FormatStringParserResult& after  = fTempoChge->getAfter();
	if (after.size() && sse->endflag == GRSystemStartEndStruct::RIGHTMOST) {
		float offset = xEnd - mPosition.x + space;
		if (fTextAlign & VGDevice::kAlignRight) offset -= fYAlign;
		if (fTextAlign & VGDevice::kAlignCenter) offset -= fYAlign/2;
		DrawFormatString(hdc, offset, dy, after);
	}

	if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		xEnd = sse->endElement->getPosition().x;
	else if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		currX = sse->startElement->getPosition().x;

    hdc.PushPenWidth(4*fNoteScale);
	float len = curLSpace * fNoteScale;
	float y = mPosition.y + dy + fYAlign - 10;
	while (currX < xEnd) {
		float x2 = (currX + len > xEnd) ? xEnd : currX + len;
		hdc.Line(currX, y, x2, y);
		currX += 2 * len;
	}
	
	if (mColRef) {
		hdc.PopPen();
		hdc.PopFillColor();
		hdc.SetFontColor(VGColor()); //black
	}

    hdc.PopPenWidth();
}

//----------------------------------------------------------------------------------------
void GRTempoChange::tellPosition(GObject * caller, const NVPoint & np)
{
	if (caller != getAssociations()->GetTail()) return;

	const GRSingleNote * note = getAssociations()->GetHead()->isSingleNote();
	if (note) 	startPos = note->getStemStartPos();

	const GRSingleNote * noteEnd = getAssociations()->GetTail()->isSingleNote();
	if (noteEnd) endPos = noteEnd->getStemStartPos();

	float maxY = startPos.y;
	for(int i=1; i<= getAssociations()->GetCount(); i++) {
		GRSingleNote * n = dynamic_cast<GRSingleNote *>(getAssociations()->Get(i));
		if(n && n->getStemDirection() == -1)
			maxY = std::max(maxY, n->getStemStartPos().y);
		else if (n)
			maxY = std::max(maxY, n->getPosition().y + LSPACE);
	}
	if (maxY < 5*LSPACE) maxY = 5*LSPACE;
	
	startPos.y = endPos.y = maxY+LSPACE-mdy;
	startPos.x += mdx;
	endPos.x += mdx;

	setPosition(startPos);
	startPos.x += 2.5f * LSPACE;
}
