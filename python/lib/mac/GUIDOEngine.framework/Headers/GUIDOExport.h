#ifndef __GUIDOExport__
#define __GUIDOExport__

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

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


#if __MINGW32__
#  define class_export		class
#  define GUIDOAPI(type)	type

#elif defined(WIN32) && !defined(GCC)

# ifdef GUIDOExport
#  define class_export		class _declspec (dllexport)
#  define GUIDOAPI(type)	_declspec (dllexport) type
# else
#  define class_export		class _declspec (dllimport)
#  define GUIDOAPI(type)	_declspec (dllimport) type
# endif

#else

# ifdef GUIDOExport
#  define class_export		class __attribute__ ((visibility("default")))
#  define GUIDOAPI(type)	__attribute__ ((visibility("default"))) type
# else
#  define class_export		class
#  define GUIDOAPI(type)	type
# endif

#endif


#endif
