#ifndef __htmlcolors__
#define __htmlcolors__


/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <map>
#include <string>

class HtmlColor {
	static std::map<std::string, long> fColorsMap;
	
	public:
				HtmlColor();

		static void init ();
		static bool get (const char* name, unsigned char* c);
        static bool get (long color, unsigned char* c);
};

#endif
