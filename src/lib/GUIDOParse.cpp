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

// - Guido AR
#include "ARFactory.h"
#include "ARBackupFactory.h"

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
    s->SetFactory(p->getFactory()); // In order that GuidoWriteStream can reload 
                                    // backuped factory juste before writing

    /* Parse ! (blocking method while stream isn't closed) */
    ar = p->parse();

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoParser2AR (GuidoParser *p)
{
    if (!p)
        return NULL;

    /* Wait for the stream to be set in parser */
    while (!p->getGuidoStream())
        sleep(10);

    /* Wait for the stream to have consumed all data written for now */
    while (!p->getGuidoStream()->GetAreAllDataRead())
        sleep(10);
    
    ARHandler ar = 0;

    ARFactory *factory = p->getFactory();

    if (factory)
    {
        /* Factory backup */
        factory->MakePartialBackup();

        /* Factory closes */
        GuidoFactoryCloseEvent(factory);
        GuidoFactoryCloseChord(factory);
        GuidoFactoryCloseVoice(factory);

        ar = guido_RegisterARMusic(factory->getMusic());
    }

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoParserGetErrorCode (GuidoParser *p, int &line, int &col)
{
    if (!p)
        return guidoErrBadParameter;

    line = p->getErrorColumn();
    col  = p->getErrorLine();

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

    /* Wait for factory to be set in GuidoStream (by GuidoStream2AR) */
    while (!s->GetFactory())
        sleep(10);

    /* Factory reloading */
    if (s->GetFactory()->GetIsBackupExisting())
        s->GetFactory()->ReloadOldFactory();

    s->WriteToStream(str);
    s->WriteToStream(" "); // Ugly and "dangerous", but without this the last
                           // character of str isn't parsed this time by the parser
                           // (we have to wait the next character or the stream's end)

	return guidoNoErr;
}