/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "BinaryFont.h"

#ifdef INDEPENDENTSVG
#include "SVGFont.h"
#include "SVGSystem.h"
SVGSystem gBinarySystem (0, 0);
#elif __APPLE__
#include "GFontOSX.h"
#include "GSystemOSX.h"
GSystemOSX gBinarySystem (0, 0);

#elif defined (WIN32)
#include "GFontWin32.h"
#include "GSystemWin32.h"
GSystemWin32 gBinarySystem (0, 0);

#elif linux
#include "CairoSystem.h"
CairoSystem gBinarySystem (0);

#else
#error "unknown system for compiling BinaryFont"

#endif



//______________________________________________________________________________
BinaryFont::BinaryFont(const char * name, int size, int properties) 
{
	fDevice = gBinarySystem.CreateMemoryDevice (10,10);
	fFont = gBinarySystem.CreateVGFont (name, size, properties);
}

BinaryFont::~BinaryFont() { delete fFont; delete fDevice; }

//______________________________________________________________________________
void BinaryFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
	fFont->GetExtent (s, inCharCount, outWidth, outHeight, fDevice);
}

//______________________________________________________________________________
void BinaryFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	fFont->GetExtent( c, outWidth, outHeight, fDevice );
}
