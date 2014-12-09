#ifndef ARNoteFormat_H
#define ARNoteFormat_H

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

/** \brief not yet documented
*/
class ARNoteFormat :  public ARMTParameter, public ARPositionTag
{
public:
             ARNoteFormat(ARNoteFormat * p_savenf = NULL, ARNoteFormat * copynf = NULL);
    virtual ~ARNoteFormat();

    const char* getStyle() const;

    const TagParameterString * getTPStyle() const { return style; }

    virtual bool IsStateTag() const { return true; }

    virtual void setTagParameterList( TagParameterList & tpl);

    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const;

    virtual ARNoteFormat * getEndTag() const		{ return new ARNoteFormat(NULL,savenf); }

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARNoteFormat() { return this; }
    /****************************************/

protected:
    ARNoteFormat * savenf;
    TagParameterString * style;
    static ListOfTPLs ltpls;
};

#endif 
