#ifndef GRTrill_H
#define GRTrill_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
		virtual void OnDraw( VGDevice & hdc, float pos, float noteY, int numVoice);
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
		bool			fShowTR;
		bool			fDrawOnNoteHead;
        float			widthOfTilde;
		
		static float &	getLastPosX(int i);
};
#endif
