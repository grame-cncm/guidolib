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

#include "PrintVisitor.h"

#include "ARMusicalObject.h"
#include "ARPositionTag.h"
#include "ARMusicalVoice.h"
#include "ARMusicalEvent.h"
#include "ARTagEnd.h"

using namespace std;

//______________________________________________________________________________
// PrintVisitor
//______________________________________________________________________________

PrintVisitor::PrintVisitor(std::ostream& stream)
{
    currentIndentNumber = 0;
    currentStartDate    = 0;

    os = &stream;
}

void PrintVisitor::visitIn(ARMusicalVoice& voice)
{
    printMusicalVoice(voice);

    currentIndentNumber++;
}

void PrintVisitor::visitOut(ARMusicalVoice& voice)
{
    printMusicalVoice(voice);

    currentStartDate    = 0;
    currentIndentNumber = 0;
}

void PrintVisitor::visit(ARMusicalObject &object)
{
    currentStartDate = object.getRelativeTimePosition();
    printMusicalObject(object);
}

void PrintVisitor::visit(ARPositionTag &positionTag)
{
    printPositionTag(positionTag);
}

void PrintVisitor::printHeader(int state) const
{
    int num   = currentStartDate.getNumerator();
    int denom = currentStartDate.getDenominator();
    
    ostringstream numStream;
    ostringstream denomStream;
    
    if (num < 10)
        numStream << "  ";
    else if (num < 100)
        numStream << " ";
    
    if (denom < 10)
        denomStream << "  ";
    else if (denom < 100)
        denomStream << " ";

    numStream   << num;
    denomStream << denom;
    
    ostringstream dateStream;

    dateStream << numStream.str() << "/" << denomStream.str() << ": ";

    std::string stateStr = (state == -1 ? "-" : (state == 0 ? "x" : "+"));

    *os << getIndentStr(currentIndentNumber) << stateStr << " " << dateStream.str();
}

void PrintVisitor::printMusicalVoice(ARMusicalVoice& voice) const
{
    voice.printName(*os);
    *os << ": ";
    voice.printParameters(*os);
    *os << std::endl;
}

void PrintVisitor::printMusicalObject(ARMusicalObject& object) const
{
    printHeader();
    object.print(*os);
}

void PrintVisitor::printPositionTag(ARPositionTag& positionTag)
{
    ARTagEnd *endTag = ARTagEnd::cast(&positionTag);

    if (endTag)
        currentIndentNumber--;

    printHeader(endTag ? -1 : 1);
    positionTag.print(*os);

    if (!endTag)
        currentIndentNumber++;
}

std::string PrintVisitor::getIndentStr(int currentIndentNumber) const
{
    std::string indentStr = "";

    for (int i = 0; i < currentIndentNumber; i++)
        indentStr += "    ";

    return indentStr;
}
