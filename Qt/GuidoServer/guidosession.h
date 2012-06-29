/*
 
 Copyright (C) 2012 Grame
 
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

#ifndef __guidosession__
#define __guidosession__

#include <string>
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"

using namespace std;

namespace guidohttpd
{
    enum GuidoSessionMapType {PAGE, STAFF, VOICE, SYSTEM, NO_TYPE};
    enum GuidoSessionParsingError { GUIDO_SESSION_PARSING_SUCCESS, GUIDO_SESSION_PARSING_FAILURE };
    enum GuidoWebApiFormat { GUIDO_WEB_API_PNG, GUIDO_WEB_API_JPEG, GUIDO_WEB_API_GIF, GUIDO_WEB_API_SVG,
        GUIDO_WEB_API_UNDEFINED };
    //--------------------------------------------------------------------------
    struct guidosession
    {
        bool resizeToPage;
        string gmn;
        string url;
        GuidoWebApiFormat format;
        int page;
        int width;
        int height;
        float zoom;
        float marginleft;
        float margintop;
        float marginbottom;
        float marginright;
        float systemsDistance;
        int systemsDistribution;
        float systemsDistribLimit;
        float force;
        float spring;
        int neighborhoodSpacing;
        int optimalPageFill;
        void initialize();
        const char* formatToMIMEType ();
        const char* formatToLayType ();
        const char* getStringRepresentationOf (const char*);
        void fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf);
        GuidoErrCode getmap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap);
    };
    
} // end namespoace

#endif