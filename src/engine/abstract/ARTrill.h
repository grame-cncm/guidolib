#ifndef ARTrill_H
#define ARTrill_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "ARMusicalVoice.h"

class ARKey;

#define kNoAccidental -9999.f

/** \brief not yet documented
*/
class ARTrill : public ARMTParameter, public ARPositionTag
{		
	public:
			enum ORNAMENT { TRILL,TURN,MORD };
			enum TYPE { kPrall, kInverted, kPrallPrall, kPrallInverted };
			enum POS  { BELOW, /*ONNOTE,*/ ABOVE, AUTO };
			enum	{ kUndefined, kOn, kOff };

						 ARTrill(ORNAMENT typ, const ARKey* currentKey);
						 ARTrill(int pid, const ARTrill * copy);
		virtual		 	~ARTrill() {}

		virtual void	setTagParameters (const TagParameterMap& params);
	  	virtual bool	MatchEndTag(const char * s);
		virtual ARMusicalObject * Copy() const;

		virtual const char*	getParamsStr() const	{ return kARTrillParams; };
		virtual const char*	getTagName() const		{ return "ARTrill"; };
		virtual std::string getGMNName() const;

//				void 	setType(ORNAMENT typ)		{ fTrillType = typ; }
				void 	setTrillPos(POS pos) 		{ fTrillPosition = pos; }
//				void	setChordType(ARMusicalVoice::CHORD_TYPE param_type) { fChordType = param_type;}
//				void	setChordAccidental(ARMusicalVoice::CHORD_ACCIDENTAL param_accidental)	{ fChordAccidental = param_accidental;}
				void	setCautionary(bool showAcc)			{ fCautionaryAccidental = showAcc;}

//				ARMusicalVoice::CHORD_TYPE	getChordType() const				{ return fChordType;}
//				ARMusicalVoice::CHORD_ACCIDENTAL	getChordAccidental() const	{ return fChordAccidental; }
				ORNAMENT 	getOrnament() const		{ return fTrillType; }
				TYPE 	getType() const				{ return fType; }
				float 	getAccidental() const		{ return fAccidental; }
				bool	getCautionary()	const		{ return fCautionaryAccidental;}
				float	getadx() const				{ return fAdx; }
				float	getady() const				{ return fAdy;}
				bool	getStatus() const			{ return fBegin; }
				bool	getShowTR() const			{ return fShowTR;}
//				bool	getAnchor()	const			{ return fDrawOnNoteHead;}
				POS		getTrillPos()const			{ return fTrillPosition; }
				int		getRepeat()	const			{ return fRepeat;}
				const std::string&	getTrilledNote()	const	{ return fTrilledNote;}

				void	setContinue()				{ fBegin = false; }

	protected:
		float getAccidental (const std::string& note, const ARKey* key) const;
		ARMusicalVoice::CHORD_TYPE		 fChordType;
//		ARMusicalVoice::CHORD_ACCIDENTAL fChordAccidental;
		float	fAccidental = 0.f;		// the accidental value to be displayed expressed as a float to cope with microtones
										// 1 is a half tone
		const ARKey* fCurrentKey;
		std::string	fTrilledNote;
		ORNAMENT	fTrillType;
		TYPE	fType;
		bool	fCautionaryAccidental;
		bool	fForceAccidental;
		bool	fShowTR;
		POS		fTrillPosition;
//		bool	fDrawOnNoteHead;
		int		fRepeat;
		float	fAdx;
		float	fAdy;
		bool	fBegin;	
};

#endif
