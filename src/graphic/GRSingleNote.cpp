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

#include "GUIDOEngine.h"
#include "GUIDOInternal.h"

// - Guido Misc
#include "GuidoDefs.h"
#include "NVPoint.h"
#include "VGDevice.h"

// - Guido AR
#include "ARAlter.h"
#include "ARNoteFormat.h"
#include "ARDotFormat.h"
#include "ARAcc.h"
#include "ARNote.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterRGBColor.h"

// - Guido GR
#include "GRSingleNote.h"
#include "GRNote.h"
#include "GRArticulation.h"
#include "GRAccidental.h"
#include "GRStdNoteHead.h"
#include "GRNoteDot.h"
#include "GRStem.h"
#include "GRGlobalStem.h"
#include "GRFlag.h"
#include "GRCompositeNote.h"
#include "GRStaff.h"
#include "GRSpecial.h"
#include "GRSystem.h"
#include "GRTrill.h"
#include "GRCluster.h"
#include "GRNewTuplet.h" // (JB) was GRTuplet
#include "GRSpring.h"
#include "GRPage.h"
#include "secureio.h"

using namespace std;

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		cout << (void*)this << " GRSingleNote::" << method << endl
#else
#define traceMethod(method)	
#endif


//____________________________________________________________________________________
GRSingleNote::GRSingleNote( GRStaff* inStaff, ARNote* arnote, const TYPE_TIMEPOSITION& pos, const TYPE_DURATION& dur)
  : GRNote( inStaff, arnote, pos, dur), mNumHelpLines(0),
	mStemDir(dirAUTO), mStemDirSet(false), mHeadState(ARTHead::NOTSET)
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

GRSingleNote::~GRSingleNote()
{
}

//____________________________________________________________________________________
/** \brief this acutally creates the note.
*/
void GRSingleNote::doCreateNote( const TYPE_DURATION & p_durtemplate )
{
	createNote(p_durtemplate);
}

//____________________________________________________________________________________
char * GRSingleNote::getGGSInfo( int infotype ) const
{
	// now we find out, where the next spring is ....
	// our sprid is set ....
	const GRSpring * prevspring = mGrStaff->getGRSystem()->getGRSpring(mSpringID - 1);
	const GRSpring * nextspring = mGrStaff->getGRSystem()->getGRSpring(mSpringID + 1);

	const GRSpring * myspring   = mGrStaff->getGRSystem()->getGRSpring(mSpringID);

	int left = (int)myspring->posx;
	int right = (int)myspring->posx;

	if (prevspring)	left = (int)prevspring->posx;
	if (nextspring)	right = (int)nextspring->posx;

	char * buf = new char[100];

	GRSystem * cursystem = mGrStaff->getGRSystem();

#ifdef VC2005
	snprintf(buf, 100, 100, "%d,%d,%d,%d,%ld,%d,%ld,%d\n",
		(int)cursystem->getGRPage()->getMarginLeft(),
		(int)cursystem->getPosition().x,
		(int)mPosition.x,
		(int)myspring->posx,
		(long int)prevspring,
		(int)left,
		(long int)nextspring,	// <- !!! pointer to int !!!
		(int)right);
#else
	snprintf(buf, 100, "%d,%d,%d,%d,%ld,%d,%ld,%d\n",
		(int)cursystem->getGRPage()->getMarginLeft(),
		(int)cursystem->getPosition().x,
		(int)mPosition.x,
		(int)myspring->posx,
		(long int)prevspring,
		(int)left,
		(long int)nextspring,	// <- !!! pointer to int !!!
		(int)right);
#endif
	return buf;
}

//____________________________________________________________________________________
void GRSingleNote::GGSOutput() const
{
	// draw ledgerlines ...
	float incy= 1;
	float posy = 0;
	int sum = mNumHelpLines;
	if (mNumHelpLines > 0)
	{ // ledgerlines above system
		incy = -mCurLSPACE;
		posy = -mCurLSPACE;
	}
	else if (mNumHelpLines < 0)
	{
		incy = mCurLSPACE;
		posy = mGrStaff->getNumlines() * mCurLSPACE;
		sum *= -1;
	}

	// draw ledgerlines
	const int zeichen = kLedgerLineSymbol;
	for (int i=0;i<sum;i++,posy+=incy)
	{
		GGSOutputAt((unsigned int) zeichen, (long)(-60 * 0.85 * mSize), (long)(posy - mPosition.y));
	}

	GuidoPos pos = First();
	while (pos)
	{
		GRNotationElement * el = GetNext(pos);
		el->setID((long int) this);
		el->GGSOutput();
	}

	const GRNEList * articulations = getArticulations();
	if( articulations )
	{
		for( GRNEList::const_iterator ptr = articulations->begin(); ptr != articulations->end(); ++ptr )
		{
			GRNotationElement * el = *ptr;
			el->setID((long int) this);
			el->GGSOutput();
		}
	}
}

//____________________________________________________________________________________
void GRSingleNote::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoEvent) {
		TYPE_DURATION dur = getDuration();
		if (dur.getNumerator() == 0) {		// notes in chords have a null duration
			dur = getDurTemplate();
		}
//		const ARNote * ar = getARNote();
//		std::cout << "mapped pos: " << ar->getStartTimePosition() << " ar pos: " << ar->getRelativeTimePosition() << " ";
//		ar->print();
		// ARNote and GRNote don't have the same time position in chords
		// actually chord notes have a wrong time position, it has been corrected in ARMusicalVoice::FinishChord
		SendMap (f, getARNote()->getStartTimePosition(), dur, kNote, infos);
	}
}
//____________________________________________________________________________________
void GRSingleNote::OnDraw( VGDevice & hdc) const
{
    int numVoice = this->getAbstractRepresentation()->getVoiceNum();
    float X = mGrStaff->getXEndPosition(getARNote()->getRelativeTimePosition(), getARNote()->getDuration());

    float incy = 1;
    float posy = 0;
    int sum = mNumHelpLines;
    if (mNumHelpLines > 0)
    { 	// ledger lines up
        incy = -mCurLSPACE;
        posy = -mCurLSPACE;
        hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );
    }
    else if( mNumHelpLines < 0 )
    {
        incy = mCurLSPACE;
        posy = mGrStaff->getNumlines() * mCurLSPACE;
        sum = - sum;
        hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );
    }

    // draw ledger lines
    const float ledXPos = -60 * 0.85f * mSize;
    for (int i = 0; i < sum; ++i, posy += incy)
        GRNote::DrawSymbol( hdc, kLedgerLineSymbol, ledXPos, ( posy - mPosition.y ));

    if (!mCluster)
    {
        const VGColor oldcolor = hdc.GetFontColor();
        if (mColRef) hdc.SetFontColor( VGColor( mColRef ));

        // - Draw elements (stems, dots...)
        DrawSubElements( hdc );

        // - draw articulations & ornament
        const GRNEList * articulations = getArticulations();
        if( articulations )
        {
            for( GRNEList::const_iterator ptr = articulations->begin(); ptr != articulations->end(); ++ptr )
            {
                GRNotationElement * el = *ptr;
                el->OnDraw(hdc);
            }
        }

        if (mOrnament)
		{
			// to draw the trill line...
            mOrnament->OnDraw(hdc,X,numVoice);
		}

        // - Restore
        if (mColRef) hdc.SetFontColor( oldcolor );
        if (gBoundingBoxesMap & kEventsBB)
            DrawBoundingBox( hdc, kEventBBColor);
    }
    else if (mClusterHaveToBeDrawn)
    {
        if (mOrnament)
            mOrnament->OnDraw(hdc,X,numVoice);

        mCluster->OnDraw(hdc);
    }
}

//____________________________________________________________________________________
void GRSingleNote::updateBoundingBox()
{
	traceMethod("updateBoundingBox");
	// - Call inherited
	GRNote::updateBoundingBox();

	// - Check for notebreite (?)
	if( mBoundingBox.left > (-mNoteBreite * 0.5f))
		mBoundingBox.left = (-mNoteBreite * 0.5f);	// (JB) test, was: 0

	if( mBoundingBox.right < (mNoteBreite * 0.5f))
		mBoundingBox.right = (mNoteBreite * 0.5f);	// width of the note. (JB) test, was: 0

	// - Check for ledger lines
	//  (note that the bounding box does not take account of ledger lines, for now)
	if (mNumHelpLines > 0 )
	{
		const float theSize = (mNoteBreite * 0.8f * mSize); // harcoded

		if (mLeftSpace < theSize)	mLeftSpace  = theSize;
		if (mRightSpace < theSize )	mRightSpace = theSize;
	}

	// - Check for stem
	if (mGlobalStem)
	{
		GRNotationElement * e = mGlobalStem->getGRFlag();
		if( e )
		{
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
	if (mDurTemplate <= DURATION_0)
	{
		mNumHelpLines = 0;
		return;		// this just does nothing more
	}

	ARNote * arNote = getARNote();
	const int pitch = arNote->getPitch();
	const int octave = arNote->getOctave();
	GDirection tmpdir =  mGrStaff->getDefaultThroatDirection( pitch, octave );

	// the creation of the standard elements ....
	// creates notehead, depends on length or templateLength
	mNoteHead =  new GRStdNoteHead( this, mDurTemplate, tmpdir );
	AddTail( mNoteHead );

	// this is dependant on size!
	mNoteBreite = ((mNoteHead->getLeftSpace() + mNoteHead->getRightSpace()) / (float) mSize);
	float tmplength = mStemLen;
	// the Stem-Straight-Flag is not used everywhere

	GRStem * tmp = NULL;
	GDirection stemTmpDirection = tmpdir;

	if (mGlobalStem == 0)
	{
		if (mStemDir == dirAUTO)
		{
			tmp =  new GRStem( this, mDurTemplate, stemTmpDirection, tmplength, mNoteBreite); // was 60?);
			mStemDir = tmp->mStemDir;
		}
		else
		{
			stemTmpDirection = mStemDir;
			tmp = new GRStem( this, mDurTemplate, stemTmpDirection, tmplength, mNoteBreite ) ; // was 60? );
			mStemDirSet = true;
		}

		mStemLen = tmp->mStemLen;
		AddTail(tmp);

		// here we have to add the flags ...
		GRFlag * tmpflag = new GRFlag( this, mDurTemplate, mStemDir, mStemLen, mNoteBreite ); // was 60?);
		if (mColRef)
		{
		}
		AddTail(tmpflag);
	}
	 
	ConstMusicalSymbolID noteHeadSymbolTmp = mNoteHead->getSymbol();
	// - Adjust stem length if it's a cross notehead
	if (noteHeadSymbolTmp == kFullXHeadSymbol)
	{
		setFirstSegmentDrawingState(false);

		if (stemTmpDirection == dirUP)
			setStemOffsetStartPosition(4);
		else if (stemTmpDirection == dirDOWN)
			setStemOffsetStartPosition(-4);
	}
	else if (noteHeadSymbolTmp == kFullTriangleHeadSymbol || noteHeadSymbolTmp == kHalfTriangleHeadSymbol)
	{
		if (stemTmpDirection == dirUP)
		    setStemOffsetStartPosition(47);
		else if (stemTmpDirection == dirDOWN)
			setFirstSegmentDrawingState(false);
	}
	else if (noteHeadSymbolTmp == kFullReversedTriangleHeadSymbol || noteHeadSymbolTmp == kHalfReversedTriangleHeadSymbol)
	{
		if (stemTmpDirection == dirUP)
		    setFirstSegmentDrawingState(false);
		else if (stemTmpDirection == dirDOWN)
			setStemOffsetStartPosition(-47);
	}

	// - dots
	createDots( mDurTemplate, mNoteBreite, NVPoint( 0, 0 ));

	// - Build the accidental list
	int accidentals = arNote->getAccidentals();
	if (accidentals > 2) accidentals = 2;
	else if (accidentals < -2) accidentals = -2;

	mGrStaff->checkSystemBar (arNote->getRelativeTimePosition());
	AccList * mylist = mGrStaff->askAccidentals(pitch, octave, accidentals, arNote->getDetune());
	if (!mylist->empty()) {
		GuidoPos pos = mylist->GetHeadPosition();
		while (pos) {
			AddTail(new GRAccidental(this, mNoteBreite, mylist->GetNext(pos)));
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
	if (!mStemLengthSet && !mGlobalStem)
	{
		GRStem * stem = getStem();
		if (stem)
		{
			if (stem->mStemDir == dirUP)
			{
				NVPoint stemendpos (stem->getPosition());
				stemendpos.y -= stem->mStemLen;

				if (stemendpos.y > 2 * mCurLSPACE)
				{
					const float newlength = (stem->getPosition().y - 2 * mCurLSPACE );
					changeStemLength(newlength);
					mStemLen = stem->mStemLen;
				}
			}
			else if (stem->mStemDir == dirDOWN)
			{
				NVPoint stemendpos (stem->getPosition());
				stemendpos.y += stem->mStemLen;
				if (stemendpos.y < 2 * mCurLSPACE)
				{
					const float newlength = (2 * mCurLSPACE - stem->getPosition().y);
					changeStemLength( newlength ) ;
					mStemLen = stem->mStemLen;
				}
			}
		}
	}

	mNumHelpLines = mGrStaff->getNumHelplines( pitch, octave );
	if (!mGlobalStem)
		adjustHeadPosition();
	updateBoundingBox();
}

//____________________________________________________________________________________
/** \brief Called by GRGlobalStem to allow the note to adjust its headposition.

	If head Headstate is set by the user the sugHeadState (suggested Head State) is just ignored.
*/
ARTHead::HEADSTATE GRSingleNote::adjustHeadPosition(ARTHead::HEADSTATE sugHeadState)
{
	ARTHead::HEADSTATE retstate = ARTHead::CENTER;
	ARTHead::HEADSTATE useheadstate;

	float offsetx = 60 * mSize; // hardcoded
	if (mHeadState != ARTHead::NOTSET)
			useheadstate = mHeadState;
	else	useheadstate = sugHeadState;

	GRStdNoteHead * head = getNoteHead();
	GDirection stemdir = dirOFF;
	if (mGlobalStem)
		stemdir = mGlobalStem->getStemDir();			// we have a shared stem
	else
	{
		GRStem * stem = getStem();
		if (stem) stemdir = stem->getStemDir();
	}

	if (useheadstate == ARTHead::NORMAL)
	{
		if (stemdir == dirUP || stemdir == dirOFF)	retstate = ARTHead::LEFT;
		else if (stemdir == dirDOWN)				retstate = ARTHead::RIGHT;
	}

	else if (useheadstate == ARTHead::REVERSE)
	{
		if (stemdir == dirUP || stemdir == dirOFF)
		{
			head->addToOffset(NVPoint((GCoord)offsetx,0));
			retstate = ARTHead::RIGHT;
		}
		else if (stemdir == dirDOWN)
		{
			head->addToOffset(NVPoint((GCoord)-offsetx,0));
			retstate = ARTHead::LEFT;
		}
	}
	else if (useheadstate == ARTHead::LEFT)
	{
		if (stemdir == dirUP || stemdir == dirOFF)
		{
		}
		else if (stemdir == dirDOWN)
			head->addToOffset(NVPoint((GCoord)-offsetx,0));
		retstate = ARTHead::LEFT;

	}
	else if (useheadstate == ARTHead::RIGHT )
	{
		if (stemdir == dirUP || stemdir == dirOFF)
			head->addToOffset(NVPoint((GCoord)offsetx,0));
		else if (stemdir == dirDOWN)
		{
		}
		retstate = ARTHead::RIGHT;
	}
	else if (useheadstate == ARTHead::CENTER)
	{
		if (stemdir == dirUP || stemdir == dirOFF)
			head->addToOffset(NVPoint((GCoord)(offsetx * 0.5f) ,0));
		else if (stemdir == dirDOWN)
			head->addToOffset(NVPoint((GCoord)(-offsetx * 0.5f) ,0));
		retstate = ARTHead::CENTER;
	}
	return retstate;
}

//____________________________________________________________________________________
void GRSingleNote::setHPosition( GCoord nx )
{
	GRNote::setHPosition(nx);
	// - Notify ornament
	if (mOrnament)
		mOrnament->tellPosition(this, getPosition());
    // - Notify cluster
    if (mCluster)
		mCluster->tellPosition(this, getPosition());
	updateBoundingBox();
}

//____________________________________________________________________________________
void GRSingleNote::setPosition( const NVPoint & inPos )
{
	// - Call inherited
	GRNote::setPosition( inPos );

	// - Notify Ornament
	if (mOrnament)
		mOrnament->tellPosition(this, getPosition());
    // - Notify cluster
    if (mCluster)
        mCluster->tellPosition(this, getPosition());

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
GRStdNoteHead * GRSingleNote::getNoteHead()
{
	return mNoteHead;
}

//____________________________________________________________________________________
void GRSingleNote::extractAccidentals( GRAccidentalList * outList )
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
	if (flag)
	{
		flag->setFlagOnOff(p);
		updateBoundingBox();
	}
}

//____________________________________________________________________________________
void GRSingleNote::setStemOnOff(bool p)
{
	GRStem * stem = getStem();
	if (stem)
	{
		if (p)
			stem->setStemDir(dirAUTO);
		else
			stem->setStemDir(dirOFF);
	}
	updateBoundingBox();
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
float GRSingleNote::setStemLength( float inLen )
{
	if (mGlobalStem)
	{
		mGlobalStem->setNoteStemLength( this, inLen);
		return (float)mGlobalStem->getStemLength();
	}

	if (mStemLengthSet)
		GuidoWarn("Stemlength already set!");

	GRStem * stem = getStem();
	if (stem)
	{
		stem->setStemLength( inLen );
		mStemLen = stem->mStemLen;
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
float GRSingleNote::changeStemLength( float inLen )
{
	if (mStemLengthSet) return mStemLen;
	setStemLength(inLen);
	// this makes sure, that we don't think that
	// the stemlength was changed with a parameter.
	mStemLengthSet = false;

// TODO: update bounding box (in setStemLength?) ?
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
		return stem->mStemLen;		// I am a friend!
	return mStemLen;
}

//____________________________________________________________________________________
/** \bug (JB) I think that the beam-error offset (beams don't fall exactly on stems), has
		something to do with these two methods: getStemStartPos() and getStemEndPos().
*/
NVPoint GRSingleNote::getStemStartPos() const
{
	const GRStem * stem = getStem();
	NVPoint pnt( mPosition );
	if (stem)
	{
		if (mStemDir == dirUP)
		{
			const GCoord leftUpOffset = - 4 * mSize;		// (JB) was (4 * size) (???)
			pnt.x += (GCoord)((mNoteBreite * 0.5f * mSize) + mOffset.x + leftUpOffset);
			pnt.y -= (GCoord)(mStemLen - mOffset.y);
		}
		else if (mStemDir == dirDOWN)
		{
			pnt.x -= (GCoord)((mNoteBreite * 0.5f * mSize) - mOffset.x - (0 * mSize));// (JB) was (0 * size)
			pnt.y += (GCoord)(mStemLen + mOffset.y);
		}
	}
	return pnt;
}

//____________________________________________________________________________________
NVPoint GRSingleNote::getStemEndPos() const
{
	const GRStem * stem = getStem();
	NVPoint pnt(mPosition);
	if (stem)
	{
		if (mStemDir == dirUP)
		{
			const GCoord rightUpOffset = - 6 * mSize;		 // (JB) was (1 * size)
			pnt.x += (GCoord)((mNoteBreite * 0.5f * mSize) + mOffset.x + rightUpOffset );
			pnt.y -= (GCoord)(mStemLen - mOffset.y);
		}
		else if (mStemDir == dirDOWN)
		{
			pnt.x -= (GCoord)((mNoteBreite * 0.5f * mSize) - mOffset.x - (0 * mSize)); // (JB) was (4 * size)
			pnt.y += (GCoord)(mStemLen + mOffset.y);
		}
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
	GRNewTuplet * mytuplet = 0; // was GRTuplet
	if (mAssociated)
	{
		GuidoPos pos = mAssociated->GetHeadPosition();
		GRNotationElement * el;
		while (pos)
		{
			el = mAssociated->GetNext(pos);
			mytuplet = dynamic_cast<GRNewTuplet *>(el); // war GRTuplet
			if (mytuplet)
				break;
		}
	}

	if (mytuplet)
	{
//		mytuplet->removeEvent(this); // now GRNewTuplet... use removeAssociation ?
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
	mSize = tpf ? tpf->getValue() : float(1.0);

	// - Get the head style (standard, diamond, square...)
	const TagParameterString * paramStyle = frmt->getTPStyle();
	if( paramStyle )
		mStyle = paramStyle->getValue();

	if (mStemLengthSet == false)
		mStemLen = (3.5f * mCurLSPACE * mSize);

	// this can only be done ONCE!
	mSize *= mGrStaff->getSizeRatio();
}

/* (JB)  moved to GREvent
void GRSingleNote::setDotFormat(const ARDotFormat * frmt)
{
	GRNoteDot * mydot = getDot();
	if (mydot && frmt)
	{
		if (frmt->getDX())
		{
			mydot->mOffset.x += (GCoord)(frmt->getDX()->getValue(mCurLSPACE));
		}
		if (frmt->getDY() && frmt->getDY()->TagIsSet())
		{
			mydot->mOffset.y -= (GCoord)(frmt->getDY()->getValue(mCurLSPACE));
		}
		else if (frmt->getDY())
		{
			// here we have a DY, that has not been set ...
			// determine, wether the note is on a line or not ...
		    const int n = (int)( mPosition.y / (mCurLSPACE * 0.5f ));
			//if ( (n / 2) * 2 == n ) // if even ? Could be tested better !

			if(( n | 1 ) != n ) // if even
			{
				// gerade ... wir sind auf einer linie ... why 0.8 ?
				mydot->mOffset.y -= (GCoord)TagParameterFloat::convertValue(0.8f,"hs",mCurLSPACE);
			}
		}
		if (frmt->getSize())
		{
			mydot->size *= frmt->getSize()->getValue();
		}
		// what about dd
	}
}
*/

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
void GRSingleNote::handleAccidental (const ARAcc* acc)
{
	// this is an Acc-Tag -> all accidentals must be shown ...
	//
	// the note has been created already ... ? has it?
	// Attention: here we do not have to look for the Accidentals that have been
	// created, but rather to the original accidentals set with the note
	ARNote * arnote = this->getARNote();
	GRAccidentalList accList;
	extractAccidentals( &accList );
	GuidoPos pos = accList.GetHeadPosition();
	if (pos == 0)
	{
		const int kNaturalAccidental = -10; // see the hard coded values in GRAccidental::accidentalID2symbol
		GRAccidental * myacc = new GRAccidental( this, mNoteBreite, (float)kNaturalAccidental );

		// no accidentals! we need to force accidentals ...
		int mynewacc = arnote->getAccidentals() * 2 + ARNote::detune2Quarters(arnote->getDetune());
		if (mynewacc != 0) 
			myacc->setAccidentalByQuarter(mynewacc, getOffset().x, mNoteBreite);

		myacc->setPosition( getPosition());
		AddTail(myacc);
		accList.AddTail(myacc);
		pos = accList.GetHeadPosition();
	}

	while (pos)
	{
		GRAccidental * el = dynamic_cast<GRAccidental *>( accList.GetNext(pos));
		// this element is an accidental...
		// now we have to set the parameters (offset and all that...)
		NVPoint pt ( el->getOffset());
		if (acc->getDX() && acc->getDX()->TagIsSet())
			pt.x += (acc->getDX()->getValue());

		if (acc->getDY() && acc->getDY()->TagIsSet())
			pt.y -= (acc->getDY()->getValue());
		el->setOffset(pt);
		if (acc->getSize() && acc->getSize()->TagIsSet())
			el->setSize(acc->getSize()->getValue());

		if (acc->getStyle() == ARAcc::kCautionary) {
			if (el) el->setCautionary (getOffset().x, mNoteBreite);			
		}
		// color...
	}
	updateBoundingBox();
}

//____________________________________________________________________________________
void GRSingleNote::addArticulation(ARMusicalTag * mtag)
{
	ARAcc * acc =  dynamic_cast<ARAcc *>(mtag);
	if( acc )		{ handleAccidental (acc); return; }
	GRNote::addArticulation(mtag); // => GREvent::addArticulation(mtag);
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
