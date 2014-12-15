#ifndef GRCompositeNotationElement_H
#define GRCompositeNotationElement_H

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
#include "GUIDOTypes.h"
#include "NEPointerList.h"

/** \brief not yet documented
*/
class GRCompositeNotationElement : virtual public GRNotationElement
{

  public:
     				 		GRCompositeNotationElement();
		virtual 			~GRCompositeNotationElement();

		virtual void 		setGRStaff( GRStaff * stf );

		virtual void 		updateBoundingBox() = 0;

		virtual GuidoPos 	AddTail( GRNotationElement * );

      NEPointerList * getElements() { return &mCompElements; }

  protected:

      NEPointerList 	mCompElements;
};

#endif


