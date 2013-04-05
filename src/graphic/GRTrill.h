#ifndef GRTrill_H
#define GRTrill_H

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

#include "GRPTagARNotationElement.h"
#include "ARTrill.h"
class ARTrill;
class GRStaff;
class GREvent;
class GRAccidental;



/** \brief GRTrill tag
*/


class GRTrill : public GRPTagARNotationElement
{
	public:
					 GRTrill( GRStaff * stf, ARTrill * arTrill );
		virtual		~GRTrill();

		virtual void OnDraw( VGDevice & hdc) const;
		virtual void OnDraw( VGDevice & hdc, float pos, int numVoice);
		virtual void print() const {};
		virtual void tellPosition( GObject * caller, const NVPoint & np );

		virtual unsigned int getTextAlign() const;
		virtual const NVPoint & getReferencePosition() const;

		
	protected:
		void placeMord(GREvent * inParents, NVPoint& newPoint);
		
		void setupTrill();
		void setupTurn();
		void setupInvertedTurn();
		void setupTurnComplex();
		void setupInvertedTurnComplex();
		void setupMord();
		void setupInvertedMord();
		void setupPrallMordent();
		void setupPrallPrall();

		
		void setTrillSymbol( unsigned int inSymbolID);

		
		GDirection chooseDirection (GREvent * inParent ) const;
		
		GRAccidental *	fAccidental;		
		ARTrill::TYPE	fType;
		NVPoint			sRefPos;
		bool			begin;
		
		static float &	getLastPosX(int i);
};
#endif
