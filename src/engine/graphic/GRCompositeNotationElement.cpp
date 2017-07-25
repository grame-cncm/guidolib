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

#include "GRCompositeNotationElement.h"
#include "GREvent.h"
#include "NEPointerList.h"

GRCompositeNotationElement::GRCompositeNotationElement()
  : mCompElements( 1 ) // (new NEPointerList(1)) // better init with NULL first?
{
}

GRCompositeNotationElement::~GRCompositeNotationElement()
{

/* ATTENTION -> either or -- not both versions of delete !

	GuidoPos pos=elements->GetHeadPosition();
	while(pos)
	{
		delete elements->GetNext(pos);
	} */

//	delete elements;
//	elements = 0;
}

GuidoPos GRCompositeNotationElement::AddTail(GRNotationElement * e)
{
	return mCompElements.AddTail(e);
}

void GRCompositeNotationElement::setGRStaff(GRStaff * stf)
{

	GRNotationElement::setGRStaff(stf);

	GuidoPos pos = mCompElements.GetHeadPosition();
	while (pos)
	{
		GRNotationElement * el = mCompElements.GetNext( pos );
		el->setGRStaff(stf);
	}
}
