#ifndef __GuidoStreamBuf__
#define __GuidoStreamBuf__

/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <iostream>
#include <iterator>

using namespace std;

class GuidoStreamBuf : public std::streambuf
{
    public:
                 GuidoStreamBuf();
        virtual ~GuidoStreamBuf();

        void WriteToBuffer(const char *str);

        void SetHaveToCloseStream() { fHaveToCloseStream = true; }

        bool GetIsJobFinished()     { return fHaveToCloseStream; }

        bool GetAreAllDataRead() { return fAllDataRead; }

    protected:
        stringstream *fTheStringStream;

        istreambuf_iterator<char> streamIteratorCurrent;
        istreambuf_iterator<char> streamIteratorEnd;

        bool fHaveToCloseStream;
        bool fAllDataRead;
        bool fSomeDataHaveBeenWritten;

    private:
        int_type underflow();
        int_type uflow();
};


#endif
