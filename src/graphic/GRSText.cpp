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

#include "GRSText.h"
#include "GRStaff.h"
#include "GRDefine.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"	// for gFontDefs.gFontText
#include "VGDevice.h"
#include "VGFont.h"
#include "GRSpecial.h"


GRSText::GRSText( GRStaff * p_staff, const NVstring & p_txt, int p_offsety ) : txt(0)
{
	grstaff = p_staff;
	txt = new NVstring(p_txt);
	offsety = p_offsety;

	mNeedsSpring = 1;
	// obsolete
	// spacing = 0;
	velement = NULL;
	
	// spacing= 0.75 * LSPACE;

	const char * cp = txt->c_str();
	
	float x = 0;
	float y = 0;
	if( gGlobalSettings.gDevice )
		FontManager::gFontText->GetExtent( cp, (int)txt->length(), &x, &y, gGlobalSettings.gDevice );

	mBoundingBox.right = (GCoord)(x);
	mBoundingBox.top = (GCoord)(y);

	mBoundingBox.left -= (GCoord)(LSPACE/4);
	mBoundingBox.bottom = (GCoord)(4*LSPACE);
}

GRSText::~GRSText()
{
	delete txt;		txt = 0;

	if (velement)
		velement->removeAssociation(this);
}

void GRSText::GGSOutput() const
{
}

void GRSText::OnDraw( VGDevice & hdc) const
{
	const char * cp = txt->c_str();
#if 1
	OnDrawText(hdc,  cp, txt->length() );
#else
	const VGFont* hmyfont = FontManager::gFontText;
	
	const int size = GRTag::getFontSize();
	const NVstring * font = GRTag::getFont();
	const unsigned char * colref = GRTag::getColRef();
	if (font && font->length() > 0)
		hmyfont = GRSpecial::FindOrCreateFont( size, font, GRTag::getFontAttrib());
	hdc.SetTextFont( hmyfont );
	const VGColor prevTextColor = hdc.GetFontColor();
	if (colref) hdc.SetFontColor( VGColor( colref ));

	// the following code doesn't check for font availability
//	GFontRef hfontold = hdc.SelectFont( gFontDefs.gFontText );
//	const int nBackmode = hdc.GetBackgroundMode();
//	hdc.SetBackgroundMode( VGDevice::kModeTransparent );

//	const unsigned int ta = hdc.GetTextAlign();
	hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignTop );
	hdc.DrawString( mPosition.x + mBoundingBox.left,
		  // (grstaff->getNumlines()-1) * LSPACE + 
			  offsety * LSPACE,	cp, (int)strlen(cp) );

	if (colref) hdc.SetFontColor( prevTextColor);

//	hdc.SetTextAlign( ta );

//	hdc.SetBackgroundMode( nBackmode );
//	hdc.SelectFont( hfontold );	// JB test for optimisation: do not restore font context.
#endif
}

void GRSText::print() const
{
}

void GRSText::addAssociation(GRNotationElement * el)
{
	// attention, otherwise this is cyclic!
	// GRNotationElement::addAssociation(el);
	if (!velement)
	{
		velement = el;
		el->addAssociation(this);
	}

}

void GRSText::tellPosition(GObject * caller, const NVPoint & pos)
{
	setPosition(pos);
}
