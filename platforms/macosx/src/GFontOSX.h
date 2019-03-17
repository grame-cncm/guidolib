#ifndef GFontOSX_H
#define GFontOSX_H

/*
  GUIDO Library
  Copyright (C) 2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <CoreText/CoreText.h>
#include "VGFont.h"
#include "GUIDOExport.h"

// quartz

#include <string>

// --------------------------------------------------------------
// 		GFontOSX class
// --------------------------------------------------------------
/** \brief OSX implementation of the VGFont device-independant font class.
	
	Using VGFont in the ScoreProcessingLib:

	The VGSystem child-object used by the client app is responsible 
	for allocating a new VGFont child-object and associating it with 
	the appropriate VGdevices. 

*/		
class_export GFontOSX : public VGFont		
{
    friend class GDeviceOSX;
	CTFontRef	fCTFont;
    CGFontRef fCGFont;
    CFDictionaryRef fCTFontDictionary;

	public:
				 GFontOSX( const char* faceName, int size, int properties);
		virtual	~GFontOSX();

		// - VGFont services ---------------------------------------------
		virtual	const char*		GetName() const			{ return mName.c_str(); }
		virtual	int 			GetSize() const			{ return mSize; }
		virtual	int				GetProperties() const	{ return mFontProp; }

		// - Symbol services ---------------------------------------------
		virtual	void			GetExtent( const char * s, int count, float * width, float * height, VGDevice * context ) const;
		virtual	void			GetExtent( int c, float * width, float * height, VGDevice * context ) const;
	
		// - OSX specific services ---------------------------------------
		/// Returns the symbol corresponding to the input index.
    CGGlyph            GetGlyph( unsigned int inSymbolID );
	protected:
		std::string		mName;
		int				mSize;
		int				mFontProp;
};

#endif
