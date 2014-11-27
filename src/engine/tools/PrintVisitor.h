#ifndef __PrintVisitor__
#define __PrintVisitor__

/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <ostream>

#include "BaseVisitor.h"

#include "defines.h"

using namespace std;

class ARMusicalObject;
class ARPositionTag;
class ARMusicalVoice;

class PrintVisitor : public BaseVisitor
{
public:
             PrintVisitor(std::ostream& stream);
    virtual ~PrintVisitor() {};
    
    void     visitIn (ARMusicalVoice&  voice);
    void     visitOut(ARMusicalVoice&  voice);
    void     visit   (ARMusicalObject& object);
    void     visit   (ARPositionTag&   positionTag);

private:
    void        printMusicalVoice(ARMusicalVoice& voice) const;
    void        printMusicalObject(ARMusicalObject& object) const;
    void        printPositionTag(ARPositionTag& positionTag);
    void        printHeader(int state = 0) const; // state = -1 : position tag end ("-")
                                                  // state =  1 : position tag beginning ("+")
                                                  // state =  0 : neutral ("x");
    std::string getIndentStr(int indentNumber) const;

    int               currentIndentNumber;
    TYPE_TIMEPOSITION currentStartDate;

    std::ostream *os;
};

#endif
