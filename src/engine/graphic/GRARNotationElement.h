#ifndef GRARNotationElement_H
#define GRARNotationElement_H

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

class ARMusicalObject;

/** \brief This class is the base class for all graphical elements that 
	have a direct counterpart in the Abstract Representation.
*/
class GRARNotationElement : public GRNotationElement
{
  public:
		GRARNotationElement(ARMusicalObject * abstractRepresentationOfNotationElement, bool p_ownsAR = false );
		virtual ~GRARNotationElement();

		virtual ARMusicalObject * getAbstractRepresentation();
		virtual ARMusicalObject * getAbstractRepresentation() const;

		// sets the ownership between abstract and grafical object
		virtual void setOwnership(bool ownsAR)      { mOwnsAR = ownsAR; }

  protected:
		ARMusicalObject * mAbstractRepresentation;
		bool mOwnsAR;   // is 1 if abstract object is owned by the grafical object
                        // In that case the abstract object will be deleted when deleting the grafical object
};

#endif


