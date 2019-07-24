#ifndef __TempoVisitor__
#define __TempoVisitor__

/*
  GUIDO Library
  Copyright (C) 2019  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <vector>

#include "ARVisitor.h"
#include "GUIDOEngine.h"

class ARTempo;
//------------------------------------------------------------------------------
class TempoVisitor : public ARVisitor
{
	std::vector<GuidoTempo>	fTempi;
	int						fCurrentVoice = -1;

	public:
				 TempoVisitor() {}
		virtual ~TempoVisitor() {}
	
		void		reset()				{ fTempi.clear(); }
		void		voice(int n)		{ fCurrentVoice = n; }
		size_t		size() const		{ return fTempi.size(); }
		GuidoTempoList	getTempi() const;
	
    virtual void visitIn  (ARMusicalTag* object);

	private:
		int 	toValue (const std::string& s) const;
		void 	getText (char* dst, const ARTempo* ar) const;

};


#endif
