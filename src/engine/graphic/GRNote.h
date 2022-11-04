#ifndef GRNote_H
#define GRNote_H

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

#include "GREvent.h"

class ARNote;
class ARNoteFormat;
class ARDotFormat;
class GRStaff;
class GRTie;
class GRTrill;
class GRCluster;

/** \brief Graphical representation of a note. 
*/

class GRNote : public GREvent
{
	public:
	  			 GRNote (GRStaff * inStaff, const ARNote * note, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration );
				 GRNote (GRStaff * inStaff, const TYPE_DURATION & duration );
				 GRNote (GRStaff * inStaff, const ARNote * note );
	virtual 	~GRNote ();

	virtual void setNoteFormat( const ARNoteFormat * frmt );


//		ARNote       * getARNote();
		const ARNote * getARNote() const;

	virtual void 	OnDraw( VGDevice & hdc ) const		= 0;
	virtual void 	recalcVerticalPosition()			= 0;
	virtual void 	drawStemOnly( int flag )			= 0;
	virtual void 	setStemDirection( GDirection dir )	= 0;
	virtual float 	setStemLength( float inLen, bool userLength )		= 0;
	
	virtual	void	getPitchAndOctave( int * outPitch, int * outOctave, int * outAccidentals ) const;

	virtual GDirection	getDefaultThroatDirection() const;
	virtual GDirection	getThroatDirection() const;

    GRCluster *getGRCluster() const			{return fCluster;}
    void       setGRCluster(GRCluster *inCluster, bool inSignificativeNote = true);
    bool       getClusterNoteBoolean() const {return fClusterNote;}
    bool       isGraceNote() const			{return fIsGraceNote;}
    void       setGraceNote(bool isGrace)	{fIsGraceNote = isGrace;}

    GRCluster *createCluster(const ARNoteFormat *inCurnoteformat);

	// Tell me: is a note splitted over several systems?
	// should only occur for CompositeNotes.
	virtual bool 	isSplit(); // const;
	void 			hideAccidentals();

    /**** Function to avoid dynamic_cast ****/
    const GRNote * isGRNote() const { return this; }
    /*****************************************/
	
protected :
		GRTrill *fOrnament;
        GRCluster *fCluster;
        bool fOwnCluster;
        bool fClusterNote;
        bool fClusterHaveToBeDrawn;
        bool fIsGraceNote;
};


#endif


