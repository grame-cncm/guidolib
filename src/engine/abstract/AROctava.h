#ifndef AROctava_H
#define AROctava_H

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

class TagParameterInt;
class TagParameterString;

/*@mkdoc

@group:Notes

@tagname:\octava
@tagalias:\oct
@tagtype:RP
@tagnotation:displays an octava sign and transposes the notes accordingly
@tagdesc
@tagend

@params:
@param:i:integer:a signed number of octaves (up or down):*none*:false
@param:hidden:bool:hides the octava sign:false:true
@paramdesc
@paramend

*/

/** \brief The Octava tag
*/
class AROctava : public ARMTParameter, public ARPositionTag
{
	const TagParameterInt * fOctava = nullptr;
	bool 					fHidden = false;
	TYPE_TIMEPOSITION		fEnd = 0;

	public:
					 AROctava(const AROctava * p_saveoct = NULL, const AROctava * copyoct = NULL);
		virtual		~AROctava() {}

		virtual bool IsStateTag() const				{ return true; }

		virtual ARMusicalObject * getEndTag() const { return new AROctava(NULL, fSaveoct); }

		virtual void 	setTagParameters(const TagParameterMap & params);
		virtual int 	getOctava() const;
		virtual bool 	getHidden() const	{ return fHidden; }

		virtual const char*	getParamsStr() const	{ return kAROctavaParams; };
		virtual const char*	getTagName() const		{ return "AROctava"; };
		virtual std::string getGMNName() const		{ return "\\octava"; };

		virtual AROctava*	isAROctava()		  { return this; }
		
		void 				setEnd (TYPE_TIMEPOSITION pos) 		{ fEnd = pos; }
		TYPE_TIMEPOSITION 	getEnd () const 					{ return fEnd; }

	private:
		const AROctava *			fSaveoct; // this is required to save the current octava state ...
};

#endif
 
