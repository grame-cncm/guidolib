#ifndef __GDeviceWin32AntiAliased__
#define __GDeviceWin32AntiAliased__

/*
	GUIDO Library
	Copyright (C) 2003, 2004	Grame
	
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

#include "GDeviceWin32.h"
#include "DecoratorDevice.h"

using namespace std;

// --------------------------------------------------------------
class GDeviceWin32AntiAliased : public DecoratorDevice
//class_export GDeviceWin32AntiAliased : public GDeviceWin32
{

	public:
							GDeviceWin32AntiAliased(VGDevice * dev) : DecoratorDevice(dev) {}
		virtual				~GDeviceWin32AntiAliased() { fDevice = 0; }  // The decorated device *should not* be desallocated

/*							GDeviceWin32AntiAliased( HDC hdc, VGSystem* sys ) : 
								GDeviceWin32(hdc, sys) {}	
							GDeviceWin32AntiAliased( int inWidth, int inHeight, VGSystem* sys ) :
								GDeviceWin32(inWidth, inHeight, sys) {}								
							GDeviceWin32AntiAliased( const char * inPath, VGSystem* sys ) :
								GDeviceWin32(inPath, sys) {}								
		virtual				~GDeviceWin32AntiAliased() {} */

 	
		// - Bitmap services (bit-block copy methods) 
		virtual bool		CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = -1.0 );	
/*							{
								GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(this);	
								if (!pSrcDC_Win32) return false;
								HDC	dc = (HDC)pSrcDC_Win32->getHDC();
//								HDC	dc = (HDC)pSrcDC_Win32->GetNativeContext();
								::SetStretchBltMode(dc, STRETCH_HALFTONE);
								return pSrcDC_Win32->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, alpha);
							}		*/
		virtual bool		CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0);
/*							{
								GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(this);	
								if (!pSrcDC_Win32) return false;
								HDC	dc = (HDC)pSrcDC_Win32->getHDC();
//								HDC	dc = (HDC)pSrcDC_Win32->GetNativeContext();
								::SetStretchBltMode(dc, STRETCH_HALFTONE);
								return pSrcDC_Win32->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, alpha);
							}		*/
};

// --------------------------------------------------------------
inline bool			
GDeviceWin32AntiAliased::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha )
{
	GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(fDevice);	
//	GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(this);	
	if (!pSrcDC_Win32) return false;

	HDC	dc = pSrcDC_Win32->getHDC();
//	HDC	dc = getHDC();
	::SetStretchBltMode(dc, STRETCH_HALFTONE);
	return pSrcDC_Win32->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, alpha);
//	return CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, alpha);
}

// --------------------------------------------------------------
inline bool			
GDeviceWin32AntiAliased::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int srcWidth, int srcHeight, float alpha )
{
	GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(fDevice);	
	if (!pSrcDC_Win32) return false;

	HDC	dc = pSrcDC_Win32->getHDC();
	::SetStretchBltMode(dc, STRETCH_HALFTONE);
	return pSrcDC_Win32->CopyPixels(xDest, yDest, dstWidth, dstHeight, pSrcDC, xSrc, ySrc, srcWidth, srcHeight, alpha);
}


#endif
