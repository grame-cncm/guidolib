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

#include "ARMTParameter.h"

/** \brief Bar tag
*/
class ARBar : // public ARMusicalObject, 
	public ARMTParameter
 {
 public:
		         ARBar(const TYPE_TIMEPOSITION & timeposition);
		         ARBar();
		virtual ~ARBar();

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		void  setMeasureNumber(int inMeasureNumber)   { measureNumber = inMeasureNumber; }
		int   getMeasureNumber()          const       { return measureNumber; }
        bool  getMeasureNumberDisplayed() const       { return measureNumberDisplayed; }
        void  setMeasureNumberDisplayed(bool display) { measureNumberDisplayed = display; }
        bool  isMeasureNumberDisplayedSet() const     { return measureNumberDisplayedIsSet; }

		float getMeasureNumberDxOffset()                         { return numDx; }
		float getMeasureNumberDyOffset()                         { return numDy; }
        
        /**** Function to avoid dynamic_cast ****/
        ARMusicalObject *isARBar() { return this; }
        /*****************************************/

  protected:
		int   measureNumber;
        bool  measureNumberDisplayed;
		float numDx;
		float numDy;

        bool measureNumberDisplayedIsSet;

		static ListOfTPLs ltpls;
};

#endif
