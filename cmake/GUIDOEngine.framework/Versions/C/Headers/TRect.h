
/*
	GUIDO Library
	Copyright (C) 2003-2004  Grame

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

#ifndef TRect_H
#define TRect_H

#include<ostream>

// ---------------------------------------------------------------------------------
// Origin is the top-left corner, when y grows, positions are descending.
template <typename T>
class TRect
{
	public:

				T left;
				T top;
				T right;
				T bottom;

				TRect() : left(0), top(0), right(0), bottom(0) { }
				TRect( T inLeft, T inTop, T inRight, T inBottom )
						: left( inLeft ), top( inTop ), right( inRight ), bottom( inBottom ) {}
				
		void	Set( T inLeft, T inTop, T inRight, T inBottom )
					{ left = inLeft; top = inTop; right = inRight; bottom = inBottom; }

		void	SetValid( T inLeft, T inTop, T inRight, T inBottom )
					{ 
						if (inLeft > inRight) {
							left = inRight; right = inLeft;
						}
						else {
							left = inLeft; right = inRight;
						}
						if (inTop > inBottom) {
							top = inBottom; bottom = inTop;
						}
						else {
							top = inTop; bottom = inBottom;
						}
					}

		void	Set( const TRect & in )
					{ left = in.left; top = in.top; right = in.right; bottom = in.bottom; }
								
		bool		IsValid() const { return (Width() > 0 && Height() > 0); }
		T			Width() const { return (right - left); }
		T			Height() const { return (bottom - top); }

		void		SetDims( T inWidth, T inHeight ) { SetWidth( inWidth ); SetHeight( inHeight ); }
		void		SetWidth( T in ) { right = left + in; }
		void		SetHeight( T in ) { bottom = top + in; }

		void		SetPos( T x, T y ) { SetXPos( x ); SetYPos( y ); }
		void		SetXPos( T inXPos ) { right += (inXPos - left); left = inXPos; }
		void		SetYPos( T inYPos ) { bottom += (inYPos - top); top = inYPos; }
		
		void		GetPos( T* x, T* y ) { *x = left; *y = top;}
		void		GetXPos( T* inXPos ) { *inXPos = left; }
		void		GetYPos( T* inYPos ) { *inYPos = top; }
		void		GetPos( T* x1, T* y1, T* x2, T* y2 ) { *x1 = left; *y1 = top; *x2 = right; *y2 =  bottom;}
		
		void		ShiftX( T inOffset ) { left += inOffset; right += inOffset; }
		void		ShiftY( T inOffset ) { top += inOffset; bottom += inOffset; }
		void		Shift( T x, T y ) { left += x; right += x; top += y; bottom += y; }
		void		Scale( T x, T y ) { left *= x; right *= x; top *= y; bottom *= y; }
		void		Expand( T val ) { left -= val; right += val; top -= val; bottom += val; }
		void		Magnify( float ratio ) { T x = Width()*ratio; T y = Height()*ratio; left -= x/2.; right += x/2.; top -= y/2.; bottom += y/2.; }
		void		Zoom( T offset ) { left -= offset; top -= offset; 
										right += offset; bottom += offset; }
		
		// ---------------------------------------------------------------------------------
		template <typename S> TRect<T> & operator += ( const S & inVector )
		{
			left += inVector.x;
			right += inVector.x;
			top += inVector.y;
			bottom += inVector.y;
			return *this;
		}

		// ---------------------------------------------------------------------------------
		template <typename S> bool operator == ( const S & inRect ) const
		{
			return ( (left == inRect.left)
				  && (right == inRect.right)
				  && (top == inRect.top)
				  && (bottom == inRect.bottom) );
		}

		// used to sort rectangles using y ordering
		// ---------------------------------------------------------------------------------
		template <typename S> bool operator < ( const S & inRect ) const
		{
			return ( (left <= inRect.left)
				  && (top < inRect.top) );
		}

		// ---------------------------------------------------------------------------------
		template <typename S> void CalcMiddle( S * outVector )
		{
			outVector->x = (left + right) * 0.5;
			outVector->y = (top + bottom) * 0.5;
		}
		
		// ---------------------------------------------------------------------------------
		void CalcMiddle( T * oX, T * oY )
		{
			*oX = (T)((left + right) * (0.5));
			*oY = (T)((top + bottom) * (0.5));
		}
		
		// ---------------------------------------------------------------------------------
		template <typename S> bool Contains( const S & inVector ) const
		{
			return (inVector.x >= left && inVector.x < right && 
						inVector.y >= top && inVector.y < bottom );
		}

		// ---------------------------------------------------------------------------------
		bool Contains( const TRect<T> & in ) const
		{
			return (in.left >= left && in.right <= right && in.top >= top && in.bottom <= bottom );
		}
		
		// ---------------------------------------------------------------------------------
		bool Contains( T inX, T inY ) const
		{
			return (inX >= left && inX < right && inY >= top && inY < bottom );
		}

		// ---------------------------------------------------------------------------------
		template <typename S> bool Collides( const S & inRect ) const
		{
			return (( inRect.left < right ) && ( inRect.right > left )
						&& ( inRect.top < bottom ) && ( inRect.bottom > top ));
		}

		// ---------------------------------------------------------------------------------
		template <typename S> void Merge( const S & inRect )
		{
			if( inRect.Width() <= 0 || inRect.Height() <= 0 ) return;
			else if( Width() <= 0 || Height() <= 0 ) Set( inRect );
			else 
			{
				if( inRect.left < left ) left = inRect.left; 
				if( inRect.right > right ) right = inRect.right; 
				if( inRect.top < top ) top = inRect.top; 
				if( inRect.bottom > bottom ) bottom = inRect.bottom; 
			}
		}

		// ---------------------------------------------------------------------------------
		template <typename S> void Intersect( const S & inRect )
		{
			if( inRect.left > left )	left = inRect.left;
			if( inRect.right < right )	right = inRect.right;
			if( inRect.top > top )		top = inRect.top;
			if( inRect.bottom < bottom ) bottom = inRect.bottom;

			if( right < left ) { right = 0; left = 0; }
			if( bottom < top ) { top = 0; bottom = 0; }
		}

		// ----------------------------------------------------------------------------
		// returns the power-of-two of the distance between input point and our 
		// nearest edge returns 0 of the point is inside us.
		T DistanceToPoint( T x, T y )
		{
			T deltaX = 0;
			if( x < left )		deltaX = left - x;
			else if( x > right ) deltaX = x - right;
			
			T deltaY = 0;
			if( y < top )		deltaY = top - y;
			else if( y > bottom ) deltaY = y - bottom;
			
			return ((deltaX * deltaX) + (deltaY * deltaY));
		}

		// ----------------------------------------------------------------------------
		void print( std::ostream& os ) const { os << "[" << left << "," << top << "," << right << "," << bottom << "]"; }
};

template<typename T> std::ostream& operator<< (std::ostream& os, const TRect<T>& r) { r.print(os); return os; }

// - Usual rects
typedef TRect<float> FloatRect;
typedef TRect<int> IntRect;

#endif
