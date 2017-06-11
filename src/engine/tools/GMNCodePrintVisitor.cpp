/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GMNCodePrintVisitor.h"

#include "ARMusic.h"
#include "ARMusicalObject.h"
#include "ARPositionTag.h"
#include "ARMusicalVoice.h"
#include "ARTagEnd.h"

using namespace std;

//______________________________________________________________________________
// GMNCodePrintVisitor
//______________________________________________________________________________

GMNCodePrintVisitor::GMNCodePrintVisitor(std::ostream& stream) : fVoicesCount(0), fOutStream(stream) {}

void GMNCodePrintVisitor::visitIn(ARMusic* music) {
	fOutStream << "{\n";
	fVoicesCount = music->countVoices();
}
void GMNCodePrintVisitor::visitOut(ARMusic* music)				{ fOutStream << "}\n"; }
void GMNCodePrintVisitor::visitIn(ARMusicalVoice* voice)		{ fOutStream << "[ (* voice " << voice->getVoiceNum() << " *)\n\t"; }
void GMNCodePrintVisitor::visitOut(ARMusicalVoice* voice)		{
	fOutStream << "\n]" << endl;
	int num = voice->getVoiceNum();
	if (num < fVoicesCount) fOutStream << "," << endl;
}
void GMNCodePrintVisitor::visitIn(ARMusicalObject* object)		{ printMusicalObject(object); }
void GMNCodePrintVisitor::visitIn(ARPositionTag* tag)			{ printPositionTag(tag); }

void GMNCodePrintVisitor::printMusicalObject(ARMusicalObject* object) const
{
	object->printGMNName(fOutStream);
    fOutStream << " ";
}

void GMNCodePrintVisitor::printPositionTag (ARPositionTag* tag)
{
	tag->printGMNName(fOutStream);
	fOutStream << " ";
}
