/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRPrintVisitor.h"

#include "GRNotationElement.h"
#include "GRMusic.h"
#include "GRPage.h"
#include "GRSystem.h"
#include "GRStaff.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"

#include "ARRest.h"
#include "ARNote.h"

using namespace std;

#define VSTART	"==> "
#define VEND	"<== "

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRNotationElement* o) { fOut << VSTART << "GRNotationElement " << o << endl; }
void GRPrintVisitor::visitEnd   (GRNotationElement* o) { fOut << VEND << "GRNotationElement " << o << endl; }

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRMusic* o)	{ fOut << VSTART << "GRMusic " << endl; }
void GRPrintVisitor::visitEnd   (GRMusic* o)	{ fOut << VEND << "GRMusic " << endl; }

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRPage* o)		{ fOut << VSTART << "GRPage " << endl; }
void GRPrintVisitor::visitEnd   (GRPage* o)		{ fOut << VEND << "GRPage " << endl; }

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRSystem* o)	{ fOut << VSTART << "GRSystem " << endl; }
void GRPrintVisitor::visitEnd   (GRSystem* o)	{ fOut << VEND << "GRSystem " << endl; }

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRStaff* o)	{ fOut << VSTART << "GRStaff " << endl; }
void GRPrintVisitor::visitEnd   (GRStaff* o)	{ fOut << VEND << "GRStaff " << endl; }

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRSingleNote* o)	{ fOut << VSTART << " "; o->getARNote()->printGMNName(fOut); fOut << endl; }
void GRPrintVisitor::visitEnd   (GRSingleNote* o)	{ fOut << VEND << "end note " << endl; }

//-------------------------------------------------------------------------------
void GRPrintVisitor::visitStart (GRBeam* o)			{ fOut << VSTART << "GRBeam " << endl; }
void GRPrintVisitor::visitStart (GRRange* o)		{ fOut << VSTART << "GRRange " << endl; }
void GRPrintVisitor::visitStart (GRText* o)			{ fOut << VSTART << "GRText " << endl; }
void GRPrintVisitor::visitStart (GREmpty* o)		{ fOut << VSTART << "GREmpty " << endl; }
void GRPrintVisitor::visitStart (GRClef* o)			{ fOut << VSTART << "GRClef " << endl; }
void GRPrintVisitor::visitStart (GRKey* o)			{ fOut << VSTART << "GRKey " << endl; }
void GRPrintVisitor::visitStart (GRSlur* o)			{ fOut << VSTART << "GRSlur " << endl; }
void GRPrintVisitor::visitStart (GRDynamics* o)		{ fOut << VSTART << "GRDynamics " << endl; }
void GRPrintVisitor::visitStart (GRIntens* o)		{ fOut << VSTART << "GRIntens " << endl; }
void GRPrintVisitor::visitStart (GRMeter* o)		{ fOut << VSTART << "GRMeter " << endl; }
void GRPrintVisitor::visitStart (GRArticulation* o)	{ fOut << VSTART << "GRArticulation " << endl; }
void GRPrintVisitor::visitStart (GRBar* o)			{ fOut << VSTART << "GRBar " << endl; }
void GRPrintVisitor::visitStart (GRSingleRest* o)	{ fOut << VSTART << " "; o->getARRest()->printGMNName(fOut); fOut << endl; }

