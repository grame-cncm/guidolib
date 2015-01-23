/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "json2osc.h"
#include "json_object.h"
#ifndef JSON_ONLY
# include "osc_stream.h"
#endif

using namespace std;

namespace json
{

const char* kOSCMarker		= "OSCBundle";
const char* kSenderField	= "OSCSender";
const char* kDestField		= "OSCDest";
const char* kPortField		= "OSCPort";
const char* kAddressField	= "OSCAddress";
const char* kParamsField	= "OSCParams";

} // end namespoace

