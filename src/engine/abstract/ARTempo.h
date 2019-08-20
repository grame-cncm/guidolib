#ifndef ARTempo_H
#define ARTempo_H

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
#include "ARFontAble.h"
#include "FormatStringParser.h"

/*@mkdoc

@group:Tempo

@tagname:\tempo
@tagalias:
@tagtype:P
@tagnotation:a tempo mark
@tagdesc
@tagend

@params:
@param:tempo:string:a tempo string:*none*:false
@param:bpm:string::*none*:true
@fontparams:
@paramdesc
- **tempo** is an arbitrary string that may contain a marker for note duration in the form "[n/d]" where 'n' and 'd' are integers.
The corresponding mark is decoded as a note duration and replaced with the corresponding note symbol. <br/>Example: "Andante [1/4] = 80"

**Note**: [font parameters](../../tagsparams/#text-parameters) are supported but not horizontal alignment (see *textformat*)

See the [Tempo](@EXAMPLES/tempo/) example. <br />
See the [Lutkin](@EXAMPLES/lutkin/) example.
@paramend

*/

/** \brief The tempo tag parameter.

	 new specs (2004)

	\\tempo<s1> indicates a tempo marking

	\\tempo<s1, s2> indicates a tempo marking s1 and associates 
			it with an absolute tempo as specified by string s2.

		s1 (tempo):	graphical information.
		s2 (bpm):	musical intormation (optional)


	\\tempo
	- tempo: support [x/y]=i, [x1/y1]=[x2/y2] in marking strings
			([...] indicates notation as graphical durations;
			use \[ and \] for [,] characters)
	- bpm: allow "x/y=i", "x1/y1=x2/y2"
	- dy: top edge of top staffline, center of bounding box


	example:
	\\tempo<"Moderato [1/4]=120">

*/
class ARTempo : /* public ARMTParameter, */ public ARFontAble
{
	public:
				 ARTempo();
		virtual ~ARTempo() {}

		virtual const char*	getParamsStr() const	{ return kARTempoParams; };
		virtual const char*	getTagName() const		{ return "ARTempo"; };
		virtual std::string getGMNName() const		{ return "\\tempo"; };

		virtual void setTagParameters(const TagParameterMap& map);

		//! Gives the tempo information strings vector.
		const FormatStringParserResult& getTempoMark() const { return mTempoMark; }		

		//! Converts a string in the form "a/b" into a duration
		TYPE_DURATION getDuration (const char * str) const;
	
		//! Tells if the optional bpm informations have been specified.
		bool hasBpmInfos() const { return mHasBpmInfos; }

		/** Tells if bpm is in "note equivalent" format.
		\return true when we have a/b=c/d, false for a/b=c.
		*/
		bool isNoteEquiv() const { return mBpmNoteEquiv; }


		/** Gives the unit of the bpm tempo.

		example: for 1/4=120, 1/4 is the unit.
		\return a musical duration, (1/4 is a quarter note)
		*/
		TYPE_DURATION getBpmUnit() const { return mBpmUnit; }	


		/** Gives the value of the bpm tempo.

		example: for 1/4=120, 120 is the value.
		\return a tempo as bpm or as a note equivalent.
		*/
		TYPE_DURATION getBpmValue() const { return mBpmValue; }	

		/** Gives the value of the tempo in quarters per minute.
		*/
		float getQpmValue() const;	

		virtual void browse(TimeUnwrap& mapper) const;
		virtual ARTempo	*isARTempo()		  { return this; }

	private:
		void		ParseBpm(const TagParameterString * inTag );

		FormatStringParserResult	mTempoMark;

		TYPE_DURATION	mBpmUnit;			// bpm="mBpmUnit=mBpmValue">
		TYPE_DURATION	mBpmValue;
		bool			mBpmNoteEquiv;	// true if bpm value is a note equivalent.
		bool			mHasBpmInfos;
};


#endif


