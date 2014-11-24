#ifndef GRAccelerando_H
#define GRAccelerando_H

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

#include "GRPTagARNotationElement.h"


class ARAccelerando;
class GRStaff;


/** \brief The Accelerando notation element
*/
class GRAccelerando : public GRPTagARNotationElement
{
	public:
						GRAccelerando( GRStaff * stf, ARAccelerando * artrem );
		virtual			~GRAccelerando();

		virtual unsigned int getTextAlign() const;
		virtual void	OnDraw( VGDevice & hdc) const;

	    
		virtual void	tellPosition( GObject * caller,const NVPoint & np );

	private:
		bool isTempoAbsSet;
		bool isTempoSet;
		NVPoint startPos;
		NVPoint endPos;
		std::string tempo1;
		std::string tempo2;
		float mdx;
		float mdy;
};

#endif
