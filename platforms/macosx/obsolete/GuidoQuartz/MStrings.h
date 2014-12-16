#ifndef MStrings_H
#define MStrings_H

/*
 * file : MStrings.h
 * 
 *	Light version, pascal string support
 *
 */

#include <Carbon/Carbon.h>

// ---------------------------------------------------------------------------
namespace MStrings
{	
			enum { kReturn = 0x0D, kEndChar = '\0' };
			
		// - Infos
		UInt32	CStringLength( const char * inCString );
		
		// - Pascal strings support
		char *	PascalToCString( const unsigned char * inPascalString, char * ioCString, int inMaxLength = 255 );
		unsigned char *	CToPascalString( const char * inCString, unsigned char * ioPascalString, int inMaxLength = 255 );
		
		bool	AppendToPString( unsigned char * inPString, const unsigned char * inAddPString, int inMaxLen = 255 );
		bool	AppendToPString( unsigned char * inPString, const char * inAddCString, int inMaxLen = 255 );
		bool	EqualPStrings( const unsigned char * inPString1, const unsigned char * inPString2 );
		unsigned char * BufferToPascalString( char * inBuffer, unsigned char * ioPascalString, int inLength ); 
}

#endif
