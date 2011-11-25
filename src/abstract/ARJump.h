#ifndef __ARJump__
#define __ARJump__

/*
	GUIDO Library
	Copyright (C) 2004  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

    virtual void print() const { PrintName(std::cout); }
	virtual void setTagParameterList( TagParameterList & tpl );

    const FormatStringParserResult&   getMark() const     { return mMark; }
    int								  getID() const       { return mID; }

protected:
    static ListOfTPLs ltpls;
private:
	FormatStringParserResult	mMark;
    int							mID;             // the tag ID
};

#endif
