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

#ifndef __MuseData2Guido__
#define __MuseData2Guido__

#ifdef WIN32
#ifdef MuseData2GuidoExport
#define MuseData2Guido_API __declspec(dllexport)
#elif defined(MuseDataImport)
#define MuseData2Guido_API __declspec(dllimport)
# else
#  define MuseData2Guido_API
#endif

#else

# ifdef MuseData2GuidoExport
#  define MuseData2Guido_API	__attribute__ ((visibility("default")))
# else
#  define MuseData2Guido_API
# endif

#endif


#ifdef __cplusplus
extern "C" {
#endif

MuseData2Guido_API int md2gmn(int argc,char**argv, const char * logfilename);

#ifdef __cplusplus
}
#endif

#endif
