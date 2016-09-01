#ifndef GuidoTiming_H
#define GuidoTiming_H

/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

class GuidoTiming {
public:
    static long getCurrentmsTime();

protected:
#ifdef WIN32
    static int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif
};

#endif
