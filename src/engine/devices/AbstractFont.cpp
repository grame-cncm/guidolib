/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "AbstractFont.h"

#ifdef INDEPENDENTSVG
#include "SVGFont.h"
#include "SVGSystem.h"
SVGSystem gAbstractSystem (0);
#elif __APPLE__
#include "GFontOSX.h"
#include "GSystemOSX.h"
GSystemOSX gAbstractSystem (0, 0);

#elif defined (WIN32)
#include "GFontWin32.h"
#include "GSystemWin32.h"
GSystemWin32 gAbstractSystem (0, 0);

#elif linux || __linux
#include "CairoSystem.h"
CairoSystem gAbstractSystem (0);

#else
#error "unknown system for compiling AbstractFont"

#endif



//______________________________________________________________________________
AbstractFont::AbstractFont(const char * name, int size, int properties) 
{
	fDevice = gAbstractSystem.CreateMemoryDevice (10,10);
	fFont = gAbstractSystem.CreateVGFont (name, size, properties);
}

AbstractFont::~AbstractFont() { delete fFont; delete fDevice; }

//______________________________________________________________________________
void AbstractFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
	fFont->GetExtent (s, inCharCount, outWidth, outHeight, fDevice);
}

//______________________________________________________________________________
void AbstractFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	fFont->GetExtent( c, outWidth, outHeight, fDevice );
}
