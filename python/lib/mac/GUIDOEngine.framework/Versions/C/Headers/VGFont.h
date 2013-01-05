#ifndef VGFont_H
#define VGFont_H

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

#include "VGDevice.h"

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

// --------------------------------------------------------------
// 		VGFont class
// --------------------------------------------------------------
/** \brief Generic pure virtual & device-independant font class.

	This class replaces the previously defined GFontInfos class
	that was attached to a VGDevice. It declares the minimal set
	of necessary methods/attributes to use a font. Fonts can now
	be seen as independant objects to be created by the VGSystem
	and then associated when necessary to the VGDevice.

*/
class VGFont
{
	public:
		/// Font properties
		enum {
			kFontNone		= 0,
			kFontBold		= 1,
			kFontItalic		= 2,
			kFontUnderline	= 4
		};

		virtual					~VGFont() {}

		// - VGFont services ---------------------------------------------
		/// Returns the current object's name (as a string)
		virtual	const char *	GetName() const = 0;
		/// Returns the current object's size (as an int)
		virtual	int 			GetSize() const = 0 ;
		/// Returns the current object's property value(s) (see enum above)
		virtual	int				GetProperties() const = 0 ;

		// - Symbol services ---------------------------------------------
		/// Computes the width and height of the input string using the
		/// current font capabilities in the input VGDevice
		virtual	void			GetExtent( const char * s,
										   int inCharCount,
										   float * outWidth,
										   float * outHeight,
										   VGDevice * context ) const = 0;

		/// Computes the width and height of the input character using the
		/// current font capabilities in the input VGDevice
		virtual	void			GetExtent( unsigned char c,
										   float * outWidth,
										   float * outHeight,
										   VGDevice * context ) const = 0;

	protected:
		void * GetContext( VGDevice * context) const	{ return context->GetNativeContext(); }
};

/*! @} */

#endif
