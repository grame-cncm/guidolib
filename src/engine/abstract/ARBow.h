
#pragma once

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

#include "ARArticulation.h"

/*@mkdoc

@group:Articulations

@tagname:\bow
@tagalias:
@tagtype:RP
@tagnotation:bowing indication
@tagdesc
Strings specific tag.
@tagend

@params:
@param:type:string:up or down:up:false
@param:position:string:above or below:above:true
@paramdesc

@paramend

*/

/** \brief String specific: bowing
*/
class ARBow : public ARArticulation
{		
	public:			
				 ARBow();
		virtual ~ARBow() {}

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARBowParams; };
		virtual const char*	getTagName() const		{ return "ARBow"; };
		virtual std::string getGMNName() const		{ return "\\bow"; };	
		virtual unsigned int getSymbol() const;

		bool	  up() const						{ return fUp; }

	protected:
		bool		fUp = true;
};
