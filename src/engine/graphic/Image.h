#ifndef Image_H
#define Image_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


class VGDevice;
class NVRect;
class NVPoint;

class Image 
{
  public:
      virtual ~Image();

		virtual void OnDraw( VGDevice & hdc, const NVRect & where) const = 0;
		virtual void OnDrawW(VGDevice & hdc, const NVPoint & where, float width) const = 0;
		virtual void OnDrawH(VGDevice & hdc, const NVPoint & where, float height) const = 0;

  protected:

		Image();

};


#endif


