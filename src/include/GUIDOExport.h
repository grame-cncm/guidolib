#ifndef __GUIDOExport__
#define __GUIDOExport__

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
