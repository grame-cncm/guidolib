#ifndef GROctava_H
#define GROctava_H

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

#include <map>
#include <vector>

#include "TagParameterString.h"
#include "GRARNotationElement.h"
#include "GRPositionTag.h"
#include "GRVisitor.h"

class NVstring;
class GRStaff;
class NVPoint;
class VGDevice;
class AROctava;

/** \brief not yet documented
*/
class GROctava : public GRARNotationElement, public GRPositionTag
{
	public:
					 GROctava( GRStaff *, const NVstring & txt, const AROctava* ar, bool bassa );
		virtual 	~GROctava();

		virtual void accept (GRVisitor& visitor);
		virtual void tellPosition(GObject *caller, const NVPoint & );

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void setColRef(const TagParameterString *tps);
		virtual bool DeleteStaff(GRStaff * grstaff);
		virtual int  getOctava() const { return fOctava; };
	
	private:
		typedef struct segment {
			float x1 = 0;
			float x2 = 0;
			float x3 = 0;
			float y = 0;
			int   index = 0;
			void reset() { x1 = x2 = x3 = y = 0; }
		} TSegment;
		std::vector<TSegment> fSegments;
		std::map<const GRSystem*, TSegment>	fSegmentsMap;
		TSegment fCurrent;
		
		int		fOctava = 0;
		
		NVRect	getExtensionLine (const NEPointerList * assoc, GuidoPos start, GuidoPos end) const;
		NVRect	getEltBox (const GRNotationElement* el) const;

		TSegment nvrect2Segment(const NVRect& r) const;
		void 	 drawText (const TSegment& seg, VGDevice & hdc ) const;
		void 	 drawLine (const TSegment& seg, bool last, VGDevice & hdc ) const;

		GRStaff *	fStaff;
		NVstring 	fText;
		bool		fBassa;
		bool		fHidden;
		float		fTextHeight;
		float		fDx = 0;
		float 		fDy = 0;
};

#endif

