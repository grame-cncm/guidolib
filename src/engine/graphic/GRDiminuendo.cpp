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

#include "ARDiminuendo.h"
#include "VGDevice.h"
#include "GRStaff.h"
#include "TagParameterFloat.h"
#include "GRStdNoteHead.h"
#include "GRSingleNote.h"
#include "GRRest.h"
#include "GREmpty.h"
#include "MusicalSymbols.h"

#include "GRDiminuendo.h"

extern GRSystem *gCurSystem;

GRDiminuendo::GRDiminuendo(GRStaff * grstaff) : GRPTagARNotationElement(new ARDiminuendo, true)
{
    flaststartElement = NULL;

    initGRDiminuendo(grstaff);
}

GRDiminuendo::GRDiminuendo(GRStaff * grstaff, ARDiminuendo * abstractRepresentationOfDiminuendo)
 : GRPTagARNotationElement(abstractRepresentationOfDiminuendo)
{
    assert(abstractRepresentationOfDiminuendo);
    flaststartElement = NULL;

	initGRDiminuendo(grstaff);
}

GRDiminuendo::~GRDiminuendo()
{
}

GRSystemStartEndStruct *GRDiminuendo::initGRDiminuendo(GRStaff * grstaff)
{
	assert(grstaff);

	setGRStaff(grstaff);

	GRSystemStartEndStruct * sse = new GRSystemStartEndStruct;
	sse->grsystem = grstaff->getGRSystem();

	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	sse->endflag = GRSystemStartEndStruct::RIGHTMOST;

	mStartEndList.AddTail(sse);
		
	fDimInfos = new GRDiminuendoSaveStruct;
    fDimInfos->numPoints = 3;

	sse->p = (void *)fDimInfos;
	
	return sse;
}

void GRDiminuendo::tellPosition(GObject *caller, const NVPoint & newPosition)
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
		updateDiminuendo(staff);
}

void GRDiminuendo::updateDiminuendo(GRStaff * inStaff)
{
	GRSystemStartEndStruct *sse = getSystemStartEndStruct(inStaff->getGRSystem());
	if (sse == 0)
        return;

    // Collects informations about the context
	fDimContext.staff = inStaff;
	getDiminuendoBeginingContext(&fDimContext, sse);
	getDiminuendoEndingContext(&fDimContext, sse);

    ARDiminuendo *arDim = static_cast<ARDiminuendo *>(getAbstractRepresentation());
	const float staffLSpace = inStaff->getStaffLSPACE();
	assert(arDim);
	
	/**** information gathering of parameters from the AR ****/

    const char* dynamicMarking = arDim->getDynamicMarking().c_str();
    if (!strcmp(dynamicMarking,"p"))
		fDimInfos->fMarkingSymbol = kIntensPSymbol;
	else if (!strcmp(dynamicMarking,"f"))
		fDimInfos->fMarkingSymbol = kIntensFSymbol;
	else if (!strcmp(dynamicMarking,"ff"))
		fDimInfos->fMarkingSymbol = kIntensFFSymbol;
	else if (!strcmp(dynamicMarking,"fff"))
		fDimInfos->fMarkingSymbol = kIntensFFFSymbol;
	else if (!strcmp(dynamicMarking,"ffff"))
		fDimInfos->fMarkingSymbol = kIntensFFFFSymbol;
	else if (!strcmp(dynamicMarking,"mf"))
		fDimInfos->fMarkingSymbol = kIntensMFSymbol;
	else if (!strcmp(dynamicMarking,"mp"))
		fDimInfos->fMarkingSymbol = kIntensMPSymbol;
	else if (!strcmp(dynamicMarking,"sf"))
		fDimInfos->fMarkingSymbol = kIntensSFSymbol;
	else if (!strcmp(dynamicMarking,"pp"))
		fDimInfos->fMarkingSymbol = kIntensPPSymbol;
	else if (!strcmp(dynamicMarking,"ppp"))
		fDimInfos->fMarkingSymbol = kIntensPPPSymbol;
	else if (!strcmp(dynamicMarking,"pppp"))
		fDimInfos->fMarkingSymbol = kIntensPPPPSymbol;
    else
        fDimInfos->fMarkingSymbol = 0;

    float dx1     = arDim->getDx1();
	float dx2     = arDim->getDx2();
    float dy      = arDim->getDy();
	float deltaY  = arDim->getDeltaY();

    float XLeft  = 0;
    float XRight = 0;

    if (fDimContext.leftHead)
        XLeft = fDimContext.leftHead->getPosition().x + fDimContext.leftNoteDX;
    else
        XLeft = sse->startElement->getPosition().x;
    if (fDimContext.rightHead)
        XRight = fDimContext.rightHead->getPosition().x + fDimContext.rightNoteDX;
    else
        XRight = sse->endElement->getPosition().x;

    fDimInfos->points[0].x = fDimInfos->points[2].x = XLeft + dx1;
    fDimInfos->points[1].x = XRight + dx2;

    if (fDimInfos->points[0].x > fDimInfos->points[1].x)
    {
        fDimInfos->points[0].x = fDimInfos->points[2].x = XLeft;
        fDimInfos->points[1].x = XRight;
    }

    mPosition.y = (GCoord)(6 * staffLSpace);

    fDimInfos->points[0].y = mPosition.y + dy + deltaY / 2;
	fDimInfos->points[1].y = mPosition.y + dy;
    fDimInfos->points[2].y = mPosition.y + dy - deltaY / 2;

    fDimInfos->thickness = arDim->getThickness();
}

void GRDiminuendo::getDiminuendoBeginingContext(GRDiminuendoContext *ioContext, GRSystemStartEndStruct *sse )
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

void GRDiminuendo::getDiminuendoEndingContext(GRDiminuendoContext *ioContext, GRSystemStartEndStruct * sse)
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

void GRDiminuendo::addAssociation(GRNotationElement * grnot)
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

void GRDiminuendo::OnDraw( VGDevice & hdc) const
{
	if (!mDraw)
		return;

    if (fDimInfos->points[0].x == fDimInfos->points[1].x)
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

    hdc.PushPenWidth(fDimInfos->thickness);

    hdc.Line(fDimInfos->points[0].x , fDimInfos->points[0].y, fDimInfos->points[1].x , fDimInfos->points[1].y);
	hdc.Line(fDimInfos->points[2].x , fDimInfos->points[2].y, fDimInfos->points[1].x , fDimInfos->points[1].y);

    const float xMarkingOffset = fDimInfos->points[1].x + 30;
    const float yMarkingOffset = fDimInfos->points[1].y - 277 + (mTagSize - 1) * 25;

    OnDrawSymbol(hdc, fDimInfos->fMarkingSymbol, xMarkingOffset, yMarkingOffset, mTagSize);
    
    hdc.PopPenWidth();

    if (mColRef)
    {
        hdc.PopPenColor();
        hdc.PopFillColor();
        hdc.SetFontColor(VGColor());
    }
}

void GRDiminuendo::print(std::ostream& os) const
{
}



