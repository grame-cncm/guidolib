// ===========================================================================
//	FontEncodingConverter.cp 	
// ===========================================================================

#include "FontEncodingConverter.h"

// ---------------------------------------------------------------------------
//		* FontEncodingConverter
// ---------------------------------------------------------------------------			
FontEncodingConverter::FontEncodingConverter() : mString( 0 ), mSize( 0 ), 
												mTextConverter( 0 )
{
}

// ---------------------------------------------------------------------------
//		* ~FontEncodingConverter
// ---------------------------------------------------------------------------			
FontEncodingConverter::~FontEncodingConverter()
{
	if( mTextConverter )
	{	
		::TECDisposeConverter( mTextConverter );
		mTextConverter = 0;
	}	

    delete [] mString;
}

// ---------------------------------------------------------------------------
//		* Setup
// ---------------------------------------------------------------------------			
// kTextEncodingWindowsLatin1, kTextEncodingMacRoman 
bool
FontEncodingConverter::Setup( unsigned long srcEncoding, unsigned long  dstEncoding )
{
	OSStatus status;
	bool result = false;
	if( mTextConverter == 0 )
	{
		status = ::TECCreateConverter( &mTextConverter, srcEncoding, dstEncoding );
		result = (status == noErr);
	}
	else
	{
		result = false; // TODO: update previous mTextConverter
	}
	return result;
}

// ---------------------------------------------------------------------------
//		* Convert
// ---------------------------------------------------------------------------			
char * 	
FontEncodingConverter::Convert( const char * inString, int inCharCount )
{
	if( inString == 0 ) return 0;
	if( inCharCount == 0 ) return 0;
	
	OSStatus status;
	// - Prepare string buffer
	if( inCharCount > mSize  )
	{
		delete [] mString;		
		const int newSize = 32 + inCharCount + (inCharCount / 10); // 10 arbitraire
		mString = new char [ newSize ];
		mSize = newSize;
	}
	
	if( mString == 0 ) return 0;
	
	mString[ 0 ] = '\0';
	
	// - Perform convertion

	ConstTextPtr inputBuffer = (ConstTextPtr)inString;
	ByteCount inputBufferLen = (ByteCount)inCharCount;
	ByteCount actualInputLen = 0;
  	TextPtr theString = (TextPtr)mString;
 	ByteCount outputBufferLen = (ByteCount)mSize;  	
  	ByteCount actualOutputLen = 0;
  	
	status = ::TECConvertText( mTextConverter, inputBuffer, inputBufferLen, &actualInputLen,
									 theString, outputBufferLen, &actualOutputLen );
									
	if( status == noErr )
		return mString;
	else
		return 0;
}







