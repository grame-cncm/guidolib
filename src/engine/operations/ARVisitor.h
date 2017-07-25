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

class ARMusic;
class ARMusicalObject;
class ARMusicalTag;
class ARMusicalVoice;

class ARVisitor
{
public:
    virtual ~ARVisitor() {};

    virtual void visitIn (ARMusic* ) {};
    virtual void visitOut(ARMusic* ) {};

    virtual void visitIn (ARMusicalObject* ) {};
    virtual void visitOut(ARMusicalObject* ) {};
	
    virtual void visitIn (ARMusicalVoice* )  {};
    virtual void visitOut(ARMusicalVoice* )  {};
	
    virtual void visitIn (ARMusicalTag* )	{};
    virtual void visitOut(ARMusicalTag* )  {};
};

#endif
