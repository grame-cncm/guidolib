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

#include "GUIDOParse.h"
#include "ARFactory.h"
#include "GUIDOInternal.h"
/*#include <iostream>
#include <fstream>
#include <sstream>*/

using namespace std;


// ==========================================================================
// - Guido Parser API
// ==========================================================================

// ------------------------------------------------------------------------
GUIDOAPI(GuidoParser*) GuidoOpenParser (GuidoStream *s)
{

    GuidoParser *newParser = new GuidoParser(s->getStream());

    return newParser;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoCloseParser (GuidoParser* p)
{
    delete p;

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(ARHandler) GuidoFile2AR (GuidoParser* p, const char* file)
{
    //écrire le contenu du fichier dans le stream du parser
    
    ARHandler ar = GuidoParser2AR(p);

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoString2AR (GuidoParser* p, const char* str)
{
    //écrire la string dans le stream du parser
    
    ARHandler ar = GuidoParser2AR(p);

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoStream2AR (GuidoParser* p, const GuidoStream* stream) //REM: méthode bloquante qui va tourner dans un thread ?
{
    //écrire le contenu du stream dans le stream du parser
    
    ARHandler ar = GuidoParser2AR(p);

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(ARHandler)	GuidoParser2AR (GuidoParser* p)
{
    ARFactory *factory = p->getFactory();
    
    //backup de la factory
    GuidoFactoryCloseEvent(factory);
    GuidoFactoryCloseVoice(factory);
    GuidoFactoryCloseMusic(factory);
    GuidoFactoryClose(factory);

    ARHandler ar = guido_RegisterARMusic(factory->getMusic());

    //REM: il faut remettre la factory dans l'état précédent à un certain moment !
    //Juste avant de faire un WriteStream ?
    //Pas ici en tout cas, puisque sinon les close auront servi à rien.

	return ar;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoParserGetErrorCode (GuidoParser* p, int& line, int& col)
{
    line = p->getErrorColumn();
    col  = p->getErrorLine();

    return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoStream*) GuidoOpenStream ()
{
    GuidoStream *stream = new GuidoStream();

    return stream;

}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoCloseStream (GuidoStream* s)
{
    delete s;

    return guidoNoErr;
}

// --------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoWriteStream (GuidoStream* stream, const char* str)
{
    stream->WriteStream(str); //Ecrire str dans le stream

	return guidoNoErr;
}