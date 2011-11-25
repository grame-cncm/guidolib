#ifndef __ARVolta__
#define __ARVolta__

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
#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief for different repeat endings
*/
class ARVolta : public ARMTParameter, public ARPositionTag
{
public:	
				ARVolta();
	virtual 	~ARVolta();

	virtual void print() const { }

	virtual void browse (TimeUnwrap& mapper) const;
	virtual void PrintName( std::ostream & os ) const;
//	virtual void PrintParameters(std::ostream & os) const;
	virtual void setTagParameterList( TagParameterList & tpl );
	virtual bool MatchEndTag(const char * s);
   
    const std::string getString() const     { return mMark; }
    const std::string getPass() const       { return mPass; }
    const std::string getFormat() const     { return mFormat; }

    virtual std::ostream & operator<<(std::ostream & os) const;

protected:
	static ListOfTPLs ltpls;
private:
    std::string mPass;       // the pass(es) at which notes are played
    std::string mMark;       // text to be displayed under the bracket
    std::string mFormat;     // format string
};

#endif
