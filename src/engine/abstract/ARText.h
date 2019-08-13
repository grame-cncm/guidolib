#ifndef ARText_H
#define ARText_H

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

#include <string>

#include "ARPositionTag.h"
#include "ARFontAble.h"

class ARLyrics;

/*@mkdoc

@group:Text

@tagname:\text
@tagalias:\label <br/> \t
@tagtype:RP
@tagnotation:textual elements
@tagdesc
The **\text** tag is both a range and a position tag. When a range is specified, the text doesn't take place: it is placed over the range.
With no range, the text is inserted at its position.
@tagend

@params:
@param:text:string:the text:*none*:false
@fontparams:
@paramdesc
See the [Notes](@EXAMPLES/notes/) example.
@paramend

*/

/** \brief The text tag
*/
class ARText : public ARFontAble, public ARPositionTag
{
public:
			            ARText(const std::string& p_txt, float p_offsety, bool isLyric=false);
					    ARText();
	virtual 	       ~ARText() {}

	virtual void		setTagParameters (const TagParameterMap& params);
	virtual const char* getText() const					{ return fText.c_str(); }
	virtual void		copyLyricsParams(const ARLyrics * lyrics);

	virtual const char*	getParamsStr() const	{ return kARTextParams; };
	virtual const char*	getTagName () const		{ return "ARText"; };
	virtual std::string getGMNName () const		{ return "\\text"; };
	
	void		 setAutoPos(bool state)						{ fIsAutoPos = state; }
	bool		 isLyric() const							{ return fIsLyric; }
	bool		 isAutoPos() const							{ return fIsAutoPos; }

protected:
	std::string	fText;		// the corresponding text ...
	bool	fIsLyric;
	bool	fIsAutoPos;
};

#endif
