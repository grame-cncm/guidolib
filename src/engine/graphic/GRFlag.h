#ifndef GRFlag_H
#define GRFlag_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRNotationElement.h"

class GREvent;
class GRGlobalStem;


/** \brief Flags attached to a note stem.
*/
class GRFlag : public GRNotationElement
{
public:
	friend class GREvent;

	enum FLAGTYPE { NONE = kNoneSymbol,
		 H8U = kFlag8UpSymbol, 
		 H8D = kFlag8DownSymbol,  
		 H16U = kFlag16UpSymbol,
		 H16D = kFlag16DownSymbol, 
		 H32U = kFlag32UpSymbol,
		 H32D = kFlag32DownSymbol,
		 H64U = kFlag64UpSymbol, 
		 H64D = kFlag64DownSymbol };

	GRFlag(const TYPE_DURATION & durtempl);

	GRFlag(GRGlobalStem * gstem, const TYPE_DURATION & durtempl,
		GDirection dir, float stemlength);

	GRFlag(GREvent * sngnot, const TYPE_DURATION & durtempl,
		GDirection dir, float stemlength,
		float notebreite);

	virtual ~GRFlag();
    
    virtual void configureForChord(GRGlobalStem *gstem, const TYPE_DURATION& duration);
    virtual void configureForSingleNote(GREvent *sngnot, const TYPE_DURATION& duration);

	virtual void changeStemLength (GREvent * sngnot, float inLen, GDirection stemdir);

	virtual const NVPoint & getOffset() const				{ return mOffset; }
	virtual const NVPoint & getReferencePosition() const	{ return sRefpos; }

	void setFlagOnOff(bool p);

	virtual float getSize() const		{ return mSize; }
	virtual const unsigned char * getColRef() const	{ return mColRef; }
	void setStraight(bool nstr)			{ mStraight = nstr; }
	bool getStraight() const			{ return mStraight; }

	virtual void OnDraw(VGDevice & hdc ) const;
	virtual void GGSOutput() const;

	virtual int getNumFaehnchen() const;
	virtual void setColRef(const unsigned char * tmpcolref);

protected:
			void	calcFlagExtent( float inNoteBreite );
			void	initialize(const TYPE_DURATION & duration, GDirection stemdir, float notebreite);

	static NVPoint 		sRefpos;

	float				mSize;
	unsigned char * 	mColRef;
	NVPoint 			mOffset;
	bool 				mFlagOn;

	// is set to one, if the flag should be drawn straight...
	bool 				mStraight;

    GDirection     fStemdir;
    float          fStemlength;
    float          fNotebreite;
};

#endif
