/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <ostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

#include "GMNCodePrintVisitor.h"

#include "ARMusicalObject.h"
#include "ARPositionTag.h"
#include "ARMusicalVoice.h"
#include "ARMusicalEvent.h"
#include "ARTagEnd.h"

using namespace std;

//______________________________________________________________________________
// GMNCodePrintVisitor
//______________________________________________________________________________

GMNCodePrintVisitor::GMNCodePrintVisitor(std::ostream& stream)
{
    os = &stream;
}

void GMNCodePrintVisitor::visitIn(ARMusicalVoice& voice)
{
    *os << "[ ";
}

void GMNCodePrintVisitor::visitOut(ARMusicalVoice& voice)
{
    *os << "]" << std::endl;
}

void GMNCodePrintVisitor::visit(ARMusicalObject &object)
{
    printMusicalObject(object);
}

void GMNCodePrintVisitor::visit(ARPositionTag &positionTag)
{
    printPositionTag(positionTag);
}

void GMNCodePrintVisitor::printMusicalObject(ARMusicalObject& object) const
{
    ARMusicalEvent *event = dynamic_cast<ARMusicalEvent *>(&object);
    ARMusicalTag   *tag   = dynamic_cast<ARMusicalTag *>  (&object);

    if (event)
        event->printGMNName(*os);
    else if (tag)
        tag->printGMNName(*os);

    *os << " ";
}

void GMNCodePrintVisitor::printPositionTag(ARPositionTag& positionTag)
{
    ARTagEnd *endTag = ARTagEnd::cast(&positionTag);

    if (!endTag) {
        positionTag.printGMNName(*os);
        *os << " ( ";
    }
    else
        *os << ") ";
}