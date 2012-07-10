#ifndef VGDevice_H
#define VGDevice_H

/*
	GUIDO Library
	Copyright (C) 2003--2006	Grame

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

#include "VGColor.h"

class VGFont;
class VGSystem;

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

// --------------------------------------------------------------
// 		VGDevice class
// --------------------------------------------------------------
/** \brief Generic platform independant drawing device.


	VGDevice is a pure virtual class that declares the minimal set
	of methods required by the GGR (Guido Graphic Representation)
	objects to communicate their graphical operations. Implementations
	of VGDevice derived classes must then be provided by client
	applications.

	VGDevice thus provides standard graphic functions (Lines, Arc,
	Rectangles, Polygons, Text), coordinate transformation (zoom /
	scaling), and symbolic music symbols handlers.

	A VGDevice can be seen as the association of:

		- a drawing context, describing how these operations are
		performed, including pen and color state;

		- a graphic 'device' ('port' or 'output'), describing
		'where' the graphical operations are to be performed
		(these various outputs can be a screen, an offscreen
		pixmap, a printer, a file, a network stream, etc.);

		- a 'link-to-guido' class allowing the client app to
		collect informations about the score and give them to
		the guido engine (see Set/GetSymbolMap).

	To allow using a VGDevice for double buffering mechanism, it
	also provides bit-block copy operations from one VGDevice
	to another.

	Each VGDevice needs to be dynamicaly associated with external music
	and text VGFont objects, using the appropriate SetFont() method.
	Here we have to repeat that Guido, for higher abstraction, makes
	a clear distinction between text characters and music symbols,
	although music symbols are generally glyphs in a music font.

*/
class VGDevice
{

	friend class VGFont;	//to use GetNativeContext()
	friend class DecoratorDevice;

	public:

				/// Raster operation modes (color fill, bit copy, etc.)
				enum VRasterOpMode {
						kUnknown		= 0,	// unknown mode, use default
						kOpCopy			= 1,	// direct copy
						kOpAnd			= 2,	// AND
						kOpXOr			= 4,	// XOR = src invert
						kOpInvert		= 8,	// dest invert
						kOpOr			= 16	// OR
				};

				/// Text alignment modes
				enum VTextAlignMode {
						kAlignBase		= 1,
						kAlignBottom	= 2,
						kAlignTop		= 4,
						kAlignCenter	= 8,
						kAlignLeft		= 16,
						kAlignRight		= 32,
						kAlignBaseLeft	= kAlignLeft | kAlignBase
				};

		virtual					~VGDevice() {}

		/// Returns the ability of the current VGdevice to be drawn into.
		virtual bool			IsValid() const = 0;


		// - Drawing services ------------------------------------------------

		/// Prepares the device's context before a set of drawing operations
		/// and saves the current one (like the previous SaveDC() method).
		/// This method should be used before every set of drawing operation.
		virtual bool			BeginDraw() = 0;

		/// Restores the device's context after a set of drawing operations.
		/// and restore the previous one (like the previous RestoreDC() method).
		/// This method should be used after every set of drawing operation.
		virtual void			EndDraw() = 0;

		/// Invalidate a rectangle i.e. indicates the native graphic device
		/// that the corresponding rectangle needs to be refreshed.
		virtual void			InvalidateRect( float left, float top,
									   float right, float bottom ) = 0;

					// - Standard graphic primitives -------------------------

		/// Moves the current position to the point specified by (x,y).
		virtual void			MoveTo( float x, float y ) = 0;

		/// Draws a line from the current position up to, but not including,
		///	the point specified by (x,y).
		virtual void			LineTo( float x, float y ) = 0;

		/// Draws a line from the position specified by (x1,y1) up to,
		///	but not including, the point specified by (x2,y2).
		virtual void			Line( float x1, float y1,
									  float x2, float y2 ) = 0;

		/// Draws a frame using the specified coordinates. The frame is
		/// outlined by using the current pen, but not filled.
		virtual void			Frame( float left, float top,
									   float right, float bottom ) = 0;

		/// Draws a counter-clockwise elliptical arc between the startX,
		/// startY, endX & endY coordinates and inside the [left, top, right,
		/// bottom] elliptical bounding box. The distance from these points to
		/// the middle of the ellipse is not important, only the angle is
		/// taken in account.
		virtual void			Arc( float left,   float top,
									 float right,  float bottom,
									 float startX, float startY,
									 float endX,   float endY ) = 0;

					// - Filled surfaces --------------------------------------
					// The raster op mode for color filling should be specified
					// with SetRasterOpMode() before using one of these.

		/// Draws a triangle consisting of three points connected by
		/// straight lines. The triangle is NOT outlined but simply
		/// filled using the current fill color.
		virtual	void			Triangle( float x1, float y1,
										  float x2, float y2,
										  float x3, float y3 ) = 0;

		/// Draws a polygon consisting of count vertices connected by
		/// straight lines. The polygon is NOT outlined but simply
		/// filled using the current fill color.
		virtual	void			Polygon( const float * xCoords,
										 const float * yCoords, int count ) = 0;

		/// Draws a rectangle. The rectangle is NOT outlined but simply
		/// filled using the current fill color.
		virtual void			Rectangle( float left,  float top,
										   float right, float bottom ) = 0;

		// - Font services ---------------------------------------------------

		/// Selects the specified music VGFont into the current VGDevice.
		/// Warning ! this method doesn't return the previously selected
		/// font anymore. Use GetMusicFont() instead.
		virtual	void			SetMusicFont( const VGFont * font ) = 0;

		/// Returns the currently selected music VGFont.
		virtual	const VGFont *	GetMusicFont() const = 0;

		/// Selects the specified text VGFont into the current VGDevice.
		/// Warning ! this method doesn't return the previously selected
		/// font anymore. Use GetMusicFont() instead.
		virtual	void			SetTextFont( const VGFont * font ) = 0;

		/// Returns the currently selected text VGFont.
		virtual	const VGFont *	GetTextFont() const = 0;


		// - Pen & brush services --------------------------------------------

		/// Creates a new VGPen object with the specified VGColor and width
		/// and selects it into the current VGDevice.
		virtual	void			SelectPen( const VGColor & inColor,
										   float witdh ) = 0;

		/// Creates a new solid brush (to paint the interiors of filled shapes)
		/// with the specified VGColor and selects it into the current VGDevice.
		/// This method was previously called SelectBrush().
		virtual	void			SelectFillColor( const VGColor & c ) = 0;

		/// Saves the current VGPen and selects the new one using the specified
		/// VGColor and width into the current VGDevice.
		virtual	void			PushPen( const VGColor & inColor,
										 float inWidth ) = 0;

		/// Restores the previous VGPen object from the stack into the current
		/// VGDevice.
		virtual	void			PopPen() = 0;

		/// Saves the current color brush and selects the new one using the
		/// specified VGColor into the current VGDevice.
		/// This method was previously called PushBrush().
		virtual	void			PushFillColor( const VGColor & inColor ) = 0;

		/// Restores the previous color brush from the stack into the
		/// current VGDevice.
		/// This method was previously called PopBrush().
		virtual	void			PopFillColor() = 0;

		/// Sets the current foreground mix mode. We use the foreground mix
		/// mode to combine pens and interiors of filled objects with the colors
		/// already on the device. The foreground mix mode defines how colors
		/// from the brush or pen and the colors in the existing image are to be
		/// combined. See enum VRasterOpMode above.
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode) = 0;

		/// Retrieves the foreground mix mode of the specified device. The mix
		/// mode specifies how the pen or interior color and the color already
		/// on the device are combined to yield a new color.
		virtual	VRasterOpMode	GetRasterOpMode() const = 0;


		// - Bitmap services (bit-block copy methods) --------------------------

		/// Copies the entire content (pixmap) of the pSrcDC source VGDevice to
		/// the current device.
		/// The raster operation mode should be specified using SetRasterOpMode().
		/// Default alpha (-1.0) means copying bits using
		/// their own transparency values, if any; if no alpha channel value is available,
		///	opaque copy (alpha = 1.0) is performed.
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0) = 0;

		/// Makes the exact copy of the content (pixmap) of the specified rectangle
		/// from the pSrcDC source VGDevice to the specified destination of the
		/// current device.
		/// The raster operation mode should be specified using SetRasterOpMode().
		/// Default alpha (-1.0) means copying bits using
		/// their own transparency values, if any; if no alpha channel value is available,
		///	opaque copy (alpha = 1.0) is performed.
		virtual bool			CopyPixels( int xDest, int yDest,
											VGDevice* pSrcDC,
											int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0) = 0;

		// only for \DrHoos and \DrRenz
		/// Copies a pixmap from a source rectangle into a destination rectangle,
		/// stretching or compressing the pixmap to fit the dimensions of the
		/// destination rectangle, if necessary.
		/// The method stretches or compresses the pixmap using the raster mode
		/// currently set in the destination VGdevice. Default alpha (-1.0) means copying
		/// bits using their own transparency values, if any; if no alpha channel value
		/// is available, opaque copy (alpha = 1.0) is performed.
		virtual bool			CopyPixels( int xDest, int yDest,
											int dstWidth, int dstHeight,
											VGDevice* pSrcDC, float alpha = -1.0) = 0;

		/// Copies a pixmap from a source rectangle into a destination rectangle,
		/// stretching or compressing the pixmap to fit the dimensions of the
		/// destination rectangle, if necessary.
		/// The method stretches or compresses the pixmap using the raster mode
		/// currently set in the destination VGdevice. Default alpha (-1.0) means copying
		/// bits using their own transparency values, if any; if no alpha channel value
		/// is available, opaque copy (alpha = 1.0) is performed.
		virtual bool			CopyPixels( int xDest, int yDest,
											int dstWidth, int dstHeight,
											VGDevice* pSrcDC,
											int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0) = 0;

		// - Coordinate services ------------------------------------------------

		/// Sets the scale factors of the current VGDevice to the input values.
		virtual	void			SetScale( float x, float y )= 0;

		/// Specifies which VGDevice point (x,y) maps to the window origin (0,0).
		virtual	void			SetOrigin( float x, float y ) = 0;

		/// Offsets the current VGDevice's origin (see above).
		virtual	void			OffsetOrigin( float x, float y ) = 0;

		/// Computes input coordinates to get their values in the
		///	current VGDevice coordinate system.
		virtual	void			LogicalToDevice( float * x, float * y ) const = 0;

		/// Computes input VGDevice coordinates to get their values outside
		///	its VGDevice coordinate system.
		virtual	void			DeviceToLogical( float * x, float * y ) const = 0;

		/// GetXScale, GetYScale, GetXOrigin, GetYOrigin : get VGDevice's
		///	scaling/coordinate atributes.
		virtual float			GetXScale() const = 0;
		virtual	float			GetYScale() const = 0;
		virtual	float			GetXOrigin() const = 0;
		virtual	float			GetYOrigin() const = 0;

		/// Sets the size of the current VGDevice.
		/// Use this method to update the derived device's attributes and
		/// actual size.
		virtual	void			NotifySize( int inWidth, int inHeight ) = 0;

		/// Returns the width (set via NotifySize) of the current VGDevice.
		virtual	int				GetWidth() const = 0;

		/// Returns the height (set via NotifySize) of the current VGDevice.
		virtual	int				GetHeight() const = 0;


		// - Text and music symbols services -------------------------------------

		/// Writes the music symbol specified by the input inSymbolID at the
		/// specified location, using the currently selected music font,
		/// background color, and text color.
		virtual void			DrawMusicSymbol(float x, float y,
												unsigned int inSymbolID ) = 0;

		/// Writes the specified inCharCount number of text characters at the
		/// specified location, using the currently selected text font,
		/// background color, and text color.
		virtual	void			DrawString( float x, float y,
										    const char * s,
										    int inCharCount ) = 0;

		/// Sets the text/music color for the current VGDevice.
		virtual	void			SetFontColor( const VGColor & inColor ) = 0;

		/// Returns the text/music color of the current VGDevice.
		virtual	VGColor			GetFontColor() const = 0;

		/// Sets the text/music background color for the current VGDevice.
		virtual	void			SetFontBackgroundColor( const VGColor & inColor ) = 0;

		/// Returns the text/music background color of the current VGDevice.
		virtual	VGColor			GetFontBackgroundColor() const = 0;

		/// Sets the text/music alignment mode of the current VGDevice.
		/// See enum VTextAlignMode above.
		virtual	void			SetFontAlign( unsigned int inAlign ) = 0;

		/// Returns the text/music alignment mode of the current VGDevice.
		/// See enum VTextAlignMode above.
		virtual	unsigned int	GetFontAlign() const = 0;

		// - Printer informations services ----------------------------------------

		/// Sets the printing resolution of the current VGDevice.
		virtual	void			SetDPITag( float inDPI ) = 0;
		/// Returns the printing resolution of the current VGDevice.
		virtual	float			GetDPITag() const = 0;

		/// Allows pixels operations and returns a pointer to the bitmap pixels.
		virtual void*			GetBitMapPixels() = 0;
		/// Update bitmap pixels and ends pixels operations.
		virtual void			ReleaseBitMapPixels() = 0;

		/// temporary hack - must be removed asap
		virtual	VGSystem *		getVGSystem() const = 0;

		// - Data export services ----------------------------------------
		/// Exports all graphical data to an image file
//		virtual	void			ExportToFile( const char * inFilePath ) = 0;


		/// Returns the platform-specific device context object.
		virtual void *			GetNativeContext() const = 0;


	public:

		// - VGDevice extension --------------------------------------------
		/// Creates a new VGPen object with the specified VGColor
		virtual	void			SelectPenColor( const VGColor & inColor) = 0;
		
		/// Creates a new VGPen object with the specified VGColor
		virtual	void			SelectPenWidth( float witdh) = 0;

		virtual	void			PushPenColor( const VGColor & inColor) = 0;
		virtual	void			PopPenColor() = 0;
		
		virtual	void			PushPenWidth( float width) = 0;
		virtual	void			PopPenWidth() = 0;

};

/*! @} */

#endif
