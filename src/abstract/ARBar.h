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

		virtual void print() const;
       // virtual ostream & operator<<(ostream &os) const;
		virtual void PrintName(std::ostream & os) const;
		virtual void PrintParameters(std::ostream & os) const;

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		int getBarNumber() const { return barnumber; }

		void setMeasureNumber(int inMeasureNumber) { measureNumber = inMeasureNumber; }
		int  getMeasureNumber()                    { return measureNumber; }

		float  getMeasureNumberDxOffset()            { return numDx; }
		float  getMeasureNumberDyOffset()            { return numDy; }

		virtual bool operator==(const ARBar & bar)
			{ return (barnumber == bar.barnumber); }

  protected:
		int barnumber; // number of bar where barline starts
		int measureNumber;

		float numDx;
		float numDy;

		static ListOfTPLs ltpls;
  };


#endif
