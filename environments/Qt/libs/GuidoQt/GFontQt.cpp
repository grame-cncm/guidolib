/*
 * GFontQt.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
#include "GFontQt.h"

#include <QFontMetrics>
#include <iostream>

using namespace std;

//------------------------------------------------------------------------
GFontQt::GFontQt( QFont * nativeFont, const char* name, int size, int properties)
	: mNativeFont(nativeFont), mName(name), mSize(size), mFontProp(properties)
{
}

//------------------------------------------------------------------------
GFontQt::~GFontQt()		{ delete mNativeFont; }
  
//------------------------------------------------------------------------
void GFontQt::GetExtent( const char * s, int , float * outWidth, float * outHeight, VGDevice * ) const 
{	
	if(!s) {
		(*outWidth) = (*outHeight) = 0;
		return;
	}
	QFontMetrics fontMetrics(*mNativeFont);	
	*outWidth  = fontMetrics.width( QString(s) );
	*outHeight = fontMetrics.height();
}

//------------------------------------------------------------------------
void GFontQt::GetExtent( int c, float * outWidth, float * outHeight, VGDevice * ) const
{
	QFontMetrics fontMetrics(*mNativeFont);	
	*outWidth  = fontMetrics.width( QString(Symbol(c)) ); 
	*outHeight = fontMetrics.height();
	
};
