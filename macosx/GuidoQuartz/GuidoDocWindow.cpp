/*
 * 	File : GuidoDocWindow.cpp
 *
 *	This window has scrollbars. Scroll units are in pixels. 
 *
 */

#include <iostream>

// - Toolkit
#include "MacTools.h"

// - Guido
#include "GUIDOEngine.h"

// - App
#include "GuidoDocWindow.h"
#include "GuidoCarbonControl.h"
#include "GuidoViewerConsts.h"

// ---------------------------------------------------------------------------
static pascal void MyWindowScrollProc( ControlHandle control, SInt16 part ) 
{
	// - Find the parent window of the scrollbar then the corresponding guido control.
	WindowRef winRef = ::GetControlOwner( control );
	ControlRef guidoControlRef = MacTools::FindControl( winRef, kGuidoControlID );

	GuidoCarbonControl * guidoControl = GuidoCarbonControl::FindControlByRef( guidoControlRef );
	if( guidoControl == 0 ) return;

	ControlRef horizontalScroller = MacTools::FindControl( winRef, kScrollBarH );
//	ControlRef verticalScroller = MacTools::FindControl( GetWindowRef(), kScrollBarV );

	bool hScroll = (control == horizontalScroller);
	
	// - Now we can work with the guido control object.
	// We work in pixel units.
	
	int contentWidth; 
	int contentHeight;
	guidoControl->GetSize( &contentWidth, &contentHeight ); 
	
	SInt32 currPos = ::GetControl32BitValue( control );            //1 
	SInt32 minPos = ::GetControl32BitMinimum( control ); 
	SInt32 maxPos = ::GetControl32BitMaximum( control ); 

	SInt32 newPos = currPos;

	const int kScrollInc = 16; 

	switch (part) 
	{
		case kControlUpButtonPart: 	newPos -= kScrollInc;	break; 
		case kControlDownButtonPart: newPos += kScrollInc;	break; 

		case kControlPageUpPart:  
			if( hScroll ) newPos -= contentWidth; 
			else  newPos -= contentHeight; 
			break; 

		case kControlPageDownPart: 
			if( hScroll ) newPos += contentWidth; 
			else  newPos += contentHeight; 
			break; 
	
		case kControlIndicatorPart:		// Never called if live scrolling is off.
			newPos = currPos - minPos; 
			currPos = -1;
			break;
		
	}
	
	if( newPos > maxPos ) newPos = maxPos;
	else if( newPos < minPos ) newPos = minPos;

	if( newPos != currPos ) 
	{
		::SetControl32BitValue( control, newPos );

		if( hScroll )
			guidoControl->SetScrollX( newPos );		
		else
			guidoControl->SetScrollY( newPos );					
		
		// - Redraw the control
		::Draw1Control( guidoControlRef );  
	}
}

ControlActionUPP GuidoDocWindow::sScrollerUPP = 0; 
// ---------------------------------------------------------------------------
//		* GuidoDocWindow
// ---------------------------------------------------------------------------
GuidoDocWindow::GuidoDocWindow() // : mGDevice( 0, 600, 800 )
{
	mGuidoControl = nil;
	mWindowRef = 0;
	mPathSet = false;
	mXMLFile = false;
	mFileNameCF = 0;
	mFileNameUni.length = 0;
	
	// - Create our Guido custom control
	mGuidoControl = new GuidoCarbonControl;
	if( mGuidoControl )
	{
//		mGuidoControl->SetGDevice( GetGDevice());
		mGuidoControl->SetGuidoDocInfos( this );
		mGuidoControl->SetParam( GuidoCarbonControl::kDrawSymbolMap, 0 );
	}
}

// ---------------------------------------------------------------------------
//		* ~GuidoDocWindow
// ---------------------------------------------------------------------------
GuidoDocWindow::~GuidoDocWindow()
{
	delete mGuidoControl;

	// - Free the current guido handle, if any
	FreeARGR();

	if( mFileNameCF )
		::CFRelease( mFileNameCF );

	// TODO: cleaning
}

// ----------------------------------------------------------------------------
//		* SetupGuidoWindow
// ----------------------------------------------------------------------------
bool 
GuidoDocWindow::SetupGuidoWindow( WindowRef inWinRef )
{
	mWindowRef = inWinRef;
	
	WindowRef winRef = GetWindowRef();
	if( winRef == 0 )
		return false;
		
	// - Setup our Guido Control
	if( mGuidoControl )
	{
		ControlRef theControlRef = MacTools::FindControl( winRef, kGuidoControlID );
		mGuidoControl->SetControlRef( theControlRef );
	}
	
	// - Get the core-graphics context of our window
/*	GrafPtr port = ::GetWindowPort( winRef );
	CGContextRef contextRef = 0;
	OSStatus status = ::CreateCGContextForPort( port, &contextRef );
	if( status != noErr )
	{
		std::cout << "GuidoQuartzViewerApp error: cannot get the current graphic port context." << std::endl;
		return false;
	}
	
	// - Get the port bounds
	Rect frame;
	::GetPortBounds( port, &frame );
	
	// - Setup the score rendering GDevice
	
	mGDevice.SetCGContext( contextRef, frame.right - frame.left, frame.bottom - frame.top );
//	mGDevice.SetSymbolMap( &mSymbolMap );
//	mSymbolMap.SetGDevice( &mGDevice );
	
	// - The device is now the owner of the context ref
	::CGContextRelease( contextRef );
*/
	InitScrollBars();
	
	return true;	
}

// ---------------------------------------------------------------------------
//		* InitScrollBars
// ---------------------------------------------------------------------------
void
GuidoDocWindow::InitScrollBars()
{
	if( sScrollerUPP == 0 )
	{
		sScrollerUPP = ::NewControlActionUPP( MyWindowScrollProc ); 
		if( sScrollerUPP == 0 ) return;
	}

	ControlRef horizontalScroller = MacTools::FindControl( GetWindowRef(), kScrollBarH );
	ControlRef verticalScroller = MacTools::FindControl( GetWindowRef(), kScrollBarV );
	if( !horizontalScroller || !verticalScroller ) return;

	::SetControlAction( verticalScroller, sScrollerUPP );          
	::SetControlAction( horizontalScroller, sScrollerUPP ); 
	
	AdjustScrollBars();
}

// ---------------------------------------------------------------------------
//		* AdjustScrollBars
// ---------------------------------------------------------------------------
// Update the scrollbar settings according to the new size of control and/or 
// the new size of its content.
// Does not redraw anything. 
// Must be called after control resizing, zoom on music...
void
GuidoDocWindow::AdjustScrollBars()
{
	GuidoCarbonControl * control = GetGuidoControl();
	if( control == 0 ) return;
	
	float totalWidth;
	float totalHeight;
	control->GetFullScoreSize( &totalWidth, &totalHeight );

	int controlWidth;
	int controlHeight;
	control->GetSize( &controlWidth, &controlHeight );

	ControlRef horizontalScroller = MacTools::FindControl( GetWindowRef(), kScrollBarH );
	ControlRef verticalScroller = MacTools::FindControl( GetWindowRef(), kScrollBarV );

	::SetControl32BitMinimum( horizontalScroller, 0 ); 
	::SetControl32BitMinimum( verticalScroller, 0 ); 

	int xRange = (int)totalWidth - controlWidth;
	int yRange = (int)totalHeight - controlHeight;
	if( xRange < 0 ) xRange = 0;
	if( yRange < 0 ) yRange = 0;
	
	::SetControl32BitMaximum( horizontalScroller, xRange ); 
	::SetControl32BitMaximum( verticalScroller,  yRange );  

	if( control->GetScrollX() > xRange ) control->SetScrollX( xRange );	// To check
	if( control->GetScrollY() > yRange ) control->SetScrollY( yRange ); //

	::SetControl32BitValue( horizontalScroller, control->GetScrollX()); 
	::SetControl32BitValue( verticalScroller, control->GetScrollY());

	::SetControlViewSize( verticalScroller, controlHeight ); 
	::SetControlViewSize( horizontalScroller, controlWidth ); 
}

// ----------------------------------------------------------------------------
//		* PlaceScrollBars
// ----------------------------------------------------------------------------
void
GuidoDocWindow::PlaceScrollBars()
{
	// - Get the window bounds
	Rect wRect;
	::GetWindowBounds( GetWindowRef(), kWindowContentRgn , &wRect );	

	const int sizeX = wRect.right - wRect.left;
	const int sizeY = wRect.bottom - wRect.top;

	// - Get the scrollbars
	ControlRef hScroller = MacTools::FindControl( GetWindowRef(), kScrollBarH );
	ControlRef vScroller = MacTools::FindControl( GetWindowRef(), kScrollBarV );

	const int thick = GetScrollBarThickness();
	
	// - Move our scrollbars controls to their new location.

	::MoveControl( hScroller, 0, sizeY - thick );
	::SizeControl( hScroller, sizeX - thick, thick );

	::MoveControl( vScroller, sizeX - thick, 0 );
	::SizeControl( vScroller, thick, sizeY - thick );


//	::SetRect( &hsRect, wRect.left, wRect.bottom - thick, wRect.right - thick, wRect.bottom );
//	::SetRect( &vsRect, wRect.right - thick, wRect.top, wRect.right, wRect.bottom - thick );

//	::SetControlBounds( hScroller, &hsRect );
//	::SetControlBounds( vScroller, &vsRect );
}

// ---------------------------------------------------------------------------
//		* NotifyWindowScrolled
// ---------------------------------------------------------------------------
void
GuidoDocWindow::NotifyWindowScrolled()
{
	GuidoCarbonControl * guidoControl = GetGuidoControl();
	if( guidoControl == 0 ) return;

	ControlRef scrollBarH = MacTools::FindControl( GetWindowRef(), kScrollBarH );
	ControlRef scrollBarV = MacTools::FindControl( GetWindowRef(), kScrollBarV );
	
	SInt32 newX = ::GetControl32BitValue( scrollBarH );
	SInt32 newY = ::GetControl32BitValue( scrollBarV );

	guidoControl->SetScrollX( newX );		
	guidoControl->SetScrollY( newY );					
		
	// - Redraw the control
	guidoControl->Draw();
}

// ---------------------------------------------------------------------------
//		* GetScrollBarThickness
// ---------------------------------------------------------------------------
int
GuidoDocWindow::GetScrollBarThickness() const
{
	Rect hsRect;
	ControlRef hScroller = MacTools::FindControl( GetWindowRef(), kScrollBarH );
	::GetControlBounds( hScroller, &hsRect );
	return ( hsRect.bottom - hsRect.top );
}

// ---------------------------------------------------------------------------
//		* NotifyNewGuidoHandle
// ---------------------------------------------------------------------------
// The guido handle has changed, for a particular window. It happens when
// user 'reload' the file.
// The previous guido handle is relased.
// The scrollbars are repositionned, but the window not refreshed.
bool
GuidoDocWindow::NotifyNewGuidoHandle( GRHandler inHandleGR ) 
{
	bool accepted = false;	
	GuidoCarbonControl * control = GetGuidoControl();
	if( control )
	{
		ARHandler prevAR = GetAR();
		GRHandler pregGR = GetGR();		
		SetARGR( 0, inHandleGR );
		
		if( pregGR != inHandleGR )
		{
			control->SetScrollX( 0 );
			control->SetScrollY( 0 );
			SetPageNum( 1 );
			GuidoFreeGR( pregGR );
			GuidoFreeAR( prevAR );
		}
		accepted = true;
	}	
	AdjustScrollBars();
	return accepted;
}

// ----------------------------------------------------------------------------
//		* BringToScreen
// ----------------------------------------------------------------------------
// Resize and show the window.
void
GuidoDocWindow::BringToScreen()
{
 	OSStatus err;
 	
 	// - Get the max possible size onscreen.
 	WindowRef winRef = GetWindowRef();
	if( winRef == 0 ) return;
 
   	Rect maxFrame = { 0, 0, 0, 0 };
    GDHandle device = ::GetMainDevice();
 	err = ::GetAvailableWindowPositioningBounds( device, &maxFrame );
 
 	// -- Get an acceptable size closest to the score size.
	
	// - First, we get the difference between the entire window size and 
	//  the window content size.
	
	Rect winInFrame;
  	Rect winOutFrame;
 	
 	if( err == noErr )
 		err = ::GetWindowBounds( winRef, kWindowContentRgn, &winInFrame );

 	if( err == noErr )
 		err = ::GetWindowBounds( winRef, kWindowStructureRgn, &winOutFrame );
 	
 	if( err == noErr )
 	{
 		const int deltaX = (winOutFrame.right - winOutFrame.left) - (winInFrame.right - winInFrame.left);
  		const int deltaY = (winOutFrame.bottom - winOutFrame.top) - (winInFrame.bottom - winInFrame.top);
	
  		const int margin = GetScrollBarThickness();

		// - Next, we resize the window according to the score size and available screen space. 
 		GuidoCarbonControl * control = GetGuidoControl();
		if( control )
		{
			float scoreWidth = 0;
			float scoreHeight = 0;
	 	 	control->GetFullScoreSize( &scoreWidth, &scoreHeight );
	 		
	 	
	 		if( scoreWidth < 100 ) scoreWidth = 100;
	 		if( scoreHeight < 100 ) scoreHeight = 100;
	 	
	 		Rect newWinBounds = { 	maxFrame.top, 		// Rects are: top, left, bottom, right
	 								maxFrame.left, 
	 								maxFrame.top + (int)scoreHeight + margin + deltaY,
	 								maxFrame.left + (int)scoreWidth + margin + deltaX };
	 								 
	 		
	 		if( newWinBounds.right > maxFrame.right )  newWinBounds.right = maxFrame.right;
	 		if( newWinBounds.bottom > maxFrame.bottom )  newWinBounds.bottom = maxFrame.bottom;

			err = ::SetWindowBounds( winRef, kWindowStructureRgn, &newWinBounds ); 	
		} 	
 	}
 
 	// - Setup window size.
	
 	RefreshWindowTitle();	
	SizeChanged();
  
 	::SelectWindow( winRef ); // Makes the window active and bring it to front.
	err = ::RepositionWindow( winRef, 0, kWindowCascadeOnMainScreen );
	::ShowWindow( winRef );			
}

// ---------------------------------------------------------------------------
//		* RefreshGuidoView
// ---------------------------------------------------------------------------
// - Show or refresh the score window
// Called when Guido state changed (zoom, page, new guido handle...)
void
GuidoDocWindow::RefreshGuidoView()
{
	WindowRef winRef = GetWindowRef();
	if( winRef == 0 ) return;

	::SelectWindow( winRef ); // Makes the window active and bring it to front.
	::ShowWindow( winRef );			
	
	ControlRef theControlRef = 0;
	OSErr err = ::GetRootControl( winRef, &theControlRef );

	if( err == noErr && theControlRef != 0 )
	//	::DrawControls ( GetWindowRef());
		::Draw1Control( theControlRef ); 
	else
		std::cout << "GuidoDocWindow error " << err << " in RefreshGuidoView()" << std::endl;
	
	RefreshWindowTitle();	
}

// ---------------------------------------------------------------------------
//		* RefreshWindowTitle
// ---------------------------------------------------------------------------
void			
GuidoDocWindow::RefreshWindowTitle()
{
	// - Get the current page displayed.
	CFStringRef pageInfos = ::CFStringCreateWithFormat( NULL, NULL, CFSTR(" - page %d of %d"),
									 						GetPageNum(), PageCount());
	// - Build the title with the filename and current page.
	CFMutableStringRef title = ::CFStringCreateMutable( kCFAllocatorDefault, 0 );
	if( title )
	{	
		if( mFileNameCF )	::CFStringAppend( title, mFileNameCF );
		if( pageInfos )		::CFStringAppend( title, pageInfos );

		::SetWindowTitleWithCFString( GetWindowRef(), title );
		::CFRelease( title ); 
	}
	
	if( pageInfos )	
		::CFRelease( pageInfos );
}

// ---------------------------------------------------------------------------
//		* SizeChanged
// ---------------------------------------------------------------------------
// Adjust the content of the window when its size has changed.
// the window is not refreshed onscreen.
void			
GuidoDocWindow::SizeChanged()
{
	PlaceScrollBars();

	Rect contentBounds;
	::GetWindowBounds( GetWindowRef(), kWindowContentRgn, &contentBounds );	

	const int thick = GetScrollBarThickness();
	
	const int newWidth = contentBounds.right - contentBounds.left - thick;
	const int newHeight = contentBounds.bottom - contentBounds.top - thick;

//	mGDevice.NotifySize( newWidth, newHeight ); // does not depend on the window size anymore (new scrollbars)
	
	GuidoCarbonControl * control = GetGuidoControl();
	if( control )
	{
	 	// ::SetControlBounds( control->GetControlRef(), &newBounds );
		::MoveControl( control->GetControlRef(), 0, 0 );
		::SizeControl( control->GetControlRef(), newWidth, newHeight );
	}

	AdjustScrollBars();
 	// cout << "New control size: " << newWidth << " x " << newHeight << endl;
}

// ----------------------------------------------------------------------------
//		* SetShowMapping
// ----------------------------------------------------------------------------
void
GuidoDocWindow::SetShowMapping( bool inState )
{ 
	if( mGuidoControl )
	{
		mGuidoControl->SetParam( GuidoCarbonControl::kDrawSymbolMap, (float)inState );
	}
}

// ----------------------------------------------------------------------------
//		* GetShowMapping
// ----------------------------------------------------------------------------
bool
GuidoDocWindow::GetShowMapping() const
{ 
	if( mGuidoControl )
		return ((bool)(mGuidoControl->GetParam( GuidoCarbonControl::kDrawSymbolMap )));

	return false;
}

// ----------------------------------------------------------------------------
//		* HandleMouseEvent
// ----------------------------------------------------------------------------
OSStatus
GuidoDocWindow::HandleMouseEvent( EventRef inEvent )
{ 
//	OSStatus err;
	OSStatus outStatus = eventNotHandledErr;

/*	const UInt32 eventKind = ::GetEventKind( inEvent );	
	switch( eventKind )
	{
		case kEventMouseDown:
		{ 
			Point clickPoint;
			err = ::GetEventParameter( inEvent, kEventParamMouseLocation, typeQDPoint, 
													NULL, sizeof(Point), NULL, &clickPoint); 
			if( err == noErr )
				cout << "Window clicked ( " << clickPoint.h << ", " << clickPoint.v << " )" << endl;
			break; 
		}
		
		case kEventMouseUp:
		{
			cout << "Window mouseup" << endl;
		}
	} 
*/
	return outStatus;
}

// ----------------------------------------------------------------------------
//		* HandleZoomCommand
// ----------------------------------------------------------------------------
void
GuidoDocWindow::HandleZoomCommand( FourCharCode inCommand )
{ 
	GuidoCarbonControl * control = GetGuidoControl();
	if( control == 0 ) return;

	const float prevZoom = control->GetZoom();

	switch( inCommand )
	{
		case kCmdZoomIn: control->ZoomIn();				break;
		case kCmdZoomOut: control->ZoomOut();			break;
		case kCmdZoomDefault: control->SetZoom( 1 );	break;
	}

	const float newZoom = control->GetZoom();
	if( newZoom != prevZoom )
	{	
		AdjustScrollBars();
		control->Draw();
	}
}

// ----------------------------------------------------------------------------
//		* GetZoom
// ----------------------------------------------------------------------------
float GuidoDocWindow::GetZoom() const 
	{ return mGuidoControl ? mGuidoControl->GetZoom() : 1; }

// ----------------------------------------------------------------------------
//		* SetPath
// ----------------------------------------------------------------------------
void
GuidoDocWindow::SetPath( const FSRef * inPath )
{ 
	if( inPath ) 
	{
		// - First store the new path
		mFilePath = *inPath;
		mPathSet = true;

		SyncParentPath();	// Store our parent directory.
		
		// - Everything ok, get the file name.
		MakeFileName();
	}
	else
		mPathSet = false;	
}
 
 // ----------------------------------------------------------------------------
//		* GetPath
// ----------------------------------------------------------------------------
FSRef *
GuidoDocWindow::GetPath()
{ 
	if( mPathSet )
	{
		if( PathIsValid())	// Path is valid, but the file might be moved or renamed.
		{
			// If the file was moved and replaced by a file with the
			// same name, use this new file.
			
			FSRef prevLocation;
			if( BuildPath( &prevLocation ))	// Get the original location of our file.
			{
				if( ::FSCompareFSRefs( &prevLocation, &mFilePath ) != noErr )
				{
					// - File has been moved and replaced, use the new file.
					// (ie: happen if the file has been trashed).
					mFilePath = prevLocation;
				}
			}
			else
			{
				// - File has been moved or renamed without beeing replaced.
				// Just update parent path.
				SyncParentPath();	
				
				// TODO: handle file renaming.
			}
		}
		else	// Path is no more valid, try to rebuild it.
		{	
			//cout << "GuidoQuartzViewer error: invalid file path" << endl;
			if( BuildPath( &mFilePath ) == false )	
				std::cout << "GuidoQuartzViewer error: cannot find the current gmn file." << std::endl;
		}
		return &mFilePath;
	}
	else
		return 0;
}

// ----------------------------------------------------------------------------
//		* BuildPath
// ----------------------------------------------------------------------------
// Syncronizes our path to ensure that it still points to its file.
bool
GuidoDocWindow::BuildPath( FSRef * inPath )
{
	if( mPathSet == false ) return false;
	if( inPath == nil ) return false;
	
	// - Rebuild a file path with our name and parent path.
	OSErr err = ::FSMakeFSRefUnicode( &mParentFilePath, mFileNameUni.length, 
									mFileNameUni.unicode, kTextEncodingUnknown, inPath );
	return ( err == noErr );
}

// ----------------------------------------------------------------------------
//		* SynchParentPath
// ----------------------------------------------------------------------------
// Update the parent path, just in the case the file was moved.
bool
GuidoDocWindow::SyncParentPath()
{
	OSErr err = ::FSGetCatalogInfo( &mFilePath, kFSCatInfoNone, NULL, 
										NULL, NULL, &mParentFilePath );

	return (err == noErr);
}

// ----------------------------------------------------------------------------
//		* PathIsValid
// ----------------------------------------------------------------------------
// Test if our path still point to a file.
bool 
GuidoDocWindow::PathIsValid()
{
	FSRef foo;
	OSErr err = ::FSGetCatalogInfo( &mFilePath, kFSCatInfoNone, NULL, 
										NULL, NULL, &foo );
	return (err == noErr);
}

// ----------------------------------------------------------------------------
//		* MakeFileName
// ----------------------------------------------------------------------------
void
GuidoDocWindow::MakeFileName()
{ 
	if( mFileNameCF ) 
		::CFRelease( mFileNameCF );
	
	if( mPathSet )
	{
		OSStatus status = ::FSGetCatalogInfo( &mFilePath, kFSCatInfoNone, NULL, &mFileNameUni, NULL, NULL );
		if( status != noErr )
			return;	
		
		mFileNameCF = ::CFStringCreateWithCharacters( kCFAllocatorDefault, 
                                                       mFileNameUni.unicode, mFileNameUni.length );
	}
	else
		mFileNameCF = ::CFStringCreateWithFormat( NULL, NULL, CFSTR("New document"));
}

//Rect bounds;
// OSStatus GetWindowBounds ( WindowRef window, WindowRegionCode regionCode, Rect * globalBounds ); 
//::GetWindowPortBounds ( mWindowRef, &bounds ); 
//OSStatus status = ::InvalWindowRect ( mWindowRef, &bounds );

//::SelectWindow( mWindowRef );

//::BeginUpdate ( mWindowRef );

//	ControlID * theControlID = ???;
//	OSStatus status = ::GetControlByID( mWindowRef, &kGuidoControlID, &theControlRef );
//::EndUpdate ( mWindowRef );
// OSStatus SetWindowTitleWithCFString ( WindowRef inWindow, CFStringRef inString );

//	CGrafPtr winPort = ::GetWindowPort( mWindowRef );
//	RgnHandle rgn = 0;
//	::QDFlushPortBuffer ( winPort, rgn );
