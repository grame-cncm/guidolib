/*
 * file : MacGuidoPrintTask.cpp
 * 
 */

#include "MacGuidoPrintTask.h"
#include "GDeviceOSX.h"
#include "GSystemOSX.h"
#include "GUIDOEngine.h"

// ---------------------------------------------------------------------------------
//		* MacGuidoPrintTask
// ---------------------------------------------------------------------------------
MacGuidoPrintTask::MacGuidoPrintTask()
{
	mGuidoGR = 0;
}

// ---------------------------------------------------------------------------------
//		* ~MacGuidoPrintTask
// ---------------------------------------------------------------------------------
MacGuidoPrintTask::~MacGuidoPrintTask()
{
}

// ---------------------------------------------------------------------------------
//		* DoDrawPageContent
// ---------------------------------------------------------------------------------
// - User should have called GuidoAdjustPageSize() before printing.
//
// - Todo: page format, zoom, use PMSetOrientation() (before BeginDocument()) 
// when width is greater than height.
// - Todo: suppress guido margins (use a clone of the current GR)
bool
MacGuidoPrintTask::DoDrawPageContent( CGContextRef inContext, int inPageNum )	// idem MyPageDrawProc
{
	if( GetGuidoGR() == 0 ) return false;

	OSStatus err = noErr;	

	// -- Test: Get some Guido info
	/*const float guidoToCm = GuidoGetVirtualToCmFactor();
	const float cmToGuido = 1 / guidoToCm;

	float scoreWidthCm, scoreHeightCm, ml, mt, mr, mb;
	GuidoGetPageSizeCm( GetGuidoGR(), &scoreWidthCm, &scoreHeightCm, &ml, &mt, &mr, &mb );

	const int scoreWidthVirtual = GuidoGetSizeX( GetGuidoGR());
	const int scoreHeightVirtual = GuidoGetSizeY( GetGuidoGR());
*/

	GuidoPageFormat format = { 1, 1, 0, 0, 0, 0 };
	GuidoGetPageFormat( GetGuidoGR(), inPageNum, &format );
//	const int scoreWidthVirtual = (const int)format.width;
//	const int scoreHeightVirtual = (const int)format.height;

	
	// -- Test: Get some print-context info	
	
	// - Get the resolution
	PMResolution res = { 72, 72 };
	err = ::PMGetResolution( mPageFormat, &res ); 
	if( err != noErr )	return false;

//	const float cmToDotsX = res.hRes / 2.54;
//	const float cmToDotsY = res.vRes / 2.54;

	// - Get the 'paper' size
	PMRect paperRect;
	err = ::PMGetAdjustedPaperRect( mPageFormat, &paperRect );
	if( err != noErr )	return false;

//	const float paperWidthCm = (paperRect.right - paperRect.left) / cmToDotsX;
//	const float paperHeightCm = (paperRect.bottom - paperRect.top) / cmToDotsY;	

	// - Get the 'page' size
	PMRect pageRect;
	err = ::PMGetAdjustedPageRect( mPageFormat, &pageRect );
	if( err != noErr )	return false;

//	const float pageWidthCm = (pageRect.right - pageRect.left) / cmToDotsX;
//	const float pageHeightCm = (pageRect.bottom - pageRect.top) / cmToDotsY;	

	// -- Draw with Guido
	
	const int deviceHeight = (int)( paperRect.bottom - paperRect.top );	// Use 'paper'
	const int deviceWidth =  (int)( paperRect.right - paperRect.left );
	
	GSystemOSX system(NULL, inContext);
	VGDevice* dc = system.CreatePrinterDevice();
	dc->NotifySize(deviceWidth, deviceHeight);
	
	dc->BeginDraw();
	GuidoOnDrawDesc desc;
	desc.handle = GetGuidoGR();	 
	desc.hdc = dc;
	desc.updateRegion.erase = true;
	desc.scrollx = 0;
	desc.scrolly = 0;
	desc.page = inPageNum;
	desc.sizex = deviceWidth; // deviceWidth / 2;
	desc.sizey = deviceHeight; //deviceHeight / 2;

	GuidoOnDraw( &desc );
	dc->EndDraw();	
	delete dc;
	return true;
}

