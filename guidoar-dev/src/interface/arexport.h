#ifndef __export__
#define __export__

/*
	GuidoAR Library
	Copyright (C) 2003 - 2012  Grame

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

# ifdef GUIDOAR_EXPORTS
#  define gar_export		 _declspec (dllexport)
# else
#  define gar_export		 _declspec (dllimport)
# endif

# pragma warning ( disable : 4275 4251 )

#else

# ifdef GUIDOAR_EXPORTS
#  define gar_export		 __attribute__ ((visibility("default")))
# else
#  define gar_export		
# endif

#endif


#endif
