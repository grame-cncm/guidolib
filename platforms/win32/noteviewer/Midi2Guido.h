/*
	GUIDO Library
	Copyright (C) 2002 Juergen Kilian, Grame

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

#ifndef __Midi2Guido__ 
#define __Midi2Guido__

#ifndef __GNUG__

#ifdef MIDI2GMNDLL_EXPORTS
#define MIDI2GMNDLL_API __declspec(dllexport)
#else
#define MIDI2GMNDLL_API __declspec(dllimport)
#endif

#else
#define MIDI2GMNDLL_API 
#endif // __GNU__

/*!
	result 
	midifname[.mid] will be converted into midifname.gmn
	if .ini includes "FILENAME=...." -> this entry will be used instead!

*/
MIDI2GMNDLL_API int midi2gmn(const char *inifname,	
							 const char *midifname,	
							 const char *gmnfname);


/// get a version string
MIDI2GMNDLL_API const char *midi2gmnVerStr( void );	

/// get a version/build id, there is no special meaning of this number
/// newer versions have alway a higher number
MIDI2GMNDLL_API int midi2gmnVerInt( void );	


#endif
