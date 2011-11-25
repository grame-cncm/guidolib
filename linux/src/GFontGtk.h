#ifndef GFontGtk_H
#define GFontGtk_H

/*
	GUIDO Library
	Copyright (C) 2005-2007	Grame
	
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

#include "VGFont.h"
#include "pango/pango.h"

// --------------------------------------------------------------
// 		GFontGtk class
// --------------------------------------------------------------
/** \brief GTK2.0+ implementation of the VGFont device-independant font class.

	Using VGFont in the ScoreProcessingLib:

	The VGSystem child-object used by the client app is responsible 
	for allocating a new VGFont child-object and associating it with 
	the appropriate VGdevices. 

	In this case, the VGSystem is also responsible for setting up the
	character set and giving it to the font when creating it, as the
	set may be shared between various fonts.

*/		
class GFontGtk : public VGFont		
{
	public:
	
								GFontGtk( PangoFontDescription * nativeFont, 
										  const char* faceName, 
										  int size, int properties, 
										  const unsigned int * charSet );
		virtual					~GFontGtk();


		// - VGFont services ---------------------------------------------
		virtual	const char*		GetName() const
									{ return mName; }
		virtual	int 			GetSize() const
									{ return mSize; }
		virtual	int				GetProperties() const
									{ return mFontProp; }

		// - Symbol services ---------------------------------------------
		virtual	void			GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context = 0) const;
		virtual	void			GetExtent( char c, float * outWidth, float * outHeight, VGDevice * context = 0) const;
	
		// - GTK2 specific -----------------------------------------------	
		/// Returns the Pango font associated with the current object.
		PangoFontDescription *	GetNativeFont() const
									{ return mNativeFont; } 

		/// Returns the symbol corresponding to the input index.
		unsigned int			Symbol(unsigned int sym) const	
									{ return mSymbolTable[sym] ; }


	protected:
		
		PangoFontDescription *	mNativeFont;
		const char*				mName;
		int						mSize;
		int						mFontProp;
		const unsigned int *	mSymbolTable ;

};


#endif
