#ifndef __CairoFont__
#define __CairoFont__

/*
	GUIDO Library
	Copyright (C) 2010	Grame

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
#include "VGFont.h"

class VGDevice;

// --------------------------------------------------------------
// 		Cairo based font support
// --------------------------------------------------------------
class CairoFont : public VGFont
{
	public:
					 CairoFont ( const char* faceName, int size, int properties, const unsigned int * charSet=0);
		virtual		~CairoFont();

		// - VGFont services ---------------------------------------------
		virtual	const char *	GetName() const			{ return fName.c_str(); }
		virtual	int 			GetSize() const			{ return fSize; }
		virtual	int				GetProperties() const	{ return fProperties; }

		// - Symbol services ---------------------------------------------
		virtual	void			GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const;
		virtual	void			GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const;

	private:
		std::string				fName;
		int						fSize;
		int						fProperties;
		const unsigned int *	fSymbolTable ;

		void	GetExtent( const char * s, float * outWidth, float * outHeight, cairo_t * context ) const;
		void	SelectFont( cairo_t * cr ) const;
};

#endif
