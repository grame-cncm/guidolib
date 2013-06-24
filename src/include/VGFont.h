#ifndef VGFont_H
#define VGFont_H

/*
  GUIDO Library
  Copyright (C) 2006	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
