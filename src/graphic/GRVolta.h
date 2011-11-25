#ifndef __GRVolta__
#define __GRVolta__

/*
	GUIDO Library
	Copyright (C) 2004	Grame

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

#include <map>
#include "GRPTagARNotationElement.h"

class ARVolta;
class GRStaff;


/** \brief an ending bracket graphical element.
*/
class GRVolta : public GRPTagARNotationElement
{
	public:
        enum { kDefault, kRightOpened, kLeftOpened, kOpened };

						GRVolta( GRStaff * pstaff, ARVolta * ar );
		virtual			~GRVolta();
		
		virtual void	OnDraw( VGDevice & hdc ) const;
		virtual const	NVPoint & getReferencePosition() const  { return refpos; }
		virtual void    tellPosition(GObject * caller, const NVPoint & newPosition);
        virtual void    FinishPTag (GRStaff * grstaff);

		void            adjustPrevious(const GRStaff * staff);
        GRNotationElement * getEndPos() const   { return mEnd; }
        GRNotationElement * getBegPos() const   { return mBeg; }

	protected:
		GRNotationElement * getEndElt(GRNotationElement *after);
		GRNotationElement * getBegElt(GRNotationElement *before);
		void                adjustRight(const GRNotationElement *endElt);

		static NVPoint refpos;

        NVRect      mFirstPart;
        NVPoint     mFirstPosition;
        GRSystem *  mCurrSystem;        // used to detect broken volta
		bool        mBroken;            // indicates that the volta is split on two systems
        GRNotationElement * mEnd, *mBeg;// these are for graphic adjustments
        

        int         mShape;             // the volta shape (from ARVolta)
        const char* mString;            // the volta string (from ARVolta)
        int         mStringSize;
        
    static std::map<int, GRVolta *> mPrevious;       // link adjacents voltas
};

#endif
