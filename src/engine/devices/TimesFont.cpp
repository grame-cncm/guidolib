/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include "TimesFont.h"

TimesFont::TimesFont(const char *name, int size, int properties) : name(name), size(size), properties(properties)
{
}

TimesFont::~TimesFont()
{
}

inline const char * TimesFont::GetName() const
{
    return name.c_str();
}

inline int TimesFont::GetSize() const
{
	return size;
}

inline int TimesFont::GetProperties() const
{
    return properties;
}

void TimesFont::GetExtent(const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context) const
{
	*outWidth = 0;
	*outHeight = 0;
    for(int i = 0; i < inCharCount; i++)
    {
		*outWidth += kTimesFontWidth[(int)s[inCharCount]] * size / kTimesFontSize;
		*outHeight += kTimesFontHeight[(int)s[inCharCount]] * size / kTimesFontSize;
    }
}

void TimesFont::GetExtent(unsigned char c, float * outWidth, float * outHeight, VGDevice * context) const
{
	*outWidth = kTimesFontWidth[c] * size / kTimesFontSize;
	*outHeight = kTimesFontHeight[c] * size / kTimesFontSize;
}
