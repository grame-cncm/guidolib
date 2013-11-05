/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include "GuidoStreamBuf.h"

#include <sstream>

#ifdef WIN32
#include <Windows.h>
#define sleep(n)	Sleep(n);
#else
#include <unistd.h>
#define sleep(n)	usleep(n*1000);
#endif
static const int kSleepTime = 10;		// sleep time in milliseconds

using namespace std;


GuidoStreamBuf::GuidoStreamBuf() :
    fHaveToCloseStream(false),
    fAllDataRead(false),
    fSomeDataHaveBeenWritten(false)
{
    fTheStringStream = new stringstream();
    
    streamIteratorCurrent = fTheStringStream->rdbuf();
    streamIteratorEnd = streamIteratorEnd;
}

GuidoStreamBuf::~GuidoStreamBuf()
{
    delete fTheStringStream;
}

GuidoStreamBuf::int_type GuidoStreamBuf::underflow()
{
    if (streamIteratorCurrent == streamIteratorEnd)
    {
        if (fSomeDataHaveBeenWritten)   // To solve parallelism problem
            fAllDataRead = true;

        while (streamIteratorCurrent == streamIteratorEnd && !fHaveToCloseStream)
        {
            sleep(kSleepTime);
        }

        if (fHaveToCloseStream && streamIteratorCurrent == streamIteratorEnd)
            return traits_type::eof();
    }

    return traits_type::to_int_type(*streamIteratorCurrent);
}

GuidoStreamBuf::int_type GuidoStreamBuf::uflow()
{
    return traits_type::to_int_type(*streamIteratorCurrent++);
}

void GuidoStreamBuf::WriteToBuffer(const char *str)
{
    *fTheStringStream << str;

    fAllDataRead = false;
    fSomeDataHaveBeenWritten = true;

    if (streamIteratorCurrent == streamIteratorEnd)
        streamIteratorCurrent = fTheStringStream->rdbuf();
}