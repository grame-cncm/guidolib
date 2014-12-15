#ifndef __ARDalSegno__
#define __ARDalSegno__

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

/** \brief dal segno
*/
class ARDalSegno : public ARJump
{
public:	
				 ARDalSegno();
	virtual 	~ARDalSegno();

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const {};

	virtual void browse(TimeUnwrap& mapper) const;
};

/** \brief dal segno al fine
*/
class ARDalSegnoAlFine : public ARJump
{
public:	
				 ARDalSegnoAlFine();
	virtual 	~ARDalSegnoAlFine();

	virtual void printName(std::ostream& os) const;
    virtual void printGMNName(std::ostream& os) const;
    virtual void printParameters(std::ostream& os) const {};

	virtual void browse(TimeUnwrap& mapper) const;
};

#endif
