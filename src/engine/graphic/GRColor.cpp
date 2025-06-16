/*
  GUIDO Library
  Copyright (C) 2025 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <iostream>
using namespace std;

#include "ARColor.h"
#include "GRColor.h"

//----------------------------------------------------------------------------------
GRColor::GRColor( const ARColor* ar) : GRARNotationElement (ar) {
	fColor = ar->getColor();
}

//----------------------------------------------------------------------------------
void GRColor::OnDraw ( VGDevice & hdc ) const
{
	hdc.SelectPenColor(fColor);
	hdc.SelectFillColor(fColor);
	hdc.SetFontColor(fColor);
}
