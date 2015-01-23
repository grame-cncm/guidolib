#ifndef __openFrameworksFont__
#define __openFrameworksFont__

/*
	GUIDO Library
	Copyright (C) 2012	Grame

	openFrameworks Guido interface by Thomas Coffy (c) IRCAM 2014

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
*/

#include <string>
#include "VGFont.h"

#include <ofTrueTypeFont.h>

// --------------------------------------------------------------
// 		openFrameworks implementation of the VGFont class
// --------------------------------------------------------------
class openFrameworksFont : public VGFont
{
	std::string	fName;
	ofTrueTypeFont* fNativeFont;

	public:
	openFrameworksFont(const char * faceName, int size, int properties);
	virtual	~openFrameworksFont();

	// - VGFont services ---------------------------------------------
	virtual	const char *	GetName() const;
	virtual	int 			GetSize() const;
	virtual	int				GetProperties() const;

	// - Symbol services ---------------------------------------------
	virtual	void			GetExtent( const char * s, int count, float * width, float * height, VGDevice *) const;
	virtual	void			GetExtent( unsigned char c, float * width, float * height,  VGDevice *) const;

	const ofTrueTypeFont&	NativeFont() const	{ return *fNativeFont; };
};

#endif
