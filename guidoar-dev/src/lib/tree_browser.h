/*

  GUIDOEngine Library
  Copyright (C) 2007  Grame

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

#ifndef __tree_browser__
#define __tree_browser__

#include "basevisitor.h"
#include "browser.h"
#include "ctree.h"

namespace guido 
{

//______________________________________________________________________________
template <typename T> class tree_browser : public browser<T> 
{
	protected:
		basevisitor*	fVisitor;
		bool			fDone;

		virtual void enter (T& t)		{ t.acceptIn(*fVisitor); }
		virtual void leave (T& t)		{ t.acceptOut(*fVisitor); }

	public:
		typedef typename ctree<T>::treePtr treePtr;
		
				 tree_browser(basevisitor* v=0) : fVisitor(v), fDone(false) {}
		virtual ~tree_browser() {}

		virtual void stop (bool state=true)	{ fDone = state; }
		virtual bool done () const			{ return fDone; }
		virtual void set (basevisitor* v)	{ fVisitor = v; }
		virtual void browse (T& t) {
			enter(t);
			typename ctree<T>::literator iter;
			for (iter = t.lbegin(); (iter != t.lend()) && !done(); iter++)
				browse(**iter);
			leave(t);
		}
};

}

#endif
