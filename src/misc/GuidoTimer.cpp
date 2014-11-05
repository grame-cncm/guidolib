/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GuidoTimer.h"

GuidoTimer::GuidoTimer()
{
#ifdef WIN32
    startParseTime = LARGE_INTEGER();
    endParseTime   = LARGE_INTEGER();
    
    startAR2GRTime = LARGE_INTEGER();
    endAR2GRTime   = LARGE_INTEGER();
    
    startDrawTime = LARGE_INTEGER();
    endDrawTime   = LARGE_INTEGER();
    
    QueryPerformanceFrequency(&frequency); 
#else
#endif
}

void GuidoTimer::startParse() {
    keepTime(startParseTime);
}

void GuidoTimer::stopParse() {
    keepTime(endParseTime);
}

void GuidoTimer::startAR2GR() {
    keepTime(startAR2GRTime);
}

void GuidoTimer::stopAR2GR() {
    keepTime(endAR2GRTime);
}

void GuidoTimer::startDraw() {
    keepTime(startDrawTime);
}

void GuidoTimer::stopDraw() {
    keepTime(endDrawTime);
}

int GuidoTimer::getParseTime() {
#ifdef WIN32
    return getSpentTime(startParseTime, endParseTime);
#else
#endif

    return 0;
}

int GuidoTimer::getAR2GRTime() {
#ifdef WIN32
    return getSpentTime(startAR2GRTime, endAR2GRTime);
#else
#endif

    return 0;
}

int GuidoTimer::getDrawTime() {
#ifdef WIN32
    return getSpentTime(startDrawTime, endDrawTime);
#else
#endif

    return 0;
}
	
void GuidoTimer::keepTime(LARGE_INTEGER &time)
{
#ifdef WIN32
    QueryPerformanceCounter(&time);
#else
#endif
}

int GuidoTimer::getSpentTime(LARGE_INTEGER &startTime, LARGE_INTEGER &endTime) {
#ifdef WIN32
    LARGE_INTEGER spentTimeInMs;
    
    spentTimeInMs.QuadPart = endTime.QuadPart - startTime.QuadPart;

    spentTimeInMs.QuadPart *= 1000;
    spentTimeInMs.QuadPart /= frequency.QuadPart;

    return spentTimeInMs.LowPart;
#else
#endif

    return 0;
}