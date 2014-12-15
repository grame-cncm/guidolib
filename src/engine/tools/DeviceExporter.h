#ifndef __DeviceExporter__
#define __DeviceExporter__

/*
  GUIDO Library
  Copyright (C) 2007 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

//#include "DecoratorDevice.h"
#include "VGDevice.h"
#include "GUIDOExport.h"

#ifdef WIN32
# if defined(_AFXDLL)	// using mfc
#  include <afx.h>
# else	
#  include <windows.h>// without mfc
# endif
#endif

/** \brief implements a kind of VGDevice wrapper able to render
		   the wrapped device's internal graphical data to an
		   image file, using various current file formats. 

	\note this object is mainly intended to be used for 
		  curve saving within the scoreprocessing lib. 
*/	

// --------------------------------------------------------------
class_export DeviceExporter //: public DecoratorDevice
{
	protected:

			VGDevice*	fDevice;
	public:


		/// Raster operation modes (color fill, bit copy, etc.)		
		enum VImgFormat 
		{ 
			kImgFormatUnknown = 0,	// unknown mode, use default
			kImgFormatBMP,			// Win32 32 bits bitmap
			kImgFormatTIFF,			// MacOs tiff format
			kImgFormatPNG			// png format
		};				


						DeviceExporter(VGDevice * dev)	{ fDevice = dev; }
		virtual			~DeviceExporter()				{ fDevice = 0; }   
	
		// - Device data export service ----------------------------------------

		/// Exports all graphical data to an image file using the specified file format. 
		virtual	bool	ExportToFile( const char * inFilePath, VImgFormat inImgFormat );

	private:

#ifdef WIN32
		// - Win32 specific methods for bitmap management -------------------------
			PBITMAPINFO	WIN32_createBitmapInfoStruct( HWND hwnd, HBITMAP hBmp );
			void		WIN32_saveBitmapToBMPFile( HWND hwnd, LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC ); 
#endif

};

#endif
