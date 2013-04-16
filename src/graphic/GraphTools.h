#ifndef GraphTools_H
#define GraphTools_H

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

#include "NVPoint.h"

void makeCurve( float x1, float y1, float x2, float y2, float x3, float y3,
				float x4, float y4, int nSegs , NVPoint point_array[], int * index );

#endif


