/*
  GUIDO Library
  Copyright (C) 2023 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "ARTab.h"
#include "FontManager.h"
#include "GRArticulation.h"
#include "GRTab.h"
#include "GRStaff.h"
#include "GUIDOInternal.h"
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

GRTab::GRTab( GRStaff * staff, const ARTab * tab, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration)
  : GRSingleNote (staff,tab,date,duration)
{
	float lspace = staff->getStaffLSPACE();
	int pos = tab->getString() - 1;
	fSize = lspace * 1.1f;
	mPosition.y = lspace * pos + fSize / 3;
	float offset = fSize / 2;
	mBoundingBox.top = -offset * 0.85;
	mBoundingBox.bottom = mBoundingBox.top + fSize;
	mBoundingBox.left = -offset;
	mBoundingBox.right = offset;
	if (tab->isTied()) {
		string p("(");
		fDisplay = p + tab->getDisplay() + ")";
	}
	else fDisplay = tab->getDisplay();
	mNoteBreite = fSize;
}


//---------------------------------------------------------
void GRTab::OnDraw( VGDevice & hdc ) const
{
	const GRStaff* staff = getGRStaff();
	float y = staff->getPosition().y + mPosition.y;
	const VGFont* font = FontManager::FindOrCreateFont( fSize, "Helvetica");
	hdc.SetTextFont( font );
	hdc.SetFontAlign( VGDevice::kAlignBase );
	VGColor	back = hdc.GetFontBackgroundColor();
	hdc.SetFontBackgroundColor(VGColor(255, 255, 255));

	float w, h;
	font->GetExtent( fDisplay.c_str(), fDisplay.size(), &w, &h, &hdc);
	hdc.DrawString( mPosition.x - (w/2), mPosition.y, fDisplay.c_str(), fDisplay.size() );
	hdc.SetFontBackgroundColor(back);

	// - draw articulations & ornament
	for (auto elt: getArticulations()) {
		elt->OnDraw(hdc);
	}

	if (gBoundingBoxesMap & kEventsBB) 	DrawBoundingBox(hdc, kEventBBColor);
}

//---------------------------------------------------------
const ARTab * GRTab::getARTab() const 	{ return static_cast<const ARTab*>(getAbstractRepresentation()); }


//____________________________________________________________________________________
void GRTab::updateBoundingBox()
{
//	mBoundingBox -= getOffset();
//
//	float scale = getSize();
	mMapping.left 	= mPosition.x + mBoundingBox.left;
	mMapping.right 	= mPosition.x + mBoundingBox.right;
	mMapping.top 	= mPosition.y + mBoundingBox.top;
	mMapping.bottom = mPosition.y + mBoundingBox.bottom;
	mMapping += getOffset();
}
