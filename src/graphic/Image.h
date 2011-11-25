#ifndef Image_H
#define Image_H

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


