#ifndef __ARJump__
#define __ARJump__

/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <iostream>

#include "ARMTParameter.h"
#include "FormatStringParser.h"

/** \brief common implementation for \\coda, \\segno, \\toCoda etc...
*/
class ARJump : public ARMTParameter
{
public:
				ARJump(std::string mark = "");
	virtual 	~ARJump();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void setTagParameterList( TagParameterList & tpl );

    const FormatStringParserResult&   getMark() const     { return mMark; }
    int								  getID() const       { return mID; }

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARJump() { return this; }
    /****************************************/

protected:
    static ListOfTPLs ltpls;

private:
	FormatStringParserResult	mMark;
    int							mID;             // the tag ID
};

#endif
