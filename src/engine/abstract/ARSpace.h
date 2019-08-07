#ifndef ARSpace_H
#define ARSpace_H

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

/*@mkdoc

@group:Miscellaneous

@tagname:\space
@tagalias:
@tagtype:P
@tagnotation:insert space at any position.
@tagdesc
The **\space** tag can be used to save space for notation elements that do not take up space.
@tagend

@params:
@param:dd:unit:the amount of space:0:false
@paramdesc
See the [Space](@EXAMPLES/space/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARSpace :  public ARMTParameter 
{
	public:
				 ARSpace();
				 ARSpace(float val) : fVal(val) {}
		virtual ~ARSpace() {};

		virtual int	 getOrder() const				{ return kDontMove; }
		virtual const char*	getParamsStr() const	{ return kARSpaceParams; };
		virtual const char*	getTagName() const		{ return "ARSpace"; };
		virtual std::string getGMNName() const		{ return "\\space"; };

		virtual void setTagParameters (const TagParameterMap& params);
		virtual float getValue() const 				{ return fVal; }

		virtual ARSpace*	isARSpace()				{ return this; }

	private:
		float fVal;
};

#endif

