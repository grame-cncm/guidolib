/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <locale.h>
#include <istream>

#include "GuidoStream.h"

#include "NoteAndChordParser.h"
#include "ARFactory.h"

//--------------------------------------------------------------------------
NoteAndChordParser::NoteAndChordParser()
{
//	setlocale(LC_NUMERIC, "C");
//	fFactory = 0;
//    initScanner();
//	fErrorLine = fErrorColumn = 0;
//    fStream = NULL;
}

//--------------------------------------------------------------------------
NoteAndChordParser::~NoteAndChordParser()
{
//	setlocale(LC_NUMERIC, 0);
//	destroyScanner();
//	delete fFactory;
}
//--------------------------------------------------------------------------
void NoteAndChordParser::parseNoteOrChord()
{
    /* Parser reinitialization */
	fzaehlerSet = 0;
	faccidentals = 0;
	fndots = 0;
	fnt_enumSet = false;
	fnt_enum = 0;
	fnt_denom = 1;
	fErrorLine = fErrorColumn = 0;
    setlocale(LC_NUMERIC, "C");
    fText = "";

    destroyScanner();
    initScanner();

    /***************************/
	_yyparse ();
}
