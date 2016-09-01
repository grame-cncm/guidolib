#ifndef ARCluster_H
#define ARCluster_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "ARNote.h"
#include "ARMTParameter.h"
#include "ARPositionTag.h"

class TagParameterInt;
class TagParameterString;

/** \brief not yet documented
*/

class ARCluster : public ARMTParameter, public ARPositionTag
{		
public:
                     ARCluster();
	                 ARCluster(const ARCluster *inCopyCluster);
	virtual		 	~ARCluster();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void    setTagParameterList(TagParameterList & tpl);

    void setARNote(ARNote *arNote);

    ARNote *getFirstNote()  const { return firstNote; }
    ARNote *getSecondNote() const { return secondNote; }

    float getahdx() const                        { return hdx; }
    float getahdy() const                        { return hdy; }
    int   getNoteCount() const                   { return noteCount; }
    bool  getIsThereOnlyOneNoteInCluster() const { return onlyOneNoteInCluster; }

    void  setOnlyOneNoteInCluster() {onlyOneNoteInCluster = true;}

protected:
    float hdx;
    float hdy;
    int   noteCount;
    bool  onlyOneNoteInCluster;

    static ListOfTPLs ltpls;

    ARNote *firstNote;
    ARNote *secondNote;
};

#endif
