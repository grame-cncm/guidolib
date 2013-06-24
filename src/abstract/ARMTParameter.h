#ifndef ARMTParameter_H
#define ARMTParameter_H

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
#include "ARMusicalTag.h"

class TagParameterList;

/** \brief not yet documented
*/
class ARMTParameter : public ARMusicalTag
{
	public:
	
	  	ARMTParameter( const TYPE_TIMEPOSITION & tp, const ARMTParameter * copy = 0 )
		  					: ARMusicalTag( tp, copy )
	 	{ }

		ARMTParameter( int pid = -1, const ARMTParameter * copy = 0 ) 
						: ARMusicalTag( pid, copy )
		{ }
	
		virtual ~ARMTParameter();


		virtual void setTagParameterList(TagParameterList & theTagParameterList) = 0;

		virtual TagParameterList * getTagParameterList() const
		{ return 0; }

};

#endif


