#ifndef TagParameterRGBColor_h
#define TagParameterRGBColor_h
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
//
//  TagParameterRGBColor.h
//  GuidoEngineiOS
//
//  RGB Color support 
//
//  Created by Simon McCabe on 05/04/2013.
//  Copyright (c) 2013 Simon McCabe. All rights reserved.
//

#include "TagParameter.h"


/** \brief RGB Color tag parameter.
 */
class TagParameterRGBColor : public TagParameter
{
public:
	using TagParameter::setValue;

    TagParameterRGBColor( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255 );
    TagParameterRGBColor( const TagParameterRGBColor & tpRGBColor );
    
    virtual 	~TagParameterRGBColor();
    
    virtual void set( const TagParameterRGBColor & in );
    
    virtual void setValue(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
        { mRed = red, mGreen = green, mBlue = blue, mAlpha = alpha; }
    
    virtual TagParameter * getCopy() const		{ return new TagParameterRGBColor(*this); }
    virtual bool		copyValue(const TagParameter *tp);
	virtual bool        isRGBValue() const { return true; }
    
    static TagParameterRGBColor * cast( TagParameter * inTag )
        { return inTag->isRGBValue() ? static_cast<TagParameterRGBColor *>(inTag) : 0; }
    static const TagParameterRGBColor * cast( const TagParameter * inTag )
        { return inTag->isRGBValue() ? static_cast<const TagParameterRGBColor *>(inTag) : 0; }

    
    bool            getRGBColor( unsigned char colref[4] ) const; 	// returns 1 if successful
    unsigned char   getRedChannel()   const { return mRed; };
    unsigned char   getGreenChannel() const { return mGreen; };
    unsigned char   getBlueChannel()  const { return mBlue; };
    unsigned char   getAlphaChannel() const { return mAlpha; };
    
private:
    unsigned char mRed;
    unsigned char mGreen;
    unsigned char mBlue;
    unsigned char mAlpha;
    using TagParameter::set;
};


#endif
