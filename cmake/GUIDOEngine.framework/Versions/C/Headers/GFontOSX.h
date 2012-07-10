#ifndef GFontOSX_H
#define GFontOSX_H

/*
	GUIDO Library
	Copyright (C) 2006	Grame
	
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

#include <string>
#import <QuartzCore/QuartzCore.h>
#include "VGFont.h"
#include "GUIDOExport.h"

// quartz

#include <string>

// --------------------------------------------------------------
// 		GFontOSX class
// --------------------------------------------------------------
/** \brief OSX implementation of the VGFont device-independant font class.
	
	Using VGFont in the ScoreProcessingLib:

	The VGSystem child-object used by the client app is responsible 
	for allocating a new VGFont child-object and associating it with 
	the appropriate VGdevices. 

*/		
class_export GFontOSX : public VGFont		
{
	/// Computes the input string's graphical size.
	void	GetQuartzTextDims( const char * str, int count, float * width, float * height, float * baseline, CGContextRef dc ) const;
	/// Computes the input symbol's graphical size.
	void	GetQuartzGlyphDims( const CGGlyph * inGlyphs, int count, float * width, float * height, float * baseline, CGContextRef dc ) const;

	public:
				 GFontOSX( const char* faceName, int size, int properties);
		virtual	~GFontOSX();

		// - VGFont services ---------------------------------------------
		virtual	const char*		GetName() const			{ return mName.c_str(); }
		virtual	int 			GetSize() const			{ return mSize; }
		virtual	int				GetProperties() const	{ return mFontProp; }

		// - Symbol services ---------------------------------------------
		virtual	void			GetExtent( const char * s, int count, float * width, float * height, VGDevice * context ) const;
		virtual	void			GetExtent( unsigned char c, float * width, float * height, VGDevice * context ) const;
	
		// - OSX specific services ---------------------------------------
		/// Returns the symbol corresponding to the input index.
		CGGlyph			GetGlyph( unsigned int inSymbolID )		{ return inSymbolID; }
	protected:
		std::string		mName;
		int				mSize;
		int				mFontProp;
};

#endif
