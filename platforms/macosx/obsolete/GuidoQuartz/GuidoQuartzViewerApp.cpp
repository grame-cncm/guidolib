/*
 *
 * File: GuidoQuartzViewerApp.cpp
 *
 *
 *
 */

// - Std c++
#include <iostream> 
#include <string>
#include <sstream>

using namespace std;

// - Apple
//     #include <sys/time.h> // for getrusage
  //   #include <sys/resource.h>

#include <ApplicationServices/ApplicationServices.h>
//#include <ATSFont.h>

// - Toolkit
#include "MClock.h"
#include "MacFileSelector.h"
#include "MacAlertBox.h"
#include "MacTools.h"

// - App
#include "GuidoQuartzViewerApp.h"
//#include "GuidoPrefs.h"
//#include "GuidoCarbonControl.h"
#include "GuidoDocWindow.h"
#include "GuidoViewerConsts.h"
#include "GSystemOSX.h"

#include "MacGuidoPrintTask.h"
#include "ExportImageWizard.h"

// - Guido
#include "GUIDOEngine.h"
#include "GUIDOFactory.h"
// #include "GDeviceTester.h" // DEBUG

static bool gXMLFile = false;

#ifdef GUIDO_USE_MUSICXML

	#include "guidoconverter.h"
	#include "LibMusicXML.h"	
	using namespace MusicXML;

#endif

// ----------------------------------------------------------------------------
int main()
{
    GuidoQuartzViewerApp myApp;	
	myApp.Run();
	return 0;
}

GuidoQuartzViewerApp * GuidoQuartzViewerApp::sTheApp = 0;
// ----------------------------------------------------------------------------
//		* GuidoQuartzViewerApp
// ----------------------------------------------------------------------------
GuidoQuartzViewerApp::GuidoQuartzViewerApp () : fPrefs(&mSettings)
{
	// - Carbon 
    mNibRef = 0;				// reference to the bundle's applications nib file

	mAppEventHandlerRef = 0;	// carbon events application handler reference
	mAppEventCB = 0;			// carbon events application handler UPP
	mWindowEventCB = 0;			// carbon events window handler UPP
	mAEEventCB = 0;				// apple events handler upp
	sTheApp = this;				// for apple events callbacks.

	// - App
	mInitGDevice = 0;
	mFooWindow = 0;				// Just to have a valid graphic context for Guido.
	mIsSetupGood = Initialize();
	GuidoGetDefaultLayoutSettings (&mSettings);
}

// ----------------------------------------------------------------------------
//		* ~GuidoQuartzViewerApp
// ----------------------------------------------------------------------------
GuidoQuartzViewerApp::~GuidoQuartzViewerApp()
{	
	if( mAEEventCB )
		::AERemoveEventHandler ( kCoreEventClass, kAEOpenDocuments, mAEEventCB, false );

	if( mAppEventHandlerRef )
		::RemoveEventHandler( mAppEventHandlerRef );

	// - TODO: cleanup opened documents..
	
	//

	ReleaseInitGDevice();

   if( mNibRef )
    	::DisposeNibReference( mNibRef );
}

// ---------------------------------------------------------------------------
//		* Run
// ---------------------------------------------------------------------------
void 
GuidoQuartzViewerApp::Run()
{
	if( mIsSetupGood )
	{
		// SetupGuidoWindow();
		// InitGuido();

		InitGuido();

		::RunApplicationEventLoop();
	}
	else
	{
		ReportError( "Error at initialisation." );
	}
}

// ----------------------------------------------------------------------------
//		* Initialize
// ----------------------------------------------------------------------------
bool 
GuidoQuartzViewerApp::Initialize()
{
	OSStatus err; 
	bool fontsOK = false;
	bool result = false;

	InitMyEventHandlers();

	::InitCursor();
	
	// - Build application interface
    err = ::CreateNibReference( kMainNibFileName , &mNibRef );
    if( err != noErr ) 	goto doh;
    	
    err = ::SetMenuBarFromNib( mNibRef, kMenuBarNibName );
   	if( err != noErr )	goto doh;
     
   	fontsOK = SetupMusicalFont();
	result = true;
      
doh:
 
	return result;
}

// ----------------------------------------------------------------------------
//		* SetupMusicalFont
// ----------------------------------------------------------------------------
/** Loads the Fonts that are located in the resource folder of the Application Bundle.
*/
bool 
GuidoQuartzViewerApp::SetupMusicalFont()
{
	OSStatus status;
	
	// - First, locate the Resources folder 
	CFBundleRef     myAppBundle = NULL; 
	CFURLRef        myAppResourcesURL = NULL; 
	FSRef           myResourceDirRef; 
	FSSpec			myResourceDirSpec;
	
	myAppBundle = ::CFBundleGetMainBundle();
	if( myAppBundle ) 
	{
		myAppResourcesURL = ::CFBundleCopyResourcesDirectoryURL( myAppBundle );
		if( myAppResourcesURL ) 
		{
			::CFURLGetFSRef( myAppResourcesURL, &myResourceDirRef );
			status = ::FSGetCatalogInfo (&myResourceDirRef, kFSCatInfoNone, NULL, 
														NULL, &myResourceDirSpec, NULL );
		}
	}
	
	// - Then, loads the fonts files located in this folder.
	ATSFontContext context = kATSFontContextLocal;
	ATSFontFormat format = kATSFontFormatUnspecified; 	// automatic
	ATSOptionFlags options = kATSOptionFlagsDefault;
	ATSFontContainerRef container;

	status = ::ATSFontActivateFromFileSpecification( &myResourceDirSpec, context, 
																format, 0, options, &container ); 

	if( status != noErr )
	{
		ReportError( "Error at initialisation.", "Could not activate the musical font guido2.ttf" );
		return false;
	}

	return true;
	// status = ATSFontDeactivate (myFontContainer,  NULL, kATSOptionFlagsDefault); 
	
	// - Chek if font is present
//	SInt16 fontFamilyID = 0;
//	::GetFNum ( "\pguido2.ttf", &fontFamilyID );	
//	cout << "Guido2 Font Family ID = " << fontFamilyID << endl;
}

// ----------------------------------------------------------------------------
//		* InitMyEventHandlers
// ----------------------------------------------------------------------------
bool 
GuidoQuartzViewerApp::InitMyEventHandlers()
{
	// - Install the AppleEvents application callback
	mAEEventCB = ::NewAEEventHandlerUPP( MyAEOpenDocHandler );
	::AEInstallEventHandler( kCoreEventClass, kAEOpenDocuments, mAEEventCB, 0, false );

	// - Install the carbon events application callback 
    EventTypeSpec list[] = 
    {
    	{ kEventClassCommand,  	kEventProcessCommand },
		{ kEventClassMenu, 		kEventMenuBeginTracking },
		{ kEventClassMenu,		kEventMenuOpening } 
	};

	mAppEventCB = ::NewEventHandlerUPP( MyAppCarbonEventHandler );
	::InstallApplicationEventHandler( mAppEventCB, GetEventTypeCount(list), 
															list, (void*)this, &mAppEventHandlerRef );
	return true;
}

// ----------------------------------------------------------------------------
//		* GetGuidoWindowEventHandlerUPP
// ----------------------------------------------------------------------------
EventHandlerUPP
GuidoQuartzViewerApp::GetGuidoWindowEventHandlerUPP()
{
	if ( mWindowEventCB == 0 ) 
		mWindowEventCB = ::NewEventHandlerUPP( GuidoWindowEventHandler );
	
	return mWindowEventCB;
}

// ----------------------------------------------------------------------------
//		* HandleCommand
// ----------------------------------------------------------------------------
bool 
GuidoQuartzViewerApp::HandleCommand( const HICommand & inCommand )
{
	GuidoDocWindow * doc = GetFrontDocument();

	bool handled = true;
	switch( inCommand.commandID ) 
	{
		case kHICommandAbout:	DisplayAboutBox();		break;
		case kHICommandOpen:	OpenFile();				break;				
		case kHICommandPrint:	HandlePrintCommand(); 	break;

			// not implemented:
		case kHICommandSave:
		case kHICommandPageSetup:	
			handled = false;	
			break;

		case 'pref':
			DisplayPrefsDialog ();
			if( doc && doc->IncPageNum( -1 ))
				doc->RefreshGuidoView();			
			UpdateMenuState( ::GetMenuHandle( kMenuEdit ));
			break;

		case 'Prev':		
			if( doc && doc->IncPageNum( -1 ))
				doc->RefreshGuidoView();
			
			UpdateMenuState( ::GetMenuHandle( kMenuView ));
			break;
		
		case 'Next':		
			if( doc && doc->IncPageNum( 1 ))
				doc->RefreshGuidoView();

			UpdateMenuState( ::GetMenuHandle( kMenuView ));
			break;
		
		case 'pitc':	// User clicked the "Pitch only" menu item	
			if( doc ) {
				FSRef * path = doc->GetPath();
				if( path )
				 	OpenXMLFile( *path, doc, 1 );
			}
			UpdateMenuState( ::GetMenuHandle( kMenuView ));
			break;	
		
		case 'ryth':	// User clicked the "Rythm only" menu item	
			if( doc ) {
				FSRef * path = doc->GetPath();
				if( path )
				 	OpenXMLFile( *path, doc, 2 );
			}
			UpdateMenuState( ::GetMenuHandle( kMenuView ));
			break;	
		
		case 'norm':	
		case 'Reld':	// User clicked the "Reload" menu item	
			if( doc )
			{
				FSRef * path = doc->GetPath();
				if( path ) {
				 	if (doc->XML()) OpenXMLFile( *path, doc, 0 );
				 	else OpenGuidoFile( *path, doc );
				 }
			}
			UpdateMenuState( ::GetMenuHandle( kMenuView ));
			break;	
	
		case 'Expo':		// User clicked the "Export" item (save bitmap image)
			ExportBitmap();
			break;
			
		case 'SMap':		// User clicked the "Show Mapping" menu item	
			if( doc )
			{
				doc->SetShowMapping( !doc->GetShowMapping());
				doc->RefreshGuidoView();
			}
			break;
		
		case kCmdZoomIn: 
		case kCmdZoomOut:
		case kCmdZoomDefault: 	
			doc->HandleZoomCommand( inCommand.commandID );
			UpdateMenuState( ::GetMenuHandle( kMenuView ));
			break;
						
		case 'scrV':
		case 'scrH':
			doc->NotifyWindowScrolled();
			break;
		
		case 'WWeb':		// Launch browser and goto guido web site.
			DisplayGuidoWebPage();
			break;
		
		//case kHICommandQuit:	break;
		default:	handled = false;	break;
	}
	return handled;						
}

// ----------------------------------------------------------------------------
//		* HandlePrintCommand
// ----------------------------------------------------------------------------
void
GuidoQuartzViewerApp::HandlePrintCommand()
{
 	GuidoDocWindow * doc = GetFrontDocument();		
	if( doc == 0 ) return;
	
	const int pageCount = doc->PageCount();
	if( pageCount <= 0 ) return;

 	MacGuidoPrintTask task;
	task.SetTaskName( doc->GetFileNameCFString());
	task.SetPageCount( pageCount );
	task.SetGuidoGR( doc->GetGR());
	task.Start();
	
}

// ----------------------------------------------------------------------------
//		* HandleWindowEvent
// ----------------------------------------------------------------------------
OSStatus 
GuidoQuartzViewerApp::HandleWindowEvent( EventRef inEvent, GuidoDocWindow * inDoc )
{
	if( inDoc == nil ) return eventNotHandledErr;
	
	const UInt32 eventKind = ::GetEventKind( inEvent );
	const UInt32 eventClass	= ::GetEventClass( inEvent );
	OSStatus outStatus = eventNotHandledErr;
	switch ( eventClass )
	{
		case kEventClassWindow:
		{	
			switch( eventKind )
			{
				case kEventWindowActivated:	UpdateAllMenuStates();	break; 
				case kEventWindowClose:		CloseDocument( inDoc );	break;
				case kEventWindowClosed:	break;

			 	case kEventWindowClickContentRgn:
					// cout << "Guido window clicked !" << endl;
					break;
				
				case kEventWindowZoomed: 
				case kEventWindowResizeCompleted:
					// ok: cout << "Guido window resized !" << endl;
					inDoc->SizeChanged();
					break;
					

				case kEventWindowBoundsChanging:	break;
					
				case kEventWindowBoundsChanged:
				{
					//cout << "Window bounds changed !" << endl;
					/* Rect bounds;
					::GetEventParameter( inEvent, kEventParamCurrentBounds, 
											typeQDRectangle, NULL, sizeof(Rect), NULL, &bounds );
					
					inDoc->BoundsChanged( bounds );*/

					/*if ( windowStorage->overlayWindow != NULL )	//	Resize the overlay window as well
					{
						(void) GetEventParameter( inEvent, kEventParamCurrentBounds, typeQDRectangle, NULL, sizeof(Rect), NULL, &r );
						SizeWindow( windowStorage->overlayWindow, r.right-r.left, r.bottom-r.top, false );
					}*/
					break;
				}
			}
			break;
		}	
		
		case kEventClassMouse :
			outStatus = inDoc->HandleMouseEvent( inEvent );
			break;
		
		case  kEventClassCommand:
			if( eventKind == kEventProcessCommand )	
				HandleProcessCommandEvent( inEvent );
			break;
		

	}    
    return outStatus;
}

// ----------------------------------------------------------------------------
//		* HandleProcessCommandEvent
// ----------------------------------------------------------------------------
bool		
GuidoQuartzViewerApp::HandleProcessCommandEvent( EventRef inCommandEvent )
{					
    HICommand command;
    OSStatus result = noErr;

	// - Retrieve the HICommand
	result = ::GetEventParameter( inCommandEvent, kEventParamDirectObject, kEventParamHICommand, 
															NULL, sizeof(command), NULL, &command ); 
	
	return HandleCommand( command );
}

// ----------------------------------------------------------------------------
//		* UpdateAllMenuStates
// ----------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::UpdateAllMenuStates()
{
	UpdateMenuState( ::GetMenuHandle( kMenuFile ));
	UpdateMenuState( ::GetMenuHandle( kMenuEdit ));
	UpdateMenuState( ::GetMenuHandle( kMenuView ));
}

// ----------------------------------------------------------------------------
//		* UpdateMenuState
// ----------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::UpdateMenuState( MenuRef inMenuRef )
{
	GuidoDocWindow * doc = GetFrontDocument();		

	switch( ::GetMenuID( inMenuRef ))
	{
		case kMenuFile:			
		{
			SetupMenuItem( inMenuRef, kFileItemReload, (doc != 0));
			SetupMenuItem( inMenuRef, kFileItemClose, (doc != 0));
			SetupMenuItem( inMenuRef, kFileItemExport, (doc != 0));
			SetupMenuItem( inMenuRef, kFileItemPrint, (doc != 0)); // (work in progress) 
			break;
		}
		
		case kMenuEdit:			break;

		case kMenuView:			
		{
			SetupMenuItem( inMenuRef, kViewItemPrevPage, (doc && doc->GetPageNum() > 1));
			SetupMenuItem( inMenuRef, kViewItemNextPage, (doc && doc->GetPageNum() < doc->PageCount()));

			// SetupMenuItem( inMenuRef, kViewItemShowMapping, (doc != 0), doc->GetShowMapping());
			
			SetupMenuItem( inMenuRef, kViewItemPitchOnly, (doc && doc->XML()), false );
			SetupMenuItem( inMenuRef, kViewItemRythmOnly, (doc && doc->XML()), false );
			SetupMenuItem( inMenuRef, kViewItemPitchAndRythm, (doc && doc->XML()), false );

			SetupMenuItem( inMenuRef, kViewItemDefaultZoom, (doc != 0), (doc && doc->GetZoom() == 1 ));
			SetupMenuItem( inMenuRef, kViewItemZoomIn, (doc != 0));
			SetupMenuItem( inMenuRef, kViewItemZoomOut, (doc != 0));
						
			break;
		}
	}
}

// ----------------------------------------------------------------------------
//		* SetupMenuItem
// ----------------------------------------------------------------------------
void
GuidoQuartzViewerApp::SetupMenuItem( MenuRef inMenuRef, int inItem, bool enabled, bool checked ) 
{
	if( enabled )	::EnableMenuItem( inMenuRef, inItem );
	else ::DisableMenuItem( inMenuRef, inItem );
	
	::CheckMenuItem( inMenuRef, inItem, checked );
}

// ----------------------------------------------------------------------------
//		* CreateGuidoDocWindow
// ----------------------------------------------------------------------------
// inPath can be null if it is a new document window.
//
// The window created is invisible and hasn't any guido handle attached. You must 
// call NotifyNewGuidoHandle(), then BringDocWindowToScreen()
//
GuidoDocWindow *
GuidoQuartzViewerApp::CreateGuidoDocWindow( const FSRef * inPath ) 
{
	WindowRef winRef = 0; void * userData;
    OSErr err;
 	
   	// - Install event handler for the new window. The userData parameter 
   	// passed to the callback will be a pointer to the document object we 
   	// just created.
	const EventTypeSpec	windowEvents[] =
    {
		{ kEventClassWindow, kEventWindowActivated },
		{ kEventClassWindow, kEventWindowClose },
		{ kEventClassWindow, kEventWindowClosed },
		{ kEventClassWindow, kEventWindowClickContentRgn },
		{ kEventClassWindow, kEventWindowResizeCompleted },
		{ kEventClassWindow, kEventWindowZoomed },
		{ kEventClassWindow, kEventWindowBoundsChanging },
		{ kEventClassWindow, kEventWindowBoundsChanged },
		
		{ kEventClassMouse, kEventMouseUp },
		{ kEventClassMouse, kEventMouseDown }
	};

	// - Create and setup the document object, this will also intialize
	// our carbon control callbacks.
	GuidoDocWindow * outDoc = new GuidoDocWindow;
   	if( outDoc == nil ) goto doh;

	// - Create the window 
	err = ::CreateWindowFromNib( GetNibRef(), kGuidoWindowNibName, &winRef );
   	if(( err != noErr ) || ( winRef == 0 )) goto doh;
      
	userData = outDoc;
	err	= ::InstallWindowEventHandler( winRef, GetGuidoWindowEventHandlerUPP(), 
					GetEventTypeCount( windowEvents ), windowEvents, userData, NULL );
	if( err != noErr )	goto doh;
		
	// - Add to document list
	outDoc->SetPath( inPath );
	mDocList.push_back( outDoc );	

	outDoc->SetupGuidoWindow( winRef );

	return outDoc;
	
doh:
	if( winRef )	::DisposeWindow ( winRef );
	delete 	outDoc;
	
	ReportError( "Could not create a new document" );
	
	return nil;
}

// ----------------------------------------------------------------------------
//		* AdjustWindowToScreen
// ----------------------------------------------------------------------------
// Unused
bool
GuidoQuartzViewerApp::AdjustWindowToScreen( WindowRef inWinRef )
{
    // - Get available size for windows, on screen
    OSStatus status;
    Rect maxFrame = { 0, 0, 0, 0 };
    GDHandle device = ::GetMainDevice();
 	status = ::GetAvailableWindowPositioningBounds( device, &maxFrame );
 	if( status != noErr ) return false;
 	
 	int availableHeight = maxFrame.bottom - maxFrame.top;
 
 	// - We'll use 95 % of this size	
 	availableHeight = (availableHeight * 95 / 100);
 	
 	// - Calculate corresponding window bounds, while keeping its proportions.
 	Rect winBounds = { 0, 0, 0, 0 };
	status = ::GetWindowBounds( inWinRef, kWindowStructureRgn, &winBounds );
 	if( status != noErr ) return false;

	const float ratio = (float)(winBounds.right - winBounds.left) / 
								(float)(winBounds.bottom - winBounds.top);
 
 	winBounds.bottom = winBounds.top + availableHeight;
 	winBounds.right = (short)(winBounds.left + ratio * availableHeight);
 	
 	status = ::SetWindowBounds( inWinRef, kWindowStructureRgn, &winBounds ); 	
	return true;
}

// ----------------------------------------------------------------------------
//		* CloseDocument
// ----------------------------------------------------------------------------
// Pop the document objet from the list and destroy it.
void
GuidoQuartzViewerApp::CloseDocument( GuidoDocWindow * in )
{
	if( in == nil ) return;
	
	vector<GuidoDocWindow *>::iterator elt;
	for( elt = mDocList.begin(); elt != mDocList.end(); ++ elt )
	{
		if( *elt == in )
		{
			mDocList.erase( elt );
			delete in;
			break;
		}
	}
}

// ---------------------------------------------------------------------------
//		* GetFrontDocument
// ---------------------------------------------------------------------------
GuidoDocWindow *
GuidoQuartzViewerApp::GetFrontDocument()
{
	WindowRef winRef = ::FrontNonFloatingWindow(); //::FrontWindow()
	
	vector<GuidoDocWindow *>::iterator elt;
	for( elt = mDocList.begin(); elt != mDocList.end(); ++ elt )
	{
		GuidoDocWindow * doc = *elt;
		if( doc && doc->GetWindowRef() == winRef )
			return doc;
	}
	
	return nil;
}

// ---------------------------------------------------------------------------
//		* FindDocumentForPath
// ---------------------------------------------------------------------------
GuidoDocWindow *
GuidoQuartzViewerApp::FindDocumentForPath( const FSRef & inPath )
{
	vector<GuidoDocWindow *>::iterator elt;
	for( elt = mDocList.begin(); elt != mDocList.end(); ++ elt )
	{
		GuidoDocWindow * doc = *elt;
		if( doc )
		{
			FSRef *	docPath = doc->GetPath();
			if( docPath )
			{
				// - This is not enough. Does not work is the file was deleted and recreated.
				if( ::FSCompareFSRefs( &inPath, docPath ) == noErr )
					return doc;
			
				// Todo
			}
		}
	}
	return nil;
}

// ---------------------------------------------------------------------------
//		* MakeInitGDevice
// ---------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::MakeInitGDevice()
{
	if( mInitGDevice != 0 ) return;
	
	// - First, we need to create a window to create our context.
	WindowRef winRef = 0;

		// todo: use another window id than kGuidoWindowNibName.
	OSErr err = ::CreateWindowFromNib( GetNibRef(), kGuidoWindowNibName, &winRef );
   	if(( err != noErr ) || ( winRef == 0 )) return;
   	
   	mFooWindow = winRef;
   	
   	// - Create our guido init context with the core-graphics context of the new window.
 	GrafPtr port = ::GetWindowPort( winRef );
	CGContextRef contextRef = 0;
	OSStatus status = ::CreateCGContextForPort( port, &contextRef );
	if( status != noErr || contextRef == 0 )	return;
  	
	Rect frame;
	::GetPortBounds( port, &frame );
  	
	mInitGSystem = new GSystemOSX(contextRef, NULL);
	mInitGDevice = mInitGSystem->CreateMemoryDevice(frame.right - frame.left, frame.bottom - frame.top );

	// - The device is now the owner of the context ref
	::CGContextRelease( contextRef );
}

// ---------------------------------------------------------------------------
//		* ShowDebugWindow
// ---------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::ShowDebugWindow()
{
/* OK
	// - First, we need to create a window to create our context.
	WindowRef winRef = 0;

//	OSErr err = ::CreateWindowFromNib( GetNibRef(), kGuidoWindowNibName, &winRef );
   	Rect initRect = { 0, 0, 100, 100 };
   
	OSStatus err = ::CreateNewWindow( kDocumentWindowClass, 
							kWindowStandardDocumentAttributes | kWindowCompositingAttribute, 
							&initRect, &winRef );
     
   	if(( err != noErr ) || ( winRef == 0 )) return;

	// - Resize to the max available size on screen   
   	Rect maxFrame = { 0, 0, 0, 0 };
    Rect innerRect = { 0, 0, 0, 0 };
   	GDHandle device = ::GetMainDevice();
 	
 	err = ::GetAvailableWindowPositioningBounds( device, &maxFrame );
  	err = ::SetWindowBounds( winRef, kWindowStructureRgn, &maxFrame ); 	
    	
 	err = ::GetWindowBounds( winRef, kWindowContentRgn, &innerRect ); 	
    
	int width = innerRect.right - innerRect.left;
	int height = innerRect.bottom - innerRect.top;
	
 	// - Show
	::SelectWindow( winRef ); // Makes the window active and bring it to front.
	::ShowWindow( winRef );			

   	// - Get the graphic context of the window
 	
 	GrafPtr port = ::GetWindowPort( winRef );
	CGContextRef contextRef = 0;
	OSStatus status = ::CreateCGContextForPort( port, &contextRef );
	if( status != noErr || contextRef == 0 )	return;
  	
//	Rect frame;
//	::GetPortBounds( port, &frame );
  	
	GDeviceOSX * gdc = new GDeviceOSX ( contextRef, width, height );
	GDeviceTester * tester = new GDeviceTester ( gdc, width, height );

	tester->TestFont("Guido2", height / 20 ); //Font( );
	delete tester;
	delete gdc;

	// - Cleanup
	::CGContextFlush( contextRef );
	::CGContextRelease( contextRef );
*/
}

// ---------------------------------------------------------------------------
//		* ExportBitmap
// ---------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::ExportBitmap()
{
	GuidoDocWindow * doc = GetFrontDocument();
	if( doc == 0 )
	{
		ReportError( "Nothing to export.", "Please open the music score you want to convert first." );
		return;
	}
		
	ExportImageWizard wizard ( GetNibRef(), doc->GetWindowRef());
		
	// We might need to make a copy of the GR before export.	
	wizard.Invoke( doc->GetGR(), doc->GetPageNum(), doc->GetFileNameCFString());
}

// ---------------------------------------------------------------------------
//		* ReleaseInitGDevice
// ---------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::ReleaseInitGDevice()
{
	// - First, destroy the gdevice
	delete mInitGDevice; 
	mInitGDevice = 0;
	delete mInitGSystem; 
	mInitGSystem = 0;
	
	// - Destroy the foo window

	if( mFooWindow )
	{
		::DisposeWindow( mFooWindow );
		mFooWindow = 0;
	}
}

// ---------------------------------------------------------------------------
//		* InitGuido
// ---------------------------------------------------------------------------
bool
GuidoQuartzViewerApp::InitGuido() 
{
	MakeInitGDevice();

	// - Init or reinit Guido
	GuidoInitDesc desc;
	desc.graphicDevice = mInitGDevice;
//	desc.feedback = 0; // &mFeedback;
	desc.textFont = "Monaco";
	desc.musicFont = 0;
	
	GuidoErrCode result = GuidoInit( &desc );
	if( result != guidoNoErr )
	{
		ReportGuidoError( result );
		return false;
	}

// ShowDebugWindow(); 	// DEBUG

	return true;
}

// ---------------------------------------------------------------------------
//		* OpenFile
// ---------------------------------------------------------------------------
// Display the file selector
void
GuidoQuartzViewerApp::OpenFile()
{
	::DisableMenuCommand( NULL, kHICommandOpen );

	FSRef filePath;
	MacFileSelector fs;
	fs.SetQueryMessage( "Please select a Guido Music Notation (.gmn) file." );
	if( fs.SelectOneFile( & filePath ))
		OpenFile( filePath );
	else
		ReportError( "Failed to open file." );

	::EnableMenuCommand( NULL, kHICommandOpen );
}

// ---------------------------------------------------------------------------
//		* OpenFile
// ---------------------------------------------------------------------------
void 		
GuidoQuartzViewerApp::OpenFile( const FSRef & inPath )
{
/*	// - Test: get the sample guido file
	FSRef fileRef;
	OSType folderType = kDesktopFolderType;	// TEST
	OSErr err = ::FSFindFolder( 0, folderType, false, &fileRef );
 */ 
 
// 	- Convert from FSRef to c-string path
//  const int pathSize = 1024;
//	char pathString[ pathSize ];
//	OSStatus status = ::FSRefMakePath( &inPath, (unsigned char *)pathString, pathSize );

//	std::string pathStr ( filePath );
//	pathStr += "/sample.gmn";
//	OpenGuidoFile( pathStr.c_str())

	// - Check if this doc is already open.
	GuidoDocWindow * inDoc = FindDocumentForPath( inPath );

  	const int pathSize = 1024;
	char pathString[ pathSize ];
	pathString[ 0 ] = 0;
	::FSRefMakePath( &inPath, (unsigned char *)pathString, pathSize );
	char * ext = &pathString[strlen(pathString)-4];
	if (!strcmp(ext,".gmn")) 
		OpenGuidoFile( inPath, inDoc );
	else
		OpenXMLFile( inPath, inDoc );
	UpdateAllMenuStates();
}

// ---------------------------------------------------------------------------
//		* OpenGuidoFile
// ---------------------------------------------------------------------------
// Parse a Guido .gmn file and give the resulting handle to a document window.
// If the caller can provide an existing document via inDoc, it is usefull for the
// 'reload' command. If inDoc = nil, a new document is created.
void
GuidoQuartzViewerApp::OpenGuidoFile( const FSRef & inPath, GuidoDocWindow * inDoc ) 
{
	// - Convert input path from FSRef type to a c-string path.
   	const int pathSize = 1024;
	char pathString[ pathSize ];
	pathString[ 0 ] = 0;
	::FSRefMakePath( &inPath, (unsigned char *)pathString, pathSize );

	MClock clock;	// Voir getrusage

	// - Parse the gmn file to create the AR structure
	
	ARHandler handleAR = 0;
	GuidoErrCode guidoErr = GuidoParseFile( pathString, &handleAR );
	if(( guidoErr != guidoNoErr ) || ( handleAR == 0 ))
	{
		ReportGuidoError( guidoErr );
		return;
	}
	
	// - Convert AR to GR
	
	GRHandler handleGR = 0;
	guidoErr = GuidoAR2GR( handleAR, &mSettings, &handleGR );
	if(( guidoErr != guidoNoErr ) || ( handleAR == 0 ))
	{
		ReportGuidoError( guidoErr );
		GuidoFreeGR( handleGR );
		GuidoFreeAR( handleAR );
		return;
	}
	
	GuidoFreeAR( handleAR );		// Free the AR immediately (this may change: may need it later)
	
	cout << "GuidoQuartzViewer: time to parse the file: " << clock.Milliseconds() << " ms" << endl;
	
	// - Parse OK, perform some post-processing
	
	GuidoResizePageToMusic( handleGR );
	
	// - Create a new document for this new guido handle, or recycle
	// input document, if any.
	gXMLFile = false;
	bool isNewDoc = false;
	GuidoDocWindow * theDoc;
	if( inDoc )
		theDoc = inDoc;
	else
	{
		theDoc = CreateGuidoDocWindow( &inPath );
		if( theDoc == nil )
		{
			ReportError( "Error: Failed to create a new document window." );
			return;
		}	
		isNewDoc = true;
	}	
	
	// - Give the new Guido handle to the document

	bool success = theDoc->NotifyNewGuidoHandle( handleGR );

	if( isNewDoc )
		theDoc->BringToScreen();
	else
		theDoc->RefreshGuidoView();
	
	// - Cleanup if failed
	if( success == false )
	{
		GuidoFreeGR( handleGR );
		if( isNewDoc )
			CloseDocument( theDoc ); //delete theDoc;
		
		ReportError( "Error: failed to handle the result of parsing." );
	}
}

// ---------------------------------------------------------------------------
void
GuidoQuartzViewerApp::OpenXMLFile( const FSRef & inPath, GuidoDocWindow * inDoc, int mode )
{

#ifdef GUIDO_USE_MUSICXML
 
  	const int pathSize = 1024;
	char pathString[ pathSize ];
	pathString[ 0 ] = 0;
	OSStatus status = ::FSRefMakePath( &inPath, (unsigned char *)pathString, pathSize );

    TMusicXMLFile file;
	SScore score = file.read(pathString);
	if (score) {
	    guidoconverter c;
		int gr;
		switch (mode) {
			case 1: gr = c.makePitchOnlyGR(score); break;
			case 2: gr = c.makeRythmOnlyGR(score, "g"); break;
			default: gr = c.makeGR(score); break;
		}
		if( GuidoValidHandle( gr )) {
			gXMLFile = true;
			GuidoDocWindow * theDoc = CreateGuidoDocWindow();
			if( theDoc ) {
				theDoc->NotifyNewGuidoHandle( gr );
				theDoc->SetPath(&inPath);
				theDoc->XML(true);
				theDoc->BringToScreen();
			}
		}
		//else cerr << "GuidoQuartzViewer error: not a valid handle " << gr << endl;
		else ReportGuidoError( gr );
	}
	// else cerr << "GuidoQuartzViewer error: cannot read file \"" << pathString << "\"" << endl;
	else ReportError( "Error: cannot read file:", pathString );

#endif

}

// ----------------------------------------------------------------------------
//		* ReportGuidoError
// ----------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::ReportGuidoError( GuidoErrCode inGuidoErr )
{
	std::string errorStr ( GuidoGetErrorString( inGuidoErr ));
		
	// - In the case of a parse error, add the line where the error occured.
	if( inGuidoErr == guidoErrParse )
	{
		std::ostringstream errLine;
		errLine << " \nline: " << GuidoGetParseErrorLine();
		errorStr += errLine.str();
	}
	else	// Guido engine already displays parse errors.
		std::cerr << "Guido error: " << errorStr.c_str() << std::endl;	
		
	MacAlertBox box;
	box.Alert( "Guido error", errorStr.c_str());
}

// ----------------------------------------------------------------------------
//		* ReportError
// ----------------------------------------------------------------------------
void		
GuidoQuartzViewerApp::ReportError( const char * inMessage, const char * inDetail )
{
	std::cerr << "GuidoViewer error: " << inMessage;
	
	if( inDetail )
		std::cerr << ", " << inDetail;
	
	std::cerr << std::endl;

	MacAlertBox box;
	box.Alert( inMessage, inDetail );
}

// ---------------------------------------------------------------------------
//		* FindWindowByTitle
// ---------------------------------------------------------------------------
// TODO: move in toolkit !
WindowRef		
GuidoQuartzViewerApp::FindWindowByTitle( CFStringRef inTitle )
{
	WindowRef outWin = 0;
	WindowRef win = ::GetFrontWindowOfClass( kAllWindowClasses, false );
	while( win )
	{
		CFStringRef title = 0;
		::CopyWindowTitleAsCFString( win, &title );
		if( title )
		{
			CFComparisonResult result = ::CFStringCompare( inTitle, title, kCFCompareCaseInsensitive );
			::CFRelease( title );

			if( result == 0 ) // then the strings are equal
			{
				outWin = win;
				break;
			}
		}
		win = ::GetNextWindowOfClass( win, kAllWindowClasses, false );	
	}
	return outWin;
}

// ----------------------------------------------------------------------------
void	
GuidoQuartzViewerApp::UpdateAllGRs()
{
	vector<GuidoDocWindow *>::iterator elt;
	for( elt = mDocList.begin(); elt != mDocList.end(); ++ elt ) {
		GRHandler gr = (*elt)->GetGR();
		GuidoUpdateGR (gr, &mSettings);
		(*elt)->RefreshGuidoView();
	}
}

// ----------------------------------------------------------------------------
//		* Display prefs dialog
// ----------------------------------------------------------------------------
void	
GuidoQuartzViewerApp::DisplayPrefsDialog()
{
	fPrefs.OpenDialog (GetNibRef(), CFSTR("PrefsWindow"), this);
	UpdateAllGRs ();
}


// ----------------------------------------------------------------------------
//		* DisplayAboutBox
// ----------------------------------------------------------------------------
void	
GuidoQuartzViewerApp::DisplayAboutBox()
{
	WindowRef winRef = FindWindowByTitle( CFSTR("About"));
	if( winRef )
	{	
		::ShowWindow( winRef );
		return;
	}
	
	OSErr				err;
	NumVersion			version;
	CFTypeRef			bundleName	= 0;
	CFStringRef			appVersion	= 0;
	CFStringRef			libVersion	= 0;
	
    err	= ::CreateWindowFromNib( GetNibRef(), CFSTR("AboutBoxWindow"), &winRef );
	if ( err != noErr )	return;

	// - Set app version
	*((UInt32*)&version) = ::CFBundleGetVersionNumber( ::CFBundleGetMainBundle());						//	Display the version number
	appVersion	= ::CFStringCreateWithFormat( NULL, NULL, CFSTR("Version %d.%d.%d\nCopyright (c) 2003-2004 Grame"),
									 version.majorRev, version.minorAndBugRev >> 4, version.minorAndBugRev & 0x0F );

	const int appCaptionID = 1;
	MacTools::SetControlString( winRef, 'STxt', appCaptionID, appVersion, true );

	// - Set guidolib version
	int major, minor, sub;
	GuidoGetVersionNums( &major, &minor, &sub ); // deprecated
	libVersion	= ::CFStringCreateWithFormat( NULL, NULL, CFSTR("Guido Engine version %d.%d.%d\n"),
									 major, minor, sub );

	const int libCaptionID = 2;
	MacTools::SetControlString( winRef, 'STxt', libCaptionID, libVersion, true );

	// - Set Guido URL
	const int urlCaptionID = 3;
	MacTools::SetControlString( winRef, 'STxt', urlCaptionID, kGuidoURLString, true );
		
	// - Display the about box window.

	const EventTypeSpec	windowEvents[] =
    {
    	{ kEventClassCommand,  	kEventProcessCommand }
	};
      
	void * userData = 0;
//	err	= ::InstallWindowEventHandler( winRef, GetGuidoWindowEventHandlerUPP(), 
//					GetEventTypeCount( windowEvents ), windowEvents, userData, NULL );

	EventTargetRef targetRef = ::GetWindowEventTarget( winRef ); 

	::InstallEventHandler( targetRef, GetGuidoWindowEventHandlerUPP(),
   							GetEventTypeCount( windowEvents ), windowEvents, userData, NULL );

	::ShowWindow( winRef );

	if ( bundleName != 0 ) 	::CFRelease( bundleName );
	if ( appVersion != 0 )	::CFRelease( appVersion );
	if ( libVersion != 0 )	::CFRelease( libVersion );
}


// ----------------------------------------------------------------------------
//		* DisplayGuidoWebPage
// ----------------------------------------------------------------------------
void
GuidoQuartzViewerApp::DisplayGuidoWebPage()
{
	DisplayWebPage( kGuidoURLString );
//	if( result == false )
//		ReportError( "Cannot connect to the Guido Web site.", guidoURLStr );
}

// ----------------------------------------------------------------------------
//		* DisplayWebPage
// ----------------------------------------------------------------------------
// Application independant, must be in toollkit
bool
GuidoQuartzViewerApp::DisplayWebPage( CFStringRef inCFString ) 
{
	if( inCFString == 0 )	return false;

	bool success = false;
	CFURLRef theURL = ::CFURLCreateWithString( kCFAllocatorDefault, inCFString, 0 );
	if( theURL )
	{
		OSStatus status = ::LSOpenCFURLRef( theURL, 0 );
		if( status != noErr )
			success = true;
		
		::CFRelease( theURL );
	}

	return success;
}

/*
// ---------------------------------------------------------------------------
//		* OpenGuidoFile
// ---------------------------------------------------------------------------
// Parse a Guido .gmn file and gove the resulting handle to a document window.
// If the caller can provide an existing document via inDoc, it is usefull for the
// 'reload' command. If inDoc = nil, a new document is created.
void
GuidoQuartzViewerApp::OpenGuidoFile( const char * inPath, GuidoDocWindow * inDoc ) 
{
	std::cout << "Guido: opening file " << inPath << endl;

	// - Parse	the gmn file
	MClock clock;
	int guidoResult = GuidoParse( inPath );
	cout << "Guido: time to parse the file: " << clock.Milliseconds() << " ms" << endl;

	if( GuidoValidHandle( guidoResult ))
	{
		// - Parse successful, get new document window for the new guido handle.
		bool isNewDoc = false;
		GuidoDocWindow * theDoc;
		if( inDoc )
			theDoc = inDoc;
		else
		{
			theDoc = CreateGuidoDocWindow();
			isNewDoc = true;
		}	

		// - Give the parse result to the document
		bool accepted = false;
		if( theDoc ) 
		{
			accepted = theDoc->NotifyNewGuidoHandle( guidoResult );
			if( isNewDoc && accepted )
			{
				// TODO: put theDoc in the global list of documents 
			}
			else if( isNewDoc )
				delete theDoc;
		}
		
		if( accepted == false )
		{
			GuidoFreeHandle( guidoResult );
			cout << "GuidoQuartzViewer error: failed to handle the result of parse." << endl;
		}
	}
	else
	{
		std::cout << "Guido error: " << GuidoGetErrorString( guidoResult ) << std::endl;
		MacAlertBox alert;
		alert.SetupButtons( false, true, false, MacAlertBox::kButtonCancel );
		alert.Alert( "Guido error", GuidoGetErrorString( guidoResult ));
	}
}
*/
