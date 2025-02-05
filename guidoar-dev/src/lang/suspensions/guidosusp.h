/*
  GUIDO Library
  Copyright (C) 2006-2008  Grame

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

#ifndef __guidosusp__
#define __guidosusp__

#include "guidovalue.h"
#include "visitor.h"

#ifdef suspDebug
#define suspForce(expr)		cout << "Force \"" << expr << "\"" << endl
#define suspCreate(expr)	cout << "Create \"" << expr << "\"" << endl
#else
#define suspForce(expr)
#define suspCreate(expr)
#endif

namespace guidolang 
{

class guidosusp;
typedef guido::SMARTP<guidosusp> 	Sguidosusp;

//______________________________________________________________________________
/*!
\brief a guido value suspension.
*/
class export guidosusp : public guidovalue
{
    protected:
		Sguidovalue	fValue;
		
				 guidosusp() {}
		virtual ~guidosusp() {}

	public:
		virtual Sguidovalue	getValue();
		virtual Sguidovalue	force() = 0;

		virtual Sguidovalue	apply	(Sguidovalue& v)		{ return getValue()->apply(v); }
		virtual Sguidovalue	head	(unsigned int length)	{ return getValue()->head(length); }
		virtual Sguidovalue	head	(const rational& length){ return getValue()->head(length); }
		virtual Sguidovalue	tail	(unsigned int length)	{ return getValue()->tail(length); }
		virtual Sguidovalue	tail	(const rational& length){ return getValue()->tail(length); }
		virtual Sguidovalue	top		(unsigned int vnum)		{ return getValue()->top(vnum); }
		virtual Sguidovalue	bottom	(unsigned int vnum)		{ return getValue()->bottom(vnum); }
		virtual Sguidovalue	transpose(int interval)			{ return getValue()->transpose(interval); }
		virtual Sguidovalue	stretch  (rational ratio)		{ return getValue()->stretch(ratio); }
		virtual Sguidovalue	stretch  (float ratio)			{ return getValue()->stretch(ratio); }

		virtual unsigned int	length	()		{ return getValue()->length(); }
		virtual rational		duration()		{ return getValue()->duration(); }
		virtual unsigned int	voices	()		{ return getValue()->voices(); }
		virtual unsigned int	pitch	()		{ return getValue()->pitch(); }

		virtual void		acceptIn(guido::basevisitor& visitor);
		virtual void		acceptOut(guido::basevisitor& visitor);
};


} // namespace

#endif
