#ifndef GRBar_H
#define GRBar_H

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

#include "GRTagARNotationElement.h"

class ARBar;
class GRStaff;

/** \brief Measure bar.
*/
class GRBar : public GRTagARNotationElement
{
	public:
		
		GRBar( ARBar * p_arbar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos );

		GRBar( ARBar * p_arbar, GRSystem * p_grsystem, GRStaff * inStaff,
									const TYPE_TIMEPOSITION &  inTimePos);

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
		virtual void GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void print() const;
		
		virtual const NVPoint & getReferencePosition() const { return sRefPos; }
		
		ARBar * getARBar() const;
		
	protected:
				bool	isSystemSlice() const;

		static NVPoint sRefPos;

	private:
		void InitGRBar( const TYPE_TIMEPOSITION & inTimePos, const GRStaff * inStaff );		
};

#endif
