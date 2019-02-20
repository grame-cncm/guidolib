#ifndef __MeterVisitor__
#define __MeterVisitor__

/*
  GUIDO Library
  Copyright (C) 2016  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <vector>

#include "ARVisitor.h"
#include "GUIDOEngine.h"
#include "defines.h"

//------------------------------------------------------------------------------
class MeterVisitor : public ARVisitor
{
	enum { kNoLimit = 0xffffffff };
	TYPE_TIMEPOSITION	fLimit;
	GuidoMeter			fLastMeter;
	

	public:
				 MeterVisitor() : fLimit(kNoLimit, 1)		{ reset(); }
				 MeterVisitor(const TYPE_TIMEPOSITION& limit) : fLimit(limit) { reset(); }
		virtual ~MeterVisitor() {}
	
		void		reset();			// reset the last meter value to a default value (no meter)
		GuidoMeter	getMeter() const { return fLastMeter; }
	
    virtual void visitIn  (ARMusicalTag* object);
};

//------------------------------------------------------------------------------
class MetersVisitor : public ARVisitor
{
	enum { kNoLimit = 0xffffffff };
	TYPE_TIMEPOSITION	fLimit;
	std::vector<GuidoMeter>	fMeters;

	public:
				 MetersVisitor() : fLimit(kNoLimit, 1)		{ reset(); }
				 MetersVisitor(const TYPE_TIMEPOSITION& limit) : fLimit(limit) { reset(); }
		virtual ~MetersVisitor() {}
	
		void		reset();			// reset the last meter value to a default value (no meter)
		GuidoMeters	getMeters() const;
	
    virtual void visitIn  (ARMusicalTag* object);
};


#endif
