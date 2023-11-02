#ifndef GRSingleNote_H
#define GRSingleNote_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARTHead.h"
#include "GRNote.h"
#include "GRVisitor.h"

class GRAccidental;
template <class T> class KF_IPointerList;
typedef KF_IPointerList<GRAccidental> GRAccidentalList;

class GRStaff;
class GRStdNoteHead;
class GRStem;
class GRFlag;
class GRGlobalStem;
class GRBeam;

class ARTHead;
class ARAccidental;

/** \brief Graphical representation of a note event.
*/
class GRSingleNote : public GRNote
{
 
 public:
		GRSingleNote( GRStaff *, const ARNote * ar, const TYPE_TIMEPOSITION& date, const TYPE_DURATION& duration );

		virtual ~GRSingleNote() {}

		virtual void	accept (GRVisitor& visitor);
		virtual void	addToOffset( const NVPoint & pt);
		virtual ARTHead::HEADSTATE adjustHeadPosition(ARTHead::HEADSTATE sugHeadState = ARTHead::NORMAL);

		virtual void	setHeadState(const ARTHead * headstate);
		virtual void	setNoteFormat(const ARNoteFormat * frmt);
		virtual int		adjustLength(const TYPE_DURATION & ndur);
		virtual void	addArticulation(const ARMusicalTag * mtag);

				void	doCreateNote( const TYPE_DURATION & p_durtemplate /* = DURATION_0*/);

		virtual void	OnDraw( VGDevice & hdc ) const;
		virtual void	GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;

		virtual void	setSize(float nsize)
		{
			 mSize = nsize;
			 mStemLen *= mSize;
		}

		virtual void	updateBoundingBox();
		virtual void	setHPosition( float inX );
		virtual void	setPosition( const NVPoint & inPos );
		virtual void	recalcVerticalPosition();

		// This is the Position, where Ties and Slurs start
		// and end (these values includes the note-position)
		virtual NVPoint getTieStart() const;
		virtual NVPoint getTieEnd() const;
		virtual void	setFlagOnOff(bool p);

		// -- Stem related --
		virtual void setBeamStem(GRBeam * beam, float pos);

		// This tells the Note, to draw the stem only
		virtual void	drawStemOnly( int flag );	
		virtual void    setStemOffsetStartPosition(float inOffset);
		virtual void    setFirstSegmentDrawingState(bool inDrawingState);

	 	virtual GDirection getDirection() const { return mStemDir; }

		// This sets the Direction of the Stem
		virtual void	setStemDirection( GDirection dir );

		// This sets Stem On or Off (1 == on, 0 == off)
		virtual void	setStemOnOff( bool p );

		virtual float	changeStemLength( float inLen, bool force = false );
		virtual float	setStemLength( float inLen, bool userLength = false );
		virtual GDirection getStemDirection() const;
		virtual NVPoint getStemStartPos() const;
		virtual NVPoint getStemEndPos() const;
		virtual float	getStemLength() const;
	    virtual bool	getStemDirSet() const  		{ return mStemDirSet; }
		virtual bool	getStemLengthSet() const	{ return mStemLengthSet; }
		virtual bool	userLength() const			{ return fUserLength; }
		virtual bool	isTab() const				{ return false; }

		// -
		virtual const TYPE_DURATION & getDurTemplate() const { return mDurTemplate; }
		virtual int		getNumFaehnchen() const;

				void	removeElements();

		virtual void	tellPosition( GObject * caller, const NVPoint & newPosition ); 
		virtual const	GRSingleNote* isSingleNote() const		{ return this; }
		virtual 		GRSingleNote* isSingleNote() 			{ return this; }

	  	GRStdNoteHead *	getNoteHead() const;
	  	void			extractAccidentals( GRAccidentalList * outList ) const;
	  	NVRect			getEnclosingBox(bool includeAccidentals=true, bool includeSlurs = true, bool includeTrills=true) const;		// gives a position rect that enclose the note, accidentals and articulations

        ARTHead::HEADSTATE getHeadState() { return mHeadState; }

        void 			forceAppearance();
		const GRStem *	getStem() const;
		float 			getNoteWidth() const	{ return mNoteBreite; }
		void			hideHead ();

protected:
	GRStem *	  getStem();
	GRFlag *	  getFlag();
	const GRFlag *getFlag() const;

	int 		  mNumHelpLines;
	float 		  mNoteBreite; // the width of a note
	TYPE_DURATION mDurTemplate; // the template for the display

private:
	void 	handleAccidental	(const ARAccidental* acc);
	void 	createNote(const TYPE_DURATION & p_durtemplate /*= DURATION_0*/ );
	void 	drawLedges (VGDevice & hdc) const;
	float 	getLedgeWidth (VGDevice & hdc) const;

	GRStdNoteHead *	mNoteHead; // exists also in element list.
	GDirection		mStemDir;
	float		    mStemLen;
	bool 		    mStemLengthSet;
	bool 		    mStemDirSet;
	bool 		    fUserLength = false;

	static float 	fLedgeWidth;

	ARTHead::HEADSTATE mHeadState;
        
    // Used when dispNote param is set on \tuplet-tag to force note appearance
    NVstring mNoteAppearance;
};

#endif
