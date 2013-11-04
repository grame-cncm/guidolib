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

#include <istream>
#include <fstream>

using namespace std;

// - Guido Misc
#include "GUIDOInternal.h"
#include "GuidoParser.h"
#include "GuidoStream.h"
#include "GUIDOEngine.h"

#include "GUIDOParse.h"


#ifdef _WIN32
    #include <windows.h>

    void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>

    void sleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif


// ==========================================================================
// - Guido Parser API
// ==========================================================================

// ------------------------------------------------------------------------
GUIDOAPI(GuidoParser *) GuidoOpenParser ()
{
    GuidoParser *newParser = new GuidoParser();

	return newParser;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoCloseParser (GuidoParser *p)
{
    if (!p)
        return guidoErrBadParameter;

    delete p;

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(ARHandler) GuidoFile2AR (GuidoParser *p, const char *file)
{
    if (!file || !p)
        return NULL;

    std::ifstream ifs(file, ios::in);
 
    if (!ifs)
        return NULL;

    p->setStream(&ifs);

    ARHandler ar = p->parse();

    ifs.close();

    return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoString2AR (GuidoParser *p, const char *str)
{
    if( !str || !p )
        return NULL;

    std::istringstream iss(str);

    p->setStream(&iss);

    ARHandler ar = p->parse();

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoStream2AR (GuidoParser *p, GuidoStream* s)
{
    if (!p || !s)
        return NULL;

    ARHandler ar = 0;

    /* Set the GuidoStream into the GuidoParser */
    p->setStream(s);

    /* Parse ! (blocking method while stream isn't closed AND no syntax errors) */
    ar = p->parse();

    if (!ar)
        s->SetParserJobFinished(); /* There are syntax errors, we have to unblock GuidoParser2AR which
                                      may be waiting for stream to say that all data are consumed */

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoParser2AR (GuidoParser *p)
{
    if (!p)
        return NULL;

    GuidoStream *s = NULL;

    /* Wait for the stream to be set in parser */
    while ((s = p->getGuidoStream()) == NULL)
        sleep(10);

    while (!s->GetAreAllDataRead() && !s->GetParserJobFinished())
        sleep(10);
    /****************************/
    
    ARHandler ar = 0;
    int line;
    int col;
    
    GuidoParserGetErrorCode(p, line, col);

    if (line == 0) // No syntax error, then we can do synchronous parsing
    {
        GuidoParser *synchronousParser = new GuidoParser();
        stringstream *stringStreamToParse = new stringstream();
        string stringToCopy = p->getGuidoStream()->getSynchronousString();

        stringStreamToParse->str(stringToCopy);
        synchronousParser->setStream(stringStreamToParse);

        ar = synchronousParser->parseSynchronousParser();

        GuidoCloseParser(synchronousParser);
    }

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoParserGetErrorCode (GuidoParser *p, int &line, int &col)
{
    if (!p)
        return guidoErrBadParameter;

    line = p->getErrorLine();
    col  = p->getErrorColumn();

    return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoStream *) GuidoOpenStream ()
{
    /* Perso streambuf initialized /*/
    GuidoStreamBuf *guidoStreamBuffer = new GuidoStreamBuf();

    /* GuidoStream initialized, based on perso streambuf */
    GuidoStream *newStream = new GuidoStream(guidoStreamBuffer);

    return newStream;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoCloseStream (GuidoStream *s)
{
    if (!s)
        return guidoErrBadParameter;

    s->CloseStream();

    /* Wait for the parser to end his job */
    while (!s->IsParserJobFinished())
        sleep(10);

    delete s;

    return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoWriteStream (GuidoStream *s, const char *str)
{
    if( !s || !str )
        return guidoErrBadParameter;

    s->WriteToStream(str);
    s->WriteToStream(" "); // Ugly and "dangerous", but without this the last
                           // character of str isn't parsed this time by the parser
                           // (we have to wait the next character or the stream's end)

	return guidoNoErr;
}