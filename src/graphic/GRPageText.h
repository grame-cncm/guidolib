#ifndef GRPageText_H
#define GRPageText_H

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
		virtual void print() const;
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

