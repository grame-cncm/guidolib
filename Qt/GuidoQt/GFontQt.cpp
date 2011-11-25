/*
 * GFontQt.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "GFontQt.h"

#include <QFontMetrics>
#include <iostream>

#include "MusicalSymbols.h"
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
void GFontQt::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * ) const
{
	QFontMetrics fontMetrics(*mNativeFont);	
	*outWidth  = fontMetrics.width( QString(Symbol(c)) ); 
	*outHeight = fontMetrics.height();
	
};
