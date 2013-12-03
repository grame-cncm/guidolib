#ifndef GDeviceDebugStream_H
#define GDeviceDebugStream_H

/*
  GUIDO Library
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include <iostream>

#include "VGDevice.h"

// --------------------------------------------------------------
class GDeviceDebugStream : public VGDevice
{
	public:
								GDeviceDebugStream(): VGDevice( 400, 400 ) { }
		virtual					~GDeviceDebugStream() { }

		// - Fonts		
		virtual const VGFont*	CreateLFont( const char * faceName, int height, int weight = kFontWeightNormal,  
												bool italic = false, bool underline = false ) const 
									{ std::cout << "CreateLFont, h=" << height << std::endl; return 0; }

		virtual	const VGFont*	SelectFont( const VGFont* inObj )			{ std::cout << "SelectFont" << std::endl; return 0; }
		virtual	void			DeleteFont( const VGFont* inObj ) const		{ std::cout << "DeleteFont" << std::endl; }

		// - Bitmaps
		virtual	GBitMapRef		LoadBitMap( const char * inPath ) const { return 0; }
		virtual	GBitMapRef		SelectBitMap( GBitMapRef inObj ) { return 0; }
		virtual	void			DeleteBitMap( GBitMapRef inObj ) const { }
			
		// - Device 

		virtual	void			Attach( GWindowRef h )  { std::cout << "Attach" << std::endl; }		// obsolete soon ?
		virtual	void			Detach( GWindowRef h )  { std::cout << "Detach" << std::endl; }		// obsolete soon ?
		virtual VGDevice *		CreateMemoryDC() const  { std::cout << "CreateMemoryDC" << std::endl; return 0; }	// obsolete soon ?
		virtual void			DeleteDC()  { std::cout << "DeleteDC" << std::endl; }					// obsolete soon ?

		virtual bool			IsValid() const { return true; }

		virtual	void			SaveDC()		{ std::cout << "SaveDC" << std::endl; }
		virtual void			RestoreDC()		{ std::cout << "RestoreDC" << std::endl; }
		
		// - Coordinates services	

		virtual int				MaxWidth() const { std::cout << "MaxWidth" << std::endl; return 800; }
		virtual int				MaxHeight() const { std::cout << "MaxHeight" << std::endl; return 600; }

		// - Drawing services

		virtual void			MoveTo( float x, float y ) { std::cout << "MoveTo " << x << ", " << y << std::endl; }
		virtual void			LineTo( float x, float y ) { std::cout << "LineTo " << x << ", " << y << std::endl; }
		virtual void			Line( float x1, float y1, float x2, float y2 ) { MoveTo( x1, y1 ); LineTo( x2, y2 ); }

		virtual	void			Polygon( const float * xCoords, const float * yCoords, int count ) 
													{ std::cout << "Polygon, " << count << " points" << std::endl; }


		virtual void			Rectangle( float left, float top, float right, float bottom ) 
													{ std::cout << "Rectangle" << std::endl; }		
		virtual void			Arc( float left, float top, float right, float bottom, 
										float startX, float startY, float endX, float endY ) 
													{ std::cout << "Arc" << std::endl; }

		// - Text services

 		virtual void			DrawSymbol( float x, float y, int inSymbolID ) 
										{ std::cout << "DrawSymbol " << inSymbolID << " at " << x << ", " << y << std::endl; }

		virtual void			GetSymbolExtent( int inSymbolID, float * outWidth, float * outHeight ) const	
										{ std::cout << "GetSymbolExtent " << inSymbolID << std::endl; }

									// Obsolete soon ?
		virtual void			GetScreenSymbolExtent( int inSymbolID, float * outWidth, float * outHeight, const VGFont* inFontH ) const	
										{ std::cout << "GetScreenSymbolExtent " << inSymbolID << std::endl; }

		virtual void			PrintAt( float x, float y, const char * s, int inCharCount = -1 ) 
										{ std::cout << "PrintAt " << x << ", "<< y << std::endl; }

		virtual void			SetTextColor( unsigned char r, unsigned char g, 
												unsigned char b, unsigned char a = 0)
											{ std::cout << "SetTextColor (rgba) " << r << ", " << g << ", " << b << ", " << a << std::endl; }

		virtual GColor			GetTextColor() const { std::cout << "GetTextColor" << std::endl; return GColor( 0, 0, 0, 0 ); }

		virtual void			SetTextBackgroundColor( unsigned char r, unsigned char g, 
														unsigned char b, unsigned char a = 0 )
										{ std::cout << "SetTextBackgroundColor (rgba) " << r << ", " << g << ", " << b << ", " << a << std::endl; }


		virtual GColor			GetTextBackgroundColor() const 
										{ std::cout << "GetTextBackgroundColor" << std::endl; return GColor( 1, 1, 1, 1 ); }

		virtual void			GetTextExtent( const char * s, int inCharCount, 
												float * outWidth, float * outHeight ) const 
										{ std::cout << "GetTextExtent" << std::endl; }


		virtual void			GetScreenTextExtent( const char * s, int inCharCount, 
										float * outWidth, float * outHeight, const VGFont* inFontH ) const 
										{ std::cout << "GetScreenTextExtent" << std::endl; }


		// - Bitmap services 

		virtual bool			DrawBitMap( VGDevice & srcDevice, const GBitMapInfos & srcBitMap, 
											float dstX, float dstY, float dstWidth, float dstHeight, int inOp ) 
										{ std::cout << "DrawBitMap" << std::endl; return true; }

		virtual bool			GetBitMapInfos( GBitMapRef inBitMap, GBitMapInfos * outInfos ) const 
										{ std::cout << "GetBitMapInfos" << std::endl; return true; }
			
		virtual bool			MakePixMap( int inWidth, int inHeight, void * ioBuffer ) const 
										{ std::cout << "MakePixMap " << inWidth << " x " << inHeight << std::endl; return true; } 

		// - GDevicePostScript specific
		
//		virtual void			SetHeaderFilePath( const char * inFilePath );

	protected:
	
		virtual bool			DoBeginDraw()	{ std::cout << "DoBeginDraw" << std::endl; return true; }
		virtual	void			DoEndDraw()		{ std::cout << "DoEndDraw" << std::endl; }

		virtual void			DoSelectPen( const GColor & c, float witdh = 1 )
													{ std::cout << "DoSelectPen" << std::endl; }

		virtual void			DoSelectBrush( const GColor & c )
													{ std::cout << "DoSelectBrush" << std::endl; }

		virtual void			DoSetTextColor( unsigned char r, unsigned char g, 
														unsigned char b, unsigned char a = 0 )
													{ std::cout << "DoSetTextColor" << std::endl; }

		virtual void			DoSetTextBackgroundColor( unsigned char r, unsigned char g, 
																unsigned char b, unsigned char a = 0 )
													{ std::cout << "DoSetTextBackgroundColor" << std::endl; }

		virtual void			DoSetTextAlign( unsigned int inFlags ) { std::cout << "DoSetTextAlign" << std::endl; }
	
		virtual void			DoSetOrigin( float prevX, float prevY, float newX, float newY )
												{ std::cout << "DoSetOrigin " << newX << ", " << newY << std::endl; }
		virtual void			DoSetScale( float prevX, float prevY, float newX, float newY )
												{ std::cout << "DoSetScale " << newX << ", " << newY << std::endl; }

		virtual	void			DoSaveDC()		{ std::cout << "DoSaveDC" << std::endl; }
		virtual void			DoRestoreDC()	{ std::cout << "DoRestoreDC" << std::endl; }

};


#endif



