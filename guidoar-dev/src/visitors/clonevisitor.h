/*
  GUIDO Library
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

#ifndef __cloneVisitor__
#define __cloneVisitor__

#include <ostream>
#include <stack>

#include "arexport.h"
#include "ARTypes.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to print the gmn description
*/
class gar_export clonevisitor :
	public visitor<SARMusic>,
	public visitor<SARVoice>,
	public visitor<SARChord>,
	public visitor<SARNote>,
	public visitor<Sguidotag>
{
    public:
				 clonevisitor() {}
       	virtual ~clonevisitor() {}
              
		virtual Sguidoelement clone(const Sguidoelement&);
		virtual Sguidoelement result()		{ Sguidoelement res = fStack.top(); fStack.pop(); return res; }

	protected:
		virtual void visitStart( SARMusic& elt );
		virtual void visitStart( SARVoice& elt );
		virtual void visitStart( SARChord& elt );
		virtual void visitStart( SARNote& elt );
		virtual void visitStart( Sguidotag& elt );

		virtual void visitEnd  ( SARVoice& elt );
		virtual void visitEnd  ( SARChord& elt );
		virtual void visitEnd  ( Sguidotag& elt );
		
		// the copy method may be used by derived classes to filter the elements
		virtual bool copy  () const	{ return true; }

		virtual void			push (const SARNote& elt, bool stack=false);
		virtual void			push (const Sguidoelement& elt, bool stack=true);
		virtual void			copyAttributes (const Sguidoelement& src, Sguidoelement& dst) const;
		virtual Sguidoelement	copy (const Sguidoelement& elt, Sguidoelement& dst) const;
		virtual SARNote			copy (const SARNote& elt) const;

		std::stack<Sguidoelement> fStack;
};

/*! @} */

} // namespace

#endif
