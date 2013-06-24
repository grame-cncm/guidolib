/*
  GUIDO Library
  Copyright (C) 2002  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
