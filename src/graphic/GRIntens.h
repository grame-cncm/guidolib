#ifndef GRIntens_H
#define GRIntens_H

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
	
	virtual void print() const;
	
	const ARIntens* getARIntens() const;
	
protected:

	int rcount;
};

#endif


