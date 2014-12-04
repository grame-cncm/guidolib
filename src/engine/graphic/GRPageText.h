#ifndef GRPageText_H
#define GRPageText_H

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

#include <string>

#include "GRTagARNotationElement.h"
#include "GuidoDefs.h"	// for LSPACE

class GRPage;


/** \brief not yet documented
*/
class GRPageText : public GRTagARNotationElement
{

  public:
				GRPageText(ARMusicalTag *o,
					GRPage *, 
					const char* p_txt,			
					const char* p_pageform,
					const char* p_textform = NULL,
					const char* p_textfont = NULL,
					int p_textsize = (int)(4*LSPACE),
					const char* p_textattr = NULL);

		virtual ~GRPageText();

		virtual void OnDraw( VGDevice & hdc ) const;
						void setGRPage(GRPage * page) 		{ fGRPage = page; }

		virtual void			calcPosition();
		virtual unsigned int	getTextAlign() const { return fTextalign; }

  protected:
 
  	GRPage *	fGRPage;
    std::string fPageText;
	std::string fPageformat;
	std::string fTextformat;

	unsigned int fTextalign;
};

#endif

