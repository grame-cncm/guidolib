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

#include <utility>

#include "ARMeter.h"
#include "GRTagARNotationElement.h"
#include "GRDefine.h"
#include "GRVisitor.h"

class GRStaff;


/** \brief Meter notation tag
*/
class GRMeter : public GRTagARNotationElement
{

  public:
				 GRMeter(ARMeter * ar, GRStaff * curstaff, bool p_ownsAR = false);
		virtual ~GRMeter() {}

		virtual void	GGSOutput() const;
		virtual void	OnDraw(VGDevice & hdc ) const;
		virtual bool	checkCollisionWith() const	{ return true; }
		virtual void	accept (GRVisitor& visitor);
	
		ARMeter*		getARMeter();
		const ARMeter*	getARMeter() const;


  private:
		ARMeter::metertype				fType;
		const std::vector<Fraction>&	fMeters;
		float	fCurLSPACE;
		float	fNumericHeight;		// height of a numeric glyph
		bool	fGroupComplex;		// complex meter grouping flag

		typedef std::pair<std::string,std::string> TSingleMeter;

		const std::vector<Fraction>&	getMeters()	const { return fMeters; }
		std::pair<float,float>			GetXOffsets(VGDevice & hdc, const std::string& num, const std::string& dnum) const;
		std::vector<TSingleMeter>		meters2metersStr(const std::vector<Fraction>& meters) const;

		void		DrawSymbolStr(const char* str, float x, float y, VGDevice & hdc ) const;
		float		DrawNumericSingle(VGDevice & hdc, const std::string& num, const std::string& dnum, float x ) const;
		void		DrawNumericSeveral(VGDevice & hdc ) const;
		std::string	makeNumeratorString (const std::vector<Fraction>& meters) const;
		std::string	makeDenominatorString (const std::vector<Fraction>& meters) const;
		NVRect		computeBoundingBox (VGDevice* hdc) const;
		NVRect		computeBoundingBox (VGDevice* hdc, const std::string& numStr) const;
		NVRect		computeBoundingBox (VGDevice* hdc, const std::string& numStr, const std::string& dnumStr) const;
};


#endif

