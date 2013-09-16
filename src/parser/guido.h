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

/* guido.h */

#ifndef GUIDO_H
#define GUIDO_H

#include "GuidoDefs.h"

#define SHARP +1
#define FLAT (-1)

void gd_fracNorm(long int *a, long int *b);
  /* normalises fraction a/b */
void gd_fracAdd(long int *a, long int *b, const long int c, const long int d);
  /* sets a/b := a/b+c/d, normalised */
  /* normalise a/b, c/d before using this function */
int gd_fracCmp(long int a, long int b, long int c, long int d);
  /* returns +1 if a/b > c/d, 0 if a/b=c/d, -1 else */
  /* normalise a/b, c/d before using this function */

const char *	gd_pc2noteName(int pitch);
int				gd_noteName2pc (const char *name);

#endif

