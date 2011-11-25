#ifndef GRAccolade_H
#define GRAccolade_H

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

	(JB) 04/2004. Creation of GRAccolade to separate the accolade code from
				the class GRSystem. It currently derives from GObject, but
				it should probably be a GRTagARNotationElement

*/

#include "GObject.h"


/** \brief The system accolade


*/
class GRAccolade : public GObject // GRTagARNotationElement ?
{
	public:

		enum { kAccoladeNone, kAccoladeStraight, kAccoladeCurly, kAccoladeThin };
		
						GRAccolade( int inAccoladeType = kAccoladeCurly, float inDx = 0 );
		virtual			~GRAccolade();

		virtual void	setAccoladeType( int in ) { mAccoladeType = in; }
		virtual void	setDx( float in ) { mDx = in; }

		virtual void	draw( VGDevice & hdc, const NVPoint & leftTop, 
								const NVPoint & leftBottom ) const;

		virtual void	OnDraw( VGDevice &) const { }
		
		
	protected:


		void		DrawEmulatedAccolade( VGDevice & hdc, float x, 
											float yTop, float yBottom ) const;
	
		void		DrawStraightAccolade( VGDevice & hdc, float x, float yTop, 
									float yBottom, bool drawEndCurves = true ) const;
	

		
		int			mAccoladeType;
		float		mDx;
		
};

#endif
