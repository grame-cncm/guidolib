#ifndef GRTagARNotationElement_H
#define GRTagARNotationElement_H

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

class ARMusicalObject;

#include "ARMusicalTag.h"
#include "GRTag.h"
#include "GRARNotationElement.h"

/** \brief GRTagARNotationElement
*/
class GRTagARNotationElement : public GRTag, public GRARNotationElement
{
public:
	GRTagARNotationElement (ARMusicalObject * el, float curLSPACE, bool ownsAR = false )
        : GRTag(static_cast<ARMusicalTag *>(el->isARMusicalTag()), curLSPACE), GRARNotationElement(el, ownsAR)
	{}

	virtual ~GRTagARNotationElement();

	virtual const NVPoint &     getOffset() const       { return GRTag::getOffset(); }
	virtual float               getSize() const         { return GRTag::getSize(); }
	virtual const NVstring *    getFont() const         { return GRTag::getFont(); }
	virtual const NVstring *    getFontAttrib() const   { return GRTag::getFontAttrib(); }
	virtual int                 getFontSize() const     { return GRTag::getFontSize(); }
	virtual const unsigned char * getColRef() const     { return GRTag::getColRef(); }
};

#endif
