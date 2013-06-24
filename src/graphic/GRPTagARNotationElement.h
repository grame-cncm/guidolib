#ifndef GRPTagARNotationElement_H
#define GRPTagARNotationElement_H

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

#include "GRPositionTag.h"
#include "GRARNotationElement.h"

/** \brief not yet documented
*/
class GRPTagARNotationElement :
		public GRPositionTag, public GRARNotationElement
{
public:
			GRPTagARNotationElement(GuidoPos p_ep, ARMusicalObject * el, bool ownsAR = false);

			GRPTagARNotationElement(ARMusicalObject * el, bool ownsAR = false);

	virtual ~GRPTagARNotationElement();

	virtual const NVPoint &getOffset() const
			{ return GRPositionTag::getOffset(); }

	virtual float getSize() const
			{ return GRPositionTag::getSize(); }

	virtual const NVstring * getFont() const
			{ return GRPositionTag::getFont(); }
	
	virtual const NVstring *getFontAttrib() const
			{ return GRPositionTag::getFontAttrib(); }
	
	virtual int getFontSize() const
			{ return GRPositionTag::getFontSize(); }
	
	const unsigned char * getColRef() const
			{ return GRPositionTag::getColRef(); }
			
	virtual void addAssociation( GRNotationElement * grnot );
	virtual void removeAssociation( GRNotationElement * el );

};

#endif
