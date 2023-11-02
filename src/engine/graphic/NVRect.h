#ifndef NVRect_H
#define NVRect_H

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

#include <iostream>

#include "NVPoint.h"

class NVRect
{
	public:
				NVRect() : left(0), top(0), right(0), bottom(0) { }
				NVRect(float p_left, float p_top, float p_right, float p_bottom ) :
						left( p_left ), top( p_top ), right( p_right ), bottom( p_bottom ) { }

	//	virtual ~NVRect() { } // should not be overriden
	
				NVRect & operator +=( const NVPoint & in );
				NVRect & operator -=( const NVPoint & in );
				NVRect & operator *=( float v );
				bool	 operator ==( const NVRect& r ) const { return (left==r.left) && (top==r.top) && (right==r.right) && (bottom==r.bottom); }
				NVRect   operator +( const NVPoint & in ) const;
	
		void	Print( std::ostream& os ) const { os << "[" << left << ", " << top << ", " << right << ", " << bottom << "]"; }
		void	Set( const NVRect & in )
					{ Set( in.left, in.top, in.right, in.bottom ); }

		void	Set( float inL, float inT, float inR, float inB );
		void	Merge( const NVRect & in );
		NVRect	Scale( float val ) const;

		float	Height() const 		{ return bottom - top; }
		float	Width() const 		{ return right - left; }

		NVPoint	TopLeft() const 	{ return NVPoint (left, top); }
		NVPoint	BottomRight() const { return NVPoint (right, bottom); }


		float left;
		float top;
		float right;
		float bottom;

		bool Contains( const NVPoint & p ) const;
		bool Collides( const NVRect & r ) const;
};

std::ostream& operator<< (std::ostream& os, const NVRect& r);

#endif
