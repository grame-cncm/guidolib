/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
