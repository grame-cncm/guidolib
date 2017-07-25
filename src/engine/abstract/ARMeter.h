#ifndef ARMeter_H
#define ARMeter_H

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

#include <string>

#include "ARMTParameter.h"
#include "Fraction.h"

/** \brief not yet documented
*/
class ARMeter : public ARMTParameter
{
 	public:
		enum metertype { NONE, NUMERIC, C, C2  };
		enum { kNoAutoMeasureNum, kAutoMeasureNum, kAutoMeasureNumPage, kAutoMeasureNumSystem };

				 ARMeter();
				 ARMeter(int p_numerator, int p_denominator);
		virtual ~ARMeter() {}

		virtual bool IsStateTag() const				{ return true; }

		virtual int	 getOrder() const				{ return kMeterOrder; }
		virtual const char*	getParamsStr() const	{ return kARMeterParams; };
		virtual const char*	getTagName() const		{ return "ARMeter"; };
		virtual std::string getGMNName() const		{ return "\\meter"; };

		virtual void setTagParameters (const TagParameterMap& params);

		int  getAutoBarlines()                  const { return fAutoBarlines; }
		int	 getAutoMeasuresNum()               const { return fAutoMeasuresNum; }
		bool isSingleUnit()						const { return fSingleUnit; }
		bool groupComplex()						const { return fGroupComplex; }

		const std::vector<Fraction>& getMeters()const { return fMetersVector; }
        int getNumerator()                      const { return fMeterDuration.getNumerator(); }
		int getDenominator()                    const { return fMeterDuration.getDenominator(); }
		TYPE_DURATION getMeterDuration()        const { return fMeterDuration; }

		metertype	getMeterType() const	         { return fType; }
		const char* getName() const			         { return fMeterName.c_str(); } 

		int		getMeasureNum() const                { return mnum; }
		void	setMeasureNum(int p_num)             { mnum = p_num; }

		const TYPE_TIMEPOSITION & getTimeOffset() const	{ return timeoffset; }
		void setTimeOffset(const TYPE_TIMEPOSITION &tp) { timeoffset = tp; }
        ARMusicalObject *isARMeter() { return this; }

		const std::vector<Fraction>& getMetersVector()  const { return fMetersVector; }

  protected:
	  // timeoffset describes the offset from the current meter-position within a staff. 
	  // This is set and read by GRStaff
	  TYPE_TIMEPOSITION timeoffset;
	  // mnum is the MeasureNumber. It is set and read through GRStaff.
	  int mnum;

  private:
		bool						isNumeric		(const std::string& meter)  const;
		bool						singleUnit		(const std::vector<Fraction>& m)  const;
		void						setMeter		(const std::string& str);
		Fraction					str2meter		(std::string str)  const;
		Fraction					metersDuration	(const std::vector<Fraction>& m)  const;
		const std::vector<Fraction> finalizeMeters	(const std::vector<Fraction>& m)  const;
		const std::vector<Fraction> parseMeters		(std::string str)  const;

		bool	fAutoBarlines;
		int		fAutoMeasuresNum;

		std::string				fMeterName;
        std::vector<Fraction>	fMetersVector;
		Fraction				fMeterDuration;
		bool					fSingleUnit;		// a flag that indicates if one or several units are used
		bool					fGroupComplex;		// a flag that indicates if complex meters must be grouped by common denominator
		metertype				fType;
};

#endif
