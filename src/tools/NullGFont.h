#ifndef __NullGFont__
#define __NullGFont__

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
#include "VGFont.h"
#include "VGDevice.h"

// --------------------------------------------------------------
// 	Null Graphic Font class
// --------------------------------------------------------------
/** \brief Generic pure virtual & device-independant font class.

*/		
class NullGFont : public VGFont	
{
	protected:
				std::string		fName;
				int				fSize;
				int				fProperties;
	public:
								 NullGFont(std::string name, int size, int prop)
										: fName(name), fSize(size), fProperties(prop)  {}
		virtual					~NullGFont()  {}

		// - VGFont services ---------------------------------------------
		virtual const char *	GetName() const			{ return fName.c_str(); }
		virtual int 			GetSize() const			{ return fSize; }
		virtual int				GetProperties() const	{ return fProperties; }

		// - Symbol services ---------------------------------------------
		virtual void			GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context = 0) const
									{ *outWidth = float(fSize*inCharCount); *outHeight = float(fSize); }
		virtual void			GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context = 0) const	
									{ *outWidth = *outHeight = float(fSize); }
};


#endif
