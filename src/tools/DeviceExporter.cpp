/*
	GUIDO Library
	Copyright (C) 2007  Grame

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

#include "DeviceExporter.h"

#ifdef WIN32
#include "wingdi.h"
#include "../../win32/src/GDeviceWin32.h"
#endif

/////////////////////////////////////////////////////////////////
// - Data export services ---------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool			
DeviceExporter::ExportToFile( const char * inFilePath, VImgFormat inImgFormat )
{
	bool result = false;

#ifdef WIN32
	GDeviceWin32* devW32 = dynamic_cast<GDeviceWin32*>(fDevice);
	if (!devW32) return result;
	HDC dc = devW32->getHDC();	//can't use dc from hWnd
	
	switch(inImgFormat)
	{
		case kImgFormatBMP:
		{
			HBITMAP bmp = CreateBitmap( devW32->GetWidth(), devW32->GetHeight(), 1, 32, devW32->GetBitMapPixels() );
			PBITMAPINFO bmi = WIN32_createBitmapInfoStruct( 0, bmp );
			WIN32_saveBitmapToBMPFile( 0, inFilePath, bmi, bmp, dc );
			result = true;
		}
		break;

		case kImgFormatUnknown:
		case kImgFormatTIFF:
		case kImgFormatPNG:
		default:
		break;
	}
#elif MACOS
#elif UNIX
#endif

	return result;
}


// --------------------------------------------------------------
// - Win32 SPECIFIC
// --------------------------------------------------------------
#ifdef WIN32

//The following example code defines a function that initializes the remaining 
//structures, retrieves the array of palette indices, opens the file, copies the data, and closes the file.
//-----------------------------------------------------------------------
PBITMAPINFO DeviceExporter::WIN32_createBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{ 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
	{
        //errhandler("GetObject", hwnd); 
	}

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4) 
        cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

     if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits)); 

     // There is no RGBQUAD array for the 24-bit-per-pixel format. 

     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
                    sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0; 

	 pbmi->bmiHeader.biHeight = pbmi->bmiHeader.biHeight;

     return pbmi; 
} 

//-----------------------------------------------------------------------
void DeviceExporter::WIN32_saveBitmapToBMPFile( HWND hwnd, LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC) 
 { 
    HANDLE hf;                  // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 
    DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits) 
	{
         //errhandler("GlobalAlloc", hwnd); 
	}

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
        DIB_RGB_COLORS)) 
    {
        //errhandler("GetDIBits", hwnd); 
    }

	pbi->bmiHeader.biHeight = -pbi->bmiHeader.biHeight;

    // Create the .BMP file. 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) 
	{
        //errhandler("CreateFile", hwnd); 
	}
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
                    pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if ( !WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
        (LPDWORD) &dwTmp,  NULL) ) 
    {
       //errhandler("WriteFile", hwnd); 
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
                  + pbih->biClrUsed * sizeof (RGBQUAD), 
                  (LPDWORD) &dwTmp, (NULL)) ) 
	{
        //errhandler("WriteFile", hwnd); 
	}

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
	{
           //errhandler("WriteFile", hwnd); 
	}

    // Close the .BMP file. 
     if (!CloseHandle(hf)) 
	 {
           //errhandler("CloseHandle", hwnd); 
	 }

    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);
}

#endif
// --------------------------------------------------------------
// - MacOS SPECIFIC		//todo
// --------------------------------------------------------------
// --------------------------------------------------------------
// - Unix SPECIFIC		//todo
// --------------------------------------------------------------
