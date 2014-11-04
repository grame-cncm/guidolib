/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRCluster.h"

#include "GRStaff.h"

#include "ARCluster.h"

#include "VGDevice.h"
#include "GRTagARNotationElement.h"
#include "GRSingleNote.h"
#include "GRGlobalStem.h"
#include "ARNoteFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

GRCluster::GRCluster(GRStaff * stf, ARCluster * arcls, GRSingleNote *sngNote, ARNoteFormat * curnoteformat) :
						GRARCompositeNotationElement(arcls),
						GRPositionTag(arcls->getEndPosition(), arcls),
                        gDuration(0),
                        gClusterOrientation(ARTHead::NORMAL),
                        gStemDir(dirAUTO),
                        noteFormatColor(0)
{
	assert(stf);

    mGrStaff = stf;

    gDuration = sngNote->getDurTemplate();

    const TagParameterString *tmpColor;

    mTagSize = stf->getSizeRatio();

    firstNote = secondNote = sngNote;

    if (curnoteformat) {
        // - Size
        const TagParameterFloat * tmp = curnoteformat->getSize();
        if (tmp)
            mTagSize = tmp->getValue();

        // - Color
        tmpColor = curnoteformat->getColor();
        if (tmpColor) {
            noteFormatColor = new unsigned char[4];
            tmpColor->getRGB(noteFormatColor);
        }

        // - Offset
		const TagParameterFloat * tmpdx = curnoteformat->getDX();
		const TagParameterFloat * tmpdy = curnoteformat->getDY();

		if (tmpdx)
			mTagOffset.x = (GCoord)(tmpdx->getValue(stf->getStaffLSPACE()));
		if (tmpdy)
			mTagOffset.y = (GCoord)(tmpdy->getValue(stf->getStaffLSPACE()));
    }

    gdx   = arcls->getadx();
    gdy   = arcls->getady();
    ghdx  = arcls->getahdx();
    ghdy  = arcls->getahdy();
    gSize = arcls->getSize();

    GREvent *grEvent = dynamic_cast<GREvent *>(sngNote);
    if (grEvent) {
        grEvent->getGlobalStem()->setOffsetXY(gdx, -gdy);
        grEvent->getGlobalStem()->setMultiplicatedSize(gSize);
    }

    gdy += mTagOffset.y;
    gdx += mTagOffset.x;

    gNoteCount = arcls->getNoteCount();
}

GRCluster::~GRCluster() {}

void GRCluster::updateBoundingBox()
{
    float curLSpace = mGrStaff->getStaffLSPACE();
    float dx        = getBoundingBox().left + gdx + ghdx;
    float dy        = - gdy - ghdy;

    float xOrientation = 0;

    if (gClusterOrientation == ARTHead::LEFT && gStemDir == dirDOWN)
        xOrientation = - 55;
    else if (gClusterOrientation == ARTHead::RIGHT && gStemDir == dirUP)
        xOrientation =   55;

    NVPoint pFirstNote  = (firstNote->getPosition().y < secondNote->getPosition().y ? firstNote->getPosition() : secondNote->getPosition());
    NVPoint pSecondNote = (firstNote->getPosition().y > secondNote->getPosition().y ? firstNote->getPosition() : secondNote->getPosition());

    mMapping.top    = dy + pFirstNote.y + curLSpace / 2;
    mMapping.bottom = dy + pSecondNote.y + 3 * curLSpace / 2;
    mMapping.left   = dx + (xOrientation - 31) * mTagSize * gSize;
    mMapping.right  = dx + (xOrientation + 29) * mTagSize * gSize;

    mMapping += mPosition;
}

void GRCluster::OnDraw(VGDevice &hdc) const
{
    if (mDraw) {
        if (noteFormatColor) {
            VGColor color(noteFormatColor);
            hdc.PushPen(color, 1);

            if (!mColRef)
                hdc.PushFillColor(color);
        }

    	if (mColRef) {
            VGColor color(mColRef);
            hdc.PushFillColor(color);
            hdc.PushPenColor(color);
        }

    	// - Quarter notes and less
    	if (gDuration < DURATION_2) {
            const float xCoords [] = {
                mMapping.left,
                mMapping.right,
                mMapping.right,
                mMapping.left};
            const float yCoords [] = {
                mMapping.top,
                mMapping.top,
                mMapping.bottom,
                mMapping.bottom};

            hdc.Polygon(xCoords, yCoords, 4);
        }
	    else {
            const float xCoords1 [] = {
                mMapping.left,
                mMapping.right,
                mMapping.right,
                mMapping.left};
            const float yCoords1 [] = {
                mMapping.top,
                mMapping.top,
                mMapping.top + 6 * mTagSize * gSize,
                mMapping.top + 6 * mTagSize * gSize};
            const float xCoords2 [] = {
                mMapping.right - 6 * mTagSize * gSize,
                mMapping.right,
                mMapping.right,
                mMapping.right - 6 * mTagSize * gSize};
            const float yCoords2 [] = {
                mMapping.top,
                mMapping.top,
                mMapping.bottom,
                mMapping.bottom};
            const float xCoords3 [] = {
                mMapping.left,
                mMapping.right,
                mMapping.right,
                mMapping.left};
            const float yCoords3 [] = {
                mMapping.bottom - 6 * mTagSize * gSize,
                mMapping.bottom - 6 * mTagSize * gSize,
                mMapping.bottom,
                mMapping.bottom};
            const float xCoords4 [] = {
                mMapping.left,
                mMapping.left + 6 * mTagSize * gSize,
                mMapping.left + 6 * mTagSize * gSize,
                mMapping.left};
            const float yCoords4 [] = {
                mMapping.top,
                mMapping.top,
                mMapping.bottom,
                mMapping.bottom};

            hdc.Polygon(xCoords1, yCoords1, 4);
            hdc.Polygon(xCoords2, yCoords2, 4);
            hdc.Polygon(xCoords3, yCoords3, 4);
            hdc.Polygon(xCoords4, yCoords4, 4);
        }

	    // - Restore context
	    if (mColRef) {
            hdc.PopPenColor();
		    hdc.PopFillColor();
        }

        if (noteFormatColor) {
            if (!mColRef)
                hdc.PopFillColor();

            hdc.PopPen();
        }
	}
}

//-------------------------------------------------------------------
void GRCluster::tellPosition(GObject * caller, const NVPoint & np)
{
	GREvent *ev = dynamic_cast<GREvent *>(caller);
	
    if (ev) {
        NVPoint newPoint (np);
        newPoint.y = -LSPACE;

        setPosition(newPoint);
	}
}

void GRCluster::setClusterOrientation(GDirection inStemDir, ARTHead::HEADSTATE inHeadStateOrientation)
{
    gClusterOrientation = inHeadStateOrientation;
    gStemDir            = inStemDir;
}

ARCluster *GRCluster::getARCluster() const
{
	return /*dynamic*/static_cast<ARCluster*>(getAbstractRepresentation());
}

void GRCluster::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent)
        SendMap(f, firstNote->getARNote()->getStartTimePosition(), gDuration, kNote, infos);
}