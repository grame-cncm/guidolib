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

#include <vector>

#include "GRPTagARNotationElement.h"
#include "ARTrill.h"

class GRStaff;
class GREvent;
class GRAccidental;
class GRSingleNote;
class GRSystem;
class GRVisitor;


/** \brief GRTrill tag
*/
class GRTrill : public GRPTagARNotationElement
{
	private:
		GRNotationElement* fNextEvent = 0;

//		GRAccidental * createAccidental (ARMusicalVoice::CHORD_ACCIDENTAL acc, bool cautionary) const;
		GRAccidental * createAccidental (const ARTrill * ar) const;

	public:
					 GRTrill( GRStaff * stf, const ARTrill * arTrill );
		virtual		~GRTrill();

		virtual void accept   (GRVisitor& visitor);
		virtual void OnDraw( VGDevice & hdc) const;
		virtual void OnDraw( VGDevice & hdc, float pos, float noteY, int numVoice) const;
//        virtual void tellNoteX( const GRSingleNote * note, float x );
//        virtual void tellNoteY( const GRSingleNote * note, float y );
        virtual void tellPosition( GObject * caller, const NVPoint & np );
	  	NVRect		 getEnclosingBox() const;		// gives a rect that enclose the ornament including accidentals

		virtual unsigned int getTextAlign() const;
		virtual const NVPoint & getReferencePosition() const;
	
		void setNextEvent(GRNotationElement* ev)			{ fNextEvent = ev; }
		void setStartOffset (float offset)					{ fStartOffset = offset; }
		bool getBegin() const								{ return fBegin; }
		virtual const GRTrill *		isGRTrill() const		{ return this; }

		ARTrill::POS getTrillPos() const					{ return fPosition; }

		
	protected:
		void placeMord(GREvent * inParents, NVPoint& newPoint);
		
		void setupTrill();
		void setupTurn();
		void setupInvertedTurn(bool bar);
//		void setupTurnComplex();
//		void setupInvertedTurnComplex();
		void setupMord();
		void setupInvertedMord();
		void setupPrallMordent();
		void setupPrallPrall();

		
		void setTrillSymbol( unsigned int inSymbolID);

		GDirection chooseDirection (GREvent * inParent ) const;

		GRAccidental *	fAccidental;		
		ARTrill::ORNAMENT fType;
		NVPoint			sRefPos;
		bool			fBegin;
		bool			fShowTR;
		bool			fShowWavy;
        float			fTildeWidth;
		float			fStartOffset = 0;
		ARTrill::POS	fPosition;
	
	private:
		typedef std::vector<std::pair<const GRSystem*, NVRect> > TDrawRects;
		TDrawRects	getAllAssociatedBoundingBox (const NEPointerList * assoc, const GRStaff* staff) const;
		NVRect		getCurrentRect (const GRSystem* sys) const;

		TDrawRects	fDrawRects;

	static float &	getLastPosX(int i);
};
#endif
