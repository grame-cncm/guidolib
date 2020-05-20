#ifndef __JuceFont__
#define __JuceFont__

/*
	GUIDO Library
	Copyright (C) 2012	Grame

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

namespace juce { class Font; }
// --------------------------------------------------------------
// 		Juce implementation of the VGFont class
// --------------------------------------------------------------
class JuceFont : public VGFont
{
	std::string	fName;
	juce::Font * fNativeFont;

	public:
								 JuceFont(const char * faceName, int size, int properties);
		virtual					~JuceFont();

		// - VGFont services ---------------------------------------------
		virtual	const char *	GetName() const;
		virtual	int 			GetSize() const;
		virtual	int				GetProperties() const;

		// - Symbol services ---------------------------------------------
		virtual	void			GetExtent( const char * s, int count, float * width, float * height, VGDevice *) const;
		virtual	void			GetExtent( int c, float * width, float * height,  VGDevice *) const;

		const juce::Font&	NativeFont() const	{ return *fNativeFont; };

};

#endif
