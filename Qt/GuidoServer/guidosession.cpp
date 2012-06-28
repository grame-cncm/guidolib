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

#include <iostream>

#include "guidosession.h"
#include "GUIDOEngine.h"

using namespace std;

namespace guidohttpd
{
    void guidosession::initialize() {
        resizeToPage = false;
        gmn  = "[c]";	
        format = GUIDO_WEB_API_PNG;
        page = 1;
        width = 400;
        height = 600;
        zoom = 2.0f;
        marginleft = 10.0f;
        margintop = 10.0f;
        marginright = 10.0f;
        marginbottom = 10.0f;
        systemsDistance = 75.0f;
        systemsDistribution = kAutoDistrib;
        systemsDistribLimit = 0.25f;
        force = 750.0f;
        spring = 1.1f;
        neighborhoodSpacing = 0;
        optimalPageFill = 1;
    }
    const char* guidosession::formatToMIMEType ()
    {
        switch(format)
        {
            case GUIDO_WEB_API_PNG : return "image/png";
            case GUIDO_WEB_API_JPEG : return "image/jpeg";
            case GUIDO_WEB_API_GIF : return "image/gif";
            case GUIDO_WEB_API_SVG : return "image/xml+svg";
            default : return "image/png";
        }
        return "image/png";
    }
} // end namespoace
