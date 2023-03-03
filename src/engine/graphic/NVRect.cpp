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

#include "NVRect.h"
#include "NVPoint.h"

std::ostream& operator<< (std::ostream& os, const NVRect& r) { r.Print (os); return os;}
	
void NVRect::Set( float inL, float inT, float inR, float inB )
{
	left = inL;
	top = inT;
	right = inR;
	bottom = inB;
}

/** \brief Checks if input point is inside the rectangle (include edges)
*/
bool NVRect::Contains( const NVPoint & p ) const
{
	return (p.x >= left && p.x <= right && p.y >= top && p.y <= bottom );
}

/** \brief Checks if a rectangle collides us.
*/
bool NVRect::Collides( const NVRect & r ) const
{
	if (!r.Width()*r.Height()) return false;
	return (( r.left < right ) && ( r.right > left )
		&& ( r.top < bottom ) && ( r.bottom > top ));
//	return (( r.left <= right ) && ( r.right >= left )
//		&& ( r.top <= bottom ) && ( r.bottom >= top ));
}

/** \brief Grow ourself to include input rectangle.
*/
void NVRect::Merge( const NVRect & in )
{
	if( in.Width() <= 0 && in.Height() <= 0 ) return;
	else if( Width() <= 0 || Height() <= 0 ) Set( in );
	else 
	{
		if( in.left < left ) left = in.left; 
		if( in.right > right ) right = in.right; 
		if( in.top < top ) top = in.top; 
		if( in.bottom > bottom ) bottom = in.bottom; 
	}

}

/** \brief Rect scaling centered on the rect center
*/
NVRect NVRect::Scale( float val ) const
{
	float ow = (right - left) / 2 * (val - 1);
	float oh = (bottom - top) / 2 * (val - 1);
	NVRect r (left - ow, top - oh, right + ow, bottom + oh);
	return r;
}

NVRect & NVRect::operator *= (float v)
{
	left	*= v;
	right	*= v;
	top		*= v;
	bottom	*= v;
	return *this;
}

NVRect & NVRect::operator += (const NVPoint & in)
{
	left += in.x;
	right += in.x;
	top += in.y;
	bottom += in.y;
	return *this;
}

NVRect NVRect::operator + (const NVPoint & in) const
{
	return NVRect (left + in.x, top + in.y, right + in.x, bottom + in.y);
}

NVRect & NVRect::operator -= (const NVPoint & in)
{
	left -= in.x;
	right -= in.x;
	top -= in.y;
	bottom -= in.y;
	return *this;
}
