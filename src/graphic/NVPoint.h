#ifndef NVPoint_H
#define NVPoint_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003	Grame

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

class NVPoint
{
	public:

				NVPoint() : x( 0 ), y( 0 ) { }
				NVPoint( float p_x, float p_y ) : x( p_x ), y( p_y ) { }
				NVPoint( const NVPoint & nvp )
					{
						x = nvp.x;
						y = nvp.y;
					}

				~NVPoint() { }

		void	Print( std::ostream& os ) const { os << "[" << x << "," << y << "]"; }
		void	Set( float inX, float inY ) { x = inX; y = inY; }
		
		NVPoint & operator=(const NVPoint & p)
		{
			x = p.x;
			y = p.y;
			return *this;
		}

		NVPoint & operator-=(const NVPoint & p)
		{
			x -= p.x;
			y -= p.y;
			return *this;
		}

		NVPoint & operator+=(const NVPoint & p)
		{
			x += p.x;
			y += p.y;
			return *this;
		}

		bool operator==(const NVPoint & p) const
		{
			return (x == p.x && y == p.y);
		}

		bool operator!=(const NVPoint & p) const
		{
			return (x != p.x || y != p.y);
		}

		float x;
		float y;
};


std::ostream& operator<< (std::ostream& os, const NVPoint& p);

// - Globals -
NVPoint operator-( const NVPoint & p1, const NVPoint & p2 );
NVPoint operator+( const NVPoint & p1, const NVPoint & p2 );

inline NVPoint operator-( const NVPoint & p1, const NVPoint & p2 )
{
	return (NVPoint( p1.x - p2.x, p1.y - p2.y ));
}

inline NVPoint operator+( const NVPoint & p1, const NVPoint & p2 )
{
	return (NVPoint( p1.x + p2.x, p1.y + p2.y ));
}



#endif
