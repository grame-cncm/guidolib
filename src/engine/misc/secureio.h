#ifndef __secureio__
#define __secureio__

/*
  GUIDO Library
  Copyright (C) 2008  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef WIN32

# include <stdio.h>

#ifdef VC2005
# define sscanf							sscanf_s
# define snprintf						_snprintf_s
# define snprsize(n)					(n),_TRUNCATE
# define strncpy(dst,src,n)				strncpy_s(dst,n,src,_TRUNCATE )
# define strncat(dst,src,n)				strncat_s(dst,n,src,_TRUNCATE)
# define fopen(file,mode)				fopens(file,mode)

inline FILE * fopens (const char * file, const char* mode) 
{ FILE* fd; errno_t err = fopen_s(&fd, file,mode); if(err) fd=0; return fd; }

#elif defined (VC6)
# define snprintf						_snprintf
# define snprsize(n)					(n)
#endif

#else

# include <cstdio>
# define snprsize(n)					(n)

#endif

#endif
