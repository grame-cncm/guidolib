#ifndef __ARDaCapo__
#define __ARDaCapo__

/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARJump.h"

/** \brief da capo
*/
class ARDaCapo : public ARJump
{
public:	
				 ARDaCapo();
	virtual 	~ARDaCapo();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const {};

	virtual void browse(TimeUnwrap& mapper) const;
};

/** \brief da capo al fine
*/
class ARDaCapoAlFine : public ARJump
{
public:	
				 ARDaCapoAlFine();
	virtual 	~ARDaCapoAlFine();

	virtual void printName(std::ostream& os) const;
    virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const {};

	virtual void browse(TimeUnwrap& mapper) const;
};

#endif
