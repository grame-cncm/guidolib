/*
 * file : MStrings.cpp
 *
 *
 */

#include "MStrings.h"

// ---------------------------------------------------------------------------
//		* CStringLength			
// ---------------------------------------------------------------------------
UInt32
MStrings::CStringLength( const char * inCString )
{
	UInt32 strLength = 0;
	if( inCString )
	{
		while( inCString[ strLength ] != kEndChar )
			++ strLength;
	}
	return strLength;
}

// ---------------------------------------------------------------------------
//		* PascalToCString			
// ---------------------------------------------------------------------------
// inPascalString et ioCString peuvent designer la meme chaine, avant et apres.
// Renvoie la chaine C passee en argument.
char *	
MStrings::PascalToCString( const unsigned char * inPascalString, char * ioCString, int inMaxLength )
{
	if( ioCString )
	{
		if( inPascalString )
		{
			int len = inPascalString[0];
			if( len > inMaxLength ) len = inMaxLength;
			
			// OS::BlockMove( &inPascalString[1], ioCString, len );
			for( int index = 0; index < len; index ++ )
				ioCString[ index ] = inPascalString[ index + 1 ];
			
			ioCString[ len ] = kEndChar; 
		}
		else 
			ioCString[0] = kEndChar;
	}
	return ioCString;
}

// ---------------------------------------------------------------------------
//		* CToPascalString			
// ---------------------------------------------------------------------------
// Renvoie la chaine pascal passee en argument.
// Copie le contenu de la chaine C dans la chaine pascal, dans la limite
// des 255 caracteres d'une chaine pascal.
// La chaine pascal doit etre assez grande pour accueilir les inMaxLength 
// caracteres de la chaine C.
unsigned char *	
MStrings::CToPascalString( const char * inCString, unsigned char * ioPascalString, int inMaxLength )
{
	if( ioPascalString )
	{
		if( inCString )
		{
			int len = CStringLength( inCString );
			if( len > inMaxLength ) len = inMaxLength;
			if( len > 255 ) len = 255;
			
			for( int index = len - 1; index >= 0; index -- )
			{
				short val = inCString[ index ];
				ioPascalString[ index + 1 ] = val;
			}
			
			ioPascalString[ 0 ] = len; 
		}
		else
			ioPascalString[ 0 ] = 0;
		
	}
	return ioPascalString;
}

// ---------------------------------------------------------------------------
//		* BufferToPascalString			
// ---------------------------------------------------------------------------
unsigned char *	
MStrings::BufferToPascalString( char * inBuffer, unsigned char * ioPascalString, int inLength )
{
	if( ioPascalString )
	{
		if( inBuffer )
		{
			int len = inLength;
			if( len > 255 ) len = 255;
			
			for( int index = len - 1; index >= 0; index -- )
			{
				short val = inBuffer[ index ];
				ioPascalString[ index + 1 ] = val;
			}
			
			ioPascalString[ 0 ] = len; 
		}
		else
			ioPascalString[ 0 ] = 0;
		
	}
	return ioPascalString;
}

// ---------------------------------------------------------------------------
//		* EqualPStrings			
// ---------------------------------------------------------------------------
bool
MStrings::EqualPStrings( const unsigned char * inPString1, const unsigned char * inPString2 )
{
	if( inPString1 && inPString2 )
	{
		int len1 = inPString1[ 0 ];
		for( int index = 0; index <= len1; index ++ )
		{
			if( inPString1[ index ] != inPString2[ index ] )
				return false;
		}
		return true;	
	}
	return false;
}

// ---------------------------------------------------------------------------
//		* AppendToPString			
// ---------------------------------------------------------------------------
bool
MStrings::AppendToPString( unsigned char * inPString, const unsigned char * inAddPString, int inMaxLen )
{
	if( inPString == 0 ) return false;
	if( inAddPString == 0 ) return true; // Has sucessfully added nothing

	if( inMaxLen > 255 )
		inMaxLen = 255;
		
	int orgLen = inPString[ 0 ];
	
	int charToCopy = inAddPString[ 0 ];
	if( orgLen + charToCopy > inMaxLen )
		return false;	//charToCopy = inMaxLen - orgLen;
	
	for( int index = 1; index <= charToCopy; index ++ )
		inPString[ orgLen + index ] = inAddPString[ index ];		

	inPString[ 0 ] += charToCopy;
	return true;
}

// ---------------------------------------------------------------------------
//		* AppendToPString			
// ---------------------------------------------------------------------------
bool
MStrings::AppendToPString( unsigned char * inPString, const char * inAddCString, int inMaxLen )
{
	if( inPString == 0 ) return false;
	if( inAddCString == 0 ) return true; // Has sucessfully added nothing

	if( inMaxLen > 255 )
		inMaxLen = 255;
		
	int orgLen = inPString[ 0 ];
	
	int charToCopy = CStringLength( inAddCString );		// ameliorer: eviter ce calcul. 
	if( orgLen + charToCopy > inMaxLen )
		return false;	//charToCopy = inMaxLen - orgLen;
	
	for( int index = 0; index < charToCopy; index ++ )
		inPString[ orgLen + index + 1 ] = inAddCString[ index ];		

	inPString[ 0 ] += charToCopy;
	return true;
}

