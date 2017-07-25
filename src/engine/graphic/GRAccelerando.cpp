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

#include "ARAccelerando.h"
#include "GRStaff.h"
#include "GRAccelerando.h"
#include "VGDevice.h"
#include "GRSingleNote.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "FontManager.h"

extern GRSystem * gCurSystem;

GRAccelerando::GRAccelerando( GRStaff * inStaff, const ARAccelerando * artrem )
  : GRPTagARNotationElement(artrem)
{
  assert(inStaff);
  GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
  sse->grsystem = inStaff->getGRSystem(); 
  sse->startflag = GRSystemStartEndStruct::LEFTMOST;
  
  sse->p = (void *) getNewGRSaveStruct();
  
  mStartEndList.AddTail(sse);
  
  if(artrem->getTempo()) {
	  tempo1 = artrem->getTempo()->getValue();

	  if (tempo1 != "")
		   isTempoSet = true;
	  else
		  isTempoSet = false;
  }
  else
	  isTempoSet = false;

  if (artrem->getAbsTempo()) {
	  tempo2 = artrem->getAbsTempo()->getValue();

	  if (tempo2 != "")
		  isTempoAbsSet = true;
	  else
		  isTempoAbsSet = false;
  }
  else
	  isTempoAbsSet = false;

  if (artrem->getDX())
	  mdx = artrem->getDX()->getValue();
  else
      mdx = 0;

  if (artrem->getDY())
	  mdy = artrem->getDY()->getValue();
  else
      mdy = 0;

  float curLSPACE = LSPACE;

  if (inStaff)
	  curLSPACE = inStaff->getStaffLSPACE();

//  mFontSize = artrem->getFSize(curLSPACE);
  mFontSize = artrem->getFSize() * curLSPACE / LSPACE;

  if (mFontSize == 0)
	  mFontSize = (int)(1.5f * LSPACE);

  font       = new NVstring(artrem->getFont());
  fontAttrib = new NVstring(artrem->getTextAttributes());
//  fontAttrib = new NVstring(artrem->getFAttrib());
}

unsigned int GRAccelerando::getTextAlign() const
{
	return (VGDevice::kAlignLeft | VGDevice::kAlignTop);
}

void GRAccelerando::OnDraw( VGDevice & hdc ) const
{
	if(!mDraw || !mShow)
        return;
	
	assert(gCurSystem);

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0)
		return; // don't draw

	float xStart = startPos.x;
	float xEnd   = endPos.x;

	// - Setup font ....
	const VGFont *hTextFont;

	if (font && font->length() > 0)
		hTextFont = FontManager::FindOrCreateFont(mFontSize, font, fontAttrib);
	else
		hTextFont = FontManager::gFontText;

	// set up color
	if (mColRef) {
		VGColor color (mColRef); 	// custom or black
		hdc.PushFillColor(color);
		hdc.PushPen(color, 1);
		hdc.SetFontColor(color);
	}

	if (isTempoSet && sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
		std::string toPrint ("= ");
		toPrint += tempo1;
		toPrint += " accel.";
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

        hdc.SetTextFont(hTextFont);
		hdc.DrawString(getPosition().x + LSPACE, getPosition().y, t1, (int)n);
		
        xStart += (n - 4) * LSPACE / 2;
	}
	else if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
        hdc.SetTextFont(hTextFont);
		hdc.DrawString(getPosition().x, getPosition().y, "accel.", 6);
    }

	if (isTempoAbsSet && sse->endflag == GRSystemStartEndStruct::RIGHTMOST)
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

        hdc.SetTextFont(hTextFont);
		hdc.DrawString(endPos.x - (n - 1) * LSPACE, endPos.y, t2, (int)n);

		xEnd -= (n + 1) * LSPACE;
	}

	if (sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		xEnd = sse->endElement->getPosition().x;
	else if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		xStart = sse->startElement->getPosition().x;

    hdc.PushPenWidth(2);

	while (xStart < xEnd) {
		if (xStart + LSPACE > xEnd)
			hdc.Line(xStart, startPos.y, xEnd, endPos.y);
		else
			hdc.Line(xStart, startPos.y, xStart + LSPACE, endPos.y);

		xStart += 2 * LSPACE;
	}
	
	if (mColRef) {
		hdc.PopPen();
		hdc.PopFillColor();
		hdc.SetFontColor(VGColor()); //black
	}

    hdc.PopPenWidth();
}

void GRAccelerando::tellPosition(GObject * caller, const NVPoint & np)
{
	if (caller != /*dynamic cast<GObject *>*/(getAssociations()->GetTail())) // useless dynamic cast ?
		return;

	GRSingleNote * note = dynamic_cast<GRSingleNote *>(getAssociations()->GetHead());
	if (note)
		startPos = note->getStemStartPos();

	GRSingleNote * noteEnd = dynamic_cast<GRSingleNote *>(getAssociations()->GetTail());
	if(noteEnd)
		endPos = noteEnd->getStemStartPos();
/*
	float minY = startPos.y;
	for(int i=1; i<= getAssociations()->GetCount(); i++)
	{
		GRSingleNote * n = dynamic_cast<GRSingleNote *>(getAssociations()->Get(i));
		if(n && n->getStemDirection() == 1)
			minY = std::min(minY, n->getStemStartPos().y);
		else if(n)
			minY = std::min(minY, n->getPosition().y - LSPACE/2);
	}
	if(minY > 0)
		minY = 0;
*/	
	float maxY = startPos.y;
	for(int i=1; i<= getAssociations()->GetCount(); i++)
	{
		GRSingleNote * n = dynamic_cast<GRSingleNote *>(getAssociations()->Get(i));
		if(n && n->getStemDirection() == -1)
			maxY = std::max(maxY, n->getStemStartPos().y);
		else if (n)
			maxY = std::max(maxY, n->getPosition().y + LSPACE);
	}
	if(maxY<5*LSPACE)
		maxY = 5*LSPACE;
	
	startPos.y = endPos.y = maxY+LSPACE-mdy;
	startPos.x += mdx;
	endPos.x += mdx;

	setPosition(startPos);
	startPos.x += 4.5f * LSPACE;
}
