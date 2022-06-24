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


/*@mkdoc

@group:Ornaments

@tagname:\mordent
@tagalias:\mord
@tagtype:R
@tagnotation:a mordent
@tagdesc
@tagend

@params:
@param:note:string:a note, part of the ornament:*none*:true
@param:type:string:prall, prallprall, inverted or prallinverted:prall:true
@param:detune:float:note tuning (for microtonality):0.0:true
@param:accidental:string:cautionary or force:*none*:true
@param:adx:unit:accidental displacement (if any):0hs:true
@param:ady:unit:accidental displacement (if any):0hs:true
@param:position:string:above, below:above:true
@paramdesc
- **note** is intended to indicate accidentals, the note itself is not displayed but the possible accidental does.
- **detune** is applied to the **note** parameter and works similarly as [\alter](../Accidentals#alter)
- **accidental** is intended is intended to control how the accidental is displayed

See the [Ornaments](@EXAMPLES/ornaments/) example.<br />
See the [Bach](@EXAMPLES/bach/) example.<br />
See the [Sonata](@EXAMPLES/cpebach/) example.
@paramend


@tagname:\trill
@tagalias:\trillBegin \trillEnd
@tagtype:R
@tagnotation:a trill ornament
@tagdesc
@tagend

@params:
@param:note:string:a note, part of the ornament:*none*:true
@param:detune:float:note tuning (for microtonality):0.0:true
@param:accidental:string:cautionary or force:*none*:true
@param:adx:unit:accidental displacement (if any):0hs:true
@param:ady:unit:accidental displacement (if any):0hs:true
@param:begin:boolean:for trill continuations:on:true
@param:tr:boolean:show/hide the leading "tr" string:true:true
@param:wavy:boolean:show/hide the wavy line:true:true
@param:position:string:above, below:above:true
@paramdesc
- **begin** is intended for trill continutation. When false, the trill continues the preceding one (if any). It allows to write new trills with changing accidentals without breaking it.
- **tr** show or hide the leading "tr" string

other parameters have the same effect as for [\mordent](#mordent)

See the [Ornaments](@EXAMPLES/ornaments/) example.<br />
See the [Sonata](@EXAMPLES/cpebach/) example.
@paramend



@tagname:\turn
@tagalias:
@tagtype:R
@tagnotation:a turn ornament
@tagdesc
@tagend

@params:
@param:note:string:a note, part of the ornament:*none*:true
@param:type:string:inverted, invertedb or regular:regular:true
@param:detune:float:note tuning (for microtonality):0.0:true
@param:accidental:string:cautionary or force:*none*:true
@param:adx:unit:accidental displacement (if any):0hs:true
@param:ady:unit:accidental displacement (if any):0hs:true
@param:position:string:above, below:above:true
@paramdesc
- **invertedb** displays as a regular turn with a bar.

The parameters have the same effect as for [\mordent](#mordent)

See the [Ornaments](@EXAMPLES/ornaments/) example.<br />
See the [Sonata](@EXAMPLES/cpebach/) example.
@paramend


*/

/** \brief not yet documented
*/
class ARTrill : public ARMTParameter, public ARPositionTag
{		
	public:
			enum ORNAMENT { TRILL,TURN,MORD };
			enum TYPE { kPrall, kInverted, kInvertedBar, kPrallPrall, kPrallInverted };
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

				void 	setTrillPos(POS pos) 		{ fTrillPosition = pos; }
				void	setCautionary(bool showAcc)			{ fCautionaryAccidental = showAcc;}

				ORNAMENT 	getOrnament() const		{ return fTrillType; }
				TYPE 	getType() const				{ return fType; }
				float 	getAccidental() const		{ return fAccidental; }
				bool	getCautionary()	const		{ return fCautionaryAccidental;}
				float	getadx() const				{ return fAdx; }
				float	getady() const				{ return fAdy;}
				bool	getStatus() const			{ return fBegin; }
				bool	getShowTR() const			{ return fShowTR;}
				bool	getShowWavyLine() const		{ return fShowWavyLine;}
				POS		getTrillPos()const			{ return fTrillPosition; }
				const std::string&	getTrilledNote()	const	{ return fTrilledNote;}

				void	setContinue()				{ fBegin = false; }

	protected:
		float getAccidental (const std::string& note, const ARKey* key) const;
		ARMusicalVoice::CHORD_TYPE		 fChordType;
		float	fAccidental = 0.f;		// the accidental value to be displayed expressed as a float to cope with microtones
										// 1 is a half tone
		const ARKey* fCurrentKey;
		std::string	fTrilledNote;
		ORNAMENT	fTrillType;
		TYPE	fType;
		bool	fCautionaryAccidental;
		bool	fForceAccidental;
		bool	fShowTR;
		bool 	fShowWavyLine;
		POS		fTrillPosition;
		float	fAdx;
		float	fAdy;
		bool	fBegin;	
};

#endif
