#ifndef GRStem_H
#define GRStem_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

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

#include "NVPoint.h"
#include "GRNotationElement.h"

class GREvent;
class GRSingleNote;
class GRChord;
class GRGlobalStem;

/** \brief Graphical representation of a stem
*/
class GRStem : public GRNotationElement
{
public:

	friend class GRChord;
	friend class GRSingleNote;

	// the LS-Parameters mean:
	// LENGTH - SET (the stemlength
	// was set explicitly)

	// the constructur for global stems ...
	GRStem(GRGlobalStem * gstem);

	GRStem(GREvent * sngnot,
		const TYPE_DURATION & durtempl,
		GDirection dir,
		float length,
		float notebreite);

	virtual ~GRStem();


	virtual void setStemLength( float inLen );
	virtual void setStemDir(GDirection dir);
	virtual const NVPoint & getOffset() const		{ return mOffset; }
	virtual void addToOffset(const NVPoint &pt)		{ mOffset += pt; }
	virtual const NVPoint & getReferencePosition() const;

	virtual const unsigned char * getColRef() const	{ return mColRef; }

	virtual float getSize() const	{ return mSize; }

	virtual void OnDraw( VGDevice & hdc ) const;
	virtual void GGSOutput() const;

	virtual GDirection getStemDir() const	{ return mStemDir; }
	virtual float getStemLength() const	{ return mStemLen; }

	virtual void setColRef( const unsigned char * inColor );

protected:

	float 		mStemLen;
	GDirection 	mStemDir;

	float 		mSize;		// (JB) is it related to the height of the 
							// 'stem' glyph in the musical font ? 
							//	stem height = 0.5 * LSPACE * mSize
	unsigned char * mColRef;
	NVPoint 	mOffset;

	static NVPoint sRefpos;
};

#endif

