/*
	FontEncodingConverter.h

	Convert strings between different encodings.




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
