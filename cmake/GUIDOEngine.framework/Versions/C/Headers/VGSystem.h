#ifndef VGSystem_H
#define VGSystem_H

/*
	GUIDO Library
	Copyright (C) 2003, 2008	Grame

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

class VGDevice;
class VGFont;

/*!
\addtogroup VGSys Virtual Graphic System

The virtual graphic system is intended as an abstract layer covering platform
dependencies at graphic level. It represents a set of abstract classes covering
the basic needs of an application: printing text, drawing to the screen or to an
offscreen, etc...
The set of abstract classes includes:
- a VGSystem class: to cover allocation of specific VGDevice and VGFont objects.
- a VGDevice class: specialized on drawing onscreen of offscreen
- a VGFont class: to cover fonts management

This set of classes is implemented for different target platforms:
implementations are provided for Windows GDI and Mac OSX Quartz, implementations
for Windows GDI+, OpenGL and Linux GTK are in progress.

@{
*/

// --------------------------------------------------------------
// 		VGSystem class
// --------------------------------------------------------------
/** \brief Generic pure virtual class for manipulating platform
	independant drawing devices and fonts.

	A VGSystem object (for Virtual Guido drawing System) is the highest
	graphical abstraction whose child-object can be used by a client app
	to obtain a platform dependant graphical system able to perform guido
	drawing or printing operations (like double-buffered display on the
	current screen or printing). To do so, the client app simply needs to
	intantiate an object of the appropriate platform dependant VGSystem
	derived class using the correct platform dependant parameters.

	A VGSystem object deals mainly with two kinds of graphical objects:

		- VGDevice objects, which can be display devices, memory devices
		or printer devices;
		- VGFont objects, used to describe device independant fonts (for
		music or text).
*/

class VGSystem
{
	public:

		virtual					~VGSystem() {}

		// - VGDevice services -------------------------------------------

		/// Creates and returns a pointer to a new display VGDevice
		///	which can be used to draw directly on the screen.
		virtual VGDevice*		CreateDisplayDevice( ) = 0;

		/// Creates and returns a pointer to a new memory VGDevice
		///	compatible with the application's current screen. This device
		/// can be used to draw into a bitmap.
		virtual VGDevice*		CreateMemoryDevice( int inWidth,
													int inHeight ) = 0;
		/// Creates and returns a pointer to a new memory VGDevice
		///	compatible with the file (pixmap) located at the specified
		/// path.
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath ) = 0;

		/// Creates and returns a pointer to a new printer VGDevice.
		virtual VGDevice*		CreatePrinterDevice( ) = 0;

		/// Creates and returns a pointer to a new memory VGDevice
		///	compatible with the application's current screen and
		/// using anti-aliasing capabilities. This device
		/// can be used to draw into a bitmap.
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth,
															   int inHeight ) = 0;

		// - Font services -----------------------------------------------

		/// Creates and returns a pointer to a new VGFont using the specified parameters.
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const = 0;

};

/*! @} */

#endif
