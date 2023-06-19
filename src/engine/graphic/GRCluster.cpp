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
#include "ARNote.h"

#include "VGDevice.h"
#include "GRTagARNotationElement.h"
#include "GRSingleNote.h"
#include "GRGlobalStem.h"
#include "ARNoteFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

GRCluster::GRCluster(GRStaff * stf, const ARCluster * arcls, GRSingleNote *sngNote, const ARNoteFormat * curnoteformat) :
						GRARCompositeNotationElement(arcls),
						GRPositionTag(arcls->getEndPosition(), arcls),
                        fDuration(0),
                        fNoteFormatColor(0),
                        fClusterOrientation(ARTHead::NORMAL),
                        fStemDir(dirAUTO)
{
	assert(stf);

    mGrStaff = stf;

    fDuration = sngNote->getDurTemplate();

    const TagParameterString *tmpColor;

    mTagSize = stf->getSizeRatio();

    firstNote = secondNote = sngNote;

    int noteFormatDx = 0;
    int noteFormatDy = 0;

    if (curnoteformat) {
        // - Size
        const TagParameterFloat * tmp = curnoteformat->getSize();
        if (tmp)
            mTagSize = tmp->getValue();

        // - Color
        tmpColor = curnoteformat->getColor();
        if (tmpColor) {
            fNoteFormatColor = new unsigned char[4];
            tmpColor->getRGB(fNoteFormatColor);
        }

        // - Offset
		const TagParameterFloat * tmpdx = curnoteformat->getDX();
		const TagParameterFloat * tmpdy = curnoteformat->getDY();

		if (tmpdx)
			noteFormatDx = (int) (tmpdx->getValue(stf->getStaffLSPACE()));
		if (tmpdy)
			noteFormatDy = (int) (tmpdy->getValue(stf->getStaffLSPACE()));
    }

    float clusterDx = mTagOffset.x;
    float clusterDy = mTagOffset.y;

    mTagOffset.x += noteFormatDx;
    mTagOffset.y += noteFormatDy;

    fdx   = mTagOffset.x;
    fdy   = mTagOffset.y;
    fhdx  = arcls->getahdx();
    fhdy  = arcls->getahdy();
    fsize = (arcls->getSize() ? arcls->getSize()->getValue() : 1.0f);

    GREvent *grEvent = dynamic_cast<GREvent *>(sngNote);
    if (grEvent) {
        grEvent->getGlobalStem()->setOffsetXY(clusterDx, - clusterDy);
        grEvent->getGlobalStem()->setMultiplicatedSize(fsize);
    }

    fNoteCount = arcls->getNoteCount();
}

GRCluster::~GRCluster() { delete [] fNoteFormatColor; }

void GRCluster::updateBoundingBox()
{
    float curLSpace = mGrStaff->getStaffLSPACE();
    float dx        = getBoundingBox().left + fdx + fhdx;
    float dy        = - fdy - fhdy;

    float xOrientation = 0;

    if (fClusterOrientation == ARTHead::LEFT && fStemDir == dirDOWN)
        xOrientation = - 55;
    else if (fClusterOrientation == ARTHead::RIGHT && fStemDir == dirUP)
        xOrientation =   55;

    NVPoint pFirstNote  = (firstNote->getPosition().y < secondNote->getPosition().y ? firstNote->getPosition() : secondNote->getPosition());
    NVPoint pSecondNote = (firstNote->getPosition().y > secondNote->getPosition().y ? firstNote->getPosition() : secondNote->getPosition());

    mMapping.top    = dy + pFirstNote.y + curLSpace / 2;
    mMapping.bottom = dy + pSecondNote.y + 3 * curLSpace / 2;
    mMapping.left   = dx + (xOrientation - 31) * mTagSize * fsize;
    mMapping.right  = dx + (xOrientation + 29) * mTagSize * fsize;

    mMapping += mPosition;
}

void GRCluster::OnDraw(VGDevice &hdc) const
{
	if (!mDraw || !mShow) return;

	if (fNoteFormatColor) {
		VGColor color(fNoteFormatColor);
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
	if (fDuration < DURATION_2) {
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
			mMapping.top + 6 * mTagSize * fsize,
			mMapping.top + 6 * mTagSize * fsize};
		const float xCoords2 [] = {
			mMapping.right - 6 * mTagSize * fsize,
			mMapping.right,
			mMapping.right,
			mMapping.right - 6 * mTagSize * fsize};
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
			mMapping.bottom - 6 * mTagSize * fsize,
			mMapping.bottom - 6 * mTagSize * fsize,
			mMapping.bottom,
			mMapping.bottom};
		const float xCoords4 [] = {
			mMapping.left,
			mMapping.left + 6 * mTagSize * fsize,
			mMapping.left + 6 * mTagSize * fsize,
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

	if (fNoteFormatColor) {
		if (!mColRef)
			hdc.PopFillColor();

		hdc.PopPen();
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
    fClusterOrientation = inHeadStateOrientation;
    fStemDir            = inStemDir;
}

const ARCluster *GRCluster::getARCluster() const
{
	return /*dynamic*/static_cast<const ARCluster*>(getAbstractRepresentation());
}

void GRCluster::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent || sel == kGuidoBarAndEvent)
        SendMap(f, firstNote->getARNote()->getRelativeTimePosition(), fDuration, kNote, infos);
}
