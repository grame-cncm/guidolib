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
#include <vector>
#include "GRPTagARNotationElement.h"

class ARVolta;
class GRStaff;


/** \brief an ending bracket graphical element.
*/
class GRVolta : public GRPTagARNotationElement
{
	private:
	std::vector<NVRect> fSegments; 	// segments of the volta
	NVRect				fCurrentSegment;
	bool 				fStartTell = true;
	
	void drawText   (VGDevice & hdc, float x, float y) const;
	void drawEndings (VGDevice & hdc, size_t index, float dx, float dy) const;

	const ARVolta * 	getARVolta() const 	{ return (const ARVolta*)getAbstractRepresentation(); }
	void  				adjustLeft (const GRNotationElement * startElt);	// adjust first segment to the previous bar (if any)
	void  				adjustRight(const GRNotationElement * endElt);		// adjust last segment to the next bar (if any)
	void  				adjustToNext (GRVolta * next);						// vertical adjustment for adjacent voltas
	GRVolta * 			getNextVolta(GRStaff * staff) const;

	public:
        enum { kDefault, kRightOpened, kLeftOpened, kOpened };

						GRVolta( GRStaff * pstaff, const ARVolta * ar );
		virtual			~GRVolta();
		
		virtual void	OnDraw( VGDevice & hdc ) const;
		virtual const	NVPoint & getReferencePosition() const  { return refpos; }
		virtual void    tellPosition(GObject * caller, const NVPoint & newPosition);
        virtual void    FinishPTag (GRStaff * grstaff);

        GRNotationElement * getEndPos() const   { return mEnd; }
        GRNotationElement * getBegPos() const   { return mBeg; }
        NVRect 			getFirstSegment() const	{ return fSegments[0]; }
        size_t 			getSegmentsSize() const	{ return fSegments.size(); }
        void 			setFirstSegment(NVRect r) { fSegments[0] = r; }

	protected:
		GRNotationElement * getEndElt(GRNotationElement *after);
		GRNotationElement * getBegElt(GRNotationElement *before);

		static NVPoint refpos;
        GRNotationElement * mEnd, *mBeg;// these are for graphic adjustments
        
        int         mShape;             // the volta shape  (from ARVolta)
        const char* mString;            // the volta string (from ARVolta)
        int         mStringSize;
};

#endif
