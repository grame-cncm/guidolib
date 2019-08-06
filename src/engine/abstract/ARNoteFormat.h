#ifndef ARNoteFormat_H
#define ARNoteFormat_H

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

/*@mkdoc

@group:Notes

@tagname:\noteFormat
@tagalias:
@tagtype:RP
@tagnotation:control the notes format
@tagdesc
The **\noteFormat** tag supports both a range and a position form.
For the latter, the format is applied up to the next format specification or to the end of the voice.

**\noteFormat** is a way to introduce [common parameters](/refs/tagsparams/#common-parameters) to notes.
@tagend

@params:
@param:style:string:the notehead style (see below):standard:true
@paramdesc
The notehead style should be among "diamond", "x", "square", "round", "triangle" and "reversedTriangle".

In addition, each style supports 3 types of variations that consist in enclosing the string into (), <> or [].<br />
For standard notehead, these variations are simply the  "()", "<>" or "[]" strings.
@paramend

*/

/** \brief not yet documented
*/
class ARNoteFormat :  public ARMTParameter //, public ARPositionTag
{
	public:
				 ARNoteFormat(const ARNoteFormat * p_savenf = NULL, const ARNoteFormat * copynf = NULL);
		virtual ~ARNoteFormat() {}

		const TagParameterString *	getTPStyle() const; // { return style; }

		virtual const char*	getParamsStr() const	{ return kARNoteFormatParams; };
		virtual const char*	getTagName() const		{ return "ARNoteFormat"; };
		virtual std::string getGMNName() const		{ return "\\noteFormat"; };

		virtual ARNoteFormat * getEndTag() const	{ return new ARNoteFormat(NULL,fSaveNF); }

		virtual bool IsStateTag() const					{ return true; }
		const ARMusicalObject *isARNoteFormat() const	{ return this; }

	private:
		const ARNoteFormat * fSaveNF;
};

#endif 
