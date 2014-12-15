#ifndef ARTrill_H
#define ARTrill_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "ARMusicalVoice.h"

class TagParameterInt;
class TagParameterFloat;


/** \brief not yet documented
*/
class ARTrill : public ARMTParameter, public ARPositionTag
{		
	public:
			enum TYPE { TRILL,TURN,MORD };

						ARTrill(TYPE typ);
						ARTrill(int pid, const ARTrill * copy);
						
		virtual		 	~ARTrill();

		virtual void 	setTagParameterList(TagParameterList & tpl);

	    virtual void    printName(std::ostream& os) const;
	    virtual void    printGMNName(std::ostream& os) const;
	    virtual void    printParameters(std::ostream& os) const;

				TYPE 	getType() const 		{ return mTrillType; }
				void 	setType(TYPE typ) 		{ mTrillType = typ; }
				void	setChordType(ARMusicalVoice::CHORD_TYPE param_type) {chordType = param_type;}
				ARMusicalVoice::CHORD_TYPE	getChordType() const	{return chordType;}
				void	setChordAccidental(ARMusicalVoice::CHORD_ACCIDENTAL param_accidental) {chordAccidental = param_accidental;}
				ARMusicalVoice::CHORD_ACCIDENTAL	getChordAccidental() const	{return chordAccidental;}
				bool	getCautionary()	const				{ return fShowCautionaryAccidentals;}
				void	setCautionary(bool showAcc)			{ fShowCautionaryAccidentals = showAcc;}
				float	getadx() const;
				float	getady() const;
				bool	getStatus() const;
				void	setContinue();
				bool	getShowTR() {return fShowTR;}
				bool	getAnchor() {return fDrawOnNoteHead;}

	protected:
		TagParameterInt	  * mDur;
		TYPE 				mTrillType;
		ARMusicalVoice::CHORD_TYPE			chordType;
		ARMusicalVoice::CHORD_ACCIDENTAL	chordAccidental;
		bool fShowCautionaryAccidentals;
		bool fShowTR;
		bool fDrawOnNoteHead;
		float adx;
		float ady;
		bool  begin;

		static ListOfTPLs ltpls;
};

#endif
