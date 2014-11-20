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
// GRDummy.cpp: implementation of class GRDummy.
//
//////////////////////////////////////////////////////////////////////

#include "GRDummy.h"
#include "GRPositionTag.h"
#include "GRDefine.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////


GRDummy::GRDummy( GRPositionTag * p_myptag )
{
	mNeedsSpring = 0;
	myptag = p_myptag;
}

GRDummy::~GRDummy()
{
}

void GRDummy::print(std::ostream& os) const
{
}

void GRDummy::OnDraw( VGDevice & hdc) const
{
	if (myptag && mDraw)
	{	
		GObject * myobj = dynamic_cast<GObject *>(myptag);
		if (myobj)
			myobj->OnDraw(hdc);
	}
}
void GRDummy::tellPosition(GObject *caller,const NVPoint &newPosition)
{
	if (myptag)
	{	
		GObject * myobj = dynamic_cast<GObject *>(myptag);
		if (myobj)
			myobj->tellPosition(caller,newPosition);
	}
}
