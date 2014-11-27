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
#include "TagParameterString.h"

/** \brief for different repeat endings
*/
class ARVolta : public ARMTParameter, public ARPositionTag
{
public:	
				ARVolta();
	virtual 	~ARVolta();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void browse (TimeUnwrap& mapper) const;

	virtual void setTagParameterList( TagParameterList & tpl );
	virtual bool MatchEndTag(const char * s);
   
	const char* getPass() const   { return pass   ? pass->getValue() : ""; }
	const char* getMark() const   { return mark   ? mark->getValue() : ""; }
	const char* getFormat() const { return format ? format->getValue() : ""; }

protected:
	static ListOfTPLs ltpls;

private:
    TagParameterString *pass;   // the pass(es) at which notes are played
    TagParameterString *mark;   // text to be displayed under the bracket
    TagParameterString *format; // format string
};

#endif
