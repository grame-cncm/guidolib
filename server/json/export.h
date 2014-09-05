/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef export__
#define export__


#if __MINGW32__
#  define jsonexport

#elif defined(WIN32) && !defined(GCC)

# ifdef JSONExport
#  define jsonexport		_declspec (dllexport)
# else
#  define jsonexport		_declspec (dllimport)
# endif

#else

# ifdef JSONExport
#  define jsonexport		__attribute__ ((visibility("default")))

# else
#  define jsonexport
# endif

#endif


#endif
