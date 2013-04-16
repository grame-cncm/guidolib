#ifndef GRChord_H
#define GRChord_H

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

#include "GREvent.h"

class ARChord;
class ARDotFormat;
class GRStem;
class GRFlag;
class GRStaff;


/** \brief Chord. Notes may share the same stem.
*/

class GRChord : public GREvent
{
	public:

		GRChord( GRStaff *, 
				ARChord * archord,
				const TYPE_TIMEPOSITION & von,
				const TYPE_TIMEPOSITION & bis,
				const TYPE_TIMEPOSITION & dtempl );


		virtual void setDotFormat( const ARDotFormat * inFormat );
		virtual int adjustLength( const TYPE_DURATION & ndur );
	
		virtual NVPoint getTieEnd() const;
		virtual NVPoint getTieStart() const;
	
	//	virtual void setPosition( const NVPoint & inPos );


		virtual void print() const;

		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & dev ) const;

		virtual void updateBoundingBox();

		virtual int getNumFaehnchen() const;
		virtual float changeStemLength( float inLen );

		virtual void setFlagOnOff( bool p );

		virtual NVPoint getStemStartPos() const;
		virtual NVPoint getStemEndPos() const;

		virtual GDirection getStemDirection() const
					{ return mStemDir; }
	
	  virtual void setStemDirection( GDirection pdir );


	protected:


		const 	GRFlag * 	getGRFlag() const;
				GRFlag * 	getGRFlag();
	    const 	GRStem * 	getStem() const;
	    		GRStem *	getStem();

		float 		mStemLen;
		GDirection	mStemDir;

		float 		mPosYBottom;
		float		mPosYTop;

};

#endif

