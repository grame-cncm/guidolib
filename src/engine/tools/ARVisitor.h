/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___ARVisitor___
#define ___ARVisitor___

#include "BaseVisitor.h"

class ARMusicalObject;
class ARPositionTag;
class ARMusicalVoice;

class ARVisitor : public BaseVisitor
{
public:
    virtual ~ARVisitor() {};
    
    virtual void visitIn (ARMusicalVoice& voice)   = 0;
    virtual void visitOut(ARMusicalVoice& voice)   = 0;
    virtual void visit   (ARMusicalObject &object) = 0;
    virtual void visit   (ARPositionTag   &tag)    = 0;
};

#endif
