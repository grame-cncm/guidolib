#ifndef Bitmap_H
#define Bitmap_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

		virtual int		GetWidth()  const      { return fDevice ? fDevice->GetWidth() : 0; }
		virtual int		GetHeight() const      { return fDevice ? fDevice->GetHeight() : 0; }

        const VGDevice *getDevice() const      { return fDevice; }

        const bool      getIsSVGDevice() const { return isSVGDevice; }

    protected:
        bool isSVGDevice;
};

#endif


