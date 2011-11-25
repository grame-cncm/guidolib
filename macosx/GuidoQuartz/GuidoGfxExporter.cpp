/*
 * file : GuidoGfxExporter.cpp
 * 
 *	May/17/2004	(JB)
 *
 */

// - System
/*#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>
#include <QuickTime/ImageCompression.h>
*/
#include <iostream>

#include "GuidoGfxExporter.h"
#include "GUIDOEngine.h"
#include "GDeviceOSX.h"
#include "GSystemOSX.h"

using namespace std;

// ---------------------------------------------------------------------------------
//		* GuidoGfxExporter
// ---------------------------------------------------------------------------------
GuidoGfxExporter::GuidoGfxExporter()
{
	mWidth = 0;			
	mHeight = 0;
	mRed = mGreen = mBlue = 255;

	mGWorld = 0;
}

// ---------------------------------------------------------------------------------
//		* ~GuidoGfxExporter
// ---------------------------------------------------------------------------------
GuidoGfxExporter::~GuidoGfxExporter()
{
	DisposeOffscreen();
}

// ---------------------------------------------------------------------------------
//		* ExportGuidoPage
// ---------------------------------------------------------------------------------
// The hi-level export method.
bool
GuidoGfxExporter::ExportGuidoPage( GRHandler inGuidoHandleGR, int inPage, 
									int inWidth, int inHeight, FSSpec * inFile )
{
	bool result;
	
	result = Initialize( inWidth, inHeight );

	if( result )
		result = Draw( inGuidoHandleGR, inPage );

	if( result )
		result = Save( inFile );
	
	return result;
}

// ---------------------------------------------------------------------------------
//		* Initialize
// ---------------------------------------------------------------------------------
// Prepare the invisible window. The same invisible window can be used to export
// several graphic files with different sizes.
bool
GuidoGfxExporter::Initialize( int inWidth, int inHeight ) 
{
 	if( ChangeSize( inWidth, inHeight ))
 	{
		return true;
	}	
	return false;
}

// ---------------------------------------------------------------------------------
//		* ChangeSize
// ---------------------------------------------------------------------------------
// Allocate or change the size of the graphic buffer.
bool
GuidoGfxExporter::ChangeSize( int inWidth, int inHeight )
{
	if(( inWidth == mWidth ) && ( inHeight == mHeight )) 
		return true;

	DisposeOffscreen();
	
	// - Allocate offscreen
	GWorldPtr newGWorld = 0;
	const short pixelDepth = 32;
	Rect boundsRect = { 0, 0, inHeight, inWidth }; // top, left, bottom, right

	QDErr err = ::NewGWorld( &newGWorld, pixelDepth, &boundsRect, NULL, NULL, 0 );
	if( err != noErr )
		return false;
	
	mGWorld = newGWorld;
	
	mWidth = inWidth;
	mHeight = inHeight;
	
	return (mGWorld != 0 );
}

// ---------------------------------------------------------------------------------
//		* DisposeOffscreen
// ---------------------------------------------------------------------------------
void
GuidoGfxExporter::DisposeOffscreen() 
{
	if( mGWorld )
	{
		::DisposeGWorld( mGWorld );
		mGWorld = 0;
	}
}

// ---------------------------------------------------------------------------------
//		* Draw
// ---------------------------------------------------------------------------------
// inPage >= 1
bool
GuidoGfxExporter::Draw( GRHandler inGuidoHandleGR, int inPage )
{
	if( inGuidoHandleGR == 0 ) return false;
	
	// - Make an association between the offscreen GWorld and a CGContext
	CGContextRef theContext;
	OSStatus status = ::QDBeginCGContext( mGWorld, &theContext );
	if( status != noErr )
		return false;

	// - Create a Guido graphic device
	GSystemOSX system(theContext, NULL);
	VGDevice* device = system.CreateMemoryDevice(mWidth, mHeight);
	//GDeviceOSX * device = new GDeviceOSX( theContext, mWidth, mHeight );	

	// - Draw the background, using specified color
	device->BeginDraw();
	device->PushPen( VGColor( 0, 0, 0, ALPHA_OPAQUE ), 1 );
	device->PushFillColor( VGColor( mRed, mGreen, mBlue, ALPHA_TRANSPARENT ));
	device->Rectangle( 0, 0, mWidth, mHeight );
	device->PopFillColor();
	device->PopPen();
			
	// - Draw the score, uding Guido.
	GuidoOnDrawDesc desc;
	desc.handle = inGuidoHandleGR;	 
	desc.hdc = device;
	desc.updateRegion.erase = true;
//	desc.zoom = 1;
	desc.scrollx = 0;
	desc.scrolly = 0;
	desc.page = inPage;
	desc.isprint = 0;							// > 0 during print
	desc.sizex = mWidth;
	desc.sizey = mHeight;

	GuidoOnDraw( &desc );
	
	device->EndDraw();
	
	::CGContextFlush( theContext );
	delete device;

	// - Cleanup
	status = ::QDEndCGContext( mGWorld, &theContext );

	return true;
}

// ---------------------------------------------------------------------------------
//		* Save
// ---------------------------------------------------------------------------------
bool
GuidoGfxExporter::Save( const FSSpec * inDestinationFile )
{
	if( inDestinationFile == 0 ) return false;

	GWorldPtr img = mGWorld;
	
	CGrafPtr savePort;
	GDHandle saveDev;

	::GetGWorld( &savePort, &saveDev );
	::SetGWorld( img, NULL );

	PixMapHandle pixMap = ::GetGWorldPixMap( mGWorld );
	::LockPixels( pixMap );
	
	bool result = Export( inDestinationFile, img );
		
	::UnlockPixels( pixMap );
	::SetGWorld( savePort, saveDev );

	return result;
}
	
/*
// ---------------------------------------------------------------------------------
//		* ChangeSize
// ---------------------------------------------------------------------------------
// Change the size of the invisible window.
bool
GuidoGfxExporter::ChangeSize( int inWidth, int inHeight )
{
	if(( inWidth == mWidth ) && ( inHeight == mHeight ))
		return true;

 	// - Release the old CG context (useless ?)
 	if( mContextRef )
 	{
 		::CGContextRelease( mContextRef );
 		mContextRef = 0;
 	}
 	
 	// - Release the old memory  	
	const size_t componentsPerPixel = 4;
	const size_t bitsPerComponent = 8;	
	const size_t bytesPerRow = (inWidth * bitsPerComponent * componentsPerPixel ) / 8;
	
	const int oldSize = mHeight * bytesPerRow;
	const int newSize = inHeight * bytesPerRow;	
	if( newSize > oldSize )
	{
		delete [] mOffscreenMemory;
		mOffscreenMemory = new char [ newSize ];
	}
	
		
	const CGImageAlphaInfo alphaInfo = kCGImageAlphaNoneSkipLast; // Should be kCGImageAlphaLast, but not yet implemented by Apple.

	mContextRef = ::CGBitmapContextCreate( mOffscreenMemory, (size_t)inWidth, (size_t)inHeight,
	 								bitsPerComponent, bytesPerRow, mColorSpace, alphaInfo ); 	

		
 	mWidth = inWidth;
 	mHeight = inHeight;

	// - Get the new CG context
// 	GrafPtr port = ::GetWindowPort( mInvisibleWindow );
//	status = ::CreateCGContextForPort( port, &mContextRef );
	return ( mContextRef != 0 );
}
*/

/* - TEST (draw a big X) ->
	::ShowPen();

	RGBColor backCol = { 100, 100, 100 };
	RGBColor foreCol = { 20000, 250, 250 };
	
	::RGBBackColor( &backCol );
	::RGBForeColor( &foreCol );

	::MoveTo( 0, 0 );
	::LineTo( mWidth, mHeight );
	
	::MoveTo( mWidth, 0 );
	::LineTo( 0, mHeight );
*/
	
