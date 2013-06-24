#ifndef __GRJump__
#define __GRJump__

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <map>
#include <string>

#include "GRTagARNotationElement.h"
//#include "GDeviceDefs.h"

class ARJump;
class  VGFont;

typedef std::map<std::string, unsigned int> SymbolMap;

/** \brief common part for coda, segno, da capo etc...
*/
class GRJump : public GRTagARNotationElement
{
	public:
						GRJump( ARJump * ar, bool rightJustified=true );
		virtual			~GRJump();
		
		virtual const ARJump * getARJump() const;

		virtual void	OnDraw( VGDevice & hdc ) const;
		virtual const	NVPoint & getReferencePosition() const  { return refpos; }

	protected:
		enum { kTextFontSize = 110 };
		
		virtual float	getSymbolSize () const;
		virtual const VGFont* SelectTextFont( VGDevice & hdc ) const;
		
		bool	mRightJustified;
		
		static const VGFont* mFont;
		static NVPoint		refpos;
		static SymbolMap	mSymbols;
};

#endif
