/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <locale.h>
#include "json_parser.h"

namespace json
{

//--------------------------------------------------------------------------
json_parser::json_parser(std::istream* stream)
	: fStream(stream), fJSON(0)
{
	setlocale(LC_NUMERIC, "C");
	initScanner();
}

//--------------------------------------------------------------------------
json_parser::~json_parser()
{
	setlocale(LC_NUMERIC, 0);
	destroyScanner();
}

} // end namespace

