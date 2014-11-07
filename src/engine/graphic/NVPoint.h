#ifndef NVPoint_H
#define NVPoint_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
