/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARNoteName.h"

std::string ARNoteName::c("c");
std::string ARNoteName::do_("do");
std::string ARNoteName::ut("ut");
std::string ARNoteName::cis("cis");

std::string ARNoteName::d("d");
std::string ARNoteName::re("re");
std::string ARNoteName::dis("dis");

std::string ARNoteName::e("e");
std::string ARNoteName::mi("mi");

std::string ARNoteName::f("f");
std::string ARNoteName::fa("fa");
std::string ARNoteName::fis("fis");

std::string ARNoteName::g("g");
std::string ARNoteName::so("so");
std::string ARNoteName::sol("sol");
std::string ARNoteName::gis("gis");

std::string ARNoteName::a("a");
std::string ARNoteName::la("la");
std::string ARNoteName::ais("ais");

std::string ARNoteName::h("h");
std::string ARNoteName::ti("ti");
std::string ARNoteName::si("si");
std::string ARNoteName::b("b");
std::string ARNoteName::empty("empty");

std::string ARNoteName::unknown("unknown");

ARNoteName::ARNoteName(std::string name) : std::string(name)	{}
ARNoteName::~ARNoteName()	{}

std::string ARNoteName::getName(std::string name) const
{
	if(name==c) return c;
	else if (name==do_) return do_;
	else if (name==ut) return ut;
	else if(name==d) return d;
	else if (name==re) return re;
	else if(name==e) return e;
	else if (name==mi) return mi;
	else if(name==f) return f;
	else if (name==fa) return fa;
	else if(name==g) return g;
	else if (name==so) return so;
	else if (name==sol) return sol;
	else if(name==a) return a;
	else if (name==la) return la;
	else if(name==h) return h;
	else if (name==ti) return ti;
	else if (name==si) return si;
    else if(name==b) return b;
	else if(name==cis) return cis;
	else if(name==dis) return dis;
	else if(name==fis) return fis;
	else if(name==gis) return gis;
	else if(name==ais) return ais;
	else if (name == empty) return empty;
	else { return unknown;} // unknown notename!
}
