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

#include <string.h>

#include "ARCrescendo.h"
#include "VGDevice.h"
#include "GRStaff.h"
#include "TagParameterFloat.h"
#include "GRStdNoteHead.h"
#include "GRSingleNote.h"
#include "GRRest.h"
#include "GREmpty.h"
#include "MusicalSymbols.h"

#include "GRCrescendo.h"

extern GRSystem *gCurSystem;

GRCrescendo::GRCrescendo(GRStaff * grstaff) : GRPTagARNotationElement(new ARCrescendo, true)
{
    flaststartElement = NULL;

    initGRCrescendo(grstaff);
}

GRCrescendo::GRCrescendo(GRStaff * grstaff, ARCrescendo * abstractRepresentationOfCrescendo)
 : GRPTagARNotationElement(abstractRepresentationOfCrescendo)
{
    assert(abstractRepresentationOfCrescendo);
    flaststartElement = NULL;

	initGRCrescendo(grstaff);
}

GRCrescendo::~GRCrescendo()
{
}

GRSystemStartEndStruct *GRCrescendo::initGRCrescendo(GRStaff * grstaff)
{
	assert(grstaff);

	setGRStaff(grstaff);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();

	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endflag = GRSystemStartEndStruct::RIGHTMOST;

	mStartEndList.AddTail(sse);
		
	fCrescInfos = new GRCrescendoSaveStruct;
    fCrescInfos->numPoints = 3;

	sse->p = (void *)fCrescInfos;
	
	return sse;
}

void GRCrescendo::tellPosition(GObject *caller, const NVPoint & newPosition)
{
	GRNotationElement * grel = dynamic_cast<GRNotationElement *>(caller);
	if (grel == 0)
        return;

	GRStaff * staff = grel->getGRStaff();
	if (staff == 0)
        return;

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(staff->getGRSystem());
	if (sse == 0)
        return;

	const GRNotationElement * const endElement = sse->endElement;

	if (grel == endElement)
		updateCrescendo(staff);
}

void GRCrescendo::updateCrescendo(GRStaff * inStaff)
{
	GRSystemStartEndStruct *sse = getSystemStartEndStruct(inStaff->getGRSystem());
	if (sse == 0)
        return;

    // Collects informations about the context
	fCrescContext.staff = inStaff;
	getCrescendoBeginingContext(&fCrescContext, sse);
	getCrescendoEndingContext(&fCrescContext, sse);

	ARCrescendo *arCresc = static_cast<ARCrescendo *>(getAbstractRepresentation());
	const float staffLSpace = inStaff->getStaffLSPACE();
	assert(arCresc);
	
	/**** information gathering of parameters from the AR ****/

    const char* dynamicMarking = arCresc->getDynamicMarking().c_str();
    if (!strcmp(dynamicMarking,"p"))
		fCrescInfos->fMarkingSymbol = kIntensPSymbol;
	else if (!strcmp(dynamicMarking,"f"))
		fCrescInfos->fMarkingSymbol = kIntensFSymbol;
	else if (!strcmp(dynamicMarking,"ff"))
		fCrescInfos->fMarkingSymbol = kIntensFFSymbol;
	else if (!strcmp(dynamicMarking,"fff"))
		fCrescInfos->fMarkingSymbol = kIntensFFFSymbol;
	else if (!strcmp(dynamicMarking,"ffff"))
		fCrescInfos->fMarkingSymbol = kIntensFFFFSymbol;
	else if (!strcmp(dynamicMarking,"mf"))
		fCrescInfos->fMarkingSymbol = kIntensMFSymbol;
	else if (!strcmp(dynamicMarking,"mp"))
		fCrescInfos->fMarkingSymbol = kIntensMPSymbol;
	else if (!strcmp(dynamicMarking,"sf"))
		fCrescInfos->fMarkingSymbol = kIntensSFSymbol;
	else if (!strcmp(dynamicMarking,"pp"))
		fCrescInfos->fMarkingSymbol = kIntensPPSymbol;
	else if (!strcmp(dynamicMarking,"ppp"))
		fCrescInfos->fMarkingSymbol = kIntensPPPSymbol;
	else if (!strcmp(dynamicMarking,"pppp"))
		fCrescInfos->fMarkingSymbol = kIntensPPPPSymbol;
    else
        fCrescInfos->fMarkingSymbol = 0;

	float dx1      = arCresc->getDx1();
	float dx2      = arCresc->getDx2();
    float dy       = arCresc->getDy();
	float deltaY   = arCresc->getDeltaY();

    float XLeft  = 0;
    float XRight = 0;

    if (fCrescContext.leftHead)
        XLeft = fCrescContext.leftHead->getPosition().x + fCrescContext.leftNoteDX;
    else
        XLeft = sse->startElement->getPosition().x;
    if (fCrescContext.rightHead)
        XRight = fCrescContext.rightHead->getPosition().x + fCrescContext.rightNoteDX;
    else
        XRight = sse->endElement->getPosition().x;

    fCrescInfos->points[0].x = XLeft + dx1;
	fCrescInfos->points[2].x = fCrescInfos->points[1].x = XRight + dx2;

    if (fCrescInfos->points[0].x > fCrescInfos->points[2].x)
    {
        fCrescInfos->points[0].x = XLeft;
        fCrescInfos->points[2].x = fCrescInfos->points[1].x = XRight;
    }

    mPosition.y = (GCoord)(6 * staffLSpace);

    fCrescInfos->points[0].y = mPosition.y + dy;
	fCrescInfos->points[1].y = fCrescInfos->points[0].y + deltaY / 2;
    fCrescInfos->points[2].y = fCrescInfos->points[0].y - deltaY / 2;

    fCrescInfos->thickness = arCresc->getThickness();
}

void GRCrescendo::getCrescendoBeginingContext(GRCrescendoContext *ioContext, GRSystemStartEndStruct *sse )
{
    GRNotationElement * startElement = sse->startElement;
	if (sse->startflag == GRSystemStartEndStruct::OPENLEFT)
		startElement = flaststartElement;
		
	GRSingleNote *note  = dynamic_cast<GRSingleNote *>(startElement);
	if (note)
	{
		ioContext->leftHead = note->getNoteHead();
		ioContext->leftNoteDX = note->getOffset().x;
	}

}

void GRCrescendo::getCrescendoEndingContext(GRCrescendoContext *ioContext, GRSystemStartEndStruct * sse)
{	
    GRNotationElement * endElement = sse->endElement;
	if(sse->endflag == GRSystemStartEndStruct::OPENRIGHT)
		endElement = lastendElement;

	GRSingleNote *note  = dynamic_cast<GRSingleNote *>(endElement);
	if (note)
	{
		ioContext->rightHead = note->getNoteHead();
		ioContext->rightNoteDX = note->getOffset().x;
	}
}

void GRCrescendo::addAssociation(GRNotationElement * grnot)
{
	if (error)
        return;

		if ( GREvent::cast(grnot)  && 	// stop immediately if it's not an event.
		(dynamic_cast<GRNote *>(grnot) ||
		 dynamic_cast<GRRest *>(grnot) ||
		 dynamic_cast<GREmpty *>(grnot)))
	{
	  	GRARNotationElement::addAssociation(grnot);
	}
	else
	{
		setError(1);
	}

	if (!error)
		GRPositionTag::addAssociation(grnot);
}

void GRCrescendo::OnDraw( VGDevice & hdc) const
{
	if (!mDraw)
		return;

    if (fCrescInfos->points[0].x == fCrescInfos->points[1].x)
        return;

    assert(gCurSystem);

	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0)
		return;

    if (mColRef)
    {
        hdc.PushPenColor(VGColor(mColRef));
        hdc.PushFillColor(VGColor(mColRef));
        hdc.SetFontColor(VGColor(mColRef));
    }

    hdc.PushPenWidth(fCrescInfos->thickness);

    hdc.Line(fCrescInfos->points[0].x , fCrescInfos->points[0].y, fCrescInfos->points[1].x , fCrescInfos->points[1].y);
	hdc.Line(fCrescInfos->points[0].x , fCrescInfos->points[0].y, fCrescInfos->points[2].x , fCrescInfos->points[2].y);

    const float xMarkingOffset = fCrescInfos->points[1].x + 30;
    const float yMarkingOffset = fCrescInfos->points[0].y - 277 + (mTagSize - 1) * 25;

    if (fCrescInfos->fMarkingSymbol != 0)
        OnDrawSymbol(hdc, fCrescInfos->fMarkingSymbol, xMarkingOffset, yMarkingOffset, mTagSize);
    
    hdc.PopPenWidth();

    if (mColRef)
    {
        hdc.PopPenColor();
        hdc.PopFillColor();
        hdc.SetFontColor(VGColor());
    }
}
