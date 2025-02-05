/*

  MusicXML Library
  Copyright (C) 2006,2007  Grame

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

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include <iostream>

#include "suspVisitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
Sguidovalue suspVisitor::force(const Sguidovalue& exp)
{ 
	fValue = 0;
	exp->acceptIn (*this);
	return fValue;
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void suspVisitor::visitStart( Sguidovalue& exp )
{
	fValue = exp;
}

//______________________________________________________________________________
void suspVisitor::visitStart( Sguidosusp& exp )
{
	cerr << __FILE__ << ": unexpected Sguidosusp received" << endl;
}

//______________________________________________________________________________
void suspVisitor::visitStart( SguidoApplySusp& exp )
{
	fValue = exp->apply (fArg);

}

//______________________________________________________________________________
void suspVisitor::visitStart( SguidoClosureSusp& exp )
{
}

//______________________________________________________________________________
void suspVisitor::visitStart( SguidoSeqSusp& exp )
{
}

//______________________________________________________________________________
void suspVisitor::visitStart( SguidoParSusp& exp )
{
}

}
