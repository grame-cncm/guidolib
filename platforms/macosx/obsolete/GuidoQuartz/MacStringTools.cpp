/*
 * file : MacStringTools.cpp
 * 
 */

#include <iostream>
#include "MacTools.h"

// ---------------------------------------------------------------------------
//		* PrintCFString	
// ---------------------------------------------------------------------------
void
MacTools::PrintCFString( CFStringRef inStringRef )
{	
	bool success = false;
	
	if( inStringRef )
	{
		// - Convert to c-string
		const int len = ::CFStringGetLength( inStringRef );
		if( len == 0 )
		{
			std::cout << "[Empty string]" << std::endl;	
			success = true;
		}
		else
		{		
			char * cString = new char [len + 1];
			cString[0] = '\0';
			
			::CFStringGetCString( inStringRef, cString, len + 1, kCFStringEncodingASCII );
			success = true;

			// - Print
			std::cout << cString << std::endl;
			delete [] cString;
		}			
	}		

	if( success == false )
		std::cout << "[NULL string ref.]" << std::endl;
}






