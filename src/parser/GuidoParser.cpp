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

#include "GuidoParser.h"
#include "ARFactory.h"

namespace guido
{

//--------------------------------------------------------------------------
GuidoParser::GuidoParser(std::istream* stream) : fStream(stream)
{
	setlocale(LC_NUMERIC, "C");
	fFactory = new ARFactory();
    initScanner();
}

//--------------------------------------------------------------------------
GuidoParser::~GuidoParser() 
{
	setlocale(LC_NUMERIC, 0);
	destroyScanner();
	delete fFactory;
}

} // end namespace

