/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "AROctava.h"

#include "GROctava.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRDefine.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"	// for gFontDefs.gFontText
#include "VGDevice.h"
#include "VGFont.h"
#include "GRSpecial.h"
#include "GRSingleNote.h"

using namespace std;

extern GRSystem * gCurSystem;

GROctava::GROctava( GRStaff * staff, const NVstring & text, bool bassa)
	: fStaff(staff), fText (text), fBassa(bassa), fSegmentsCount(0) //, fElement(0)
{
	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = staff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);

	mNeedsSpring = 1;
	float x = 0;
	float y = 0;
	if( gGlobalSettings.gDevice )
		FontManager::gFontText->GetExtent( fText.c_str(), fText.length(), &x, &y, gGlobalSettings.gDevice );

	mBoundingBox.right = (GCoord)(x);
	mBoundingBox.top = (GCoord)(y - staff->getStaffLSPACE());
	mBoundingBox.left -= (GCoord)(LSPACE/4);
	mBoundingBox.bottom = (GCoord)(4*LSPACE);
	fTextHeight = y - 10;
}

GROctava::~GROctava()
{
	mAssociated = 0;
}

void GROctava::GGSOutput() const
{
}

void GROctava::setColRef(const TagParameterString *tps) {
    if (!mColRef)
        mColRef = new unsigned char[4];

    tps->getRGB(mColRef);
}

//---------------------------------------------------------------------------------
NVRect GROctava::getExtensionLine (const NEPointerList * assoc, int num) const
{
	NVRect outRect;
	float space = fStaff->getStaffLSPACE();
	float staffTop		= fStaff->getPosition().y - space * 0.5;
	float staffBottom	= staffTop + fStaff->getDredgeSize() + space * 2.5;
	GuidoPos pos = assoc->GetHeadPosition();
	float currentXPos = 0;
	while (pos) {
		const GRNotationElement* el = assoc->GetNext(pos);
		const GRSingleNote * note = dynamic_cast<const GRSingleNote *>(el);
		if (note) {

			NVRect bb = note->getEnclosingBox();
			if (bb.left < currentXPos) {		// there is a line break
				if (!num) {						// and it is the required segment
					outRect.right = gCurSystem->getPosition().x + gCurSystem->getBoundingBox().Width();
					return outRect;
				}
				num--;
			}
			if (!num) {					// this is the required segment
				if (!outRect.left)		// set the left border
					outRect.left = bb.left + space;
				outRect.right = bb.right + space /3;	// and adjust the right border
				if (fBassa) {
					float y = bb.bottom + space * 1.7;
					float bottom = (y > staffBottom) ? y : staffBottom;
					if (bottom > outRect.bottom) outRect.top = outRect.bottom = bottom;
				}
				else {
					float y = bb.top - space * 0.2;
					float top = (y < staffTop) ? y : staffTop;
					if (top < outRect.top) outRect.top = outRect.bottom = top - fTextHeight;
				}
			}
			currentXPos = bb.right;
		}
	}
	return outRect;
}

//---------------------------------------------------------------------------------
int GROctava::countSegments()
{
	if (fSegmentsCount) return fSegmentsCount;
	float currentXPos = 0;
	int fSegmentsCount = 1;
	GuidoPos pos = getAssociations()->GetHeadPosition();
	while (pos) {
		const GRNotationElement* el = getAssociations()->GetNext(pos);
		const GRSingleNote * note = dynamic_cast<const GRSingleNote *>(el);
		if (note) {
			float nx = note->getPosition().x;
			if (nx < currentXPos) fSegmentsCount++;
			currentXPos = nx;
		}
	}
	return fSegmentsCount;
}

//---------------------------------------------------------------------------------
void GROctava::OnDraw( VGDevice & hdc) const
{
	if (fText.empty()) return;		// nothing to draw

	static int drawingstate = 0;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	int nsegments = const_cast<GROctava*>(this)->countSegments();

	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT && (nsegments == 1))
		return;		// nothing to do : this is due to the bar before the octava change
	
	bool endSegment =   (sse->endflag == GRSystemStartEndStruct::NOTKNOWN) || (nsegments == 1);
	float space = fStaff->getStaffLSPACE() / 2;
	NVRect r = getExtensionLine (getAssociations(), drawingstate);
	VGColor savedColor = hdc.GetFontColor();
	VGColor color;	// custom or black
   if (mColRef) {
		color = VGColor(mColRef);
		hdc.SetFontColor(color);
	}
	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST) {
		NVPoint pos (r.left - fStaff->getStaffLSPACE(), r.top);
		pos.y += fBassa ? -5 : fTextHeight - 5;
		OnDrawText(hdc, pos, fText.c_str(), int(fText.length()) );
	}
	
	// next, draw the extension lines
    if (mColRef) hdc.PushPenColor(color);
	hdc.PushPenWidth(6);
	float xStart = r.left;
	float xEnd = r.right;
	float linespace = drawingstate ? 0 : fStaff->getStaffLSPACE() / 4;
	while (xEnd >= (xStart + linespace)) {
		if ((xEnd == r.right) && endSegment)		// is it the last extension line ?
			hdc.Line(xEnd, r.top, xEnd, fBassa ? r.top - space : r.top + space);
		hdc.Line(xEnd - space, r.top, xEnd, r.top);
		xEnd -= 2.5 * space;
	}
	hdc.PopPenWidth();
    if (mColRef) {
		hdc.PopPenColor();
        hdc.SetFontColor(savedColor);
	}
	if (endSegment) drawingstate = 0;
	else drawingstate++;
}

//void GROctava::addAssociation(GRNotationElement * el)
//{
//	GRNotationElement::addAssociation(el);
//}

void GROctava::tellPosition(GObject * caller, const NVPoint & position)
{
// useless: contextual information seems not to be correct
//	setPosition (NVPoint(left, y));
}
