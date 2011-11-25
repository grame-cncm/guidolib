/*
 * File : GuidoCarbonControl.h
 *
 *	Display a music score, with the GuidoLib.
 *	
 *	The GuidoLib uses virtual "logical" units to manage the size of the score internaly.
 *	This logical size is usually about thousands of pixels wide (4000, 6000...)
 *	
 *	This control can be embeded in scrollable windows. Thus, only a small part of the
 *	score might be visible at the same time. The whole (visible + invisible) size, 
 *	in pixel units,	is calculated as follow: pixel size = zoom * logical size / 10.
 *
 *	Example: to fully display a logical 4000 * 3000 score, with zoom = 2, the size
 *	of this control should be 800 * 600 pixels.
 *	
 *
 */

// - Mac, Std c++
#include <Carbon/Carbon.h>
#include <iostream>

// - App
#include "GuidoCarbonControl.h"
#include "CGuidoDoc.h"
#include "GSystemOSX.h"


// - Toolkit
#include "MClock.h"

// - Guido
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"

// ----------------------------------------------------------------------------
pascal OSStatus GuidoCarbonControlEventHandler( EventHandlerCallRef inHandlerCallRef,
														EventRef inEvent, void * inUserData )
{
    // - Get the control ref (not supported by every events).
    ControlRef controlRef; 
	OSStatus err = ::GetEventParameter( inEvent, kEventParamDirectObject, typeControlRef, 
									NULL, sizeof( ControlRef ), NULL, &controlRef );

  	if( err == noErr )
  	{
  		GuidoCarbonControl * control = GuidoCarbonControl::FindControlByRef( controlRef );
         if( control )
         	return control->HandleControlEvent( inEvent );
	}

	return eventNotHandledErr;
}
    
// ----------------------------------------------------------------------------
int	GuidoCarbonControl::sInstanceNumGen = 0;
ToolboxObjectClassRef GuidoCarbonControl::sControlClassRef = 0;
EventHandlerUPP GuidoCarbonControl::sEventHandlerProc = 0;
std::vector<GuidoCarbonControl*> GuidoCarbonControl::sControlList;
// ----------------------------------------------------------------------------
//		* GuidoCarbonControl
// ----------------------------------------------------------------------------
GuidoCarbonControl::GuidoCarbonControl()
{
	for( int index = 0; index < kParamCount; index ++ )
		mParams [ index ] = 0;

	mScrollX = 0;
	mScrollY = 0;
	mControlRef = 0;
//	mScoreWidth = 100;
//	mScoreHeight = 100;
	
//	mGDevice = nil;
//	mGuidoRef = 0;
	mGuidoInfos	= nil;
	
 	mInstanceNum = (++ sInstanceNumGen);	
 
 	if( sControlList.size() == 0 )
 		Register();

	sControlList.push_back( this );
	
 	// cout << "Creating guido control: " << mInstanceNum << endl;
 }

// ----------------------------------------------------------------------------
//		* ~GuidoCarbonControl
// ----------------------------------------------------------------------------
GuidoCarbonControl::~GuidoCarbonControl()
{
	std::vector<GuidoCarbonControl *>::iterator elt;
	for( elt = sControlList.begin(); elt != sControlList.end(); ++ elt )
	{
		if( *elt == this )
		{	
			sControlList.erase( elt );
			break;
		}	
	}
	
	if( sControlList.size() == 0 )
		Unregister();
}

// ----------------------------------------------------------------------------
//		* Unregister
// ----------------------------------------------------------------------------
void 
GuidoCarbonControl::Unregister()
{
	if( sEventHandlerProc )
	{	
		::DisposeEventHandlerUPP( sEventHandlerProc );
		sEventHandlerProc = 0;
	}
	
/*	if( sControlClassRef )	// Crash :-/
	{
		::UnregisterToolboxObjectClass( sControlClassRef );
		sControlClassRef = 0;
	}
*/
}

// ----------------------------------------------------------------------------
//		* Register
// ----------------------------------------------------------------------------
bool 
GuidoCarbonControl::Register()
{
    //ControlDefSpec		defSpec;

	ToolboxObjectClassRef baseClass = NULL; 

 	const EventTypeSpec kEventList[] =
	{
		{ kEventClassControl, kEventControlDraw },
		{ kEventClassControl, kEventControlHitTest },
		{ kEventClassControl, kEventControlTrack }
		 
	//	{ kEventClassControl, kEventControlClick },
		//{ kEventClassControl, kEventControlHit }
		
		/* kEventControlActivate 
		kEventControlDeactivate 
		kEventControlHitTest 
		kEventControlTrack 
		kEventControlBoundsChanged 
		kEventControlInitialize 
		kEventControlDispose 
		kEventControlSimulateHit 
		kEventControlSetFocusPart 
		kEventControlGetFocusPart 
		kEventControlGetPartRegion */
	};
		
	void * userData = 0;	
	sEventHandlerProc = ::NewEventHandlerUPP( GuidoCarbonControlEventHandler );
	
    // - Register my event based custom control
    OSStatus status = ::RegisterToolboxObjectClass( CFSTR("GuidoCarbonControl"), baseClass, 
    								GetEventTypeCount( kEventList ), kEventList,
                              		sEventHandlerProc, userData, &sControlClassRef );
	
	// std::cout << "Guido control registration = " << status << std::endl;
	
	return ( status == noErr );
}

// ----------------------------------------------------------------------------
//		* HandleControlEvent
// ----------------------------------------------------------------------------
OSStatus
GuidoCarbonControl::HandleControlEvent( EventRef inEvent )
{
    OSStatus result = noErr;	// handled by default.
  	
    switch( ::GetEventKind( inEvent ))
    {
        case kEventControlDraw:		DoDraw();	 break;  
  		case kEventControlHitTest: 	HitTest( inEvent );	break;	
  		case kEventControlTrack:	TrackMouse( inEvent );	break;
  
  
	/*	case kEventControlClick:
        {
         	int x = 0;
         	int y = 0;
         	cout << "Guido control clicked ( " << x << ", " << y << " )" << endl;
         	break;
         }*/
         
         default:
          	result = eventNotHandledErr;
          	break;
    }
    
    return result;
}

// ----------------------------------------------------------------------------
//		* FindControlByRef
// ----------------------------------------------------------------------------
GuidoCarbonControl *
GuidoCarbonControl::FindControlByRef( ControlRef inRef )
{
	for( size_t index = 0; index < sControlList.size(); index ++ )
	{
		GuidoCarbonControl * control = sControlList[ index ];
		if( control && control->GetControlRef() == inRef )
			return control;
	}
	return nil;	
}

/*
static void drawMap( GRHandler gr, VGDevice * device, int pagenum )
{
	GuidoEltRef page = GuidoMapGetRootElement(gr, pagenum);
	if( !page ) return;
	
	int n = 1;	 GuidoScoreElement type = kGuidoScoreElementSystemSlice; // kGuidoScoreElementBar;
	int border = 0;
	GuidoEltRef slice = GuidoMapFindDeepNthElementOfType( page, type, n);
	while (slice) {
		float left, top, right, bottom;
		GuidoMapGetElementBoundingBox( slice, &left, &top, &right, &bottom );
		float date = GuidoMapGetElementDate ( slice );
		device->Frame(left, top-border, right, bottom+border);
		slice = GuidoMapFindDeepNthElementOfType( page, type, ++n);
	}
}
*/

// ----------------------------------------------------------------------------
//		* DoDraw
// ----------------------------------------------------------------------------
void
GuidoCarbonControl::DoDraw()
{
 	const GRHandler guidoRef = GetGuidoGR();	
	if( guidoRef == 0 ) return;
	
  	ControlRef theControl = GetControlRef();
  	if( theControl == 0 ) return;
 	
 	// - Get our local size
 	int localWidth;
	int localHeight;
	GetSize( &localWidth, &localHeight );
	CGRect deviceRect = ::CGRectMake( 0, 0, localWidth, localHeight );

	//	VGDevice * device = GetGDevice(); 	// <- was
	const int kScrollBarHeight = 15;
	
	// - Create the graphic context. It must be initialized with the actual size of the window,
	// otherwise the y-origin is not calculated properly.
	WindowRef winRef = ::GetControlOwner( theControl );
	GrafPtr port = ::GetWindowPort( winRef );
	CGContextRef contextRef = 0;
	OSStatus status = ::CreateCGContextForPort( port, &contextRef );
	if( status != noErr ) return;
	
	GSystemOSX system(contextRef, NULL);
	VGDevice* device = system.CreateDisplayDevice();
	device->NotifySize(localWidth, localHeight + kScrollBarHeight );
	
	::CGContextRelease(contextRef);	// because the device now owns the context.	

	// - Draw the background
	device->BeginDraw();
	device->PushPen( VGColor( 0, 0, 0, ALPHA_OPAQUE ), 1 );
	device->PushFillColor( VGColor( 255, 255, 255, ALPHA_TRANSPARENT ));
	device->Rectangle( 0, 0, localWidth, localHeight );
	device->PopFillColor();
	device->PopPen();
	
	// - Setup the clipping rectangle, to avoid drawing on scrollbars.
	::CGContextClipToRect( contextRef, deviceRect ); 

	// - Draw the music score
	int virtualScrollX = (int)((float)mScrollX * 10 / GetZoom());
	int virtualScrollY = (int)((float)mScrollY * 10 / GetZoom());
	float virtualVisibleWidth = localWidth * 10 / GetZoom();
	float virtualVisibleHeight = localHeight * 10 / GetZoom();

	float fullScoreWidth;
	float fullScoreHeight;
	GetFullScoreSize( &fullScoreWidth, &fullScoreHeight );

	GuidoOnDrawDesc desc;
	desc.handle = guidoRef;	 
	desc.hdc = device;
	desc.updateRegion.erase = false;
	desc.updateRegion.left = virtualScrollX;	// absolute virtual coordinates clip rect
	desc.updateRegion.top = virtualScrollY;
	desc.updateRegion.right = (int)(virtualScrollX + virtualVisibleWidth);
	desc.updateRegion.bottom = (int)(virtualScrollY + virtualVisibleHeight);
	desc.scrollx = (int)virtualScrollX;
	desc.scrolly = (int)virtualScrollY;
	desc.page = GetPageNum();
	desc.sizex = (int)fullScoreWidth;
	desc.sizey = (int)fullScoreHeight;
	
	MClock clock;
	GuidoOnDraw( &desc );
 
// debug
#if 0
	std::cout << "Score : sizex = " << desc.sizex << " ; sizey = " << desc.sizey << std::endl; 
	std::cout << "Guido: time do draw the score: " << clock.Milliseconds() << " ms" << std::endl;
//	std::cout << "Guido: graphical objects: " << GuidoGRObjectCount() << std::endl;
#endif
 
 	if( GetParam( kDrawSymbolMap ) == 1 )
 		DrawSymbolMap();
	
//	drawMap (guidoRef, device, GetPageNum());

	device->EndDraw();
	delete device;
}
 
// ----------------------------------------------------------------------------
//		* GetSize
// ----------------------------------------------------------------------------
// Returns the size of the control, that is: the visible portion of the score.	
void
GuidoCarbonControl::GetSize( int * oX, int * oY ) const
{
	Rect bounds = { 0, 0, 0, 0 };
    ::GetControlBounds( GetControlRef(), &bounds );
	if( oX ) *oX = bounds.right - bounds.left;
	if( oY ) *oY = bounds.bottom - bounds.top;
}

// ----------------------------------------------------------------------------
//		* GetFullScoreSize
// ----------------------------------------------------------------------------
/** \brief Returns the whole size ("scrollable") of the music score, in pixels.

	It can be much bigger than the current visible portion of the score in the control.
*/
void		
GuidoCarbonControl::GetFullScoreSize( float * outWidth, float * outHeight ) const
{
	GuidoPageFormat format = { 100, 100, 10, 10, 10, 10 };
	GuidoGetPageFormat( GetGuidoGR(), GetPageNum(),  &format );
	
	*outWidth = format.width * GetZoom() * 0.1f;
	*outHeight = format.height * GetZoom() * 0.1f;
}
	
/*		
// ----------------------------------------------------------------------------
//		* PrecalcScoreSize
// ----------------------------------------------------------------------------
void		
GuidoCarbonControl::PrecalcScoreSize( VGDevice * inDevice )
{
	const int guidoRef = GetGuidoRef();	
	if( inDevice && guidoRef > 0 )
	{
		float tmpX = (float)GuidoGetSizeX( guidoRef );
		float tmpY = (float)GuidoGetSizeY( guidoRef );
		GuidoConvertToDeviceUnits( *inDevice, guidoRef, GetZoom(), &tmpX, &tmpY );
	
		mScoreWidth = (int)tmpX;
		mScoreWidth = (int)tmpY;
	}
}*/
 
// ----------------------------------------------------------------------------
//		* GetGuidoRef
// ----------------------------------------------------------------------------
GRHandler			
GuidoCarbonControl::GetGuidoGR() const
{
	return mGuidoInfos ? mGuidoInfos->GetGR() : 0; 
}

// ----------------------------------------------------------------------------
//		* GetPageNum
// ----------------------------------------------------------------------------
int			
GuidoCarbonControl::GetPageNum() const
{
	return mGuidoInfos ? mGuidoInfos->GetPageNum() : 1; 
}

// ----------------------------------------------------------------------------
//		* Zoom
// ----------------------------------------------------------------------------
float GuidoCarbonControl::GetZoom() const
	{ return mGuidoInfos ? mGuidoInfos->GetZoom() : 1;  }

void GuidoCarbonControl::SetZoom( float in ) 
	{ if( mGuidoInfos ) mGuidoInfos->SetZoom( in ); }

void GuidoCarbonControl::ZoomIn() 
	{ if( mGuidoInfos ) mGuidoInfos->ZoomIn(); }

void GuidoCarbonControl::ZoomOut()
	 { if( mGuidoInfos ) mGuidoInfos->ZoomOut(); }


// ----------------------------------------------------------------------------
//		* DrawSymbolMap
// ----------------------------------------------------------------------------
void
GuidoCarbonControl::DrawSymbolMap()
{
 /*		OK, but I need to re-commit the score map class files.
 	
 ControlRef theControl = GetControlRef();

	VGDevice * dc = GetGDevice();
	if( dc == 0 ) return;

	ScoreSymbolMap * map = GetSymbolMap();
	if( map == 0 ) return;

	Rect bounds;
    ::GetControlBounds( theControl, &bounds );

	
	CScoreElement * root = map->GetRootElement();
	if( root )
		root->DeepDraw( *dc );

	std::cout << "Mapped elements: " << map->ElementCount() << std::endl;
*/


// TODO: setup the scale and origins ? dc->SetScale();

//	std::cout << "------------------------------------------------------------------" << std::endl;

	
//		std::cout << "Symbol pos( " << infos.Left() << ", " << infos.Top() << " )";
//		std::cout << ", size( " << infos.mFrame.Width() << ", " << infos.mFrame.Height() << " )"; 
//		std::cout << ", date = " << infos.Date() << std::endl;

}


// ----------------------------------------------------------------------------
//		* GetSymbolMap
// ----------------------------------------------------------------------------
/* OK
ScoreSymbolMap *
GuidoCarbonControl::GetSymbolMap()
{
	VGDevice * dc = GetGDevice();
	if( dc == nil ) return nil;

	return dynamic cast<ScoreSymbolMap *>(dc->GetSymbolMap());
}
*/

// ----------------------------------------------------------------------------
//		* TrackMouse
// ----------------------------------------------------------------------------
void
GuidoCarbonControl::TrackMouse( EventRef inEvent )
{
/* OK
 	OSStatus    err;
	Point localClick;
    err = ::GetEventParameter( inEvent, kEventParamMouseLocation, 
       							typeQDPoint, NULL, sizeof(Point), NULL, &localClick); 

	if( err != noErr )	return;
	
	const int x = localClick.h;
	const int y = localClick.v;
//	cout << "clicked at " << localClick.h << ", " << localClick.v << endl;

	ScoreSymbolMap * map = GetSymbolMap();
	if( map == nil ) return;


	CScoreElement * root = map->GetRootElement();
	if( root )
	{
		const CScoreElement * hitElt = root->FindDeepSubElementHitBy( (float)x, (float)y );
		//ScoreSymbolMap::DispSymbolInfos( hitElt );
	
		if( hitElt )
			hitElt->PrintChilds();
	}
	
//	ScoreElementDesc * desc = map->FindNearestElement( localClick.h, localClick.v );
//	ScoreSymbolMap::DispSymbolInfos( desc );

*/
}

// ----------------------------------------------------------------------------
//		* HitTest
// ----------------------------------------------------------------------------
void
GuidoCarbonControl::HitTest( EventRef inEvent )
{
 	OSStatus    err;

 // Extract the mouse location
	Point localClick;
     err = ::GetEventParameter( inEvent, kEventParamMouseLocation, 
       							typeQDPoint, NULL, sizeof(Point), NULL, &localClick); 
 
 // Is the mouse in the view?
	Rect localBounds;
    ::GetControlBounds( GetControlRef(), &localBounds );

//	printf( "Hit test (%d %d), bounds: %d, %d, %d, %d \n", where.h, where.v, bounds.left, 
//				 bounds.top, bounds.right, bounds.bottom );

	ControlPartCode part = kControlNoPart;
	if( ::PtInRect( localClick, &localBounds ))
		part = 1;

 // Send back the value of the hit part
	 err = ::SetEventParameter( inEvent, kEventParamControlPart, typeControlPartCode,
   															sizeof( ControlPartCode ), &part ); 

} 

 /*   Rect		bounds;
    Str255		title;
   	GrafPtr		currentPort;
    SInt16		savedFont;
    Style		savedFace;
    SInt16		savedMode;
    SInt16		savedSize;
    ThemeDrawingState	state;
    
 
    ::GetControlBounds( theControl, &bounds );
    ::GetControlTitle( theControl, title );
    
    // prepare to draw in the current port
    ::GetPort( &currentPort );
    savedFont = GetPortTextFont( currentPort );
    savedFace = GetPortTextFace( currentPort );
    savedMode = GetPortTextMode( currentPort );
    savedSize = GetPortTextSize( currentPort );
  //  GetThemeDrawingState( &state );
   // ::NormalizeThemeDrawingState();
  //  ::UseThemeFont( kThemeSystemFont, smSystemScript );

    // just draw our title within our bounding box
  //  ::TETextBox( &title[1], title[0], &bounds, teFlushDefault );
    ::FrameRect(&bounds);

    // restore what we did to the port
//    SetThemeDrawingState( state, true );
    TextFont( savedFont );
    TextFace( savedFace );
    TextMode( savedMode );
    TextSize( savedSize );*/

/*

static pascal OSStatus MyCustomControlHandler ( 
EventHandlerCallRef myHandler, 
EventRef theEvent, void* userData) 
{
#pragma unused (myHandler, userData) 

OSStatus result = eventNotHandledErr; 
UInt32 whatHappened; 

ControlRef theControl; 
Rect controlBounds; 
ControlPartCode whatPart; 
UInt16 hiliteState; 
RgnHandle controlRegion; 

Point mouseLocation; 

ThemeButtonDrawInfo myButtonInfo; 

myButtonInfo.state = kThemeStateActive;         //1     myButtonInfo.value = kThemeButtonOff;   myButtonInfo.adornment = kThemeAdornmentDefault; 

hiliteState = 0; 

whatHappened = GetEventKind (theEvent); 

switch (whatHappened) 
{
case kEventControlHitTest:         //2 

GetEventParameter (theEvent, kEventParamDirectObject, 
typeControlRef, NULL, sizeof(ControlRef), 
NULL, &theControl); 
GetEventParameter (theEvent, kEventParamMouseLocation,              //3 
typeQDPoint, NULL, sizeof (Point), NULL, 
&mouseLocation); 

GetControlBounds(theControl, &controlBounds); 
GetThemeButtonRegion (&controlBounds,           //4 
kThemeRoundedBevelButton, 
&myButtonInfo, controlRegion); 

if (PtInRgn(mouseLocation, controlRegion) == true         //5 
whatPart = 5; // == kSomeAppDefinedPart 
else 
whatPart = kControlNoPart; 

SetEventParameter (theEvent, kEventParamControlPart,                //6 
typeControlPartCode, 
sizeof(ControlPartCode), 
&whatPart); 

result = noErr; 
break; 

case kEventControlDraw:          //7 

GetEventParameter (theEvent, kEventParamDirectObject,               //8 
typeControlRef, NULL, sizeof(ControlRef), 
NULL, &theControl); 

GetControlBounds (theControl, &controlBounds);              //9 

hiliteState = GetControlHilite (theControl);             //10 
if (hiliteState !=0)            //11 
myButtonInfo.value = kThemeButtonOn; 
else 
myButtonInfo.value = kThemeButtonOff; 

DrawThemeButton (&controlBounds, kThemeRoundedBevelButton,         //12 
&myButtonInfo,NULL, NULL, NULL,0); 

result = noErr; 
break; 

case kEventControlHit:             //13 
SysBeep (1); 
result = noErr; 
break; 
}

return (result); 
}
*/





