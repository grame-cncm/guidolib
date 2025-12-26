/*

  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include "streambeautifuller.h"

using namespace std;

streambeautifuller::streambeautifuller (ostream& stream, int max, int indentSize) 
		: fStream(stream), fSize(0), fMaxSize(max), fIndent(0), fIndentSize(indentSize)
{}

void streambeautifuller::newline ()
{
	int n = fIndent;
	fStream << '\n';
	while (n--) fStream << ' ';
	fSize = fIndent;
}

void streambeautifuller::pbreak ()
{
	if (fSize > fMaxSize) newline();	
}

void streambeautifuller::print (const string& s)
{
	size_t n = s.size();
	for (int i=0; i<n; i++) {
		if (s[i] == '\n') newline();
		else {
			fStream << s[i];
			fSize++;
		}
	}
}

streambeautifuller& streambeautifuller::operator << (char p)
{ 
	if (p == '\n') newline();
	else fStream << p;
	return *this;
}
