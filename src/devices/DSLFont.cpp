/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "DSLFont.h"

#ifdef __APPLE__
#include "GFontOSX.h"
#include "GSystemOSX.h"
GSystemOSX gDSLSystem (0, 0);

#elif defined (WIN32)
#include "GFontWin32.h"
#include "GSystemWin32.h"
GSystemWin32 gDSLSystem (0, 0);

#elif linux
#include "CairoSystem.h"
CairoSystem gDSLSystem (0);

#else
#error "unknown system for compiling DSLFont"

#endif



//______________________________________________________________________________
DSLFont::DSLFont(const char * name, int size, int properties) 
{
	fDevice = gDSLSystem.CreateMemoryDevice (10,10);
	fFont = gDSLSystem.CreateVGFont (name, size, properties);
}

DSLFont::~DSLFont() { delete fFont; delete fDevice; }

//______________________________________________________________________________
void DSLFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
	fFont->GetExtent (s, inCharCount, outWidth, outHeight, fDevice);
}

//______________________________________________________________________________
void DSLFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	fFont->GetExtent( c, outWidth, outHeight, fDevice );
}
