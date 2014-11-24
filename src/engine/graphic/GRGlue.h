#ifndef GRGlue_H
#define GRGlue_H

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

#include "GRNotationElement.h"

class GRStaff;


/** \brief not yet documented
*/
class GRGlue : public GRNotationElement  
{
	public:
						GRGlue( GRStaff * grstf, int isstart = true );
		virtual 		~GRGlue();
		
		virtual void 	OnDraw(VGDevice &) const	{ }
		
				void 	tellPosition( GObject *, const  NVPoint & );
	
				float	getSConst() const	{ return mSConst; }
		
		virtual void 	setHPosition( float nx )	{ GRNotationElement::setHPosition(nx); }

		GRGlue * mPartner;
		int		 mIsStart;	// (JB) sometimes = -1 should be a bool...

	protected:

		float mSConst;
};

#endif
