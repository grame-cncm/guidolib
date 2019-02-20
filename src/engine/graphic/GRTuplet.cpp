/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/*
	Tuplet positionning:
		
		Manual: a least one parameter was specified in a \tuplet tag. Keeps
				compatibility with older versions of the library. 

		Automatic: decides if tuplet braces are required or not, decides
				the "direction" of the tuplet marks (above or below the note group),
				minimize collisions.
*/

#include <iostream>
#include <string.h>
#include <sstream>

#include "ARTuplet.h"

#include "GRTuplet.h"
#include "GRStaff.h"
#include "GRSingleNote.h"
#include "GRPage.h"
#include "GRVoice.h"
#include "VGDevice.h"
#include "VGFont.h"
#include "FontManager.h"
#include "secureio.h"

using namespace std;

// ----------------------------------------------------------------------------
GRTuplet::GRTuplet ( GRStaff * inStaff, const ARTuplet * artuplet )
				   : GRPTagARNotationElement(artuplet)
{
	assert(artuplet);
	assert(inStaff);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	GRTupletSaveStruct * st = new GRTupletSaveStruct;
	sse->p = (void *) st;

	mStartEndList.AddTail(sse);
	mDirection = dirUP;
	mShowLeftBrace  = artuplet->getLeftBrace();
	mShowRightBrace = artuplet->getRightBrace();
//	mShowLeftBrace = false;
//	mShowRightBrace = false;
	mDirection = (artuplet->isPositionAbove() < 0) ? dirDOWN : dirUP;

	const int numerator = artuplet->getNumerator();
	if (numerator > 0) {
		std::stringstream textStream;
		const int denominator = artuplet->getDenominator();
		if (denominator > 0)
			textStream << numerator << ":" << denominator;
		else
			textStream << numerator;
		fText = textStream.str();
	}
}

// ----------------------------------------------------------------------------
GRTuplet::~GRTuplet()
{
	delete mAssociated;	// base destructor does a more complex job, should be removed ? 
	mAssociated = 0;
}

// ----------------------------------------------------------------------------
const ARTuplet * GRTuplet::getARTuplet() const	{ return static_cast<const ARTuplet *>(mAbstractRepresentation); }

// ----------------------------------------------------------------------------
void GRTuplet::tellPosition(GObject * caller, const NVPoint & inPos )
{
	// start to make sure that params and env is correct
	GREvent * event = GREvent::cast( caller );
	if( event == 0 ) return;
	GRStaff * staff = event->getGRStaff();
	if( staff == 0 ) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if( sse == 0 ) return;

	// start to compute the automatic position
	bool done = automaticPosition( event, inPos, staff, sse );
	if(done && getARTuplet()->isDySet()) {
		manualPosition( event, inPos, staff, sse );
	}
}

// ----------------------------------------------------------------------------
// manual position is computed as an offset to the regular position
void GRTuplet::manualPosition(GREvent * event, const NVPoint & inPos, const GRStaff * staff, GRSystemStartEndStruct * sse )
{
	GRTupletSaveStruct * st = (GRTupletSaveStruct *)sse->p;

	const ARTuplet * arTuplet = getARTuplet();
	st->p1.y -= arTuplet->getDy1();
	st->p2.y -= arTuplet->getDy2();
	st->textpos.x = (st->p2.x - st->p1.x) * 0.5f + st->p1.x;
	float height = st->p2.y - st->p1.y;
	st->textpos.y = height * 0.5f + st->p1.y + 40;
}

// ----------------------------------------------------------------------------
bool GRTuplet::getElementsBoundingBox (NVRect& outRect) const
{
	NVRect r;
	int stemsUp		= 0;
	int stemsDown	= 0;
	GuidoPos pos = mAssociated->GetHeadPosition();
	while( pos )
	{
		GRNotationElement* elt = mAssociated->GetNext(pos);
		const GRSingleNote * note = elt->isSingleNote();
		if (note) {
			GDirection stemDir = note->getStemDirection();
			if( stemDir == dirUP ) stemsUp++;
			else if( stemDir == dirDOWN ) stemsDown++;
		
			NVRect nbb = note->getEnclosingBox(false, false);
			r.Merge(nbb);
		}
		else if (elt->isRest()) {
			NVRect nbb = elt->getBoundingBox();
			nbb += elt->getPosition();
			if (!nbb.Height()) nbb.bottom = nbb.top +1;		// hack to catch strange Merge behaviour
			r.Merge(nbb);
		}
	}
	outRect = r;
	return (stemsUp >= stemsDown);
}

// ----------------------------------------------------------------------------
/** Places the tuplet bracket and/or numeral, close to its group of notes.

	Calculates the positions of the two possible tuplet bracket (above 
	 and below) then choose the best one.
*/
bool GRTuplet::automaticPosition(GREvent * callerEv, const NVPoint & inPos, const GRStaff * staff, GRSystemStartEndStruct * sse )
{
	GRTupletSaveStruct * st = (GRTupletSaveStruct *)sse->p;
	GREvent * startElement = GREvent::cast( sse->startElement );
	if( !st || !startElement ) return false;
	GREvent * endElement = GREvent::cast( sse->endElement );
	if( endElement == 0 ) return false;		// computes the metrics at the end only

	// - Accept being positioned only once, if possible by the last tuplet element.
	if( callerEv != endElement )
		return false;
	
	const ARTuplet* ar = getARTuplet();
	int dir = ar->isPositionAbove();
	NVRect notesBB;
	bool above = getElementsBoundingBox(notesBB);
	if (dir) above = (dir == dirUP);

	float lspace = staff->getStaffLSPACE();
	st->p1.x = notesBB.left;
	st->p2.x = notesBB.right;
	if (above) {
		st->p1.y = ((notesBB.top < 0) ? notesBB.top : 0) - lspace; // * 1.2;
		st->p2.y = st->p1.y;
		mDirection = dirUP;
	}
	else {
		float staffBottom = staff->getDredgeSize();
		st->p1.y = ((notesBB.bottom > staffBottom) ? notesBB.bottom : staffBottom) + lspace; // * 1.2;
		st->p2.y = st->p1.y;
		mDirection = dirDOWN;
	}

	st->textpos.x = (st->p2.x - st->p1.x) * 0.5f + st->p1.x;
	st->textpos.y = st->p1.y + 40;

	mShowLeftBrace  = ar->getLeftBrace();
	mShowRightBrace = ar->getRightBrace();
	return true;
}

// ----------------------------------------------------------------------------
void GRTuplet::OnDraw(VGDevice & hdc) const
{ 
	if(!mDraw || !mShow) return;

	assert(gCurSystem);
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0) return;

    VGColor prevFontColor = hdc.GetFontColor();
    if (mColRef) hdc.SetFontColor(VGColor(mColRef));
	const ARTuplet * arTuplet = getARTuplet();
	GRTupletSaveStruct * st = (GRTupletSaveStruct *)sse->p;

	if (st->p1.x == st->p2.x)	return;		// nothing to draw: the metrics is an empty box

	size_t charCount = 0;
    float thickness = arTuplet->getThickness();
    float dxOffset  = (arTuplet->getDX() ? arTuplet->getDX()->getValue() : 0);

	// - Draws the number
	charCount = fText.size();
	if (charCount) {
        charCount = fText.size();

        const VGFont *font = 0;
        const NVstring fontName("Times New Roman");
        NVstring attrs ("i");
        if (arTuplet->isTextBold())
            attrs += "b";

        font = FontManager::FindOrCreateFont(int(80 * arTuplet->getTextSize()), &fontName, &attrs);
        hdc.SetTextFont(font);

        /* In order that numerator/denominator stays at the same vertical position even if size is changed */
        float xextent, yextent;
#ifdef SMUFL
        FontManager::gFontText->GetExtent(fText.c_str(), (int)charCount, &xextent, &yextent, &hdc);
#else
        FontManager::gFontScriab->GetExtent(fText.c_str(), (int)charCount, &xextent, &yextent, &hdc);
#endif
        int offset = int(yextent / 11.2 * arTuplet->getTextSize() - 40);
        /***************************************************************************************************/
		hdc.SetFontAlign(VGDevice::kAlignCenter | VGDevice::kAlignBottom);
		hdc.DrawString(st->textpos.x + dxOffset, st->textpos.y + offset, fText.c_str(), (int)charCount);
	}

	// - Draws the braces
	const float middleX = (st->p1.x + st->p2.x) * 0.5f;
	const float middleY = (st->p1.y + st->p2.y) * 0.5f;
	const float slope	= (st->p2.y - st->p1.y) / (st->p2.x - st->p1.x); //<- could be stored
    const float textSpace = (charCount + 0.5f) * LSPACE * 0.5f * arTuplet->getTextSize();

	hdc.PushPenWidth(thickness);
    if (mColRef) hdc.PushPenColor(VGColor(mColRef));
	float thickoffset = thickness * mDirection / LSPACE;
	if (mShowLeftBrace) { //arTuplet->getLeftBrace()) // (mBraceState & BRACELEFT)
		float p1X = st->p1.x + dxOffset;

		if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
			hdc.Line(p1X, st->p1.y + 0.5f * LSPACE * mDirection, p1X, st->p1.y - thickoffset );
		}
		if (charCount)
			hdc.Line(p1X, st->p1.y, middleX - textSpace + dxOffset, middleY - slope * textSpace );
		else
			hdc.Line(p1X, st->p1.y, st->p2.x + dxOffset, middleY - slope * textSpace );
	}

	if (mShowRightBrace) { //arTuplet->getRightBrace()) // (mBraceState & BRACERIGHT)
		float p2X = st->p2.x + dxOffset;
		if (charCount)
			hdc.Line(middleX + textSpace + dxOffset, middleY + slope * textSpace, p2X, st->p2.y);

		if (sse->endflag == GRSystemStartEndStruct::RIGHTMOST) {
			hdc.Line(p2X, st->p2.y - thickoffset, p2X, st->p2.y + 0.5f * LSPACE * mDirection);
		}
	}
	hdc.PopPenWidth();
    if (mColRef) {
        hdc.SetFontColor(prevFontColor);
        hdc.PopPenColor();
    }
}
