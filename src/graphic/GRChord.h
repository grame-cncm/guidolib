#ifndef GRChord_H
#define GRChord_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

