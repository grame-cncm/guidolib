#ifndef GRSystemTagInterface_H
#define GRSystemTagInterface_H

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


class GRSystem;

/** \brief not yet documented
*/
class GRSystemTagInterface
{
	public:

				GRSystemTagInterface() : mIsSystemCall(false) {}
		virtual ~GRSystemTagInterface() {}		
		virtual void checkPosition(const GRSystem * grsys);

	protected:
		// this variable is set to 1, if
		// a call to tellPosition is made from checkPosition
		// which mas made by the system ....
		// (look into GRBeam)
		bool mIsSystemCall;
};

#endif
