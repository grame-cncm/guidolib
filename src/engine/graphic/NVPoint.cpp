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

#include "NVPoint.h"

std::ostream& operator<< (std::ostream& os, const NVPoint& p)	{ p.Print (os); return os;}

/*
NVPoint operator-( const NVPoint & p1, const NVPoint & p2 )
{
	return (NVPoint( p1.x - p2.x, p1.y - p2.y ));
}

NVPoint operator+( const NVPoint & p1, const NVPoint & p2 )
{
	return (NVPoint( p1.x + p2.x, p1.y + p2.y ));
}

*/
