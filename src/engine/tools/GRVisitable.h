/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___GRVisitable___
#define ___GRVisitable___

class GRVisitor;

class GRVisitable
{
	public:
		virtual ~GRVisitable() {};
		
		virtual void accept   (GRVisitor& visitor) {};
};

#endif
