#ifndef GRPageText_H
#define GRPageText_H

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

#include <string>

#include "GRTagARNotationElement.h"
#include "GuidoDefs.h"	// for LSPACE

class ARText;
class GRPage;


/** \brief not yet documented
*/
class GRPageText : public GRTagARNotationElement
{
	public:
				 GRPageText (const ARText *ar, GRPage *, const char* text, const char* pageform);
		virtual ~GRPageText() {}

		virtual void OnDraw( VGDevice & hdc ) const;
				void setGRPage(GRPage * page)	{ fGRPage = page; }

		virtual void			calcPosition();
		virtual unsigned int	getTextAlign() const	{ return fTextalign; }

	protected:
		GRPage *	fGRPage;
		std::string fPageText;
		std::string fLocation;

		unsigned int fTextalign;
};

#endif

