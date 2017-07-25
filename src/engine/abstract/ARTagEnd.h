#ifndef ARTagEnd_H
#define ARTagEnd_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cassert>

#include "ARMTParameter.h"
#include "ARPositionTag.h"

class ARMusicalObject;

/** \brief not yet documented
*/
class ARTagEnd : public ARMTParameter, public ARPositionTag
{
	public:
					 ARTagEnd(int id = -1,const ARTagEnd * copy = 0)
						 : ARMTParameter(id,copy) { setAssociation(ARMusicalTag::LA); }
		virtual		~ARTagEnd() {}
		
		virtual ARMusicalObject * Copy() const 	{ return 0; }

		virtual const char*	getTagName() const		{ return "ARTagEnd"; };
		virtual std::string getGMNName() const		{ return "\\tagEnd"; };

		virtual void		setEndPosition(GuidoPos p_pos)			{ assert(false); }
		virtual GuidoPos	getEndPosition() const					{ return (GuidoPos)0; }
		
		virtual bool		isEndTagClass() const { return true; }
		
		static ARTagEnd * cast( ARPositionTag * inTag )
					{ return ( inTag && inTag->isEndTagClass() ? static_cast<ARTagEnd *>( inTag ) : 0 ); }
};

#endif
