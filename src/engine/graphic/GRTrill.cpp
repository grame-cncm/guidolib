/*
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr


	-- Rules of ornaments --
	
	(1) Ornament symbols are placed above the staff regardless of stem direction
	(2) When two parts share a staff, an ornament symbol applied to the lower part is placed
		below the staff

*/

#include <algorithm>
#include <iostream>
#include <map>

#include "ARTrill.h"
#include "ARNote.h"
#include "TagParameterFloat.h"

#include "GRAccidental.h"
#include "GRStaff.h"
#include "GRSystem.h"
#include "GRTrill.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"
#include "GRStem.h"

#include "VGDevice.h"
#include "VGFont.h"


using namespace std;

#define kAccidentalSize		0.6f

//-------------------------------------------------------------------------------
GRTrill::GRTrill(GRStaff * inStaff, const ARTrill * ar ) : GRPTagARNotationElement(ar) //, fEmptyMarker(0)
{
	assert(inStaff);
//	setGRStaff(inStaff);
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();
	addSystemStartEndStruct (sse);

	fType = ar->getOrnament();

	ARTrill::TYPE type = ar->getType();
	fBegin = ar->getStatus();
	fShowTR = ar->getShowTR();
	fPosition = ar->getTrillPos();
	fTildeWidth = GObject::GetSymbolExtent(kTilde)*mTagSize;
	
	// - Creation of the accidental symbol -
	fAccidental = createAccidental (ar);
	fAccidental->setOffsetX(this->getOffset().x + ar->getadx());
	fAccidental->setOffsetY(this->getOffset().y - ar->getady()); // this offset affects each type of ornament
	
	switch ( fType ) {
		case ARTrill::TRILL:
			setupTrill();
			break;

		case ARTrill::TURN:
			switch (type) {
				case ARTrill::kInverted:			setupInvertedTurn();
					break;
				default :							setupTurn();
			}
			break;

		case ARTrill::MORD:
			switch (type) {
				case ARTrill::kInverted:		setupInvertedMord();
					break;
				case ARTrill::kPrallInverted:	setupPrallMordent();
					break;
				case ARTrill::kPrallPrall:		setupPrallPrall();
					break;
				default :						setupMord();
			}
			break;
	}
}

//-------------------------------------------------------------------------------
GRTrill::~GRTrill()
{
	delete fAccidental;
}

//-------------------------------------------------------------------------------
void GRTrill::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	visitor.visitEnd (this);
}

//-------------------------------------------------------------------------------
GRAccidental * GRTrill::createAccidental (const ARTrill * ar) const
{
	float acc = ar->getAccidental();
	bool cautionary = ar->getCautionary();
	GRAccidental* gr = (acc == kNoAccidental) ? new GRAccidental() : new GRAccidental(acc, cautionary, kAccidentalSize);
	return gr;
}

//-------------------------------------------------------------------------------
NVRect GRTrill::getEnclosingBox() const		// gives a rect that enclose the ornament including accidentals
{
	NVRect outrect = getBoundingBox();
	outrect += getPosition();
	if (fAccidental) {
		NVRect r = fAccidental->getBoundingBox();
		r *= fAccidental->getSize();
		r += fAccidental->getPosition();
		outrect.Merge(r);
	}
	return outrect;
}

//-------------------------------------------------------------------------------
/** \brief Manage the drawing of trill line
*/
//-------------------------------------------------------------------------------
void GRTrill::OnDraw(VGDevice & hdc , float right, float noteY, int nVoice) const
{
	return;
}

extern GRSystem * gCurSystem;

//-------------------------------------------------------------------------------
NVRect GRTrill::getCurrentRect (const GRSystem* sys) const
{
	NVRect currentrect;
	for (size_t i = 0; i < fDrawRects.size(); i++) {
		if (fDrawRects[i].first == gCurSystem) {
			currentrect = fDrawRects[i].second;
			break;
		}
	}
	return currentrect;
}

//-------------------------------------------------------------------------------
void GRTrill::OnDraw( VGDevice & hdc) const
{
	if (!mDraw || !mShow) return;
	GRSystemStartEndStruct * sse = getSystemStartEndStruct(gCurSystem);
	if (sse == 0) return;
	
	const ARTrill * ar = static_cast<const ARTrill *>(mAbstractRepresentation);
	NVRect currentrect = getCurrentRect(gCurSystem);

	GRTrill* nextTrill = dynamic_cast<GRTrill*>(fNextEvent);
	float endTrill = currentrect.Width();
	float offset = fStartOffset;
	if (nextTrill && (nextTrill->getPosition().x < getPosition().x)) {	// this is a line break
		endTrill = gCurSystem->getBoundingBox().right; // - getPosition().x;
		offset = 0;
	}
	else if ((sse->endflag == GRSystemStartEndStruct::RIGHTMOST) && fNextEvent) {
		endTrill = fNextEvent->getPosition().x;
		if (nextTrill) {
			if (nextTrill->getBegin()) {
				endTrill += fNextEvent->getBoundingBox().left - LSPACE;
			}
		}
		else if (fNextEvent->isGRNote()) {
//			endTrill += fNextEvent->getBoundingBox().left;
			endTrill -= LSPACE/3;
		}
	}
	else endTrill = gCurSystem->getBoundingBox().right;
	endTrill -= ar->getDX()->getValue();

	float x = fStartOffset;
	bool drawTr = false;
	if (sse->startflag == GRSystemStartEndStruct::LEFTMOST)
		drawTr = fShowTR && fBegin;
	else x = currentrect.left - getPosition().x - ar->getDX()->getValue();

	bool drawAcc = (sse->startflag == GRSystemStartEndStruct::LEFTMOST);
	if (!fBegin && ar->getIsAuto()) drawAcc = false;

	VGColor oldColor = hdc.GetFontColor();
	if (mColRef) hdc.SetFontColor(VGColor(mColRef));

	if (fType == ARTrill::TRILL) {
		if (drawTr) {
			GRNotationElement::OnDraw(hdc);
			x += getBoundingBox().Width() * mTagSize;
		}
		if (drawAcc) fAccidental->OnDraw(hdc);

		if (fTildeWidth != 0) {		// width of tilde may be 0 if font is not installed for example
			float nextPoint = x + fTildeWidth + getPosition().x;
			while (nextPoint <= endTrill) {
				GRNotationElement::OnDrawSymbol(hdc, kTilde, x, 0, mTagSize);
				x += fTildeWidth;
				nextPoint += fTildeWidth;
			}
			if (nextTrill && !nextTrill->getBegin() && (nextTrill->getPosition().x > getPosition().x)) {
				float offset = getPosition().x + x - nextTrill->getPosition().x;
				nextTrill->setStartOffset (offset);
			}
		}
//		float w = LSPACE/3;
//		hdc.Rectangle(endTrill-w, -LSPACE-w, endTrill+w, -LSPACE+w);
	}
	else {
		if (drawTr) GRNotationElement::OnDraw(hdc);
		if (drawAcc) fAccidental->OnDraw(hdc);
	}
	if (mColRef) hdc.SetFontColor(oldColor);
}

//-------------------------------------------------------------------------------
const NVPoint & GRTrill::getReferencePosition() const
{
	return sRefPos;
}

extern GRSystem * gCurSystem;

//-------------------------------------------------------------------
#ifndef WIN32
#warning ("TODO: report getAllAssociatedBoundingBox to GRNotationElement")
#endif
GRTrill::TDrawRects GRTrill::getAllAssociatedBoundingBox (const NEPointerList * assoc, const GRStaff* staff) const
{
	TDrawRects out;

	const GRSystem* last = 0;
	NVRect bb;
	float lspace = staff->getStaffLSPACE();
	GuidoPos pos = assoc->GetHeadPosition();
	while (pos) {
		const GRNotationElement* elt = mAssociated->GetNext(pos);
		const GRSystem* system = elt->getGRStaff()->getGRSystem();
		const GRSingleNote* note = elt->isSingleNote();
		if (note) {
			if (last && (system != last)) {
				bb.right = last->getBoundingBox().right - lspace;
				out.push_back( make_pair(last, bb));
				bb.Set (0,0,0,0);
			}
			NVRect r = note->getEnclosingBox(false, true, false);
//			NVRect r = getElementBoundingBox(note);
			r.top -= lspace;
			r.bottom += lspace*1.5f;
			r.left += note->getNoteWidth()/2;
			bb.Merge (r);
		}
		last = system;
	}
	if (last) {
		bb.right = last->getBoundingBox().right - lspace;
		out.push_back( make_pair(last, bb));
	}
	return out;
}

//-------------------------------------------------------------------
void GRTrill::tellPosition( GObject * caller, const NVPoint & np )
{
	GRSystemStartEndStruct * sse = getSystemStartEndStruct( gCurSystem );
	if( sse == 0) return; // don't draw

//	const ARTrill * ar = static_cast<const ARTrill *>(mAbstractRepresentation);
	const GRNotationElement* elt = dynamic_cast<const GRNotationElement*>(caller);
	NVRect bb;
	map<const GRSystem*, NVRect> bbs;
	if (elt == sse->startElement) {
		setPosition (np);
	}
	if (elt == sse->endElement) {
		fAccidental->setSize(mTagSize/2);
		const GRStaff* staff = elt->getGRStaff();
		float staffLSpace = staff->getStaffLSPACE();
		if (mAssociated) {
			fDrawRects = getAllAssociatedBoundingBox (mAssociated, elt->getGRStaff());
		}
		else mDraw = false;

		if (fPosition == ARTrill::BELOW) {
			float miny = staff->getDredgeSize() + staffLSpace * 2.5f;
			for_each(fDrawRects.begin(), fDrawRects.end(), [miny] (pair<const GRSystem*, NVRect>& e)
				-> void { if (e.second.bottom < miny) e.second.bottom = miny; });
		}
		else {
			float maxy = - staffLSpace;
			for_each(fDrawRects.begin(), fDrawRects.end(), [maxy] (pair<const GRSystem*, NVRect>& e)
				-> void { if (e.second.top > maxy) e.second.top = maxy; });
		}

		if (fDrawRects.size()) {
			NVRect r = fDrawRects[0].second;
			float y = (fPosition == ARTrill::BELOW) ? r.bottom : r.top;
			NVPoint pos = NVPoint(r.left, y);
			setPosition (pos);
			pos.x += GetSymbolExtent(mSymbol)*kAccidentalSize + 0.4f*LSPACE;
			if (!fBegin) pos.x -= LSPACE;
			pos.y -= 1.5f*LSPACE;
			fAccidental->setPosition(pos);
		}
	}
}

//----------------------------------------------------------------
void GRTrill::placeMord (GREvent * inParent, NVPoint & newPoint)
{
	// Checks the direction of the stems
	const GDirection dir = chooseDirection( inParent ); // dir = stemDir
	const GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	const float halfSpace = float(0.5) * currLSpace;
	
	if( dir == dirUP )
	{
		if (newPoint.y >= inParent->getStemLength())
			newPoint.y = -halfSpace;
		else
			newPoint.y -= (halfSpace + inParent->getStemLength());
	}
	else	// stemDown
	{
		if ( newPoint.y > 0)	newPoint.y = -halfSpace;
		else					newPoint.y -= halfSpace;
	}
}

//-------------------------------------------------------------------
GDirection GRTrill::chooseDirection (GREvent * inParent ) const
{
	const GRSingleNote * sng = dynamic_cast<GRSingleNote *>(inParent);
	GDirection direction = sng->getStemDirection(); // was getDirection()
	return direction;
}

//-------------------------------------------------------------------
unsigned int GRTrill::getTextAlign() const
{
	return (VGDevice::kAlignLeft | VGDevice::kAlignBase);
}

//-------------------------------------------------------------------
/** \brief Memorize the last position of trill line for each voice
*/
float & GRTrill::getLastPosX(int i){
	static std::map<int, float> lastPosX;
	return lastPosX[i];
}


//-------------------------------------------------------------------------------
void GRTrill::setupTrill()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kTrillSymbol);
	NVPoint offsetAccidental = NVPoint(0.5f * hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 1.17f;
	sRefPos = NVPoint(-mLeftSpace, 0);	
	mBoundingBox.Set(-mLeftSpace, -height, mRightSpace + 0.24f * LSPACE, 0);
}

void GRTrill::setupTurn()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kTurnSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.98f;
	sRefPos = NVPoint(-mLeftSpace, 0); 
	mBoundingBox.Set(-mLeftSpace, -height/2, mRightSpace, height/2);
}

void GRTrill::setupInvertedTurn()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kInvertedTurnSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.98f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height/2, mRightSpace, height/2);
}

//void GRTrill::setupTurnComplex()
//{
//	const float hs = LSPACE * 0.5f;
//	setTrillSymbol (kTurnSymbol);
//	NVPoint offsetAccidental = NVPoint(2*hs, 0);
//	fAccidental->addToOffset(offsetAccidental);
//	
//	const float height = LSPACE * 0.98f;
//	sRefPos = NVPoint(LSPACE * 1.5f, 0);
//	mBoundingBox.Set(1.5f*LSPACE, -height/2, 2*mRightSpace+1.5f*LSPACE, height/2);
//}

//void GRTrill::setupInvertedTurnComplex()
//{
//	const float hs = LSPACE * 0.5f;
//	setTrillSymbol (kInvertedTurnSymbol);
//	NVPoint offsetAccidental = NVPoint(2*hs, 0);
//	fAccidental->addToOffset(offsetAccidental);
//	
//	const float height = LSPACE * 0.98f;
//	sRefPos = NVPoint(LSPACE * 1.5f, 0);
//	mBoundingBox.Set(1.5f*LSPACE, -height/2, 2*mRightSpace+1.5f*LSPACE, height/2);
//}

void GRTrill::setupMord()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kMordSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, -hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.97f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height, mRightSpace, 0);
}

void GRTrill::setupInvertedMord()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kInvertedMordSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, -1.5f*hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 1.6f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height+0.36f * LSPACE, mRightSpace, 0.36f * LSPACE);
}

void GRTrill::setupPrallMordent()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kPrallMordentSymbol);
	NVPoint offsetAccidental = NVPoint(-4*hs, -hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 1.31f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height+0.19f*LSPACE, mRightSpace, 0.19f*LSPACE);
}

void GRTrill::setupPrallPrall()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kPrallPrallSymbol);
	NVPoint offsetAccidental = NVPoint(-4*hs, -hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.95f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height, mRightSpace, 0);
}

void GRTrill::setTrillSymbol( unsigned int inSymbolID )
{
	mSymbol = inSymbolID;
	const float extent = GetSymbolExtent( inSymbolID );
	mLeftSpace = (extent * 0.5f);
	mRightSpace = (extent * 0.5f);
}

