/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/** \file GUIDOParse.cpp

	The Guido parser API
 */

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <istream>
#include <fstream>
#include <string.h>

using namespace std;

// - Guido Misc
#include "GUIDOInternal.h"
#include "GuidoParser.h"
#include "GuidoStream.h"
#include "GUIDOEngine.h"

#include "GUIDOParse.h"

//#define TIMING
#include "GuidoTiming.h"
#include "ARMusic.h"

//#define DEBUG_AR
#ifdef DEBUG_AR
#include "PrintVisitor.h"
#include "GMNCodePrintVisitor.h"
#endif

// ==========================================================================
// - Guido Parser API
// ==========================================================================

// ------------------------------------------------------------------------
GUIDOAPI GuidoParser * GuidoOpenParser ()
{
    GuidoParser *newParser = new GuidoParser();

	return newParser;
}

// ------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoCloseParser (GuidoParser *p)
{
    if (!p)
        return guidoErrBadParameter;

    delete p;

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI ARHandler GuidoFile2AR (GuidoParser *p, const char *file)
{
    if (!file || !p)
        return NULL;

    std::ifstream ifs(file, ios::in);
 
    if (!ifs)
        return NULL;

    p->setStream(&ifs);

    long startTime = GuidoTiming::getCurrentmsTime();
    ARHandler ar = p->parse();
    long endTime = GuidoTiming::getCurrentmsTime();
    
    if (ar) {
        ar->armusic->setParseTime(endTime - startTime);

#ifdef TIMING
        int parseTime = ar->armusic->getParseTime();
        std::cerr << "  --> " << parseTime << "ms spent for AR generation" << std::endl;
#endif
    }

    ifs.close();

#ifdef DEBUG_AR
    if (ar) {
        cout << std::endl;
        //GMNCodePrintVisitor *visitor = new GMNCodePrintVisitor(cout);
        PrintVisitor *visitor = new PrintVisitor(cout);
        visitor->visitIn(ar->armusic);
        cout << std::endl;
    }
#endif

    return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI ARHandler	GuidoString2AR (GuidoParser *p, const char *str)
{
    if( !str || !p )
        return NULL;

    std::istringstream iss(str);

    p->setStream(&iss);
    
    long startTime = GuidoTiming::getCurrentmsTime();
    ARHandler ar = p->parse();
    long endTime = GuidoTiming::getCurrentmsTime();

    if (ar) {
        ar->armusic->setParseTime(endTime - startTime);

#ifdef TIMING
        int parseTime = ar->armusic->getParseTime();
        std::cerr << "  --> " << parseTime << "ms spent for AR generation" << std::endl;
#endif
    }
    
#ifdef DEBUG_AR
    if (ar) {
        cout << std::endl;
        //GMNCodePrintVisitor *visitor = new GMNCodePrintVisitor(cout);
        PrintVisitor *visitor = new PrintVisitor(cout);
        visitor->visitIn(ar->armusic);
        cout << std::endl;
    }
#endif

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI ARHandler	GuidoStream2AR (GuidoParser *p, GuidoStream* s)
{
    if (!p || !s)
        return NULL;

    /* Prepare stream (principally, clear eof flag) */
//    s->Prepare();

    /* Set the GuidoStream into the GuidoParser */

	stringstream gmn; // (s->getGuidoCode());
	gmn << s->getGuidoCode();
	p->setStream(&gmn);

    /* Parse ! */
    long startTime = GuidoTiming::getCurrentmsTime();
    ARHandler ar = p->parse();
    long endTime = GuidoTiming::getCurrentmsTime();
    
    if (ar) {
        ar->armusic->setParseTime(endTime - startTime);

#ifdef TIMING
        long parseTime = ar->armusic->getParseTime();
        std::cerr << "  --> " << parseTime << "ms spent for AR generation" << std::endl;
#endif
    }

#ifdef DEBUG_AR
    if (ar) {
        cout << std::endl;
        //GMNCodePrintVisitor *visitor = new GMNCodePrintVisitor(cout);
        PrintVisitor *visitor = new PrintVisitor(cout);
        visitor->visitIn(ar->armusic);
        cout << std::endl;
    }
#endif

	return ar;
}


// --------------------------------------------------------------------------
GUIDOAPI const char*	GuidoStream2GuidoString ( GuidoStream* stream)
{
	const string s (stream->getGuidoCode());
	char *str = new char[s.size()+1];
	strcpy(str, s.c_str());
	return str;
}


// --------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoParserGetErrorCode (GuidoParser *p, int &line, int &col, const char** msg)
{
    if (!p)
        return guidoErrBadParameter;

    line = p->getErrorLine();
    col  = p->getErrorColumn();
    if (msg)
        *msg = p->getErrorMsg();
	
	return (line) ? guidoErrParse : guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI GuidoStream * GuidoOpenStream ()
{
    GuidoStream *newStream = new GuidoStream();

    return newStream;
}

// --------------------------------------------------------------------------
GUIDOAPI const char * GuidoGetStream (const GuidoStream * guidoStream)
{
	const string s (guidoStream->getStreamStr());
	char *str = new char[s.size()+1];
	strcpy(str, s.c_str());
	return str;
}


// --------------------------------------------------------------------------
GUIDOAPI void GuidoFreeStreamString (const char * str)
{
	delete[] str;
}

// --------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoCloseStream (GuidoStream *s)
{
    if (!s)
        return guidoErrBadParameter;

    delete s;

    return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoWriteStream (GuidoStream *s, const char *str)
{
    if (!s || !str)
        return guidoErrBadParameter;

    s->WriteToStream(str);

	return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoResetStream (GuidoStream *s)
{
    if (!s)
        return guidoErrBadParameter;

    s->ReinitStream();

	return guidoNoErr;
}
