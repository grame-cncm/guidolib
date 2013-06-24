#ifndef ARNoteName_H
#define ARNoteName_H

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

#include <string>
//#include "ARName.h"

//  Provides all possible notenames as copy-template
// Therefore the string-baseclass (in MFC CString)
//	must be on top of a reference mechanism.
// all copies of a stringoject point to the same C-String.
// So for every possible notename will only a single
// representation created in memory, undependend from 
// the number of created notes

/** \brief not yet documented
*/
class ARNoteName : public std::string
{
  public:
					 ARNoteName(std::string name);
		virtual 	~ARNoteName();

		// returns a copy of the stringobject that aquivalent to "name"
		//	wrong "name" triggers assertion
		virtual std::string getName(std::string name) const;

		static std::string c;
		static std::string do_;
		static std::string ut;
		static std::string cis;

		static std::string d;
		static std::string re;
		static std::string dis;

		static std::string e;
		static std::string mi;

		static std::string f;
		static std::string fa;
		static std::string fis;

		static std::string g;
		static std::string so;
		static std::string sol;
		static std::string gis;

		static std::string a;
		static std::string la;
		static std::string ais;

		static std::string h;
		static std::string ti;
		static std::string si;
        static std::string b;

		static std::string empty;
		static std::string unknown;

		using std::string::operator=;
};

#endif
