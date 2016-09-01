#ifndef __NullGFont__
#define __NullGFont__

/*
  GUIDO Library
  Copyright (C) 2006 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
