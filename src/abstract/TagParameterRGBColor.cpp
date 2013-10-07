/*
 GUIDO Library
 Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
 Copyright (C) 2004	Grame
 
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
//  TagParameterRGBColor.cpp
//  GuidoEngineiOS
//
//  Created by Simon McCabe on 05/04/2013.
//  Copyright (c) 2013 Simon McCabe. All rights reserved.
//

#include "TagParameterRGBColor.h"

TagParameterRGBColor::TagParameterRGBColor( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha )
    : mRed(red), mGreen(green), mBlue(blue), mAlpha(alpha) {}

TagParameterRGBColor::TagParameterRGBColor( const TagParameterRGBColor & tpRGBColor )
    : TagParameter(tpRGBColor), mRed(tpRGBColor.getRedChannel()), mGreen(tpRGBColor.getGreenChannel()), mBlue(tpRGBColor.getBlueChannel()), mAlpha(tpRGBColor.getAlphaChannel())
{}

TagParameterRGBColor::~TagParameterRGBColor() {}

bool TagParameterRGBColor::copyValue(const TagParameter *tp ) {
	const TagParameterRGBColor * tpRGBColor = TagParameterRGBColor::cast(tp);
	if (tpRGBColor == 0)	return false;
	mRed = tpRGBColor->getRedChannel();
    mGreen = tpRGBColor->getGreenChannel();
    mBlue = tpRGBColor->getBlueChannel();
    mAlpha = tpRGBColor->getAlphaChannel();
    return true;
}


bool TagParameterRGBColor::getRGBColor( unsigned char colref[4] ) const {
    colref[0] = mRed;
    colref[1] = mGreen;
    colref[2] = mBlue;
    colref[3] = mAlpha;
    return true;
}


void TagParameterRGBColor::set( const TagParameterRGBColor & in ) {
	TagParameter::set( in );
    mRed = in.getRedChannel();
    mGreen = in.getGreenChannel();
    mBlue = in.getBlueChannel();
    mAlpha = in.getAlphaChannel();
}


