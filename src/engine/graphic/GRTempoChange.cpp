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

#include "TempoChange.h"
#include "FontManager.h"
#include "GRTempoChange.h"
#include "GRSingleNote.h"
#include "GRStaff.h"
#include "TagParameterFloat.h"
//#include "TagParameterString.h"
#include "VGDevice.h"

extern GRSystem * gCurSystem;

//----------------------------------------------------------------------------------------
GRTempoChange::GRTempoChange( GRStaff * inStaff, const TempoChange * ar, const char* text )
  : GRPTagARNotationElement(ar), fText (text)
{
	assert(inStaff);

	setGRStaff(inStaff);
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);

	tempo1 = ar->getBefore();
	tempo2 = ar->getAfter();
//	if(ar->getTempo()) {
//		tempo1 = ar->getTempo()->getValue();
//		if (tempo1 != "") isTempoSet = true;
//	}

//	if (ar->getAbsTempo()) {
//		tempo2 = ar->getAbsTempo()->getValue();
//		if (tempo2 != "") isTempoAbsSet = true;
//	}
	mdx = ar->getDX()->getValue();
	mdy = ar->getDY()->getValue();
	float curLSPACE = inStaff ? inStaff->getStaffLSPACE() : LSPACE;

	fTextAlign = VGDevice::kAlignLeft | VGDevice::kAlignTop;
	fFont = FontManager::GetTextFont(ar, curLSPACE, fTextAlign);
}

//----------------------------------------------------------------------------------------
void GRTempoChange::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow) return;
	
	assert(gCurSystem);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0) return;

	float xStart = startPos.x;
	float xEnd   = endPos.x;

	// set up color
	if (mColRef) {
		VGColor color (mColRef); 	// custom or black
		hdc.PushFillColor(color);
		hdc.PushPen(color, 1);
		hdc.SetFontColor(color);
	}

	float curLSpace = getGRStaff()->getStaffLSPACE();
	if (tempo1.size() && sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
		std::string toPrint ("= ");
		toPrint += tempo1;
		toPrint += " " + fText;
		const char * t1 = toPrint.c_str();
		size_t n = toPrint.length();
		
		//to draw the little note
        float scaleFactor = 0.5f;
        hdc.selectfont(1); // Not very beautiful but avoid a bug during SVG export
		hdc.SetScale(scaleFactor, scaleFactor);
		hdc.DrawMusicSymbol(2 * getPosition().x, 2 * getPosition().y, kFullHeadSymbol);

		float y = 2 * getPosition().y;
		for (int i = 0; i < 3; i++) {
			hdc.DrawMusicSymbol(2 * getPosition().x, y, kStemUp2Symbol);
			y -= LSPACE;
		}
        hdc.SetScale(1 / scaleFactor, 1 / scaleFactor);
        hdc.SetTextFont(fFont);
		hdc.DrawString(getPosition().x + LSPACE, getPosition().y, t1, (int)n);
        xStart += (n - 4) * LSPACE / 2;
	}
	else if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
        hdc.SetTextFont(fFont);
		hdc.DrawString(getPosition().x, getPosition().y, fText.c_str(), int(fText.size()));
    }

	if (tempo2.size() && sse->endflag == GRSystemStartEndStruct::RIGHTMOST)
	{
		std::string toPrint2 ("= ");
		toPrint2 += tempo2;
		const char * t2 = toPrint2.c_str();
		size_t n = toPrint2.length();

        //to draw the little note
        float scaleFactor = 0.5f;
        hdc.selectfont(1); // Not very beautiful but avoid a bug during SVG export
		hdc.SetScale(scaleFactor, scaleFactor);
		hdc.DrawMusicSymbol(2 * (endPos.x - n * LSPACE), 2 * endPos.y, kFullHeadSymbol);
		
        float y = 2 * endPos.y;
		for (int i = 0; i < 3; i++) {
			hdc.DrawMusicSymbol(2 * (endPos.x - n * LSPACE), y, kStemUp2Symbol);
			y -= LSPACE;
		}
        hdc.SetScale(1 / scaleFactor, 1 / scaleFactor);
        hdc.SetTextFont(fFont);
		hdc.DrawString(endPos.x - (n - 1) * LSPACE, endPos.y, t2, (int)n);
		xEnd -= (n + 1) * LSPACE;
	}

	if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		xEnd = sse->endElement->getPosition().x;
	else if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		xStart = sse->startElement->getPosition().x;

    hdc.PushPenWidth(2);
	float len = curLSpace * 0.7;
	while (xStart < xEnd) {
		float x2 = (xStart + len > xEnd) ? xEnd : xStart + len;
		hdc.Line(xStart, startPos.y, x2, endPos.y);
		xStart += 2 * len;
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
