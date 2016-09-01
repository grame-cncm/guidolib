/*
 * file : MacGfxExporter.cpp
 * 
 *	May/17/2004	(JB)
 *
	Todo: look at Quicktime 6.4 new functions:
		GraphicsExportSetInputCGImage()
		GraphicsExportGetInputCGImage()
		GraphicsExportSetInputCGBitmapContext(
		GraphicsExportGetInputCGBitmapContext() 
 
 	Gif export is not supported by Quicktime, due to licence issue.
 */

// - System
#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>
#include <QuickTime/ImageCompression.h>

#include <iostream>

#include "MacGfxExporter.h"

using namespace std;

// ---------------------------------------------------------------------------------
//		* MacGfxExporter
// ---------------------------------------------------------------------------------
MacGfxExporter::MacGfxExporter()
{
	mComponent = 0;
	mFormatCode = kQTFileTypeJPEG;
	mShowSettingsDialog = true;
}

// ---------------------------------------------------------------------------------
//		* ~MacGfxExporter
// ---------------------------------------------------------------------------------
MacGfxExporter::~MacGfxExporter()
{
    if( mComponent != 0 )
	    ::CloseComponent( mComponent );

	mComponent = 0;
}

// ---------------------------------------------------------------------------------
//		* EnableSettingsDialog
// ---------------------------------------------------------------------------------
void
MacGfxExporter::EnableSettingsDialog( bool inState )
{
	mShowSettingsDialog = inState;
}

// ---------------------------------------------------------------------------------
//		* SetExportFormat
// ---------------------------------------------------------------------------------
/** \brief Tells which export format to use.

	'.SGI' '8BPS' 'BMPf' 'JPEG' 'PICT' 'PNGf' 'PNTG' 'TIFF' 'TPIC'(tga)  'jp2 ' 'qtif'
*/
bool
MacGfxExporter::SetExportFormat( OSType inFormatCode )
{
	// - Check if already opened
	if( mComponent != 0 ) 
	{
	  	if( mFormatCode == inFormatCode )
	  		return true;					// ok, same format
	  	
	    ::CloseComponent( mComponent );
		mComponent = 0;
	}

	// - Open a component for this format
	mFormatCode = inFormatCode;
	OSErr err = ::OpenADefaultComponent( GraphicsExporterComponentType, inFormatCode, &mComponent );
	if( err != noErr )
	{
		cout << "GfxExporter error: OpenADefaultComponent returned " << err << endl;
		mComponent = 0;
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------------
//		* CreateDefaultFileExtensionString
// ---------------------------------------------------------------------------------
/** output string have the extension dot.
	
	The export format must have been already defined with SetExportFormat()
	
	\return a file format string (".jpg", ".tga"...) or 0 if the export format
			 has not been specified.
*/
CFStringRef
MacGfxExporter::CreateDefaultFileExtensionString()
{
	if( mComponent == 0 ) return 0;
	
	OSType fileExt;
	
	ComponentResult result = ::GraphicsExportGetDefaultFileNameExtension( mComponent, &fileExt );
  
	if( result != noErr ) return 0;
		
	CFStringRef tempStr = ::CFStringCreateWithBytes( kCFAllocatorDefault, (UInt8*)(&fileExt), 
														sizeof(OSType), kCFStringEncodingASCII, 0 );
	
	CFMutableStringRef outStr = ::CFStringCreateMutableCopy( kCFAllocatorDefault, 0, tempStr );
	::CFRelease( tempStr );

	::CFStringTrimWhitespace( outStr );
	::CFStringLowercase( outStr, 0 );
	::CFStringInsert( outStr, 0, CFSTR( "." ));
	
	return outStr;
}

// ---------------------------------------------------------------------------------
//		* Export
// ---------------------------------------------------------------------------------
/** Export a graphic file using Quicktime. See <QuickTimeComponents.h>

	\note The export format must be specified before calling this function.

	\return true on success.
		
*/
bool
MacGfxExporter::Export( const FSSpec * inDestinationFile, GWorldPtr inImage )
{
	if( inDestinationFile == 0 ) return false;
	if( inImage == 0 ) return false;
	if( mComponent == 0 ) return false;
		
	bool success = false;

    ComponentResult	cErr = 0;
//   	OSType osFileType = mFormatCode; // always ok ?

    do 
    {
		// - Set the input GWorld for the exporter
		cErr = ::GraphicsExportSetInputGWorld( mComponent, inImage );
		if (cErr != noErr)
		{
			cout << "GfxExporter error: GraphicsExportSetInputGWorld returned " << cErr << endl;
			break;
		}
	    
		// - Set the output file to our FSSpec
		cErr = ::GraphicsExportSetOutputFile( mComponent, inDestinationFile );
		if (cErr != noErr)
		{
			cout << "GfxExporter error: GraphicsExportSetOutputFile returned " << cErr << endl;
			break;
		}
	    
		// - Set the compression quality (needed for JPEG, not necessarily for other formats)
		if( ::CallComponentCanDo( mComponent, kGraphicsExportSetCompressionQualitySelect ) == noErr )
		{
			cErr = ::GraphicsExportSetCompressionQuality( mComponent, codecMaxQuality );
										// codecLosslessQuality );
			if (cErr != noErr)
			{
				cout << "GfxExporter warning: GraphicsExportSetCompressionQuality returned " << cErr << endl;
				// break;
			}
		}
	
		// - Call the standard Quicktime export settings dialog.
		
		if( mShowSettingsDialog )
			::GraphicsExportRequestSettings( mComponent, 0, 0 );   

		// - Do the export

		unsigned long sizeWritten = 0;
		cErr = ::GraphicsExportDoExport( mComponent, &sizeWritten );
		if (cErr != noErr)
		{
			cout << "GfxExporter error: GraphicsExportDoExport returned " << cErr << endl;
			break;
		}

		success = true;
	
	} while( false );
	
	return success;
}
