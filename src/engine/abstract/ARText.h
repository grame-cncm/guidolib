#ifndef ARText_H
#define ARText_H

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
#include "TagParameterString.h"


class ARLyrics;
class TagParameterFloat;

/** \brief The text tag
*/
class ARText : public ARMTParameter, 
					public ARPositionTag
{
public:
			            ARText(const NVstring & p_txt, float p_offsety);
					    ARText();
	virtual 	       ~ARText();

	virtual void 	    setTagParameterList(TagParameterList & theTagParameterList);
	
	virtual const char* getText() const					{ return text ? text->getValue() : 0; }
	virtual void		copyLyricsParams(const ARLyrics * lyrics);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;
	
	virtual int  MatchListOfTPLsWithTPL(const ListOfTPLs & ltpls, TagParameterList & tpl, TagParameterList ** rtpl);
	virtual void CreateListOfTPLs( ListOfTPLs & ltpl, ListOfStrings & lstrs);

	const char*  getTextformat() const					    { return textformat ? textformat->getValue() : 0; } 
	void		 setTextformat(const TagParameterString * tf);
	const char*  getFont() const							{ return font ? font->getValue() : 0; }
	void		 setFont(const TagParameterString * ft);
	const char*	 getFAttrib() const						    { return fattrib ? fattrib->getValue() : 0; }
	void		 setFAttrib(const TagParameterString * fa);
	int			 getFSize(float curLSPACE) const;
	void		 setFSize(const TagParameterFloat * fs);

	const TagParameterFloat * getYPos() const {	return ypos; }

protected:
	virtual const char * getTagFormat() const;	 // (JB) new

	TagParameterString * text; // der entsprechende Text ...
	TagParameterString * textformat;
	TagParameterString * font;
	TagParameterFloat  * fsize;
	TagParameterString * fattrib;

	TagParameterFloat * ypos;

	static ListOfTPLs ltpls;
};

#endif
