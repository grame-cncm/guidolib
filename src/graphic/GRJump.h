#ifndef __GRJump__
#define __GRJump__

/*
	GUIDO Library
	Copyright (C) 2004	Grame

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
