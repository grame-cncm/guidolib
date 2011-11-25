/*
 * file : ExportImageWizard.cpp
 * 
 *
 */

#include "ExportImageWizard.h"

#include "GuidoGfxExporter.h"
#include "ExportImageDialog.h"
#include "GuidoViewerConsts.h"

// - Guido
#include "GUIDOEngine.h"

// - Toolkit
#include "MacAlertBox.h"
#include "MacFileSelector.h"
#include "MacTools.h"
// ---------------------------------------------------------------------------------
//		* ExportImageWizard
// ---------------------------------------------------------------------------------
ExportImageWizard::ExportImageWizard( IBNibRef inNibRef, WindowRef inParentWindow )
{
	mNibRef = inNibRef;
	mParentWindow = inParentWindow;
	mGuidoHandleGR = 0;
	mPageNum = 0;
}

// ---------------------------------------------------------------------------------
//		* ~ExportImageWizard
// ---------------------------------------------------------------------------------
ExportImageWizard::~ExportImageWizard()
{
}

// ---------------------------------------------------------------------------------
//		* Invoke
// ---------------------------------------------------------------------------------
void		
ExportImageWizard::Invoke( GRHandler inGuidoHandleGR, int inPageNum, CFStringRef inDocName )
{
	mPageNum = inPageNum;
	mGuidoHandleGR = inGuidoHandleGR;
	
	// -- Copy the Guido GR structure and trim the page size (no more!)
/*
	const int handleAR = GuidoFindARFromGR( inGuidoHandleGR );
	const int copyHandleGR = GuidoMakeGRMusic( handleAR );
	mGuidoHandleGR = copyHandleGR;

	if( GuidoValidHandle( mGuidoHandleGR ) == false )
		return;

	GuidoAdjustPageSize( mGuidoHandleGR );
*/

	// -- Calculate the Guido page height/width ratio
	
	GuidoPageFormat format = { 1, 1, 0, 0, 0, 0 };
	GuidoGetPageFormat( inGuidoHandleGR, inPageNum, &format );
		
	const double ratio = double(format.height) / double(format.width);

	// -- Display the export settings dialog window (witdh, height, resolution...)

	ExportImageDialog * dialog = new ExportImageDialog();

	dialog->Setup( GetNibRef(), kExportDialogNibName, mParentWindow, ratio );
	
	const UInt32 command = dialog->DoDialog();

	if( command == kHICommandOK )
		ProcessDialogResult( dialog, inDocName );	
	
	delete dialog;
}

// ---------------------------------------------------------------------------------
//		* ProcessDialogResult
// ---------------------------------------------------------------------------------
void			
ExportImageWizard::ProcessDialogResult( ExportImageDialog * inDialog, CFStringRef inDocName )
{		
	if( inDialog == 0 ) return;
	
	MacAlertBox box;
	OSErr err = noErr;

	// -- Get the dialog results.
	
	const int theWidth = inDialog->GetPixelWidth();
	const int theHeight = inDialog->GetPixelHeight();		

	OSType formatCode = inDialog->GetExportFormat();
			
	// -- Prepare the graphic exporter

	GuidoGfxExporter exporter;	
	exporter.SetExportFormat( formatCode );
	exporter.EnableSettingsDialog( true );
	
	// -- Ask for the destination file

	MacFileSelector fs;
	fs.SetParentWindow( mParentWindow );
	fs.SetQueryMessage( "Destination file..." );

	// - Build the new file name

	CFMutableStringRef newName;
	if( inDocName != 0 )
		newName = ::CFStringCreateMutableCopy( kCFAllocatorDefault, 0, inDocName );
	else
		newName = ::CFStringCreateMutableCopy( kCFAllocatorDefault, 0, CFSTR("untitled"));
		
	::CFStringTrim( newName, CFSTR(".gmn"));
	::CFStringTrim( newName, CFSTR(".txt"));

	CFStringRef extensionStr = exporter.CreateDefaultFileExtensionString();

	::CFStringAppend( newName, extensionStr );
	fs.SetDefaultSaveFileName( newName );

	::CFRelease( newName );
	::CFRelease( extensionStr );

	CFURLRef fileUrl;
	if( fs.SelectSaveFile( &fileUrl ) == false )
	{
		box.Alert( "Graphic export aborted.", "" ); // to be removed
		return;	
	}

	// -- Little hack. Pre-create the file to obtain the best valid fileSpecs possible.
	// a FSSpec is required for the compatibility with old quicktime APIs.

	bool result = false;
	
	FSSpec fileSpecs; // the specs we need for the Quicktime Gfx Exporter
	FSRef fileRef;
	Boolean exists = ::CFURLGetFSRef( fileUrl, &fileRef );
	
	if( exists && 
		( ::FSGetCatalogInfo( &fileRef, kFSCatInfoNone, 0, 0, &fileSpecs, 0 ) == noErr ))
	{
		// - Case 1) File already exists, just retrieve it  FSSpec
		// (done by FSGetCatalogInfo())
		result = true;
	}
	else
	{
		// - Case 2) File does not exists, create it and gets its FSSpec
	
		result = MacTools::CreateFile( fileUrl, &fileRef, &fileSpecs );
	}

	::CFRelease( fileUrl );

	if( result == false )
	{
		box.Alert( "Could not create the output file.", "" );
		return;	
	}
	
	// -- Finally, do the export
	
	result = exporter.ExportGuidoPage( mGuidoHandleGR, mPageNum, theWidth, theHeight, &fileSpecs );
		
	// - On failure, we destroy the file created during the "hack"
	if( result == false )
	{
		err = ::FSDeleteObject( &fileRef );
		box.Alert( "Graphic export failed.", "" );		
	}
}

/*
// ---------------------------------------------------------------------------------
//		* DoGuidoExport
// ---------------------------------------------------------------------------------
bool		
ExportImageWizard::DoGuidoExport( FSSpec & fileSpecs, int inWidth, int inHeight )
{	

}
*/













