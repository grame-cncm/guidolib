#ifndef NVRect_H
#define NVRect_H

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

#include <iostream>

class NVPoint;

class NVRect
{
	public:
				NVRect() : left(0), top(0), right(0), bottom(0) { }
				NVRect(float p_left, float p_top, float p_right, float p_bottom ) :
						left( p_left ), top( p_top ), right( p_right ), bottom( p_bottom ) { }

	//	virtual ~NVRect() { } // should not be overriden
	
				NVRect & operator +=( const NVPoint & in );
				NVRect & operator -=( const NVPoint & in );
				
				
				NVRect operator +( const NVPoint & in );

		void	Print( std::ostream& os ) const { os << "[" << left << "," << top << "," << right << "," << bottom << "]"; }
		void	Set( const NVRect & in )
					{ Set( in.left, in.top, in.right, in.bottom ); }

		void	Set( float inL, float inT, float inR, float inB );
		void	Merge( const NVRect & in );

		float	Height() const { return bottom - top; }
		float	Width() const { return right - left; }

		float left;
		float top;
		float right;
		float bottom;

		bool Contains( const NVPoint & p ) const;
		bool Collides( const NVRect & r ) const;
};

std::ostream& operator<< (std::ostream& os, const NVRect& r);

#endif
