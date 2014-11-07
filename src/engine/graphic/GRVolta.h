#ifndef __GRVolta__
#define __GRVolta__

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
        

        int         mShape;             // the volta shape  (from ARVolta)
        const char* mString;            // the volta string (from ARVolta)
        int         mStringSize;
        
    static std::map<int, GRVolta *> mPrevious;       // link adjacents voltas
};

#endif
