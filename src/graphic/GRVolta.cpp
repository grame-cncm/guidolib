/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <iostream>
#ifdef VOLTADEBUG
#include <fstream>
ofstream vdebug("voltadebug.txt");
#endif
using namespace std;

#include "ARMusicalObject.h"
#include "ARMusicalTag.h"
#include "ARVolta.h"
#include "GRBar.h"
#include "GRDefine.h"
#include "GREvent.h"
#include "GRGlobalStem.h"
#include "GRNote.h"
#include "GRRest.h"
#include "GRStem.h"
#include "GRFlag.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRVolta.h"

#include "kf_ivect.h"
#include "VGDevice.h"
#include "FontManager.h"

std::map<int, GRVolta *> GRVolta::mPrevious;

NVPoint GRVolta::refpos;

GRVolta::GRVolta( GRStaff * inStaff, ARVolta * ar )
		: GRPTagARNotationElement( ar )
{
	mPrevious.clear();
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem();
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;

	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);
    mEnd = mBeg = 0;
    mBroken = false;
    mCurrSystem = 0;
    mShape = kDefault;
    if (!ar->getFormat().empty()) {
        if (ar->getFormat() == "|-")
            mShape = kRightOpened;
        else if (ar->getFormat() == "-|")
            mShape = kLeftOpened;
        else if (ar->getFormat() == "-")
            mShape = kOpened;
    }
    mString = ar->getString().c_str();
    mStringSize = int(ar->getString().size());
}

GRVolta::~GRVolta()
{
	FreeAssociatedList();
}

// -----------------------------------------------------------------------------
/*!
    This method makes final adjustments to the volta bounding boxes.
    The following strategy is applied:
    - when all the elements of a measure are enclosed in the volta, it is
      left extended to the measure beginning and right extended to the measure end
    - when only some elements are enclosed (non standard use), the volta is
      right extended to the left of the nexr element.

    The same rules are applied to each part of a broken volta (ie a volta that
    covers measures over several systems).

    \note broken volta is supported only over 2 systems. Writting a volta that may
    cover more than 2 systems (non standard use) may result in incorrect graphic
    rendering.
*/
void GRVolta::FinishPTag (GRStaff * staff)
{
    if (!staff) return;

    NEPointerList * assoc = getAssociations();
    if (!assoc) return;
    GRNotationElement * first = assoc->front();
    if (!first) return;


    NVRect r;
    if (mBroken) {
        if( first->getGRStaff() == staff) {
            if (mBeg) {
                GRSystem * system = staff->getGRSystem();
                r = mBeg->getBoundingBox();
                mFirstPosition.x = mBeg->getPosition().x + r.right - r.left;
                r = system->getBoundingBox();
                r += system->getPosition();
                mFirstPart.right = r.right - mFirstPosition.x;
                adjustPrevious (staff);
            }
        }
        else if (mEnd) adjustRight (mEnd);
    }
    else {
        if (mEnd) adjustRight (mEnd);
        if (mBeg) {
            r = mBeg->getBoundingBox();
			r += mBeg->getPosition();
            mBoundingBox.right += mPosition.x - r.right;
            mPosition.x = r.right;
            adjustPrevious (staff);
        }
    }
   mPrevious[staff->getStaffNumber()] = this;
}

// -----------------------------------------------------------------------------
/*!
    This method makes contextual adjustments to adjacent voltas.
    When two \p volta are applied to adjacent measures on the same system,
    the voltas are constrained to the same highest y position.
    This rule is not applied if any of the two voltas is manually positionned
    using a dy parameter.

    \note adjacent voltas are detected using a static map which is not suitable
    for parallel concurrent scores evaluation.

*/
void GRVolta::adjustPrevious(const GRStaff * staff)
{
    GRVolta * volta = mPrevious[staff->getStaffNumber()];
    if (!volta) return;

    GRStaff * previous = staff->getPreviousStaff();
    if (!previous) return;
    NEPointerList * staffEvs = previous->getElements();
    if (!staffEvs) return;

    // don't adjust position if the previous volta right position is not the measure end
    if (volta->getEndPos() != staffEvs->back()) return;
    // don't adjust position if the previous volta position is manually specified
    if (volta->getOffset().y) return;
    // don't adjust position if my position is manually specified
    if (getOffset().y) return;

    if (mPosition.y < volta->getPosition().y) {
        // adjust previous volta position
        NVPoint p(volta->getPosition().x, mPosition.y);
        volta->setPosition(p);
        volta->adjustPrevious(previous);
    }
    else if (volta->getPosition().y < mPosition.y) {
        // adjust myself
        mPosition.y = volta->getPosition().y;
        if (mBroken) mFirstPosition.y = mPosition.y;
    }
}

// -----------------------------------------------------------------------------
void GRVolta::adjustRight(const GRNotationElement *endElt)
{
    NVRect r = endElt->getBoundingBox();
    r += endElt->getPosition();

    NVRect tmp = mBoundingBox;
    tmp += mPosition;

    mBoundingBox.right += r.left - tmp.right;
    mBoundingBox.right -= LSPACE / 4;
}

// -----------------------------------------------------------------------------
void GRVolta::tellPosition(GObject * caller, const NVPoint & newPosition)
{
	GRNotationElement * grne = dynamic_cast<GRNotationElement *>(caller);
	if (grne == 0 ) return;
    NEPointerList * assoc = getAssociations();
	if (assoc == 0 ) return;

	bool newSystem = mCurrSystem && (mCurrSystem != grne->getGRStaff()->getGRSystem());
    if (newSystem) {
        mFirstPart = mBoundingBox;
        mFirstPosition = mPosition;
        mBroken = true;
    }

    if( (!mBeg && (grne == assoc->front())) || newSystem) {
        // this is the first element on a system, we collect the current system,
        // initialize the bounding box and set the position
        mCurrSystem = grne->getGRStaff()->getGRSystem();
        mBoundingBox.Set (0, -2*LSPACE, 0, 0);
        mPosition.x = grne->getBoundingBox().left + grne->getPosition().x;
        mPosition.y = getOffset().y;
        if (mBroken) {
            // this is to ensure the same y position for broken parts
            if (mPosition.y < mFirstPosition.y) mFirstPosition.y = mPosition.y;
            else if (mFirstPosition.y < mPosition.y) mPosition.y = mFirstPosition.y;
        }
        // here we get the element where the drawing should start
        if (!mBeg) mBeg = getBegElt(grne);
    }

    else if (grne == assoc->back()) {
        // this is the last element call, we go thru the associated elements
        GuidoPos pos = assoc->GetHeadPosition();
        while (pos) {
            GRNotationElement * e = assoc->GetNext(pos);
            if (!e) break;      // this shouldn't happend

            // ignore elements not on the current system
            GRStaff * staff = e->getGRStaff();
			if (!staff) continue;
			if (staff->getGRSystem() != mCurrSystem) continue;

            // we get the element positionned bouding box
            NVRect r = e->getBoundingBox();
            r += e->getPosition();
            // and we move the volta bounding box to the element right position
			mBoundingBox.right = r.right - e->getGRStaff()->getPosition().x;

            // the next part computes the y position, it is ignored when an y offset has been specified
            if (getOffset().y) continue;

            if (r.top < mPosition.y) mPosition.y = r.top;

            if (mBroken) {
                // this is to ensure the same y position for broken parts
                if (mPosition.y < mFirstPosition.y) mFirstPosition.y = mPosition.y;
                else if (mFirstPosition.y < mPosition.y) mPosition.y = mFirstPosition.y;
            }

            // special computation for chords that share a stem
            GREvent * ev = dynamic_cast<GREvent *>(e);
            if (!ev) continue;
            GRGlobalStem * gstem = ev->getGlobalStem();
            if (!gstem) continue;

            GRStem * stem = gstem->getGRStem();
            if (stem) {
                r = stem->getBoundingBox();
                r += stem->getPosition();
                if (r.top < mPosition.y) mPosition.y = r.top;
            }
        }
        // special computation for chords that share a stem
        mEnd = getEndElt(assoc->back());
   }
}

// -----------------------------------------------------------------------------
GRNotationElement * GRVolta::getBegElt(GRNotationElement *before) {
    if (!before) return 0;
    GRStaff * staff = before->getGRStaff();
    if (!staff) return 0;
    NEPointerList * staffEvs = staff->getElements();
    if (!staffEvs) return 0;

    // computes now the end range next element
    GuidoPos pos = staffEvs->GetElementPos(before);
    while (pos) {
        GRNotationElement * elt = staffEvs->GetPrev(pos);
        if (elt == before) continue;
        if (dynamic_cast<GRNote *>(elt) || dynamic_cast<GRRest *>(elt))
            return 0;   // range beg is not at the beginning of the measure: do nothing
    }

    // here we'll look for the previous staff (measure)
    staff = staff->getPreviousStaff();
    return staff ? staff->getElements()->GetTail() : 0;        // and returns the last staff element
}

// -----------------------------------------------------------------------------
GRNotationElement * GRVolta::getEndElt(GRNotationElement *after) {
    if (!after) return 0;
    GRStaff * staff = after->getGRStaff();
    if (!staff) return 0;
    NEPointerList * staffEvs = staff->getElements();
    if (!staffEvs) return 0;

    // computes now the end range next element
    GuidoPos pos = staffEvs->GetElementPos(after);
    while (pos) {
        GRNotationElement * elt = staffEvs->GetNext(pos);
        if (elt == after) continue;
        if (dynamic_cast<GRBar *>(elt) || dynamic_cast<GRNote *>(elt) || dynamic_cast<GRRest *>(elt)) {
            return elt;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
void GRVolta::OnDraw( VGDevice & hdc ) const
{
//  DrawBoundingBox(hdc, VGColor(0,255,0));
	if(!mDraw)
		return;
    static bool start=true;
//    hdc.PushPen( VGColor( 0, 0, 0, ALPHA_OPAQUE ), 4);	//opaque black
    hdc.PushPenWidth(4);
    int shape = mShape;
    NVRect r = getBoundingBox();
    r += getPosition ();

    bool drawText= true;
    if (mBroken) {
        if (start) {
            r = mFirstPart;
            r += mFirstPosition;
            switch (mShape) {
                case kDefault:      shape = kRightOpened;   break;
                case kLeftOpened:   shape = kOpened;        break;
            }
            start = false;
        }
        else {
            switch (mShape) {
                case kDefault:      shape = kLeftOpened; break;
                case kRightOpened:  shape = kOpened;     break;
            }
            drawText= false;
            start = true;
        }
    }

    float bottom = r.bottom - (LSPACE/2);
    hdc.Line (r.left, r.top, r.right, r.top);
    switch (shape) {
        case kDefault:
            hdc.Line (r.left, r.top, r.left, bottom);
            hdc.Line (r.right, r.top, r.right, bottom);
            break;
        case kLeftOpened:
            hdc.Line (r.right, r.top, r.right, bottom);
            break;
        case kRightOpened:
            hdc.Line (r.left, r.top, r.left, bottom);
            break;
    }
//    hdc.PopPen();
    hdc.PopPenWidth();

    if (drawText) {
//        const VGColor prevTextColor = hdc.GetFontColor();
        hdc.SetTextFont( FontManager::gFontText );
//        hdc.SetFontColor( VGColor( 0, 0, 0, ALPHA_OPAQUE ) );	//opaque black
        hdc.SetFontAlign (VGDevice::kAlignBaseLeft);
        hdc.DrawString( r.left + LSPACE/2, bottom, mString, mStringSize );
//        hdc.SetFontColor( prevTextColor );
    }
}

