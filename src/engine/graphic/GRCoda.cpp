/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
using namespace std;

#include "ARCoda.h"
#include "GRCoda.h"

//----------------------------------------------------------------------------------
GRCoda::GRCoda( const ARCoda * ar ) : GRJump( ar, false )
{
	const float extent = GetSymbolExtent( kCodaSymbol );
	const float adjust = extent * ( getSymbolSize() - 1);
	mBoundingBox.right += adjust;
}

//----------------------------------------------------------------------------------
float GRCoda::getSymbolSize () const	{ return 1.3333f; }

