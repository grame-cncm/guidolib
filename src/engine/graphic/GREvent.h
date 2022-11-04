#ifndef GREvent_H
#define GREvent_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <vector>

#include "GRARCompositeNotationElement.h"

class NVPoint;

class ARDotFormat;
class ARMusicalEvent;
class ARMusicalTag;

class GRBeam;
class GRStaff;
class GRGlobalStem;
class GRGlobalLocation;
class GRBar;
class GRNotationElement;
class GRNoteDot;
class GRArticulation;


template <typename TYPE>
void DeleteContent( TYPE * inContainer )
{
	if( inContainer == 0 ) return;

	typename TYPE::iterator thePtr;
    for( thePtr = inContainer->begin(); thePtr != inContainer->end(); ++thePtr )
		delete *thePtr;

	inContainer->clear();
}

// template <class T> class KF_IPointerList;
// typedef KF_IPointerList<GRNotationElement> GRNEList;

typedef std::vector<GRArticulation *> GRNEList;

/** \brief Graphical representation for an event (= has duration). 
  
    Because it's is possible to show only a part of the events duration 
    exists an own duration and start time independent from the AREvent.
*/

class GREvent : public GRARCompositeNotationElement
{
	public:
				 GREvent( GRStaff * inStaff, const ARMusicalEvent * ar, bool p_ownsAR = false );
				 GREvent(GRStaff * inStaff, const ARMusicalEvent * ar, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & dur);

		virtual ~GREvent();

		virtual void addToOffset(const NVPoint & offs);

		// this can be used by the derived classes
		// to find out about size. Will be set by noteFormat  
		virtual float getSize() const				{ return mSize; }

		// the offset is a global offset (defined with noteFormat ...)
		virtual const NVPoint & getOffset() const  { return mOffset; }

	    virtual const NVstring & getStyle() const	{ return mStyle; }
		virtual const unsigned char * getColRef() const { return mColRef;  }

		virtual void	setSize(float nsize)  { mSize = nsize; }
		
		virtual void	addArticulation( const ARMusicalTag * inTag );
				bool	hasArticulation( int inArticulationFlag ) const;

		virtual int		adjustLength (const TYPE_DURATION & ndur );

		virtual NVPoint getCrescEnd();
		virtual NVPoint getCrescStart();
	
	  	virtual void 	setHPosition( float inX );
		virtual	void	setPosition( const NVPoint & inPos );

		virtual void 	updateBoundingBox();

		// - Stem stuff
		virtual GRGlobalStem * getGlobalStem() const	{ return  mGlobalStem; }
		virtual void	setGlobalStem( GRGlobalStem * inGlobalStem );
		virtual void	setGlobalLocation(GRGlobalLocation * pglog);

		virtual void 	setBeamStem( GRBeam * inBeam, float pos);
		virtual float 	setStemLength( float inLen, bool userLength = false );

		virtual GDirection getStemDirection() const;

		virtual void 	setStemDirection( GDirection dir );
		virtual void 	setFlagOnOff(bool i);

		virtual bool 	getStemDirSet() const;
		virtual bool 	getStemLengthSet() const;
		virtual float 	getStemLength() const;

		virtual NVPoint getStemStartPos() const;
		virtual NVPoint getStemEndPos() const;
	   

		virtual float 	changeStemLength( float inLen, bool force = false );
		virtual int 	getNumFaehnchen() const;
		virtual int 	getBeamCount() const	{ return mBeamCount; }
		virtual void 	incBeamCount()			{ ++ mBeamCount; }
		virtual void 	decBeamCount()			{ -- mBeamCount; }
		virtual void 	setFillsBar(bool value, GRNotationElement *, GRNotationElement *, bool)  	{ mFillsBar = value; }

		virtual bool	getFillsBar() const		{ return mFillsBar; }
			
				bool	isSyncopated() const; 
	
		// - Duration dot
		virtual void	createDots( const TYPE_DURATION & duration, float notewidth, const NVPoint & inPos );
		virtual void	setDotFormat( const ARDotFormat * inFormat );
		virtual void	setDotFormat( GRNoteDot * inDot, const ARDotFormat * inFormat );

		virtual	bool	isGREventClass() const		{ return true; }
		static GREvent * cast( GObject * inObj )	{ return ( inObj->isGREventClass() ? static_cast<GREvent *>(inObj) : 0 ); }

		const GRNEList& getArticulations() const		{ return mArtilist; }
		GRNEList&		getArticulations()				{ return mArtilist; }
		virtual bool	stemHasBeenChanged() const		{return stemChanged;}
		virtual void	setStemChanged()				{stemChanged = true;}
		virtual const GREvent *	isGREvent() const		{ return this; }
		virtual 	  GREvent *	isGREvent() 			{ return this; }

		GRNoteDot *		getDot() const;

  protected:
		int		mArticulationFlags;
	  NVstring	mStyle;
	  NVPoint	mOffset;
	  float		mSize;
	  unsigned char * mColRef;
	  int		mBeamCount;

	  // this just remembers the LSPACE that is valid at time of creation (staffFormat-Tag and consorts)
	  float		mCurLSPACE;
	  // pointer to a global stem, if present. This means, that this note shares a stem with other events...
	  GRGlobalStem * mGlobalStem;
	  
	  bool		mFillsBar;
	  bool		stemChanged;

	private:
		GRNEList	mArtilist;	// our list of articulations.
};

#endif
