/*

  Copyright (C) 2011 Grame

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"
#include "CairoSystem.h"
#include "cairo_guido2img.h"

namespace guidohttpd {

guido2img* makeConverter(std::string svgfontfile) {
  cairo_guido2img *converter = new cairo_guido2img(svgfontfile);
  return converter;
}

GuidoErrCode startEngine () {
	CairoSystem * sys = new CairoSystem(0);
	VGDevice * dev = sys->CreateMemoryDevice(10,10);

	GuidoInitDesc desc;
	desc.graphicDevice = dev;
	desc.musicFont = "guido2";
	desc.textFont  = "Times";
	GuidoErrCode err = GuidoInit (&desc);
	return err;
}

void makeApplication (int argc, char **argv) {
  // do nothing
}

void stopEngine () {
  GuidoShutdown();
}

} // end namespace