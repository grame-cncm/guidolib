#ifndef ARLyrics_H
#define ARLyrics_H

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

#include "ARPositionTag.h"
#include "ARFontAble.h"

class ARText;

/*@mkdoc

@group:Text

@tagname:\lyrics
@tagalias:
@tagtype:R
@tagnotation:lyrics
@tagdesc
@tagend

@params:
@param:text:string:the lyrics:*none*:false
@param:autopos:string::off:true
@fontparams:
@paramdes:
The lyrics are mapped onto the following notes and rests according to the following rules:
- a " " (space) after a word (or syllable) progresses to the following event.
- a "~" (tilde) between characters specifies a blank within the lyrics. There is no progression to the following event. The tilde is used to place more than one word on a single note.
- a "-" (hyphen) progresses to the following event. The hyphen is also printed in the score.
- a "_" (underscore) progresses to the following event. The underscore is also printed in the score.
- a "<n>" progresses to the event that is marked with the \mark<n>-tag. If the <n> is proceeded by a hyphen ("-"), a series of hyphens is printed from the current event to the target event.

When **autopos** is on, the system moves the lyrics in case of collision.
@paramend

*/

/** \brief Implement the lyrics Tag
*/
class ARLyrics : public ARFontAble, public ARPositionTag
{
	public:

		friend class ARText;
	
					 ARLyrics(bool autopos=false);
		virtual		~ARLyrics() {}

		virtual void setTagParameters (const TagParameterMap& params);
	
		virtual const char* getText() const			{ return fText.c_str(); }
		virtual const char*	getParamsStr() const	{ return kARLyricsParams; };
		virtual const char*	getTagName() const		{ return "ARLyrics"; };
		virtual std::string getGMNName() const		{ return "\\lyrics"; };
		bool		autoPos() const					{ return fAutoPos; }

	protected:
		std::string	fText;		// the corresponding text ...
		bool		fAutoPos;
};

#endif
