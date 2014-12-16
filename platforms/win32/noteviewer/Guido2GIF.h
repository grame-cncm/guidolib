/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2004  Grame

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

#ifndef __Guido2GIF__
#define __Guido2GIF__

#ifdef Guido2GIFExport
#define Guido2GIF_API __declspec(dllexport)
#else
#define Guido2GIF_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Creates a GIF file from a Guido graphic 
		representation (GR) page.
	
	\param inHandleGR a Guido opaque handle to the GR structure to export.
	\param inPageNum the index of the score page to export. (the first page is 1)
	\param inSizeX the width of the GIF file to create (unit: pixels)
	\param inSizeY the height of the GIF file to create (unit: pixels)
	\param inGifFileName the name of the GIF file to create.
	\param inComment an additionnal text that will be wrote at the bottom of the
			page (i.e: copyright stuff) (optional)

	\return 0 if the export was successfull.
*/
Guido2GIF_API int G2GIFCreatePicture(	GRHandler inHandleGR,
										int inPageNum,
										int inWidth,
										int inHeight,
										const char * inGifFileName,
										const char * inComment );





/** \brief Calculates a page size in pixel units from a zoom and a resolution.

	A Guido page has a page format, exprimed in centimeters, which defines its 
	total width and height, and margins from the four edges of the page.

	Given a zoom and a resolution (dpi), G2GIFCalcSizeFromZoom() calculates for
	you the size to be used next with the G2GIFCreatePicture() function, using
	the current Guido page format.

	\param inHandleGR a Guido opaque handle to the GR structure to export.
	\param inPageNum the index of the concerned score page. (the first page is 1)
	\param inZoom a zoom factor (default value: 1)
	\param inDpi the resolution used for the cm to pixels conversion, in dot
				per inch. (1 inch = 2.54 cm). (default value: 72.27)
	\param outWidth on output: the width, in pixels.
	\param outHeight on output: the height, in pixels.

	\return 0 if the conversion was successfull.
*/
Guido2GIF_API int G2GIFCalcSizeFromZoom(    GRHandler inHandleGR,
											int inPageNum,
											float inZoom,
											float inDpi,
											int * outWidth,
											int * outHeight );


/*	
Old versions:

Now:the client is now responsible to adjust (crop) or not the page size 
by calling the appropriate Guido function.

Guido2GIF_API int G2GIFCreatePicture(int handle,
					 double zoom,
					 int sizex,
					 int sizey,
					 int adjustpagesize,
					 int pagenum,
					 int dops,
					 const char * psname,
					 const char * outname,
					 const char * idstr);



Guido2GIF_API void G2GIFCalcZoomFromSize(int handle,
								int *sizex,
								int *sizey,
								float * zoom);
*/



#ifdef __cplusplus
}
#endif

#endif
