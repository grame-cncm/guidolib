#ifndef GRMeter_H
#define GRMeter_H

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

		
		ARMeter* getARMeter();
		const ARMeter* getARMeter() const;

		std::vector<int> getNumeratorsVector() const { return numeratorsVector; }
		            int  getDenominator()      const { return denominator; }

  protected:
		ARMeter::metertype mtype;
        float totalNumeratorExtent;
		std::vector<int> numeratorsVector;
        int numerator;
		int denominator;

		float curLSPACE;
};


#endif

