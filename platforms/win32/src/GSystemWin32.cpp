/*
	GUIDO Library
	Copyright (C) 2006  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "GSystemWin32.h"
#include "GFontWin32.h"
#include "GDeviceWin32.h"
#include "GDeviceWin32AntiAliased.h"
#include "secureio.h"

// --------------------------------------------------------------
GSystemWin32::GSystemWin32(	HDC dispDC, HDC printDC ) 
					: mDispDC(dispDC), mPrintDC(printDC)													
{
}

// --------------------------------------------------------------
GSystemWin32::~GSystemWin32()
{
}

// --------------------------------------------------------------
// - VGDevice services ------------------------------------------
// --------------------------------------------------------------
// Creates and returns a pointer to a new display VGDevice 
// which can be used directly to draw on the screen.
VGDevice* GSystemWin32::CreateDisplayDevice()	
{
	return new GDeviceWin32( mDispDC, this );
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
//	compatible with the application's current screen. This device
// can be used to draw into a bitmap.
VGDevice* GSystemWin32::CreateMemoryDevice( int inWidth, int inHeight )
{
	return new GDeviceWin32( inWidth, inHeight, this );
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
// compatible with the file (pixmap) located at the specified 
// path.
VGDevice* GSystemWin32::CreateMemoryDevice( const char * inPath )
{
	//The graphics file formats supported by GDI are BMP, JPEG and PNG. 
	return new GDeviceWin32( inPath, this );
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
// compatible with the file (pixmap) located at the specified 
// path.
VGDevice*
GSystemWin32::CreateAntiAliasedMemoryDevice( int inWidth, int inHeight )
{
	GDeviceWin32 * memDevice = new GDeviceWin32( inWidth, inHeight, this );
	GDeviceWin32AntiAliased * aaDevice = new GDeviceWin32AntiAliased(memDevice);

	return aaDevice;
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new printer VGDevice.
VGDevice*
GSystemWin32::CreatePrinterDevice( )
{

	GDeviceWin32 * dispDevice = new GDeviceWin32( mPrintDC, this );

	return dispDevice;
}

// --------------------------------------------------------------
// - Font services ----------------------------------------------
// --------------------------------------------------------------
// "width" feature for font is not portable: has been replaced by
// horizontal scaling of device context.
const VGFont*
GSystemWin32::CreateVGFont( const char * faceName, int size, 
							  int properties ) const
{
	LOGFONT lf;
	memset( &lf, 0, sizeof(lf));

	if( faceName )
		strncpy( lf.lfFaceName, faceName, LF_FACESIZE );

	lf.lfHeight = - size; // ! see microsoft gdi doc
	lf.lfCharSet = FS_LATIN1; //ANSI_CHARSET;
	
	lf.lfWeight		= FW_NORMAL;
	if (properties)
	{
		if (properties & VGFont::kFontBold)			lf.lfWeight		= FW_BOLD;
		if (properties & VGFont::kFontItalic)		lf.lfItalic		= 1;
		if (properties & VGFont::kFontUnderline)	lf.lfUnderline	= 1;
	}

	GFontWin32 * outFont = 0;

	HFONT winFont = ::CreateFontIndirect( &lf );
	if( winFont )
		outFont = new GFontWin32( winFont, faceName, size, properties );

	return outFont;
}

// ----------------------------------------------------------------------------
void
GSystemWin32::SetPrinterDC(HDC printDC)
{
	mPrintDC = printDC;	
}


