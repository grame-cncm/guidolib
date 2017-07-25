#ifndef ARDynamic_H
#define ARDynamic_H

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


/** \brief the base class for ARCrescendo and ARDiminuendo
*/
class ARDynamic : public ARMTParameter, public ARPositionTag
{
	public:
					 ARDynamic();
					 ARDynamic(const ARDynamic* dynamic);
		virtual		~ARDynamic() {}

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARDynamicParams; };
		virtual const char*	getTagName() const		{ return "ARDynamic"; };

		float     getDx1()				const { return fDx1; }
		float     getDx2()				const { return fDx2; }
		float     getDy(float curlspace) const;
		float     getDeltaY()			const { return fDeltaY; }
		float     getThickness()		const { return fThickness; }
		bool	  autoPos()				const { return fAutoPos; }

	private:
		float    fDx1;
		float    fDx2;
		float    fDeltaY;
		float    fThickness;
		bool	 fAutoPos;
};

#endif
