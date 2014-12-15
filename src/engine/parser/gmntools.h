/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
/* guido tools - version 0.1 */
/* (c) 1998 by Holger H. Hoos */
/* for use in guido music notation import / export */

typedef double T_REAL; 		/* per default, use double precision */
typedef char *T_STRING;
typedef const char *CONST_T_STRING;

/* Conversion between units (advanced guido)
 * Returns 1 is conversion is ok, 0 if not
 * Result is in finalValue */
bool gd_convertUnits(T_REAL fromValue, CONST_T_STRING fromUnit,
                     CONST_T_STRING toUnit, double &toValue);