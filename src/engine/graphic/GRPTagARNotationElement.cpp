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

#include "ARMusicalTag.h"
#include "GRPTagARNotationElement.h"
#include "NEPointerList.h"

GRPTagARNotationElement::GRPTagARNotationElement(GuidoPos p_ep, const ARMusicalObject * el, bool ownsAR )
	: GRPositionTag( p_ep,dynamic_cast<const ARMusicalTag *>(el)),
		GRARNotationElement( el, ownsAR )
{
}

GRPTagARNotationElement::GRPTagARNotationElement(const ARMusicalObject * el, bool ownsAR )
	: GRPositionTag(dynamic_cast<const ARMusicalTag *>(el)),
		GRARNotationElement( el, ownsAR )
{
}

GRPTagARNotationElement::~GRPTagARNotationElement()
{
	if (mAssociated)
	{
		mAssociated->RemoveAll();
		delete mAssociated;
		mAssociated = 0;
	}
}

void GRPTagARNotationElement::removeAssociation(GRNotationElement * el)
{
	GRPositionTag::removeAssociation(el);
	GRARNotationElement::removeAssociation(el);
}

void GRPTagARNotationElement::addAssociation(GRNotationElement * grnot)
{	
	GRNotationElement::addAssociation(grnot);
	GRPositionTag::addAssociation(grnot);
}
