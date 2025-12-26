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

#include "exceptions.h"
#include "guidoexpression.h"
#include "guidoApplyValue.h"
#include "guidoEval.h"
#include "guidoEvalSusp.h"
#include "guidoClosureValue.h"
#include "guidoErrValue.h"
#include "guidoMixValue.h"
#include "guidoScoreValue.h"
#include "guidoSeqValue.h"
#include "valueRenderer.h"

#include "AROthers.h"
#include "seqOperation.h"
#include "parOperation.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

#ifdef WIN32
#define kSep "\\"
#else
#define kSep "/"
#endif

//______________________________________________________________________________
Sguidoelement valueRenderer::render(const Sguidoexpression& exp)
{
	fPos = 0;
	try {
		SguidoEnv env = guidoEnv::create();				
		Sguidovalue val = exp->eval(env);
		if (val) return render(val);
	} catch (const TException& e)  { 
		string file(e.file);
		size_t pos = file.find_last_of(kSep);
		if (pos != string::npos) file = file.substr(pos+1);
		cerr << e.msg << " - file: '" << file << "' line: " << e.line << endl;
	}
	return 0;
}

//______________________________________________________________________________
Sguidoelement valueRenderer::render(const Sguidovalue& val)
{ 
	fScore = 0;
cout << "valueRenderer::render " << val << endl;
	val->acceptIn (*this);
	return fScore;
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void valueRenderer::visitStart( Sguidovalue& exp )
{
	cerr << __FILE__ << ": unexpected Sguidovalue received" << endl;
}

//______________________________________________________________________________
void valueRenderer::visitStart( SguidoApplyValue& exp )
{
	fPos++;
	if (fPos < fLimit) {
		Sguidovalue v1 = exp->getArg1();
		Sguidovalue v2 = exp->getArg2();
		fScore = render (v1->apply(v2));	
	}
	else {
		cout << "SguidoApplyValue rendering limit reached" << endl;
	}
}

//______________________________________________________________________________
void valueRenderer::visitStart( SguidoEvalSusp& exp )
{
	fScore = render (exp->force());	
}

//______________________________________________________________________________
void valueRenderer::visitStart( SguidoClosureValue& exp )
{
	cout << "valueRenderer SguidoClosureValue" << endl;
}

//______________________________________________________________________________
void valueRenderer::visitStart( SguidoSeqValue& exp )
{
	Sguidovalue v2 = exp->getArg2();
	SARMusic s1 = dynamic_cast<ARMusic*> ((guidoelement*)render(exp->getArg1()));
	SARMusic s2 = dynamic_cast<ARMusic*> ((guidoelement*)render(v2));
	if (s1 && s2) {
		seqOperation seq;
		fScore = seq(s1, s2);
	}
	else {
		SguidoErrValue err1 = dynamic_cast<guidoErrValue*> ((guidovalue*)exp->getArg1());
		SguidoErrValue err2 = dynamic_cast<guidoErrValue*> ((guidovalue*)v2);
		cout << "SguidoSeqValue err " << (void*)err1 << " - " << (void*)err2 << endl;
		if (err2) fScore = s1;
		else fScore = 0;
	}
}

//______________________________________________________________________________
void valueRenderer::visitStart( SguidoScoreValue& exp )
{
	fScore = exp->getScore();
}

//______________________________________________________________________________
void valueRenderer::visitStart( SguidoMixValue& exp )
{
	SARMusic s1 = dynamic_cast<ARMusic*> ((guidoelement*)render(exp->getArg1()));
	SARMusic s2 = dynamic_cast<ARMusic*> ((guidoelement*)render(exp->getArg2()));
	if (s1 && s2) {
		parOperation mix;
		fScore = mix(s1, s2);
	}
	else fScore = 0;
}

}
