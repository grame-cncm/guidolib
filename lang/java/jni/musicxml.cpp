/*

  Guido Engine Library
  Copyright (C) 2010  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#include <iostream>
#include <sstream>
#include <string.h>

#ifdef WIN32
# define UNICODE
# include <windows.h>
#else
# include <dlfcn.h>
#endif

#include "musicxml.h"


typedef int (*musicxml2guidoptr)	(const char *file, bool generateBars, std::ostream& out);
typedef const char* (*musicxmllibVersionStrPtr)	();
typedef const char* (*musicxml2guidoVersionStrPtr)	();

static musicxml2guidoptr			gConverter;
static musicxmllibVersionStrPtr		gLibraryVers;
static musicxml2guidoVersionStrPtr	gConverterVers;
static const char* notFound = "";

using namespace std;

//------------------------------------------------------------------------------
#ifdef WIN32
bool loadMusicxml()
{
	HINSTANCE handle = LoadLibrary (L"libmusicxml2.dll");
	if (!handle) return false;

	gConverter		= (musicxml2guidoptr) GetProcAddress(handle, "musicxmlfile2guido");
	gLibraryVers	= (musicxmllibVersionStrPtr) GetProcAddress(handle, "musicxmllibVersionStr");
	gConverterVers	= (musicxml2guidoVersionStrPtr) GetProcAddress(handle, "musicxml2guidoVersionStr");
	if (gConverter && gLibraryVers && gConverterVers) {
		return true;
	}
	FreeLibrary (handle);
	return false;
}
#else
bool loadMusicxml()
{
	void * handle = dlopen("libmusicxml2.framework/libmusicxml2", RTLD_LAZY);
	if (!handle) return false;
	
	gConverter		= (musicxml2guidoptr) dlsym(handle, "musicxmlfile2guido");
	gLibraryVers	= (musicxmllibVersionStrPtr) dlsym(handle, "musicxmllibVersionStr");
	gConverterVers	= (musicxml2guidoVersionStrPtr) dlsym(handle, "musicxml2guidoVersionStr");
	if (gConverter && gLibraryVers && gConverterVers) {
		return true;
	}
	dlclose (handle);
	return false;
}
#endif

//------------------------------------------------------------------------------
const char*	musicxmllibVersionStr()		{ return gLibraryVers ? gLibraryVers() : notFound; }
const char*	musicxml2guidoVersionStr()	{ return gConverterVers ? gConverterVers() : notFound; }

//------------------------------------------------------------------------------
char* musicxml2guido (const char *file)
{
	char * guido = 0;
	if (gConverter) {
		stringstream str;
		if ( gConverter (file , true, str) == 0 ) {
			guido = new char[ str.str().size()+1];
			strcpy (guido, str.str().c_str());			
		}
	}
	return guido;
}
