#ifndef VGColor_H
#define VGColor_H

/*
	GUIDO Library
	Copyright (C) 2006	Grame
	
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

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

// define int constants to bound alpha values
#define ALPHA_TRANSPARENT	0
#define ALPHA_OPAQUE		255


// --------------------------------------------------------------
// 		VGColor class
// --------------------------------------------------------------
/** \brief Generic class to manipulate device independant colors. 
*/				
class VGColor
{
	public:		
					VGColor( unsigned char gray  = 0 ) 
						: mRed( gray ), mGreen( gray ), mBlue( gray ), mAlpha( ALPHA_OPAQUE ) { }
					VGColor( const VGColor & in ) 
						{ Set( in ); }
					VGColor( unsigned char r, unsigned char g, 
							 unsigned char b, unsigned char a = ALPHA_OPAQUE ) 
						: mRed( r ), mGreen( g ), mBlue ( b ), mAlpha( a ) { }

		explicit	VGColor( const unsigned char col [4] ) 
						{ if( col ) Set( col[0], col[1], col[2], col[3] ); else Set( 0 ); }

		void		Set( unsigned char r, unsigned char g, 
						 unsigned char b, unsigned char a = ALPHA_OPAQUE )
						{ mRed = r; mGreen = g; mBlue = b, mAlpha = a; }
		void		Set( const VGColor & in )
						{ mRed = in.mRed; mGreen = in.mGreen; mBlue = in.mBlue, mAlpha = in.mAlpha; }

				
		bool 		operator == ( const VGColor & col ) const
						{ return (( col.mRed == mRed ) && ( col.mGreen == mGreen ) 
						  && ( col.mBlue == mBlue ) && ( col.mAlpha == mAlpha )); }
		
		bool 		operator != ( const VGColor & col ) const
						{ return (( col.mRed != mRed ) || ( col.mGreen != mGreen ) 
						  || ( col.mBlue != mBlue ) || ( col.mAlpha != mAlpha )); }
				
		VGColor &	operator += ( short v );

        //modifications of this method must be applied also in the action parser.
        //This fonction displays color as html does.  ex : #45fc0a[ff]
        std::ostream& print(std::ostream& out) const
        {
            out << "#";
            char prev = out.fill ('0');
            out.width(2);out << std::hex << int(mRed);
            out.width(2);out << std::hex << int(mGreen);
            out.width(2);out << std::hex << int(mBlue);
            out.width(1);out << "[";
            out.width(2);out << std::hex << int(mAlpha) << std::dec;
            out.width(1);out << "]";
            out.fill(prev);

            return out;
        }
			
		unsigned char mRed;
		unsigned char mGreen;
		unsigned char mBlue;
		unsigned char mAlpha; // 0 = invisible, 255 = opaque. 
};

				
inline VGColor & VGColor::operator += ( short v ) 
{ 
	int sred = mRed + v; 
	mRed   = (unsigned char)((sred > 255) ? 255 : sred);
	int sgreen = mGreen + v;
	mGreen = (unsigned char)((sgreen > 255) ? 255 : sgreen);
	int sblue = mBlue + v;
	mBlue  = (unsigned char)((sblue > 255) ? 255 : sblue);
	return *this;
}

inline std::ostream& operator << (std::ostream& out, const VGColor& c)
{
    return (&c)->print(out);
}


/*! @} */

#endif /* VGColor */
