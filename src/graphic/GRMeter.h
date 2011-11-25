#ifndef GRMeter_H
#define GRMeter_H

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

#include "ARMeter.h"
#include "GRTagARNotationElement.h"
#include "GRDefine.h"

class GRStaff;


/** \brief Meter notation tag
*/
class GRMeter : public GRTagARNotationElement
{

  public:

	  virtual bool operator==(const GRMeter &meter) const;
	  virtual bool operator==(const GRTag &mtag) const;

		GRMeter(ARMeter * abstractRepresentationOfMeter,
			GRStaff * curstaff, bool p_ownsAR = false);

		virtual ~GRMeter();


		virtual void GGSOutput() const;
		virtual void OnDraw(VGDevice & hdc ) const;

		virtual void print() const;

		ARMeter* getARMeter();
		const ARMeter* getARMeter() const;

		int getNumerator() const { return numerator; }
		int getDenominator() const { return denominator; }

		const NVPoint & getReferencePosition() const;

  protected:
		ARMeter::metertype mtype;
		int numerator;
		int denominator;

		static NVPoint refposC;
		static NVPoint refposC2;
		NVPoint refpos;

		float curLSPACE;
};


#endif

