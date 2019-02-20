/*
  GUIDO Library
  Copyright (C) 2019	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "StaticFont.h"

void StaticFont::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const
{
	float ratio = fSize / fRefSize;
	*outWidth = 0;
    for (int i = 0; i < inCharCount; i++)
		*outWidth += GetExtend ((int)s[i]) * ratio;
    *outHeight = fHeight * ratio;
}

void StaticFont::GetExtent( int c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	float ratio = fSize / fRefSize;
	*outWidth = GetExtend (c) * ratio;
	*outHeight = fHeight * ratio;
}

