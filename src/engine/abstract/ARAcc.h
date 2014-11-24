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

class ARAcc : public ARMTParameter, public ARPositionTag
{
public:
	enum Style { kNone, kCautionary };
	
			 ARAcc(const ARAcc & acc);
			 ARAcc();
	virtual ~ARAcc();



	virtual ARMusicalObject * Copy() const	{ return new ARAcc(*this); }

	virtual void print(int &indent) const { }
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;

	virtual void setTagParameterList(TagParameterList & tpl);

	const char* getStyleStr() const;
	Style		getStyle() const;

protected:

	TagParameterString * mStyle;
	static ListOfTPLs ltpls;
};

#endif
