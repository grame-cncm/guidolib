#ifndef ARDefine_H
#define ARDefine_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

#include "GuidoDefs.h"


#define MUSICAL_MIN_NUMERATOR 0
#define MUSICAL_MAX_NUMERATOR 32767
#define MUSICAL_MIN_DENOMINATOR 1
#define MUSICAL_MAX_DENOMINATOR 128
#define DEFAULT_DENOMINATOR 4
#define DEFAULT_NUMERATOR 1

#define MIN_ACCIDENTALS (-2)	
#define MAX_ACCIDENTALS 2	

#define MIN_PITCH 0			// ?
#define MAX_PITCH 255		// ? 127 ?

#define UNKNOWN 0

#define MIN_INTENSITY 0
#define MAX_INTENSITY 127
#define DEFAULT_INTENSITY 80

#define MIN_REGISTER (-10)
#define MAX_REGISTER 10
#define DEFAULT_REGISTER 1

#endif
