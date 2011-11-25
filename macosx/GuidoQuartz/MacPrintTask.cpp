/*
 * file : MacPrintTask.cpp
 * 
 */

#include "MacPrintTask.h"

// ---------------------------------------------------------------------------------
//		* MacPrintTask
// ---------------------------------------------------------------------------------
MacPrintTask::MacPrintTask()
{
	mPrintSession = NULL;
	mPrintSettings = NULL;
	mPageFormat = NULL;

	mPageCount = 1;
	mPrintJobName = 0;
}

// ---------------------------------------------------------------------------------
//		* ~MacPrintTask
// ---------------------------------------------------------------------------------
MacPrintTask::~MacPrintTask()
{
	if( mPrintJobName )
		::CFRelease( mPrintJobName );
		
	mPrintJobName = 0;
}

// ---------------------------------------------------------------------------------
//		* SetTaskName
// ---------------------------------------------------------------------------------
void
MacPrintTask::SetTaskName( CFStringRef inName )
{ 
	if( inName )
		::CFRetain( inName );
		
	mPrintJobName = inName;
}

// ---------------------------------------------------------------------------------
//		* Start
// ---------------------------------------------------------------------------------
bool
MacPrintTask::Start()
{
 	OSStatus err = noErr;

	// - Open session
    err = ::PMCreateSession( &mPrintSession );
	if( err != noErr) return false;

	// - Manage print settings

	MakePrintSettings();
	
	// - Manage page format

	MakePageFormat();
	
	// - Prepare 
	::PMSetPageRange( mPrintSettings, 1, (unsigned)kPMPrintAllPages ); //GetPageCount());
	
	// - Displays the dialog
	 Boolean accepted;

	 err = ::PMSessionPrintDialog( mPrintSession, mPrintSettings, 
                                    mPageFormat,  &accepted );

	// - We could use sheets here (see Apple sample code: CarbonQuartzDrawindPrinting)
	// but we directly call the Print method.
	if( err == noErr )
		PrintDialogDone( accepted );

	if( mPageFormat ) ::PMRelease( mPageFormat ); mPageFormat = 0;
	if( mPrintSettings ) ::PMRelease( mPrintSettings ); mPrintSettings = 0;
	if( mPrintSession ) ::PMRelease( mPrintSession ); mPrintSession = 0;
	
	return true;
}

// ---------------------------------------------------------------------------------
//		* MakePrintSettings
// ---------------------------------------------------------------------------------
void
MacPrintTask::MakePrintSettings()
{
	OSStatus err = ::PMCreatePrintSettings( &mPrintSettings );
	if( err == noErr )
	{
		err = ::PMSessionDefaultPrintSettings( mPrintSession, mPrintSettings ); 

		if( err == noErr )
		{
			::PMSetJobNameCFString( mPrintSettings, mPrintJobName );
		}
	}
}


// ---------------------------------------------------------------------------------
//		* MakePageFormat
// ---------------------------------------------------------------------------------
void
MacPrintTask::MakePageFormat()
{
	OSStatus err = ::PMCreatePageFormat( &mPageFormat ); 
	if( err == noErr )
	{
		err = ::PMSessionDefaultPageFormat( mPrintSession, mPageFormat ); 
		if( err != noErr )
		{
			::PMRelease( mPageFormat );
			mPageFormat = 0;
		}	
	}	
}

// ---------------------------------------------------------------------------------
//		* PrintDialogDone
// ---------------------------------------------------------------------------------
void
MacPrintTask::PrintDialogDone( bool accepted ) // idem MyPrintDialogDoneProc
{
	if( accepted )
		LaunchPrintLoop();
}

// ---------------------------------------------------------------------------------
//		* LaunchPrintLoop
// ---------------------------------------------------------------------------------
bool
MacPrintTask::LaunchPrintLoop() // idem MyDoPrintLoop
{
	OSStatus err = noErr;
	
	// - Last preparations before printing
	Boolean defaultSet;
	err = ::PMSessionValidatePageFormat( mPrintSession, mPageFormat, &defaultSet );
	
	Boolean changed;			
	err = ::PMSessionValidatePrintSettings( mPrintSession, mPrintSettings, &changed );

	// - Print job setup
	UInt32 lastPage, totalDocPages = GetPageCount();

	// err = ::PMGetFirstPage( mPrintSettings, &firstPage );
	
	if( err == noErr )
		err = ::PMGetLastPage( mPrintSettings, &lastPage );

  	if(( err == noErr ) && ( lastPage > totalDocPages ))
        lastPage = totalDocPages;

	if( err == noErr )
		::PMSetLastPage( mPrintSettings, lastPage, false );
 
 	// - Print
 	if( err == noErr )
 		return DoPrintDocument();

	return false;
}   
 
// ---------------------------------------------------------------------------------
//		* DoPrintDocument
// ---------------------------------------------------------------------------------
bool
MacPrintTask::DoPrintDocument() // idem MyDoPrintLoop
{
	if( mPrintSession == 0 ) return false;

	// - Before beginning a new document, we need to set the kinf of graphic port
	if( IsQuartzMode())
		ApplyQuartzMode();
	
	// - Begin document prining...		
	OSStatus err = ::PMSessionBeginDocument( mPrintSession, mPrintSettings, mPageFormat );
	if( err == noErr )
	{
  		UInt32 pageNumber = 0;
		err = ::PMGetFirstPage( mPrintSettings, &pageNumber );
 
		UInt32 lastPage = 0;
		if( err == noErr )
			err = ::PMGetLastPage( mPrintSettings, &lastPage );


	 	while(( pageNumber <= lastPage ) 
	 			&& ( err == noErr )
	 			&& ( ::PMSessionError( mPrintSession ) == noErr ))
	 	{
			DoPrintPage( pageNumber );
			         
         	++ pageNumber;
        }
        err = ::PMSessionEndDocument( mPrintSession );
	}

	return ( err == noErr);
}


// ---------------------------------------------------------------------------------
//		* DoPrintPage
// ---------------------------------------------------------------------------------
bool
MacPrintTask::DoPrintPage( int inPageNum ) // idem MyDoPrintLoop
{
	bool result = false;
	OSStatus err = ::PMSessionBeginPage( mPrintSession, mPageFormat, NULL );
	if( err == noErr )
	{
	    // - Get the quartz context for this page to be printed.
	    if( IsQuartzMode())
	    {
		    CGContextRef printingCGContext;
	    	err = ::PMSessionGetGraphicsContext( mPrintSession, kPMGraphicsContextCoreGraphics,
	        									(void **) &printingCGContext );
			if( err == noErr )
				result = DoDrawPageContent( printingCGContext, inPageNum );
		}
		else
		{
			// Not yet implemented (TODO) printing with quickdraw ->
			// DoDrawPageContent( quickdrawPort, inPageNum );
		}

		err = ::PMSessionEndPage( mPrintSession );
	}
	
	return result;
}

// ---------------------------------------------------------------------------------
//		* DoDrawPageContent
// ---------------------------------------------------------------------------------
bool
MacPrintTask::DoDrawPageContent( CGContextRef inContext, int inPageNum )	// idem MyPageDrawProc
{
	// Override me.

	return true;
}


// ---------------------------------------------------------------------------------
//		* ApplyQuartzMode
// ---------------------------------------------------------------------------------
// Speficie que l'on veut utiliser un context Quartz (CGContextRef)
void
MacPrintTask::ApplyQuartzMode()
{
	CFStringRef strings[1];
	strings[0] = kPMGraphicsContextCoreGraphics;
	
	CFArrayRef gcArray = ::CFArrayCreate( kCFAllocatorDefault, (const void **)strings,
                        					1, &kCFTypeArrayCallBacks );
	if( gcArray != 0 )
	{
        ::PMSessionSetDocumentFormatGeneration( mPrintSession, kPMDocumentFormatPDF, gcArray, NULL);
        ::CFRelease( gcArray );     
	}	
}

/*

    CFArrayRef          ourGraphicsContextsArray;
    CGContextRef        printingContext;
    OSErr               err = noErr;
    PMPrintSession      printSession;
    
    //
    //    at this point you've already created a print session
    //
    strings[0] = kPMGraphicsContextCoreGraphics; // This is important!
    ourGraphicsContextsArray = CFArrayCreate (kCFAllocatorDefault,
                        (const void **)strings,
                        1, &kCFTypeArrayCallBacks);
    if (ourGraphicsContextsArray != NULL)
    {
            err = PMSessionSetDocumentFormatGeneration (printSession,
                            kPMDocumentFormatPDF,
                            ourGraphicsContextsArray, NULL);
            CFRelease (ourGraphicsContextsArray);
    }
    
    //
    //    more of your print loop
    //
    
    //
    //    then you call PMSessionBeginDocument and PMSessionBeginPage
    //
    
    //    Now you are ready to request the printing context
    err = PMSessionGetGraphicsContext (printSession,
        kPMGraphicsContextCoreGraphics, (void **) &printingContext);
        
    //
    //    render your content to the printingContext using Quartz 
    //    2D and continue your print loop
    // 


*/








