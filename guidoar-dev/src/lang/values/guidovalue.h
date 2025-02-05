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

#ifndef __guidovalue__
#define __guidovalue__

#include "export.h"
#include "visitable.h"
//#include "ctree.h"
#include "guidorational.h"
#include "gar_smartpointer.h"

#ifdef valueDebug
#define valuePrint(expr)	cout << "create \"" << expr << "\"" << endl
#else
#define valuePrint(expr)
#endif

namespace guidolang 
{

class guidovalue;
typedef guido::SMARTP<guidovalue> 	Sguidovalue;

/*!
\brief The base class for guido language expressions.
*/
class export guidovalue : virtual public guido::smartable, public guido::visitable
{
    protected:
		Sguidovalue fArg1, fArg2;
				 guidovalue() {}
				 guidovalue(Sguidovalue v1, Sguidovalue v2) : fArg1(v1), fArg2(v2) {}
		virtual ~guidovalue() {}

	public:
 		enum { kInfinite = UINT_MAX };
		
		bool		infinite(const rational& r) const	{ rational inf = infinite(); return r == inf; }
		rational	infinite() const					{ return rational(kInfinite, 1); }

		virtual Sguidovalue	apply	(Sguidovalue& v)			= 0;
		virtual Sguidovalue	head	(unsigned int length)		= 0;
		virtual Sguidovalue	head	(const rational& length)	= 0;
		virtual Sguidovalue	tail	(unsigned int length)		= 0;
		virtual Sguidovalue	tail	(const rational& length)	= 0;
		virtual Sguidovalue	top		(unsigned int vnum)			= 0;
		virtual Sguidovalue	bottom	(unsigned int vnum)			= 0;
		virtual Sguidovalue	transpose(int interval)				= 0;
		virtual Sguidovalue	stretch  (rational ratio)			= 0;
		virtual Sguidovalue	stretch  (float ratio)				= 0;

		virtual unsigned int	length	()  = 0;	///< computes the length of the value 
		virtual rational		duration()  = 0;	///< computes the duration of the value
		virtual unsigned int	voices	()  = 0;	///< computes the voices count of the value
		virtual unsigned int	pitch	()  = 0;	///< computes the pitch of the value

		virtual void		acceptIn(guido::basevisitor& visitor);
		virtual void		acceptOut(guido::basevisitor& visitor);
		virtual	void		print(std::ostream& os);
		
		virtual Sguidovalue	getArg1 () const		{ return fArg1; }
		virtual Sguidovalue	getArg2 () const		{ return fArg2; }
};

export std::ostream& operator << (std::ostream& os, const Sguidovalue& elt);

} // namespace

#endif
