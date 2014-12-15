#ifndef ARTagEnd_H
#define ARTagEnd_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
// #include "ARMusicalObject.h"
#include "ARPositionTag.h"

class ARMusicalObject;

/** \brief not yet documented
*/
class ARTagEnd : public ARMTParameter, public ARPositionTag
{
	public:
					 ARTagEnd(int id = -1,const ARTagEnd * copy = 0);
		virtual		~ARTagEnd() {}
		
		virtual ARMusicalObject * Copy() const 	{ return 0; }

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual void setEndPosition(GuidoPos p_pos);
		virtual GuidoPos getEndPosition() const;
		
		virtual bool isEndTagClass() const { return true; }
		
		static ARTagEnd * cast( ARPositionTag * inTag )
					{ return ( inTag && inTag->isEndTagClass() ? static_cast<ARTagEnd *>( inTag ) : 0 ); }
};

#endif
