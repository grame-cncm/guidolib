#ifndef GRTag_H
#define GRTag_H

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

#include "NVPoint.h"

class NVstring;

// The StaffManager holds different hashtables for the
// Synchronizsation elements to detect contraditions.

class GRPage;
class GRSystem;
class GRStaff;
class ARMusicalTag;

/** \brief The GRTag class is the base class for all GUIDO tags. 

	It is used to distinguish different types of tags. 
	There are Voice-Tags, Staff-Tags and System-Tags and even Page-Tags. This distinction is used to
	 build rods/springs and to find contradicting entries for tags on the different levels. 
*/

class GRTag
{
public:
	enum TAGTYPE { VOICETAG,STAFFTAG, SYSTEMTAG,PAGETAG, VOICEANDSYSTEMTAG };

					GRTag(const ARMusicalTag * artag = 0, float curLSPACE = 50.0f );
	virtual 		~GRTag();
	
	virtual bool operator!=(const GRTag &tag) const     { return !(this->operator==(tag)); }

	// this is overloaded in other classes ...
	virtual bool operator==(const GRTag &tag) const; 

            int			getIsAuto() const;
            TAGTYPE		getTagType() const      { return tagtype; }
            int			getError();
            bool		IsStateTag() const;
	virtual const NVPoint& getOffset() const    { return mTagOffset; }
	virtual float       getSize() const         { return mTagSize; }
	virtual float       getSConst() const       { return sconst; }

    // returns name of a font if present ...
	virtual const NVstring *        getFont() const         { return font; }
	virtual const NVstring *        getFontAttrib() const   { return fontAttrib; }
    // this is the standard size ...
	virtual int                     getFontSize() const     { return mFontSize; }
	virtual const unsigned char *   getColRef() const       { return mColRef; }

	virtual void        setSConst(float val)    { sconst = val; }
            void		setTagType(TAGTYPE ntyp) {  tagtype = ntyp; }
            void		setError(int p_error);

	virtual void StaffFinished(GRStaff * grstaff = 0);
	virtual void StaffBegin(GRStaff * grstaff = 0);
	virtual void RangeEnd(GRStaff * grstaff = 0);

protected:

	int error;
	TAGTYPE tagtype;
	int isautotag;
	float sconst; // ?
	
	unsigned char * mColRef;	// for the color-parameter ... (RGBA) [0, 255]

	NVstring * font;
	NVstring * fontAttrib;

	int mFontSize;

	float mTagSize;
	NVPoint mTagOffset;

	//int openRightRange;
	//int openLeftRange;
};

#endif
