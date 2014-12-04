#ifndef GRIntens_H
#define GRIntens_H

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

#include "GRTagARNotationElement.h"
#include "MusicalSymbols.h"

class ARIntens;
class GRStaff;
class VGDevice;


/** \brief not yet documented
*/
class GRIntens : public GRTagARNotationElement
{
	
public:
	GRIntens( GRStaff * inStaff, ARIntens * abstractRepresentationOfIntens );

	enum 
	{ 
		INTENS_F = kIntensFSymbol, // SCR_INTENSF,
		INTENS_FF = kIntensFFSymbol, // SCR_INTENSFF,
		INTENS_FFF = kIntensFFFSymbol, // SCR_INTENSFFF,
		INTENS_FFFF = kIntensFFFFSymbol, // SCR_INTENSFFFF,
		INTENS_P = kIntensPSymbol, // SCR_INTENSP,
		INTENS_PP = kIntensPPSymbol, // SCR_INTENSPP,
		INTENS_PPP = kIntensPPPSymbol,
		INTENS_PPPP = kIntensPPPPSymbol,

		INTENS_MF = kIntensMFSymbol, // SCR_INTENS_MF,
		INTENS_MP = kIntensMPSymbol, // SCR_INTENS_MP,
		INTENS_SF = kIntensSFSymbol  // SCR_INTENS_SF
	};

	virtual ~GRIntens();
	
	
	virtual void OnDraw(VGDevice & hdc ) const;
	
		
	const ARIntens* getARIntens() const;
	
protected:

	int rcount;
};

#endif


