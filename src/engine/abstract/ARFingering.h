#ifndef ARFingering_H
#define ARFingering_H

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

#include "ARText.h"

/*@mkdoc

@group:Text

@tagname:\fingering
@tagalias:\fing
@tagtype:P
@tagnotation:fingering
@tagdesc
@tagend

@params:
@param:position:string:above or below:*none*:true
@fontparams:
@paramdesc
The default position for fingering is close to the note head. When **position** is specified, the fingering is placed above or below the note, avoiding collisions with the stems and always outside the staff.

The position can be set at global voice level using the [\auto](../Miscellaneous/) tag.

See the [Beethoven](@EXAMPLES/beethoven/) example.
@paramend

*/

/** \brief not yet documented
*/
class ARFingering : public ARText
{
	public:
		enum { kDefaultPosition, kAbove, kBelow };

					 ARFingering(int pos=kDefaultPosition);
		virtual		~ARFingering() {}

		virtual void	setTagParameters (const TagParameterMap& params);
		virtual int		getFingeringPosition () const	{ return fPosition; }
		virtual void	setFingeringSize (float size)	{ fSize = size; }

		virtual const char*	getTagName() const		{ return "ARFingering"; };
		virtual std::string getGMNName() const		{ return "\\fingering"; };

	protected:
		int  	fPosition;
		float	fSize = 0;
};

#endif
