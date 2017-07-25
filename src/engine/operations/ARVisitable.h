/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___ARVisitable___
#define ___ARVisitable___

//#include "PrintVisitor.h"

class ARVisitor;

class ARVisitable
{
	public:
             ARVisitable() {};
    virtual ~ARVisitable() {};
    
    virtual void accept   (ARVisitor& visitor) {};
//    virtual void acceptIn (ARVisitor *visitor) {};
//    virtual void acceptOut(ARVisitor *visitor) {};
};

#endif
