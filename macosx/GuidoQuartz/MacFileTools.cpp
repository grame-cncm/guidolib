/*
 * file : MacFileTools.cpp
 * 
 *
 */

//#include "MacFileTools.h"
//#include "MacStringTools.h"

#include "MacTools.h"

#include <iostream>

// ---------------------------------------------------------------------------------
//		* CreateFile
// ---------------------------------------------------------------------------------
// Fail if a file with the same URL already exists.
bool 
MacTools::CreateFile( CFURLRef inFileURL, FSRef * outFileRef, FSSpec * outFileSpec )
{
	FSCatalogInfoBitmap whichInfo = kFSCatInfoNone;
   	const FSCatalogInfo * catalogInfo = 0;

    CFURLRef parentURL;
    CFStringRef fileNameRef;
    FSRef parentDirectory;
  //  UniCharPtr nameStringPtr;
    OSStatus err;

	// set locals to a known state.
    err = coreFoundationUnknownErr;

	// get the url to the parent directory.
    parentURL = ::CFURLCreateCopyDeletingLastPathComponent( 0, inFileURL );
    if (parentURL != 0) 
    {
   		// convert the URL to a FSRef.
        if (CFURLGetFSRef(parentURL, &parentDirectory)) 
        {
       		// get the leaf name from the URL.
            fileNameRef = ::CFURLCopyLastPathComponent( inFileURL );
            if( fileNameRef != 0 )
            {
              	// - Get the leaf name from the URL.
				const ByteCount nameLen = ::CFStringGetLength( fileNameRef );
	  			UniChar * uniString = new UniChar[ nameLen * 2 ]; // * 2 useless ?
				const CFRange range = { 0, nameLen };
	
				::CFStringGetCharacters( fileNameRef, range, uniString );
			
            	// - Create the file.
                err = ::FSCreateFileUnicode( &parentDirectory, nameLen, uniString,
	                         			whichInfo, catalogInfo,  outFileRef, outFileSpec );
               
				if( err != noErr ) // dupFNErr (file exists)
					std::cout << "Error: FSCreateFileUnicode returned " << err << std::endl;
				           
               	// release the file name.
               	delete [] uniString;
                ::CFRelease( fileNameRef );
            }
        }
      	// release the parent url.
        ::CFRelease( parentURL );
    }

    return (err == noErr);
}

// ---------------------------------------------------------------------------
//		* AEDescToFSRef			
// ---------------------------------------------------------------------------
bool	
MacTools::AEDescToFSRef( const AEDesc * inDesc, FSRef * outRef )
{
	if( inDesc == nil ) return false;
	if( outRef == nil ) return false;

	OSErr err = noErr;
	
	// - Best case: the ae dest type is already FSRef
	DescType type = inDesc->descriptorType;
	if( type == typeFSRef )	
	{
		err = ::AEGetDescData( inDesc, outRef, sizeof(FSRef));
		if( err == noErr )
			return true;
	}

	// - Yuk, it's a dirty FSSpec, no long filenames nor unicode support.
	// We convert it into a FSRef
	if( type == typeFSS )
	{
		FSSpec specs;
		err = ::AEGetDescData( inDesc, &specs, sizeof(FSSpec));
		if( err == noErr )
		{
			err = ::FSpMakeFSRef( &specs, outRef );
			if( err == noErr )
				return true;		
		}
	}
	
	return false;
}

// ---------------------------------------------------------------------------
//		* PrintURL	
// ---------------------------------------------------------------------------
// Caller is responsible to delete returned string (with c++ "delete []")
void // char *	
MacTools::PrintURL( CFURLRef inURL )
{
	std::cout << "Path: ";
	CFStringRef stringRef = 0;

	// - Convert to CFString
	if( inURL )
		stringRef = ::CFURLCopyPath( inURL );
	
	PrintCFString( stringRef );
	
	// - Cleanup
 	if( stringRef )
		::CFRelease( stringRef );

	std::cout << std::endl;
}












