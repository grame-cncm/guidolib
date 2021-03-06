/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __PrintVisitor__
#define __PrintVisitor__


#include <ostream>

#include "ARVisitor.h"
#include "defines.h"

class ARMusicalObject;
class ARMusicalTag;
class ARMusicalVoice;

class PrintVisitor : public ARVisitor
{
public:
             PrintVisitor(std::ostream& stream);
    virtual ~PrintVisitor() {};
    
    void     visitIn (ARMusicalVoice*  voice);
    void     visitOut(ARMusicalVoice*  voice);
    void     visitIn (ARMusicalObject* object);
    void     visitIn (ARMusicalTag*   tag);

private:
    void        printMusicalVoice	(ARMusicalVoice* voice) const;
    void        printMusicalObject	(ARMusicalObject* object) const;
    void        printTag			(ARMusicalTag* tag);
    void        printHeader(int state = 0) const; // state = -1 : position tag end ("-")
                                                  // state =  1 : position tag beginning ("+")
                                                  // state =  0 : neutral ("x");
    std::string getIndentStr(int indentNumber) const;

    int               currentIndentNumber;
    TYPE_TIMEPOSITION currentStartDate;

    std::ostream *os;
};

#endif
