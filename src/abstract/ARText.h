#ifndef ARText_H
#define ARText_H

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
	virtual 		~ARText();

	virtual void 	setTagParameterList(TagParameterList & theTagParameterList);
	
	virtual const char* getText() const					{ return text ? text->getValue() : 0; }
	virtual void		copyLyricsParams(const ARLyrics * lyrics);
	
	virtual void print() const;
	// virtual ostream & operator<<(ostream &os) const;
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	
	virtual int MatchListOfTPLsWithTPL(const ListOfTPLs & ltpls, TagParameterList & tpl, TagParameterList ** rtpl);
	virtual void  CreateListOfTPLs( ListOfTPLs & ltpl, ListOfStrings & lstrs);

	const char* getTextformat() const					{ return textformat ? textformat->getValue() : 0; } 
	void		setTextformat(const TagParameterString * tf);
	const char* getFont() const							{ return font ? font->getValue() : 0; }
	void		setFont(const TagParameterString * ft);
	const char*	getFAttrib() const						{ return fattrib ? fattrib->getValue() : 0; }
	void		setFAttrib(const TagParameterString * fa);
	int			getFSize(float curLSPACE) const;
	void		setFSize(const TagParameterFloat * fs);

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
