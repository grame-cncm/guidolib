#ifndef ARMeter_H
#define ARMeter_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
		virtual void print() const;
		virtual std::ostream & operator<<(std::ostream & os) const;

		virtual void setTagParameterList(TagParameterList & tpl);

		TYPE_TIMEPOSITION getMeterTime() const;

		int getAutoBarlines() const			{ return autoBarlines; }
		int getNumerator() const			{ return numerator; }
		int getDenominator() const			{ return denominator; }

		metertype	getMeterType() const	{ return mtype; }
		const char* getName() const			{ return mMeterName.c_str(); } 

		int		getMeasureNum() const			{ return mnum; }
		void	setMeasureNum(int p_num)		{ mnum = p_num; }

		const TYPE_TIMEPOSITION & getTimeOffset() const	{ return timeoffset; }
		void setTimeOffset(const TYPE_TIMEPOSITION &tp) { timeoffset = tp; }

  protected:
	  // timeoffset describes the offset from the current meter-position within a staff. 
	  // This is set and read by GRStaff
	  TYPE_TIMEPOSITION timeoffset;
	  // mnum is the MeasureNumber. It is set and read through GRStaff.
	  int mnum;

  protected:
	  float size;
	  int autoBarlines;
	  static ListOfTPLs ltpls;
 
  private:
		std::string mMeterName;
		int numerator;
		int denominator;
		metertype mtype;
		

};

#endif
