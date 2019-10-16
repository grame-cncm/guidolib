
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

#pragma once

#if __MINGW32__
#  define class_export		class
#  define GUIDOAPI

#elif defined(WIN32) && !defined(GCC)

# ifdef GUIDOExport
#  define class_export		class _declspec (dllexport)
#  define GUIDOAPI			_declspec (dllexport)
# else
#  define class_export		class _declspec (dllimport)
#  define GUIDOAPI			_declspec (dllimport)
# endif

#else

# ifdef GUIDOExport
#  define class_export		class __attribute__ ((visibility("default")))
#  define GUIDOAPI			__attribute__ ((visibility("default")))
# else
#  define class_export		class
#  define GUIDOAPI
# endif

#endif

