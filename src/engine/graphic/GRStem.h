#ifndef GRStem_H
#define GRStem_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "NVPoint.h"
#include "GRNotationElement.h"
#include "ARTHead.h"

class GREvent;
class GRSingleNote;
class GRGlobalStem;

/** \brief Graphical representation of a stem
*/
class GRStem : public GRNotationElement
{
public:
			// the constructur for global stems ...
			 GRStem(GRGlobalStem * gstem);
			 GRStem(GREvent * sngnot, const TYPE_DURATION & dur, GDirection dir, float length, float notebreite);

	virtual ~GRStem();

    virtual void configureStem(const TYPE_DURATION& dur);

	virtual void setStemLength( float inLen );
	virtual void setStemDir(GDirection dir);
	virtual void addToOffset(const NVPoint &pt)		{		 mOffset += pt; }

	virtual void OnDraw( VGDevice & hdc ) const;

	virtual const NVPoint& getReferencePosition() const	{ return sRefpos; }
	virtual const NVPoint& getOffset() const			{ return mOffset; }
	virtual float 		getSize() const					{ return mSize; }
	virtual GDirection 	getStemDir() const				{ return mStemDir; }
	virtual float 		getStemLength() const			{ return mStemLen; }
	virtual const unsigned char * getColRef() const		{ return mColRef; }
	
    bool getFirstSegmentDrawingState() const 			{ return fDrawActivated; }
    ARTHead::HEADSTATE getHeadOrientation() const 		{ return fHeadOrientation; }

	virtual void setColRef( const unsigned char * inColor );

	void setOffsetStartPosition(float inOffset) 			{ fOffsetStartPosition = inOffset; }
    void setFirstSegmentDrawingState(bool inDrawActivated)  { fDrawActivated = inDrawActivated; }
    /* Only for GlobalStem */
    void setLastHeadOrientation(ARTHead::HEADSTATE value) 	{ fLastHeadOrientation = value; }

private:
	void DrawWithGlyph		( VGDevice & hdc ) const;
	void DrawWithLine		( VGDevice & hdc ) const;
	void DrawStem			( VGDevice & hdc, unsigned int symbol1, unsigned int symbol2, float starty, float length ) const;
	float GetStartYOffset	( bool up, float lineSpace) const;

	float 		mStemLen;
	GDirection 	mStemDir;

	float 		mSize;		// (JB) is it related to the height of the  'stem' glyph in the musical font ?
	unsigned char * mColRef;
	NVPoint 	mOffset;

	static NVPoint sRefpos;

	float  fOffsetStartPosition;	// depends on the notehead
	bool   fDrawActivated;

    int fNoteHeadType;

    ARTHead::HEADSTATE fHeadOrientation;
    ARTHead::HEADSTATE fLastHeadOrientation;

    GREvent   *fSngnot;
    GDirection fDir;
	float      fLength;
	float      fNotebreite;
};

#endif

