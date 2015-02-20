#ifndef GRBar_H
#define GRBar_H

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

#include "GRTagARNotationElement.h"

class ARBar;
class GRStaff;

/** \brief Measure bar.
*/
class GRBar : public GRTagARNotationElement
{
	public:
		
		GRBar( ARBar * p_arbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos, float propRender);

		GRBar( ARBar * p_arbar, GRSystem * p_grsystem, GRStaff * inStaff,
									const TYPE_TIMEPOSITION &  inTimePos, float propRender);

		virtual ~GRBar();

		bool operator == (const GRBar & bar) const;
		bool operator == (const GRTag & tag) const;
		
		void tellPosition( GObject * caller, const NVPoint & newPosition);
		void setHPosition( float nx);
		
		void setPosFrom( float posy );
		void setPosTo( float posy );
		
		virtual void setGRStaff(GRStaff * pstaff);
		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void DrawWithGlyphs( VGDevice & hdc ) const;
		virtual void DrawWithLines( VGDevice & hdc ) const;
		virtual void GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
				
		virtual const NVPoint & getReferencePosition() const { return sRefPos; }
		
		ARBar * getARBar() const;
		
	protected:
				bool	isSystemSlice() const;

		static NVPoint sRefPos;
        int    fLineNumber;

        float mDx;
        float mDy;
        float mStaffRatio;
		float proportionalRender;

	private:
		void InitGRBar( const TYPE_TIMEPOSITION & inTimePos, const GRStaff * inStaff );		
};

#endif
