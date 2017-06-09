/*
  GUIDO Library
  Copyright (C) 2014-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef __GMNCodePrintVisitor__
#define __GMNCodePrintVisitor__

#include <ostream>
#include <string>

#include "ARVisitor.h"
#include "defines.h"


class GMNCodePrintVisitor : public ARVisitor
{
public:
             GMNCodePrintVisitor(std::ostream& stream);
    virtual ~GMNCodePrintVisitor() {};
    
    void     visitIn	(ARMusic*  m);
    void     visitOut	(ARMusic*  m);
    void     visitIn	(ARMusicalVoice*  voice);
    void     visitOut	(ARMusicalVoice*  voice);
    void     visitIn	(ARMusicalObject* object);
    void     visitIn	(ARPositionTag*   tag);

private:
    void          printMusicalObject(ARMusicalObject* object) const;
    void          printPositionTag(ARPositionTag* positionTag);
    std::string  *formatTagName(std::string *tagName) const;

	int	fVoicesCount;
	std::ostream& fOutStream;
};

#endif
