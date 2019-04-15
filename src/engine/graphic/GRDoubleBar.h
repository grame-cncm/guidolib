#ifndef GRDoubleBar_H
#define GRDoubleBar_H

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

#include "GRBar.h"

class GRStaff;
class ARDoubleBar;


/** \brief The double barline.
*/
class GRDoubleBar : public GRBar  
{
public:
	GRDoubleBar(const ARDoubleBar * ar, GRStaff * staff, const TYPE_TIMEPOSITION & date, float propRender);
	GRDoubleBar(const ARDoubleBar * ar, GRSystem * sys, GRStaff * inStaff, const TYPE_TIMEPOSITION & date, float propRender);
    virtual			~GRDoubleBar() {}

    const ARDoubleBar * getARDoubleBar() const;
    virtual void DrawWithLines( VGDevice & hdc ) const;

protected:
    float fStaffThickness;
};

#endif
