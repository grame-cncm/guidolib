#ifndef __JuceSystem__
#define __JuceSystem__

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

#include "VGSystem.h"

namespace juce { class Graphics; }

// --------------------------------------------------------------
// 		Juce implementation of the VGSystem class
// --------------------------------------------------------------
class JuceSystem : public VGSystem
{
	juce::Graphics* fGraphics;
	
	public:
								 JuceSystem(juce::Graphics* g) : fGraphics(g) {}
		virtual					~JuceSystem() {}

		// - VGDevice services -------------------------------------------
		virtual VGDevice*		CreateDisplayDevice( );
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );

		// - Font services -----------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;
};

#endif
