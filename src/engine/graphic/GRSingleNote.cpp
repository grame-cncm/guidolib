/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003-2011  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <algorithm>

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

// - Guido Misc
#include "GuidoDefs.h"
#include "NVPoint.h"
#include "VGDevice.h"
#include "VGFont.h"

// - Guido AR
#include "ARNoteFormat.h"
#include "ARDotFormat.h"
#include "ARAccidental.h"
#include "ARNote.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterRGBColor.h"

// - Guido GR
#include "GRSingleNote.h"
#include "GRNote.h"
#include "GRAccidental.h"
#include "GRArticulation.h"
#include "GRStdNoteHead.h"
#include "GRNoteDot.h"
#include "GRStem.h"
#include "GRGlobalStem.h"
#include "GRFlag.h"
#include "GRStaff.h"
#include "GRSpecial.h"
#include "GRSystem.h"
#include "GRTrill.h"
#include "GRCluster.h"
#include "GRTuplet.h"
#include "GRPage.h"
#include "GRSlur.h"
#include "secureio.h"

using namespace std;

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		cout << (void*)this << " GRSingleNote::" << method << endl
#else
#define traceMethod(method)	
#endif

float GRSingleNote::fLedgeWidth = 0.f;

//____________________________________________________________________________________
GRSingleNote::GRSingleNote( GRStaff* inStaff, const ARNote* arnote, const TYPE_TIMEPOSITION& pos, const TYPE_DURATION& dur)
  : GRNote( inStaff, arnote, pos, dur), mNumHelpLines(0),
  mStemDir(dirAUTO), mStemDirSet(false), mHeadState(ARTHead::NOTSET), mNoteAppearance(arnote->getAppearance())
{
	// builds a grafical sub-part of abstractRepresentation
	assert(arnote);

	mNoteBreite = 0;
	// depending on standard ...
	mStemLen = (3.5f * inStaff->getStaffLSPACE()); // HARDCODED
	mStemLengthSet = false;
	mNoteHead = 0;
	// the note is NOT created!
}

// -----------------------------------------------------------------------------
void GRSingleNote::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
//	if (fOrnament) fOrnament->accept(visitor);
	GRNEList& articulations = getArticulations();
	for_each(articulations.begin(),  articulations.end(), [&visitor] (GRNotationElement *e) -> void { e->accept(visitor); });
	visitor.visitEnd (this);
}

//____________________________________________________________________________________
/** \brief this acutally creates the note.
*/
void GRSingleNote::doCreateNote( const TYPE_DURATION & p_durtemplate )
{
	createNote(p_durtemplate);
}

//____________________________________________________________________________________
void GRSingleNote::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
    if (fCluster) {
        if (this == fCluster->getFirstNote())
            fCluster->GetMap(sel, f, infos);
    }
    else if (sel == kGuidoEvent || sel == kGuidoBarAndEvent) {
		TYPE_DURATION dur = getDuration();
		if (dur.getNumerator() == 0) {		// notes in chords have a null duration
			dur = getDurTemplate();
        }
        SendMap (f, getARNote()->getRelativeTimePosition(), dur, (isGraceNote() ? kGraceNote : kNote), infos);
	}
}

//____________________________________________________________________________________
float GRSingleNote::getLedgeWidth (VGDevice & hdc) const
{
	if (!fLedgeWidth) {
		const VGFont* font = hdc.GetMusicFont();
		if (font) {
			float foo;
			font->GetExtent(kLedgerLineSymbol, &fLedgeWidth, &foo, &hdc);
		}
	}
	return fLedgeWidth;
}


//____________________________________________________________________________________
// draw ledger lines
void GRSingleNote::drawLedges (VGDevice & hdc) const
{
    float incy = 1;
    float posy = 0;
    int count = mNumHelpLines;
    if (!count) return;

	GRStdNoteHead * head = getNoteHead();
	NVRect r = head->getBoundingBox() + head->getNoteHeadPosition();
//	hdc.Frame(r.left, r.top, r.right, r.bottom);

    if (mNumHelpLines > 0) { 	// ledger lines up
        incy = -mCurLSPACE;
        posy = -mCurLSPACE;
    }
    else if (mNumHelpLines < 0) {
        incy = mCurLSPACE;
        posy = mGrStaff->getNumlines() * mCurLSPACE;
        count = - count;
    }
	hdc.SetFontAlign(VGDevice::kAlignLeft | VGDevice::kAlignBase);

#ifdef SMUFL
    float ledXPos = -mCurLSPACE * 0.65;;
#else
    float ledXPos = - 60 * 0.85f * mSize;
#endif
	
	bool largeledge = false;
	if (!getDuration() && !fClusterNote)  { // denotes a chord and not a cluster
		const float xPos = mPosition.x + getOffset().x + getReferencePosition().x + ledXPos;
		if (xPos > r.left) {
			largeledge = true;
			ledXPos -= r.Width() * 0.8;
		}
		else if ((xPos +  getLedgeWidth(hdc)) < r.right) {
			largeledge = true;
		}
	}
//cerr << "GRSingleNote::drawLedges note " << this << " " << largeledge << " " << r << endl;
    for (int i = 0; i < count; ++i, posy += incy) {
		GRNote::DrawSymbol(hdc, largeledge ? kLedgerLargeSymbol : kLedgerLineSymbol, ledXPos, posy - mPosition.y);
	}
}

//____________________________________________________________________________________
void GRSingleNote::OnDraw( VGDevice & hdc) const
{
	if (!mDraw || !mShow) return;

	const VGColor prevFontColor = hdc.GetFontColor();
	const unsigned char* staffColor = mGrStaff->getStffrmtColRef();
    if (staffColor)
        hdc.SetFontColor(VGColor(staffColor));

    // draw ledger lines
    drawLedges(hdc);

    if (staffColor)
        hdc.SetFontColor(prevFontColor);
	if (fCluster)
		getNoteHead()->setHaveToBeDrawn(false);

	const VGColor oldcolor = hdc.GetFontColor();
	if (mColRef)
        hdc.SetFontColor(VGColor(mColRef));

	// - Draw elements (stems, dots...)
    if (getARNote()->haveSubElementsToBeDrawn())
	    DrawSubElements(hdc);

	// - draw articulations & ornament
	const GRNEList& articulations = getArticulations();
	for (GRNEList::const_iterator ptr = articulations.begin(); ptr != articulations.end(); ++ptr) {
		GRNotationElement *el = *ptr;
		el->OnDraw(hdc);
	}

	if (mColRef) 						hdc.SetFontColor(oldcolor);
	if (gBoundingBoxesMap & kEventsBB) 	DrawBoundingBox(hdc, kEventBBColor);
	if (fClusterHaveToBeDrawn) 			fCluster->OnDraw(hdc);
	
//	NVRect r = getEnclosingBox (false, false, true);
//	hdc.Frame(r.left, r.top, r.right, r.bottom);


//cerr << "GRSingleNote::OnDraw" << endl;
//	float x, y;
//	TYPE_TIMEPOSITION d = getRelativeTimePosition();
//	d += float(getDuration()) / 2;
//	GuidoDate date;
//	date.num = d.getNumerator();
//	date.denom = d.getDenominator();
//	const ARNote* ar = getARNote();
//	int pitch = ar->getMidiPitch();
//	GuidoErrCode err = GuidoGetPitchPos( gDebugGr, 1, pitch, date, x, y);
//	if (err == guidoNoErr) {
//		float w = 25;
//cerr << "GRSingleNote::OnDraw  pitch " << pitch << " pos at " << date << " " << x << " " << y <<  " note pos: " << getPosition() <<  endl;
//		hdc.PushFillColor(VGColor(250,0,0, 180));
//		hdc.Rectangle(x-w, y-w, x+w, y+w);
//		hdc.PopFillColor();
//	}
//	else
//cerr << "GRSingleNote::OnDraw  pitch err " << err << endl;


}

//____________________________________________________________________________________
void GRSingleNote::updateBoundingBox()
{
	// - Call inherited
	GRNote::updateBoundingBox();

	// - Check for ledger lines
	//  (note that the bounding box does not take account of ledger lines, for now)
	if (mNumHelpLines > 0) {
		const float theSize = (mNoteBreite * 0.8f * mSize); // harcoded

		if (mLeftSpace < theSize)
            mLeftSpace  = theSize;
		if (mRightSpace < theSize)
            mRightSpace = theSize;
	}

	// - Check for stem
	if (mGlobalStem) {
		GRNotationElement * e = mGlobalStem->getGRFlag();
		if(e) {
			if (e->getRightSpace() > mRightSpace)
				mRightSpace = e->getRightSpace();

			if (e->getLeftSpace() > mLeftSpace)
				mLeftSpace = e->getLeftSpace();
		}
	}
	mBoundingBox -= getOffset();

	float scale = getSize();
	mMapping.left = mPosition.x - mNoteBreite / 2 * scale;
	mMapping.right = mMapping.left + mNoteBreite * scale;
	mMapping.top = mPosition.y - LSPACE/2 * scale;
	mMapping.bottom = mMapping.top + LSPACE * scale;
	mMapping += getOffset();
}

//____________________________________________________________________________________
/** \brief Creates the several parts of a note.

	durtemplate is used if the note is displayed
	different to it's real length (i.e. for n-tuplets)
*/
void GRSingleNote::createNote(const TYPE_DURATION & p_durtemplate)
{
	// if template-display not wanted select normal length
	mDurTemplate = p_durtemplate;
	if (mDurTemplate <= DURATION_0) {
		mNumHelpLines = 0;
		return;		// this just does nothing more
	}

	const ARNote * arNote = getARNote();
	const int pitch = arNote->getPitch();
	const int octave = arNote->getOctave() - arNote->getOctava();
	GDirection tmpdir =  mGrStaff->getDefaultThroatDirection( pitch, octave );

	// the creation of the standard elements ....
	// creates notehead, depends on length or templateLength
	mNoteHead = new GRStdNoteHead(this, mDurTemplate, tmpdir);
	AddTail(mNoteHead);

	// this is dependant on size!
	mNoteBreite = ((mNoteHead->getLeftSpace() + mNoteHead->getRightSpace()) / (float) mSize);
	float tmplength = mStemLen;
	// the Stem-Straight-Flag is not used everywhere

	GRStem *tmp = NULL;
	GDirection stemTmpDirection = tmpdir;

	if (mGlobalStem == 0) {
		if (mStemDir == dirAUTO) {
			tmp =  new GRStem( this, mDurTemplate, stemTmpDirection, tmplength, mNoteBreite); // was 60?);
			mStemDir = tmp->getStemDir();
		}
		else {
			stemTmpDirection = mStemDir;
			tmp = new GRStem( this, mDurTemplate, stemTmpDirection, tmplength, mNoteBreite ) ; // was 60? );
			mStemDirSet = true;
		}

		mStemLen = tmp->getStemLength();
		AddTail(tmp);

		// here we have to add the flags ...
		GRFlag * tmpflag = new GRFlag( this, mDurTemplate, mStemDir, mStemLen, mNoteBreite ); // was 60?);

		AddTail(tmpflag);

        forceAppearance(); // Force note appearance for notes in tuplet where dispNote has been set
	}
	 
	ConstMusicalSymbolID noteHeadSymbolTmp = mNoteHead->getSymbol();
	// - Adjust stem length if it's a cross notehead
	if (noteHeadSymbolTmp == kFullXHeadSymbol) {
		setFirstSegmentDrawingState(false);

		if (stemTmpDirection == dirUP)
			setStemOffsetStartPosition(4);
		else if (stemTmpDirection == dirDOWN)
			setStemOffsetStartPosition(-4);
	}
	else if (noteHeadSymbolTmp == kFullTriangleHeadSymbol || noteHeadSymbolTmp == kHalfTriangleHeadSymbol) {
		if (stemTmpDirection == dirUP)
		    setStemOffsetStartPosition(47);
		else if (stemTmpDirection == dirDOWN)
			setFirstSegmentDrawingState(false);
	}
	else if (noteHeadSymbolTmp == kFullReversedTriangleHeadSymbol || noteHeadSymbolTmp == kHalfReversedTriangleHeadSymbol) {
		if (stemTmpDirection == dirUP)
		    setFirstSegmentDrawingState(false);
		else if (stemTmpDirection == dirDOWN)
			setStemOffsetStartPosition(-47);
	}

	// - dots
	if (mNoteAppearance.empty())
		createDots( mDurTemplate, mNoteBreite, NVPoint( 0, 0 ));

	// - Build the accidental list
	int accidentals = arNote->isAuto() ? 0 : arNote->getAccidentals();
	if (accidentals > 2) accidentals = 2;
	else if (accidentals < -2) accidentals = -2;

	mGrStaff->checkSystemBar (arNote->getRelativeTimePosition());
	AccList * mylist = mGrStaff->askAccidentals(pitch, octave, accidentals, arNote->getDetune());
	if (!mylist->empty()) {
		GuidoPos pos = mylist->GetHeadPosition();
		while (pos) {
			GRAccidental* acc = new GRAccidental(this, mNoteBreite, mylist->GetNext(pos));
			const ARAlter* alter = arNote->getAlter();
			if (alter) {
				NVPoint p(alter->getDX()->getValue(), alter->getDY()->getValue());
				acc->setDxy(p);

				const TagParameterFloat* size = alter->getSize();
				if (size) acc->setAlterSize(size->getValue());
			}
			AddTail(acc);
		}
	}
	delete mylist;

	// - Set the vertical position
	mPosition.y = 0;
	if (mGrStaff)
		mPosition.y = mGrStaff->getNotePosition( pitch, octave );
	// -> setting of the element-positions
	// do I need this? YES for correct dimensions including accidentals
	setPosition(mPosition);

	// now we adjust the stemlength (in certain cases, just when it is not long enough)
	if (!mStemLengthSet && !mGlobalStem) {
		const GRStem * stem = getStem();
		if (stem) {
			if (stem->getStemDir() == dirUP) {
				NVPoint stemendpos (stem->getPosition());
				stemendpos.y -= stem->getStemLength();
                float coef = 0;

                if ((float) mDurTemplate.getNumerator() / (float) mDurTemplate.getDenominator() <= 1.0f / 64.0f && mPosition.y > 250)
                    coef = 0;
                else if ((float) mDurTemplate.getNumerator() / (float) mDurTemplate.getDenominator() <= 1.0f / 32.0f && mPosition.y > 250)
                    coef = 1;
                else
                    coef = 0.5f * mGrStaff->getNumlines() - 0.5f; // Stem length is set everytime as far as the middle of the staff.

                if (stemendpos.y > coef * mCurLSPACE) {
                    const float newlength = (stem->getPosition().y - coef * mCurLSPACE);
                    changeStemLength(newlength);
                    mStemLen = stem->getStemLength();
                }
			}
			else if (stem->getStemDir() == dirDOWN) {
				NVPoint stemendpos (stem->getPosition());
				stemendpos.y += stem->getStemLength();
                float coef = 0;

                if ((float) mDurTemplate.getNumerator() / (float) mDurTemplate.getDenominator() <= 1.0f / 64.0f && mPosition.y < - 50)
                    coef = 4;
                else if ((float) mDurTemplate.getNumerator() / (float) mDurTemplate.getDenominator() <= 1.0f / 32.0f && mPosition.y < - 50)
                    coef = 3;
                else
                    coef = 0.5f * mGrStaff->getNumlines() - 0.5f; // Stem length is set everytime as far as the middle of the staff.

				if (stemendpos.y < coef * mCurLSPACE) {
					const float newlength = (coef * mCurLSPACE - stem->getPosition().y);
					changeStemLength(newlength) ;
					mStemLen = stem->getStemLength();
				}
			}
		}
	}

	mNumHelpLines = mGrStaff->getNumHelplines(pitch, octave);

	if (!mGlobalStem)
		adjustHeadPosition();

	updateBoundingBox();
}

void GRSingleNote::forceAppearance()
{
    if (mNoteAppearance.compare("")) {
        TYPE_DURATION dur = 0;
        if (mNoteAppearance == "/1")		dur = TYPE_DURATION(1, 1);
        else if (mNoteAppearance == "/2")	dur = TYPE_DURATION(1, 2);
        else if (mNoteAppearance == "/4")	dur = TYPE_DURATION(1, 4);
        else if (mNoteAppearance == "/8")	dur = TYPE_DURATION(1, 8);
        else if (mNoteAppearance == "/16")	dur = TYPE_DURATION(1, 16);
        else if (mNoteAppearance == "/32")	dur = TYPE_DURATION(1, 32);
        else if (mNoteAppearance == "/64")	dur = TYPE_DURATION(1, 64);
#ifndef WIN32
#warning ("TODO: check flags position for /32 and /64");
#endif
//        else if (mNoteAppearance == "/32")	dur = TYPE_DURATION(1, 32);		// commented due to incorrect flags rendering
//        else if (mNoteAppearance == "/64")	dur = TYPE_DURATION(1, 64);

        if ((double) dur != 0) {
            getNoteHead()->configureNoteHead(dur);
            getStem()->configureStem(dur);

            if (!mGlobalStem)
                getFlag()->configureForSingleNote(this, dur);
            else {
                getFlag()->configureForChord(mGlobalStem, dur);
			}
        }
    }
}

//____________________________________________________________________________________
void GRSingleNote::hideHead ()
{
	if (mNoteHead)
		mNoteHead->setHaveToBeDrawn(false);
	else mStyle = "none";
}

//____________________________________________________________________________________
/** \brief Called by GRGlobalStem to allow the note to adjust its headposition.

	If head Headstate is set by the user the sugHeadState (suggested Head State) is just ignored.
*/
ARTHead::HEADSTATE GRSingleNote::adjustHeadPosition(ARTHead::HEADSTATE sugHeadState)
{
	ARTHead::HEADSTATE retstate = ARTHead::CENTER;
	ARTHead::HEADSTATE useheadstate;

	float offsetx = 55 * mSize; // hardcoded
	if (mHeadState != ARTHead::NOTSET)
			useheadstate = mHeadState;
	else	useheadstate = sugHeadState;

	GRStdNoteHead * head = getNoteHead();
	GDirection stemdir = dirOFF;
	if (mGlobalStem)
		stemdir = mGlobalStem->getStemDir();			// we have a shared stem
	else {
		const GRStem * stem = getStem();
		if (stem) stemdir = stem->getStemDir();
	}

	if (useheadstate == ARTHead::NORMAL) {
		if (stemdir == dirUP || stemdir == dirOFF)	retstate = ARTHead::LEFT;
		else if (stemdir == dirDOWN)				retstate = ARTHead::RIGHT;
	}
	else if (useheadstate == ARTHead::REVERSE) {
		if (stemdir == dirUP || stemdir == dirOFF) {
			head->addToOffset(NVPoint((GCoord)offsetx,0));
			retstate = ARTHead::RIGHT;
		}
		else if (stemdir == dirDOWN) {
			head->addToOffset(NVPoint((GCoord)-offsetx,0));
            retstate = ARTHead::LEFT;
        }
        if (GRCluster *grcluster = this->getGRCluster())
            grcluster->setClusterOrientation(stemdir, retstate);
    }
	else if (useheadstate == ARTHead::LEFT) {
		if (stemdir == dirUP || stemdir == dirOFF) {
		}
		else if (stemdir == dirDOWN)
			head->addToOffset(NVPoint((GCoord)-offsetx,0));
        retstate = ARTHead::LEFT;

        if (GRCluster *grcluster = this->getGRCluster())
            grcluster->setClusterOrientation(stemdir, retstate);
    }
	else if (useheadstate == ARTHead::RIGHT ) {
		if (stemdir == dirUP || stemdir == dirOFF)
			head->addToOffset(NVPoint((GCoord)offsetx,0));
		else if (stemdir == dirDOWN) {
		}
        retstate = ARTHead::RIGHT;

        if (GRCluster *grcluster = this->getGRCluster())
            grcluster->setClusterOrientation(stemdir, retstate);
	}
	else if (useheadstate == ARTHead::CENTER) {
		if (stemdir == dirUP || stemdir == dirOFF)
			head->addToOffset(NVPoint((GCoord)(offsetx * 0.5f) ,0));
		else if (stemdir == dirDOWN)
			head->addToOffset(NVPoint((GCoord)(-offsetx * 0.5f) ,0));
		retstate = ARTHead::CENTER;
    }

	if (this->getDot())
		this->getDot()->adjustHorizontalDotPosition(mSize, retstate, stemdir);

    // - Adjust horizontal notehead position, particularly for non-standard noteheads
    this->getNoteHead()->adjustPositionForChords(retstate, stemdir);

    mHeadState = retstate;
	return mHeadState;
}

//____________________________________________________________________________________
NVRect GRSingleNote::getEnclosingBox(bool includeAccidentals, bool includeSlurs, bool includeTrills) const
{
	NVRect outrect = getBoundingBox();
	outrect += getPosition();
	if (!getDuration()) {
		// we're in a chord, needs to add the stem
		const GRStem * stem = getStem();
		if (stem) {
			NVRect r = stem->getBoundingBox();
			r += stem->getPosition();
			outrect.Merge (r);
		}
	}
	if (!includeAccidentals) {
		outrect.left = mPosition.x - mNoteBreite / 2 * getSize();
	}
	const GRNEList& articulations = getArticulations();
	for (GRNEList::const_iterator ptr = articulations.begin(); ptr != articulations.end(); ++ptr) {
		GRNotationElement *el = *ptr;
		NVRect r = el->getBoundingBox();
		r += el->getPosition();
		r.left = outrect.left;		// force left to the note left for articulations
		outrect.Merge (r);
	}

//	if (fOrnament) {
//		NVRect r = fOrnament->getEnclosingBox();
//		outrect.Merge (r);
//	}
	// not all the ornaments are stored in the fOrnament field
	const NEPointerList * assoc = getAssociations();
	GuidoPos pos = assoc ? assoc->GetHeadPosition() : 0;
	while (pos) {
		const GRNotationElement* el = getAssociations()->GetNext(pos);
		if (includeTrills) {
			const GRTrill * trill = el->isGRTrill();
			if (trill) {
				NVRect r = trill->getEnclosingBox();
				outrect.Merge (r);
			}
		}
		if (includeSlurs) {
			const GRSlur * slur = dynamic_cast<const GRSlur *>(el);
			if (slur) {
				NVRect r = slur->getBoundingBox();
				if (r.top < outrect.top) outrect.top = r.top;
				if (r.bottom > outrect.bottom) outrect.bottom = r.bottom;
			}
		}
	}
	return outrect;
}

//____________________________________________________________________________________
void GRSingleNote::setHPosition( GCoord nx )
{
	GRNote::setHPosition(nx);
    // - Notify cluster
    if (fCluster)
		fCluster->tellPosition(this, getPosition());
	updateBoundingBox();
}

//____________________________________________________________________________________
void GRSingleNote::setPosition( const NVPoint & inPos )
{
	// - Call inherited
	GRNote::setPosition( inPos );

    // - Notify cluster
    if (fCluster)
        fCluster->tellPosition(this, getPosition());

	// - Watch for the Accidentals
	GRAccidentalList accList;
	extractAccidentals( &accList );

	GuidoPos pos = accList.GetHeadPosition();
	NVPoint pnt ( getPosition());
	pnt.x -= mNoteBreite * 0.5f;
	while (pos)
	{
		// if more than one accidental (like
		// natural AND sharp/flat)
		pnt.x -= LSPACE / 5;
		GRNotationElement * e = accList.GetNext(pos);
		e->setPosition(pnt);
		pnt.x -= // old: e->getSizeX();
			e->getLeftSpace() + e->getRightSpace();
	}
	updateBoundingBox(); // DEBUG
}

//____________________________________________________________________________________
/** \brief Executes an update of the vertical position
	(because the key may have changed)
*/
void GRSingleNote::recalcVerticalPosition()
{
	int pitch;
	int octave;
	int acc;
	getPitchAndOctave( &pitch, &octave, &acc );

	// Here the vertical psoition is calculated and set
	NVPoint newPos = getPosition();

	newPos.y = 0;
	if (mGrStaff)
		newPos.y = mGrStaff->getNotePosition( pitch, octave );
	setPosition( newPos );
	mNumHelpLines = mGrStaff->getNumHelplines( pitch, octave );
	updateBoundingBox();
}

//____________________________________________________________________________________
GRStdNoteHead * GRSingleNote::getNoteHead() const
{
	return mNoteHead;
}

//____________________________________________________________________________________
void GRSingleNote::extractAccidentals( GRAccidentalList * outList ) const
{
	GuidoPos pos = First();
	while (pos)
	{
		GRAccidental * e = dynamic_cast<GRAccidental *>( GetNext(pos));
		if( e ) outList->AddTail( e );
	}
}

//____________________________________________________________________________________
NVPoint GRSingleNote::getTieStart() const
{
  // eventually improve later
  NVPoint pos (mPosition);
  pos.x += (mCurLSPACE * 0.75f );
  pos.y += mCurLSPACE ;
  return pos;
}

//____________________________________________________________________________________
NVPoint GRSingleNote::getTieEnd() const
{
  // eventually improve later
  NVPoint pos (mPosition);
  pos.x += (mCurLSPACE * 0.25f );
  pos.y += mCurLSPACE ;
  return pos;
}

//____________________________________________________________________________________
void GRSingleNote::drawStemOnly(int flag)
{
	/*
	GRNoteHals * hals = getNoteHals();
	if (hals)
		hals->drawStemOnly(flag);
		*/
}

//____________________________________________________________________________________
void GRSingleNote::setStemOffsetStartPosition(float inOffset)
{
	GRStem *stem = getStem();

	if (stem)
		stem->setOffsetStartPosition(inOffset);
}

//____________________________________________________________________________________
void GRSingleNote::setFirstSegmentDrawingState(bool inDrawingState)
{
	GRStem *stem = getStem();

	if (stem)
		stem->setFirstSegmentDrawingState(inDrawingState);
}

//____________________________________________________________________________________
void GRSingleNote::setStemDirection(GDirection dir)
{
	// (JB) Don't we have to deal with the global stem ?
	mStemDir = dir;
	GRStem * stem = getStem();
	if (stem)
	{
		stem->setStemDir(mStemDir);
		updateBoundingBox();
	}

	GRStdNoteHead *notehead = this->getNoteHead();

	if (notehead)
	{
		notehead->setGlobalStemDirection(mStemDir);

		ConstMusicalSymbolID noteHeadSymbolTmp = notehead->getSymbol();
		// - Adjust stem length if it's a cross notehead
		if (noteHeadSymbolTmp == kFullXHeadSymbol)
		{
			setFirstSegmentDrawingState(false);

			if (mStemDir == dirUP)
				setStemOffsetStartPosition(4);
			else if (mStemDir == dirDOWN)
				setStemOffsetStartPosition(-4);
		}
		else if (noteHeadSymbolTmp == kFullTriangleHeadSymbol || noteHeadSymbolTmp == kHalfTriangleHeadSymbol)
		{
			if (mStemDir == dirUP)
				setStemOffsetStartPosition(47);
			else if (mStemDir == dirDOWN)
				setFirstSegmentDrawingState(false);
		}
		else if (noteHeadSymbolTmp == kFullReversedTriangleHeadSymbol || noteHeadSymbolTmp == kHalfReversedTriangleHeadSymbol)
		{
			if (mStemDir == dirUP)
			{
				setFirstSegmentDrawingState(false);
				setStemOffsetStartPosition(0);
			}
			else if (mStemDir == dirDOWN)
				setStemOffsetStartPosition(-47);
		}
	}
}

//____________________________________________________________________________________
void GRSingleNote::setFlagOnOff(bool p)
{
	GRFlag * flag = getFlag();
	if (flag) {
		flag->setFlagOnOff(p);
		updateBoundingBox();
	}
}

//____________________________________________________________________________________
void GRSingleNote::setStemOnOff(bool p)
{
	GRStem * stem = getStem();
	if (stem) {
		stem->setStemDir( p ? dirAUTO : dirOFF);
		updateBoundingBox();
	}
}

//____________________________________________________________________________________
int GRSingleNote::getNumFaehnchen() const // number of flags attached to a stem.
{
	if (mGlobalStem)
		return mGlobalStem->getNumFaehnchen();

	const GRFlag * flag = getFlag();
	return flag ? flag->getNumFaehnchen() : 0;
}

//____________________________________________________________________________________
// pos = -1 : draw own (straight) flags left of stem-line
// pos = 1 : draw own (straight) flags right of stem-line
// pos = 0 : draw own (straight) flags centered through stem-line
void GRSingleNote::setBeamStem(GRBeam * beam, GCoord pos)
{
}

//____________________________________________________________________________________
float GRSingleNote::setStemLength( float inLen, bool userLength)
{
	fUserLength = userLength;
	if (mGlobalStem) {
		mGlobalStem->setNoteStemLength( this, inLen);
		return (float)mGlobalStem->getStemLength();
	}

	if (mStemLengthSet)
		GuidoWarn("Stemlength already set!");

	GRStem * stem = getStem();
	if (stem)
	{
		stem->setStemLength( inLen );
		mStemLen = stem->getStemLength();
	}
	else mStemLen = inLen;

	GRFlag * flag = getFlag();
	if (flag)
		flag->changeStemLength( this, mStemLen, mStemDir );
	mStemLengthSet = true;
	updateBoundingBox();	// TEST
	return 0;
}

//____________________________________________________________________________________
float GRSingleNote::changeStemLength( float inLen, bool force )
{
	if (mStemLengthSet && !force)
        return mStemLen;

	setStemLength(inLen);
	// this makes sure, that we don't think that the stem length was changed with a parameter.
	mStemLengthSet = false;

	GRNEList::iterator ptr;
	GRNEList& articulations = getArticulations();
	sort (articulations.begin(), articulations.end(), GRArticulation::compare);
	for(auto art: articulations) {
		art->tellPosition( this, mPosition );
	}
	return mStemLen;
}

//____________________________________________________________________________________
GDirection GRSingleNote::getStemDirection() const
{
	return mGlobalStem ? mGlobalStem->getStemDir() : mStemDir;
}

//____________________________________________________________________________________
float GRSingleNote::getStemLength() const
{
	const GRStem * stem = getStem();
	if (stem)
		return stem->getStemLength();
	return mStemLen;
}

//____________________________________________________________________________________
/** \bug (JB) I think that the beam-error offset (beams don't fall exactly on stems), has
		something to do with these two methods: getStemStartPos() and getStemEndPos().
*/
NVPoint GRSingleNote::getStemStartPos() const
{
	NVPoint pnt( mPosition );
	GDirection dir = getStemDirection();
	if (dir == dirUP)
	{
		const GCoord leftUpOffset = - 4 * mSize;
		pnt.x += (GCoord)((mNoteBreite * 0.5f * mSize) + mOffset.x + leftUpOffset);
		pnt.y -= (GCoord)(mStemLen - mOffset.y);
	}
	else if (dir == dirDOWN)
	{
		pnt.x -= (GCoord)((mNoteBreite * 0.5f * mSize) - mOffset.x - (0 * mSize));
		pnt.y += (GCoord)(mStemLen + mOffset.y);
	}
	return pnt;
}

//____________________________________________________________________________________
NVPoint GRSingleNote::getStemEndPos() const
{
	NVPoint pnt( mPosition );
	GDirection dir = getStemDirection();
	if (dir == dirUP)
	{
		const GCoord rightUpOffset = - 6 * mSize;		 // (JB) was (1 * size)
		pnt.x += (GCoord)((mNoteBreite * 0.5f * mSize) + mOffset.x + rightUpOffset );
		pnt.y -= (GCoord)(mStemLen - mOffset.y);
	}
	else if (dir == dirDOWN)
	{
		pnt.x -= (GCoord)((mNoteBreite * 0.5f * mSize) - mOffset.x - (0 * mSize)); // (JB) was (4 * size)
		pnt.y += (GCoord)(mStemLen + mOffset.y);
	}
	return pnt;
}

//____________________________________________________________________________________
const GRStem * GRSingleNote::getStem() const
{
	if (mGlobalStem) return mGlobalStem->getGRStem();
	GuidoPos pos = First();
	while (pos)
	{
		GRStem * e = dynamic_cast<GRStem *>( GetNext(pos));
		if( e ) return e;
	}
	return 0;
}

//____________________________________________________________________________________
GRStem * GRSingleNote::getStem()
{
	if (mGlobalStem) return mGlobalStem->getGRStem();
	GuidoPos pos = First();
	while (pos)
	{
		GRStem * e = dynamic_cast<GRStem *>( GetNext(pos));
		if( e ) return e;
	}
	return 0;
}


/** \brief Set the stemlength.

	it returns a negative Value if
	the beam is to close to the note!
*/

//____________________________________________________________________________________
/** \brief Called when a newline at the end of staff is introduced and the
	shown note-length does no longer match!

	 returns :
	1, if the new length could be achieved
	-1, if not implemented
	0, if the new length could only be partially done
*/
int GRSingleNote::adjustLength( const TYPE_DURATION & ndur )
{
	assert(false);

	// ATTENTION,  what happens when the note was within a tuplet!!!!
	// tuplet is not handled yet ....
	GRTuplet * mytuplet = 0;
	if (mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		GRNotationElement * el;
		while (pos)
		{
			el = mAssociated->GetNext(pos);
			mytuplet = dynamic_cast<GRTuplet *>(el);
			if (mytuplet)
				break;
		}
	}

	if (mytuplet)
	{
//		mytuplet->removeEvent(this); // use removeAssociation ?
	}

	mDurationOfGR = ndur;
	// does it own the stuff? YES
	removeElements();
	// create a new note (with all its elements)
	createNote( DURATION_0 ); // DURATION_0 was the default argument.
    // tell all the elements their positions.
	setPosition( mPosition );

	// now add the Association again
	return 1;
}

//____________________________________________________________________________________
void GRSingleNote::setNoteFormat(const ARNoteFormat * frmt)
{
	if (frmt == 0) return;

	// - Get the color
	if (frmt->getColor())
	{
		if (mColRef == 0 )
			mColRef = new unsigned char [4];

		if (frmt->getColor()->getRGB( mColRef ) == false )
		{
			delete [] mColRef;
			mColRef = 0;
		}
	}
    else if (frmt->getRGBColor()) {
        
        if (mColRef == 0 )
			mColRef = new unsigned char [4];

        if (frmt->getRGBColor()->getRGBColor( mColRef ) == false ) {
			delete [] mColRef;
			mColRef = 0;
        }
    }

	// - Get the offsets and size
	const TagParameterFloat * tpf1 = frmt->getDX();
	if (tpf1)
		mOffset.x = tpf1->getValue();

	const TagParameterFloat * tpf2 = frmt->getDY();
	if (tpf2)
		mOffset.y -= tpf2->getValue();

	const TagParameterFloat * tpf = frmt->getSize();
    if (tpf) mSize = tpf->getValue();
//    mSize = tpf ? tpf->getValue() : float(1.0);

	// - Get the head style (standard, diamond, square...)
	const TagParameterString * paramStyle = frmt->getTPStyle();
	if( paramStyle )
		mStyle = paramStyle->getValue();

	if (mStemLengthSet == false)
		mStemLen = (3.5f * mCurLSPACE * mSize);

	// this can only be done ONCE!
	mSize *= mGrStaff->getSizeRatio();
}

//____________________________________________________________________________________
GRFlag * GRSingleNote::getFlag()
{
	if (mGlobalStem)
		return mGlobalStem->getGRFlag();

	GuidoPos pos = First();
	GRNotationElement * e;
	while (pos)
	{
		e = GetNext(pos);
		if (typeid(*e) == typeid(GRFlag)) // we should consider using a dynamic cast only
		{
			// was return dynamic cast<GRFlag *>(e);
			return static_cast<GRFlag *>(e);
		}
	}
	return 0;
}

//____________________________________________________________________________________
const GRFlag * GRSingleNote::getFlag() const
{
	GuidoPos pos = First();
	GRNotationElement * e;
	while (pos)
	{
		e = GetNext(pos);
		if (typeid(*e) == typeid(GRFlag)) // we should consider using a dynamic cast only
		{
			// was return dynamic cast<GRFlag *>(e);
			return static_cast<GRFlag *>(e);
		}
	}
	return 0;
}

//____________________________________________________________________________________
void GRSingleNote::removeElements()
{
	RemoveAllSubElements();
	mNoteHead = 0;
}

//____________________________________________________________________________________
void GRSingleNote::handleAccidental (const ARAccidental* acc)
{
	// this is an Acc-Tag -> all accidentals must be shown ...
	//
	// the note has been created already ... ? has it?
	// Attention: here we do not have to look for the Accidentals that have been
	// created, but rather to the original accidentals set with the note
	const ARNote * arnote = this->getARNote();
	GRAccidentalList accList;
	extractAccidentals( &accList );
	GuidoPos pos = accList.GetHeadPosition();
	if (pos == 0)
	{
		const int kNaturalAccidental = -10; // see the hard coded values in GRAccidental::accidentalID2symbol
		GRAccidental * myacc = new GRAccidental( this, mNoteBreite, (float)kNaturalAccidental );
        if (acc && acc->getColor())
            myacc->setColor(acc->getColor());

		// no accidentals! we need to force accidentals ...
		int mynewacc = arnote->getAccidentals() * 2 + ARNote::detune2Quarters(arnote->getDetune());
		if (mynewacc != 0) 
			myacc->setAccidentalByQuarter(mynewacc, (int)getOffset().x, mNoteBreite);

		myacc->setPosition( getPosition());
		AddTail(myacc);
		accList.AddTail(myacc);
		pos = accList.GetHeadPosition();
	}

	while (pos)
	{
		GRAccidental * el = dynamic_cast<GRAccidental *>( accList.GetNext(pos));
		if (!el) continue;

		// this element is an accidental...
		// now we have to set the parameters (offset and all that...)
		NVPoint pt ( el->getOffset());

		bool setOffset = false;
		if (acc->getDX() && acc->getDX()->TagIsSet()) {
			setOffset =true;
			pt.x += (acc->getDX()->getValue());
		}

		if (acc->getDY() && acc->getDY()->TagIsSet()) {
			setOffset =true;
			pt.y -= (acc->getDY()->getValue());
		}

		if (setOffset)
			el->setOffset(pt);

		if (acc->getSize() && acc->getSize()->TagIsSet())
			el->setSize(acc->getSize()->getValue());

		if (acc->getStyle() == ARAccidental::kCautionary)
			el->setCautionary((int)getOffset().x, mNoteBreite);
		else if (acc->getStyle() == ARAccidental::kNone)
			el->setStyleNone();

        if (acc->getColor() && acc->getColor()->TagIsSet()) {
			el->setColor(acc->getColor());
        }
	}

	updateBoundingBox();
}

//____________________________________________________________________________________
void GRSingleNote::addArticulation(const ARMusicalTag * mtag)
{
	const ARAccidental * acc =  dynamic_cast<const ARAccidental *>(mtag);
	if( acc )		{ handleAccidental (acc); return; }
	GRNote::addArticulation(mtag);
}


//____________________________________________________________________________________
/** This is needed for \\shareStem-Tag. If this tag is set, the
	event does not get a spring on its own but depends on the \\shareStem-Spring.
*/
void GRSingleNote::tellPosition( GObject * caller, const NVPoint & inPos )
{
	// this is a security-measure, otherwise, we could get recursive calling loops (because setHPosition
	// calls associated elements, which could themselves again call tellPosition ...)
	if ((mNeedsSpring == 0) && (mSpringID == -1))
	   setHPosition( inPos.x );
}

//____________________________________________________________________________________
/** This is needed for implementing the headTag.
*/
void GRSingleNote::setHeadState( const ARTHead * pheadstate )
{
	mHeadState = pheadstate->getHeadState();
}

//____________________________________________________________________________________
void GRSingleNote::addToOffset( const NVPoint & pt )
{
	mOffset += pt;
}
