/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "SVGFont.h"

#ifdef INDEPENDENTSVG
#include "SVGSystem.h"
SVGSystem gSystem (0);
#elif __APPLE__
#include "GFontOSX.h"
#include "GSystemOSX.h"
GSystemOSX gSystem (0, 0);

#elif defined (WIN32)
#include "GFontWin32.h"
#include "GSystemWin32.h"
GSystemWin32 gSystem (0, 0);

#elif defined(__linux__) || defined(__FreeBSD__)
#include "CairoSystem.h"
CairoSystem gSystem (0);

#else
#error "unknown system for compiling SVGFont"

#endif

/*
  This file makes use of a forked version of nanosvg on github
  (github.com/mikesol/nanosvg). nanosvg is originally intended
  for parsing SVG files and giving information about the contents
  in terms of paths.  The fork on github expands this to work on
  font files.  To see comments indicating when nanosvg is coming
  into play, grep this file looking for nSVG.
*/

//______________________________________________________________________________
SVGFont::SVGFont(const char * name, int size, int properties) : fName(name)
{
    fDevice = gSystem.CreateMemoryDevice (10,10);
	fFont = gSystem.CreateVGFont (name, size, properties);
}

SVGFont::~SVGFont()
{
    delete fFont;
    delete fDevice;
}

//______________________________________________________________________________
void SVGFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
    fFont->GetExtent (s, inCharCount, outWidth, outHeight, fDevice);
}

//______________________________________________________________________________
void SVGFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
    fFont->GetExtent( c, outWidth, outHeight, fDevice );
}
