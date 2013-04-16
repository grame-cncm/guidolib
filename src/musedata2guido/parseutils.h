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
#ifndef __parseutils_h__
#define __parseutils_h__

#include "MuseDataNoteRecord.h"
int isCharInString(char c, char * s);
int isBlanks(char* c, int count);
int parsePitchInformation(char *s, EPitch *p, int *c, int *o);
int parseDurationInformation(char * s, int*d);
int parseBarNumber(char*s,int*b);

#endif
