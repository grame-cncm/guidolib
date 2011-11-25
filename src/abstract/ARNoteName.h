#ifndef ARNoteName_H
#define ARNoteName_H

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
