#ifndef __secureio__
#define __secureio__

/*
	GUIDO Library
	Copyright (C) 2008  D.Fober Grame

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
