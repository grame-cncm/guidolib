/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json2osc__
#define json2osc__

namespace json
{

/*
	Names definitions
	These names are used as element name by the osc to json converter
	The json to osc converter looks for these names to retrieve the osc fields
*/
extern const char* kAddressField;	///< [required] the osc message osc address
extern const char* kParamsField;	///< [required] the osc message parameters

extern const char* kOSCMarker;		///< [optional] mark a list of json objects that are expected to send to osc
extern const char* kSenderField;	///< [optional] indicates the sender address (set by the osc to json converter)
extern const char* kDestField;		///< [optional] indicates the message destination,
									///< could be a numeric value or a string (host name or formatted ip number)
extern const char* kPortField;		///< [optional] indicates the sender address (set by the osc to json converter)

} // end namespoace

#endif
