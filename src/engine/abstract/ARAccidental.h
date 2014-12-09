#ifndef ARAcc_H
#define ARAcc_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief Abstract representation of an accidental element.
*/

class ARAccidental : public ARMTParameter, public ARPositionTag
{
public:
	enum Style { kNone, kCautionary };
	
			 ARAccidental(const ARAccidental & acc);
			 ARAccidental();
	virtual ~ARAccidental();



	virtual ARMusicalObject * Copy() const	{ return new ARAccidental(*this); }

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void setTagParameterList(TagParameterList & tpl);

	Style		getStyle() const;

protected:

	TagParameterString *mStyle;
	static ListOfTPLs ltpls;
};

#endif
