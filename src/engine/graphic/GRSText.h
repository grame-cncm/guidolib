#ifndef GRSText_H
#define GRSText_H

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

#include "TagParameterString.h"
#include "GRNotationElement.h"
#include "GRTag.h"
// #include "GRDefine.h"

class NVstring;
class GRStaff;
class NVPoint;
class VGDevice;

/** \brief not yet documented
*/
class GRSText : public GRNotationElement, public GRTag
{

  public:
					 GRSText( GRStaff *, const NVstring & txt, int p_offsety );
		virtual 	~GRSText();

	 	virtual void tellPosition(GObject *caller, const NVPoint & );
	  	virtual void addAssociation(GRNotationElement *el);
		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;
        virtual void setColRef(const TagParameterString *tps);
			
		
  protected:
  	
  	GRStaff * grstaff;
    NVstring * txt;
	GRNotationElement * velement;
	int  offsety;
};

#endif

