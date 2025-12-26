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

#ifndef __ARTag__
#define __ARTag__

#include <iostream>
#include <string>

#include "arexport.h"
#include "guidoelement.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup abstract

The <b>Guido Abstract Representation</b> (GAR) is close to the
gmn files. It supports logical layout thru GAR to GAR transformations.
The GAR is organised as a Tree composed of the following elements:
- ARMusic : the root element of the music description
- ARVoice : a sequence of Guido elements
- ARChord : a guido chord representation
- ARNote  : a guido note representation
- ARTag   : a set of types build over guidotag to handle all of the guido tags.
@{
*/


//______________________________________________________________________________
/*!
\brief A type for guido tags.
*/
class guidotag;
typedef SMARTP<guidotag>	Sguidotag;
class gar_export guidotag : public guidoelement
{
	public:
        virtual void acceptIn(basevisitor& v);
        virtual void acceptOut(basevisitor& v);
				
		int  getType() const			{ return fType; }	/// return the tag type ie the tag template integer
		long getID() const				{ return fID; }		/// return the tag id i.e. the xx of the \\tag:xx form
		void setID(long id)				{ fID = id; }
		operator std::string () const;

		bool	beginTag () const;		/// return true when the tag is in the form xxxBegin (tieBegin, slurBegin etc...)
		bool	endTag () const;		/// return true when the tag is in the form xxxEnd (tieEnd, slurEnd etc...)
		bool	matchTag (const Sguidotag&) const;	/// return true if tags are in the form xxxBegin and xxxEnd and xxx matches 
		std::string	matchTag () const;		/// return the name of the matching xxxBegin or xxxEnd form if any 

		//________________________________________________________________________
		virtual bool operator ==(const Sguidotag& i) const;
		virtual bool operator !=(const Sguidotag& i) const		{ return !(*this == i); }

    protected:	
				 guidotag(long id) : fID(id) {}
		virtual ~guidotag() {}
		std::string escape(const std::string&) const;
		long fID;						/// represents the tag id i.e the \\tag:xx form of the tags where xx is a discriminant id
		int  fType;						/// the tag template integer
};

//______________________________________________________________________________
/*!
\brief A template class to type all guido tags with integers

	The class implements the acyclic visitor accept method.
*/
template <int elt> class ARTag : public guidotag
{
	public:
		static SMARTP<ARTag<elt> > create(long id=0)
			{ ARTag<elt>* o = new ARTag<elt>(id); assert(o!=0); o->fType=elt; return o; }

        virtual void acceptIn(basevisitor& v) {
			if (visitor<SMARTP<ARTag<elt> > >* p = dynamic_cast<visitor<SMARTP<ARTag<elt> > >*>(&v)) {
				SMARTP<ARTag<elt> > sptr = this;
				p->visitStart(sptr);
			}
			else guidotag::acceptIn(v);
		}
        virtual void acceptOut(basevisitor& v) {
			if (visitor<SMARTP<ARTag<elt> > >* p = dynamic_cast<visitor<SMARTP<ARTag<elt> > >*>(&v)) {
				SMARTP<ARTag<elt> > sptr = this;
				p->visitEnd(sptr);
			}
			else guidotag::acceptOut(v);
		}

    protected:	
				 ARTag(long id) : guidotag(id) {}
		virtual ~ARTag() {}
};

/*! @} */

} // namespace

#endif
