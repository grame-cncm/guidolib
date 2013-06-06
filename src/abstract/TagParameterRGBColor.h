#ifndef TagParameterRGBColor_h
#define TagParameterRGBColor_h

/*
 GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
	using TagParameter::setValue;    TagParameterRGBColor( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255 );
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
