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

using namespace std;

// - Guido AR
#include "ARFactory.h"

// - Guido Misc
#include "GUIDOInternal.h"
#include "GuidoParser.h"
#include "GuidoStream.h"
#include "GUIDOEngine.h"

#include "GUIDOParse.h"

/* REM: tmp */
#include <windows.h>
/************/


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
    /*if (!file || !p)
        return NULL;

    std::ifstream fileStream(file);
 
    if (!fileStream)
        return NULL;

    GuidoStream *parserStream = p->getGuidoStream();

    *parserStream << fileStream.rdbuf();

    fileStream.close();

    std::cout <<  parserStream->str();*/

    ARHandler ar = GuidoParser2AR(p);

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoString2AR (GuidoParser *p, const char *str)
{
    /*if( !str || !p )
        return NULL;

    std::istringstream iss(str);

    GuidoStream *parserStream = (GuidoStream *)p->getStream();

    //while (iss)
      //  *parserStream << iss.get();*/

    ARHandler ar = GuidoParser2AR(p);

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoStream2AR (GuidoParser *p, GuidoStream* s)
{
    if (!p || !s)
        return NULL;

    ARHandler ar = 0;

    p->setStream(s);

    ar = p->parse();

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoParser2AR (GuidoParser *p)
{
    if (!p)
        return NULL;

    /* Wait for the parser to be initialized */
    while (!p->getGuidoStream())
    {
        Sleep(10);
    }

    /* Wait for the stream to have consumed all data written at this moment */
    while (!p->getGuidoStream()->GetAreAllDataRead())
    {
        Sleep(10);
    }
    
    ARFactory *factory = p->getFactory();

    //REM: make factory backup

    GuidoFactoryCloseEvent(factory);
    GuidoFactoryCloseChord(factory);
    GuidoFactoryCloseVoice(factory);

    ARHandler ar = guido_RegisterARMusic(factory->getMusic());

    //REM: il faut remettre la factory dans l'état précédent à un certain moment !
    //Juste avant de faire un WriteStream ?
    //Pas ici en tout cas, puisque sinon les "close" auront servi à rien.

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
    GuidoStreamBuf *guidoStreamBuffer = new GuidoStreamBuf();

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
    {
        Sleep(10);
    }

    delete s;

    return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoWriteStream (GuidoStream *s, const char *str)
{
    if( !s || !str )
        return guidoErrBadParameter;

    s->WriteToStream(str);
    s->WriteToStream(" "); //bad, but without this the last character of str isn't taken in account


	return guidoNoErr;
}