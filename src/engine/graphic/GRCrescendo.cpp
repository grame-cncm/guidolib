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

//#include <iostream>
//#include <string.h>
//
//#include "ARCrescendo.h"
//#include "VGDevice.h"
//#include "VGFont.h"
//#include "GRStaff.h"
//#include "TagParameterFloat.h"
//#include "GRStdNoteHead.h"
//#include "GRSingleNote.h"
//#include "GRRest.h"
//#include "GREmpty.h"
//#include "MusicalSymbols.h"
//#include "GRSystem.h"


#include "GRCrescendo.h"

//using namespace std;

//extern GRSystem *gCurSystem;

//---------------------------------------------------------------------------------
//void GRCrescendo::addAssociation(GRNotationElement * grnot)
//{
//	if (error) return;
//
//	if ( GREvent::cast(grnot)  && 	// stop immediately if it's not an event.
//		(dynamic_cast<GRNote *>(grnot) ||
//		 dynamic_cast<GRRest *>(grnot) ||
//		 dynamic_cast<GREmpty *>(grnot)))
//	{
//	  	GRARNotationElement::addAssociation(grnot);
//	}
//	else setError(1);
//
//	if (!error)
//		GRPositionTag::addAssociation(grnot);
//}

//---------------------------------------------------------------------------------
void GRCrescendo::OnDraw( VGDevice & hdc) const		{ DrawDynamic (hdc, true); }
