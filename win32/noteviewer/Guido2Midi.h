/*
	GUIDO Library
	Copyright (C) 2002  Grame

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

#ifndef __Guido2Midi__
#define __Guido2Midi__

// added by jk for testing on non windows system
#ifdef WIN32
 #ifdef Guido2MidiExport
#define Guido2Midi_API(type) __declspec(dllexport) type
 #else
  #define Guido2Midi_API(type) __declspec(dllimport) type
 #endif
#else

 #define Guido2Midi_API(type) type
#endif





#ifdef __cplusplus
extern "C" {
#endif

	Guido2Midi_API(int) gmn2midi(int argc,char*argv[]);

#ifdef __cplusplus
}
#endif

#endif
