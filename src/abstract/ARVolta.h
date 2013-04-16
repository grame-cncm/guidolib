#ifndef __ARVolta__
#define __ARVolta__

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
