/*
  GUIDO Library
  Copyright (C) 2025 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "GRARNotationElement.h"
#include "VGDevice.h"

class ARColor;

/** \brief Voice color management
*/

class GRColor : public GRARNotationElement
{
	VGColor fColor;
	
public:
					 GRColor(const ARColor* color);
	virtual 		~GRColor() {}
	
	virtual void OnDraw( VGDevice & hdc ) const;
};


