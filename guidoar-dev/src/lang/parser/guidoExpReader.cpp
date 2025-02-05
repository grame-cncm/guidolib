/*
  GUIDO Library
  Copyright (C) 2008  Grame

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

#ifndef __guidoreader__
#define __guidoreader__

#include <iostream>

#include "AROthers.h"
#include "guidoExpFactory.h"
#include "guidoExpReader.h"
#include "guidoelement.h"
#include "guidoparser.h"
#include "glparser.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
int guidoExpReader::version () const			{ return 91; }
const char* guidoExpReader::versionStr () const
{ 
	static char versBuff[32];
	int v = version();
	sprintf (versBuff, "%d.%d", v/100, v%100);
	return versBuff;
}

//______________________________________________________________________________
bool guidoExpReader::parseFile  (FILE* fd)				{ return glparser::readfile (fd, this);	}
bool guidoExpReader::parseFile  (const char* file)		{ return glparser::readfile (file, this);	}
bool guidoExpReader::parseString  (const char* str)		{ return glparser::readstring (str, this);	}


//______________________________________________________________________________
SGLExpr guidoExpReader::getId(const string& id)
{ 
	pair<ExpList::const_iterator, ExpList::const_iterator> i = fExprMap.equal_range(id);
	if (i.first != i.second)
		return (*i.first).second;
	return 0;
}

//______________________________________________________________________________
void guidoExpReader::newIDExpr (const char * id, SGLExpr* e)
{
	if (e) fExprMap[id] = *e;
}

SGLExpr* guidoExpReader::newGroupExpr (SGLExpr* e)
{
	if (!e) return 0;
	SGLExpr* expr = new SGLExpr;
	*expr = guidoExpFactory::instance().create("group", *e);
	return expr;
}

SGLExpr* guidoExpReader::newNamedExpr (const char * name)
{
	SGLExpr e = getId(name);
	if (e) {
		SGLExpr* expr = new SGLExpr;
		*expr = guidoExpFactory::instance().createNamed(name, e);
		return expr;		
	}
	return 0;
}

SGLExpr* guidoExpReader::newScoreExpr (const char * gmnCode)
{
	SGLExpr* expr = new SGLExpr;
	guidoparser gp;
	Sguidoelement score = gp.parseString (gmnCode);
	if (score) {
		*expr = guidoExpFactory::instance().create(score);
		return expr;
	}
	return 0;  
}

SGLExpr* guidoExpReader::newBinaryExpr (const char * name, SGLExpr* e1, SGLExpr* e2)
{
	if (!e1 || !e2) return 0;
	SGLExpr* expr = new SGLExpr;
	*expr = guidoExpFactory::instance().create(name, *e1, *e2);
	return expr;
}

SGLExpr* guidoExpReader::newAbstractExpr (const char * name, SGLExpr* e1, SGLExpr* e2)
{
	if (!e1 || !e2) return 0;
	SGLExpr* expr = new SGLExpr;
	*expr = guidoExpFactory::instance().createAbstract(name, *e1, *e2);
	return expr;
}

int guidoExpReader::error(const char * msg, int lineno)
{
	cerr << msg << " on line " << lineno << endl;
	return 0;
}


} // namespace

#endif
