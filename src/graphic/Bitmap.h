#ifndef Bitmap_H
#define Bitmap_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003  Grame

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

#include "Image.h"
#include "VGDevice.h"

/** \brief a Bitmap image that can be drawn on the score.
		(not fully implemented yet)
*/
class Bitmap : public Image
{
	private:
			VGDevice *  fDevice;

	public:
						Bitmap( const char * inName );
		virtual			~Bitmap();

		virtual void	OnDraw( VGDevice & hdc, const NVRect & where ) const;
		virtual void	OnDrawW( VGDevice & hdc, const NVPoint & where, float width ) const;
		virtual void	OnDrawH( VGDevice & hdc, const NVPoint & where, float height ) const;

		virtual float	GetProportionalHeight( float width ) const;
		virtual float	GetProportionalWidth( float height ) const;

		virtual int		GetWidth()  const { return fDevice->GetWidth(); }
		virtual int		GetHeight() const { return fDevice->GetHeight(); }
};

#endif


