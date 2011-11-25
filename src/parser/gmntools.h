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
/* guido tools - version 0.1 */
/* (c) 1998 by Holger H. Hoos */
/* for use in guido music notation import / export */

typedef double T_REAL; 		/* per default, use double precision */
typedef char * T_STRING;
typedef const char * CONST_T_STRING;

#define REAL_INVALID -9.99999e99 /* used for indicating invalid real numbers */

/* conversion between units (advanced guido) */
T_REAL gd_convertUnits(T_REAL fromValue,
                       CONST_T_STRING fromUnit, CONST_T_STRING toUnit);


