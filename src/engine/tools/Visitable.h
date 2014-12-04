#ifndef __Visitable__
#define __Visitable__

/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "PrintVisitor.h"

class Visitable
{
public:
             Visitable() {};
    virtual ~Visitable() {};
    
    virtual void accept   (BaseVisitor *visitor) {};
    virtual void acceptIn (BaseVisitor *visitor) {};
    virtual void acceptOut(BaseVisitor *visitor) {};
};

#endif
