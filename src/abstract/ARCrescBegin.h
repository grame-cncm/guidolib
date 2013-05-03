#ifndef ARCrescBegin_H
#define ARCrescBegin_H

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

/** (unused) not yet documented
#include "ARTagBegin.h"

class ARCrescBegin : public ARTagBegin
{
	public:
		ARCrescBegin(int id = 0) :
		ARTagBegin(id) {}
		virtual ~ARCrescBegin() {}

		// virtual ostream & operator<<(ostream &os) const
		//{
		//	return os << " //crescBegin";
		//	if (id!= -1)
		//		os << ":" << id;
		//	os << " ";
		//} 
		virtual void PrintName(ostream & os) const
		{
			os << " //crescBegin";
		}

};

*/

#endif
