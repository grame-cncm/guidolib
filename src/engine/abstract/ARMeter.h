#ifndef ARMeter_H
#define ARMeter_H

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

#include <string>

#include "ARMTParameter.h"

/** \brief not yet documented
*/
class ARMeter : public ARMTParameter
	// , public ARMusicalObject
{
  public:

		enum metertype { NONE, NUMERIC, C, C2  };

				ARMeter();
				ARMeter(int p_numerator, int p_denominator);

		virtual ~ARMeter();

		virtual bool IsStateTag() const;

	    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList & tpl);

		TYPE_TIMEPOSITION getMeterTime() const;

		int  getAutoBarlines()                  const { return autoBarlines; }
		bool getAutoMeasuresNum()               const { return autoMeasuresNum; }
		std::vector<int> getNumeratorsVector()  const { return numeratorsVector; }
        int getNumerator()                      const { return numerator; }
		int getDenominator()                    const { return denominator; }

		metertype	getMeterType() const	         { return mtype; }
		const char* getName() const			         { return mMeterName.c_str(); } 

		int		getMeasureNum() const                { return mnum; }
		void	setMeasureNum(int p_num)             { mnum = p_num; }

		const TYPE_TIMEPOSITION & getTimeOffset() const	{ return timeoffset; }
		void setTimeOffset(const TYPE_TIMEPOSITION &tp) { timeoffset = tp; }

        float getSize() const { return size; }

        /**** Function to avoid dynamic_cast ****/
        ARMusicalObject *isARMeter() { return this; }
        /****************************************/

  protected:
	  // timeoffset describes the offset from the current meter-position within a staff. 
	  // This is set and read by GRStaff
	  TYPE_TIMEPOSITION timeoffset;
	  // mnum is the MeasureNumber. It is set and read through GRStaff.
	  int mnum;

  protected:
	  float  size;
	  bool   autoBarlines;
	  bool   autoMeasuresNum;
	  static ListOfTPLs ltpls;
 
  private:
		std::string mMeterName;
        std::vector<int> numeratorsVector;
        int numerator;
		int denominator;
		metertype mtype;
};

#endif
