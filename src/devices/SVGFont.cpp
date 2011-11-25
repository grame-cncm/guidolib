/*
	GUIDO Library
	Copyright (C) 2011	Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "SVGFont.h"

#ifdef __APPLE__
#include "GFontOSX.h"
#include "GSystemOSX.h"
GSystemOSX gSystem (0, 0);

#elif defined (WIN32)
#include "GFontWin32.h"
#include "GSystemWin32.h"
GSystemWin32 gSystem (0, 0);

#elif linux
#include "CairoSystem.h"
CairoSystem gSystem (0);

#else
#error "unknown system for compiling SVGFont"

#endif



//______________________________________________________________________________
SVGFont::SVGFont(const char * name, int size, int properties) 
{
	fDevice = gSystem.CreateMemoryDevice (10,10);
	fFont = gSystem.CreateVGFont (name, size, properties);
}

SVGFont::~SVGFont() { delete fFont; delete fDevice; }

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
