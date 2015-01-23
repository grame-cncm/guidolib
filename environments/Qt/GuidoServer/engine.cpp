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

#include "QGuidoPainter.h"
#include <QApplication>
#include "qt_guido2img.h"

namespace guidohttpd {

guido2img* makeConverter(std::string svgfontfile) {
  qt_guido2img *converter = new qt_guido2img(svgfontfile);
  return converter;
}

GuidoErrCode startEngine () {
    QGuidoPainter::startGuidoEngine(); // starts the guido engine
    return guidoNoErr;
}

void makeApplication (int argc, char **argv) {
    QApplication *app = new QApplication(argc , argv); // required by Qt
}

void stopEngine () {
    QGuidoPainter::stopGuidoEngine(); // stop the guido engine
}

} // end namespace