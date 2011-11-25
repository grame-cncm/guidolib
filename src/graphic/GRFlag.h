#ifndef GRFlag_H
#define GRFlag_H

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

};

#endif
