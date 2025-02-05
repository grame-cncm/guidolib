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

#ifndef __guidoSeqValue__
#define __guidoSeqValue__

#include "guidovalue.h"

namespace guidolang 
{

/*!
\brief The base class for guido language expressions.
*/
class export guidoSeqValue : public guidovalue
{
	protected:
				 guidoSeqValue(Sguidovalue v1, Sguidovalue v2) : guidovalue(v1, v2) {}
		virtual ~guidoSeqValue() {}

	public:
		static Sguidovalue create(Sguidovalue v1, Sguidovalue v2);

		virtual Sguidovalue	apply	(Sguidovalue& v);
		virtual Sguidovalue	head	(unsigned int length);
		virtual Sguidovalue	head	(const rational& length);
		virtual Sguidovalue	tail	(unsigned int length);
		virtual Sguidovalue	tail	(const rational& length);
		virtual Sguidovalue	top		(unsigned int vnum);
		virtual Sguidovalue	bottom	(unsigned int vnum);
		virtual Sguidovalue	transpose(int interval);
		virtual Sguidovalue	stretch  (rational ratio);
		virtual Sguidovalue	stretch  (float ratio);

		virtual unsigned int	length	();
		virtual rational		duration();
		virtual unsigned int	voices	();
		virtual unsigned int	pitch	();

		virtual void		acceptIn(guido::basevisitor& visitor);
		virtual void		acceptOut(guido::basevisitor& visitor);
		virtual	void		print(std::ostream& os);
};
typedef guido::SMARTP<guidoSeqValue> 	SguidoSeqValue;


} // namespace

#endif
