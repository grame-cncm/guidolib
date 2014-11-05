#ifndef GuidoTimer_H
#define GuidoTimer_H

/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
    #include <windows.h>
#else
#endif

class GuidoTimer {
public:
    GuidoTimer();

    void startParse();
    void stopParse();

    void startAR2GR();
    void stopAR2GR();

    void startDraw();
    void stopDraw();

    int  getParseTime();
    int  getAR2GRTime();
    int  getDrawTime();

protected:
    void keepTime(LARGE_INTEGER &startTime);
    int getSpentTime(LARGE_INTEGER &startTime, LARGE_INTEGER &endTime);

    LARGE_INTEGER frequency;

    LARGE_INTEGER startParseTime;
    LARGE_INTEGER endParseTime;

    LARGE_INTEGER startAR2GRTime;
    LARGE_INTEGER endAR2GRTime;

    LARGE_INTEGER startDrawTime;
    LARGE_INTEGER endDrawTime;
};

#endif
