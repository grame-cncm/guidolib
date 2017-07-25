#ifndef ARPizz_H
#define ARPizz_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARArticulation.h"

/*brief	The pizzicato articulation tag */
class ARPizzicato : public ARArticulation
{
	public :
		enum	tTypePizz {SNAP, BUZZ, FINGERNAIL, LEFTHAND};

					 ARPizzicato();
		virtual		~ARPizzicato() { };
	
		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARPizzicatoParams; };
		virtual const char*	getTagName() const		{ return "ARPizzicato"; };
		virtual std::string getGMNName() const		{ return "\\pizzicato"; };
	
		tTypePizz	 getType() const				{ return fType;}
	
	private:
		tTypePizz fType;
};

#endif


