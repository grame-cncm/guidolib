/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include "GuidoFont.h"

GuidoFont::GuidoFont(const char *name, int size, int properties) : name(name), size(size), properties(properties)
{
}

GuidoFont::~GuidoFont()
{
}

inline const char * GuidoFont::GetName() const
{
    return name.c_str();
}

inline int GuidoFont::GetSize() const
{
	return size;
}

inline int GuidoFont::GetProperties() const
{
    return properties;
}

void GuidoFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
	*outWidth = 0;
	*outHeight = 0;
    for(int i = 0; i < inCharCount; i++)
    {
		*outWidth += kGuidoFontWidth[s[inCharCount]] * size / kFontSize;
		*outHeight += kGuidoFontHeight[s[inCharCount]] * size / kFontSize;
    }
}

void GuidoFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	*outWidth = kGuidoFontWidth[c] * size / kFontSize;
	*outHeight = kGuidoFontHeight[c] * size / kFontSize;
}
