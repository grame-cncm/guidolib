#ifndef ARBAR_H
#define ARBAR_H

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

#include <vector>

#include "ARMTParameter.h"

/** \brief Bar tag
*/
class ARBar : // public ARMusicalObject, 
	public ARMTParameter
 {
 public:
		typedef std::vector<pair<int, int> >	TRanges;
		enum { kNoNum, kNumAll, kNumPage };
		         ARBar(const TYPE_TIMEPOSITION & timeposition);
		         ARBar();
		virtual ~ARBar();

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		void  setMeasureNumber(int inMeasureNumber)   { measureNumber = inMeasureNumber; }
		int   getMeasureNumber()          const       { return measureNumber; }
        int   getMeasureNumberDisplayed() const       { return measureNumberDisplayed; }
        void  setMeasureNumberDisplayed(int mode)	  { measureNumberDisplayed = mode; }
        bool  isMeasureNumberDisplayedSet() const     { return measureNumberDisplayedIsSet; }

		float getMeasureNumberDxOffset() const        { return numDx; }
		float getMeasureNumberDyOffset() const        { return numDy; }
		
		const ARBar* previousBar () const				{ return fLastBar; }
		void		 setPreviousBar (const ARBar* bar)	{ fLastBar = bar; }
	 
        /**** Function to avoid dynamic_cast ****/
        ARMusicalObject *isARBar()			{ return this; }
        /*****************************************/
		const TRanges&	getRanges () const				{ return fRanges; }
		void			setRanges (const TRanges& r)	{ fRanges = r; }

  protected:
		TRanges	fRanges;

		int   measureNumber;
        int   measureNumberDisplayed;
		float numDx;
		float numDy;
		const ARBar*	fLastBar;

        bool measureNumberDisplayedIsSet;

		static ListOfTPLs ltpls;
};

#endif
