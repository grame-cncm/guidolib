#ifndef __SVGFont__
#define __SVGFont__

/*
	GUIDO Library
	Copyright (C) 2011	Grame

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

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/
class VGDevice;
// --------------------------------------------------------------
/** \brief a SVG font class.
*/
class SVGFont : public VGFont
{
	const VGFont *	fFont;
	VGDevice *	fDevice;

	public:
					 SVGFont(const char * name, int size, int properties=kFontNone);
		virtual		~SVGFont();

		virtual	const char *	GetName() const			{ return fFont->GetName(); }
		virtual	int 			GetSize() const			{ return fFont->GetSize(); }
		virtual	int				GetProperties() const	{ return fFont->GetProperties(); }

		virtual	void	GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const;
		virtual	void	GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const;
};

/*! @} */

#endif
