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
#include "VGDevice.h"
#include "VGFont.h"

using namespace std;

GRTab::GRTab( GRStaff * staff, const ARTab * tab, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration)
  : GRSingleNote (staff,tab,date,duration)
{
	float lspace = staff->getStaffLSPACE();
	int pos = tab->getString() - 1;
	mSize = lspace * 1.3f;
	mPosition.y = lspace * pos + mSize / 3;
	fDisplay = tab->getDisplay();
}


//---------------------------------------------------------
void GRTab::OnDraw( VGDevice & hdc ) const
{
	const GRStaff* staff = getGRStaff();
	float y = staff->getPosition().y + mPosition.y;
	cerr << "GRTab::OnDraw " << this << " pos: " << getPosition() << endl;
	const VGFont* font = FontManager::FindOrCreateFont( mSize, "Helvetica");
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
//	const GRNEList& articulations = getArticulations();
//	for (GRNEList::const_iterator ptr = articulations.begin(); ptr != articulations.end(); ++ptr) {
//		GRNotationElement *el = *ptr;
//		el->OnDraw(hdc);
//	}
}

//---------------------------------------------------------
const ARTab * GRTab::getARTab() const 	{ return static_cast<const ARTab*>(getAbstractRepresentation()); }
