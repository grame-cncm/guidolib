/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef FontEncodingConverter_H
#define FontEncodingConverter_H

#pragma once

#include <CoreServices/CoreServices.h>
//#include <CoreServices/TextEncodingConverter.h>
// --------------------------------------------------------------
class FontEncodingConverter
{
	public:
			enum 
			{ 
				kWindowsLatin1 	= kTextEncodingWindowsLatin1,
				kMacRoman 		= kTextEncodingMacRoman,
				kIsoLatin1 		= kTextEncodingISOLatin1
			};
			
						FontEncodingConverter();
		virtual			~FontEncodingConverter();

		virtual bool	Setup( unsigned long srcEncoding, unsigned long dstEncoding );

		virtual char * 	Convert( const char * inString, int inCharCount );
		
	protected:
	
			char * 		mString;
			int 		mSize;		
			
			TECObjectRef 	mTextConverter;
};

#endif
