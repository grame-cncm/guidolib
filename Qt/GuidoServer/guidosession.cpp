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
#include <sstream>

#include "guidosession.h"
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"

using namespace std;

namespace guidohttpd
{
    void guidosession::initialize() {
        resizeToPage = false;
        gmn  = "[c]";	
        url = "";
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
        return ("image/" + string(formatToLayType())).c_str();
    }
    const char* guidosession::formatToLayType ()
    {
        switch(format)
        {
            case GUIDO_WEB_API_PNG : return "png";
            case GUIDO_WEB_API_JPEG : return "jpeg";
            case GUIDO_WEB_API_GIF : return "gif";
            case GUIDO_WEB_API_SVG : return "xml+svg";
            default : return "png";
        }
        return "png";
    }
    const char* guidosession::getStringRepresentationOf (const char* toget)
    {
        stringstream out;
        if (strcmp (toget, "resizetopage") == 0)
            out << resizeToPage;
        else if (strcmp(toget, "gmn") == 0)
            out << gmn;
        else if (strcmp(toget, "format") == 0)
            out << formatToLayType();
        else if (strcmp(toget, "page") == 0)
            out << page;
        else if (strcmp(toget, "width") == 0)
            out << width;
        else if (strcmp(toget, "height") == 0)
            out << height;
        else if (strcmp(toget, "zoom") == 0)
            out << zoom;
        else if (strcmp(toget, "marginleft") == 0)
            out << marginleft;
        else if (strcmp(toget, "margintop") == 0)
            out << margintop;
        else if (strcmp(toget, "marginright") == 0)
            out << marginright;
        else if (strcmp(toget, "marginbottom") == 0)
            out << marginbottom;
        else if (strcmp(toget, "systemdistance") == 0)
            out << systemsDistance;
        else
            out << "";
        return out.str().c_str();
    }
    
    void guidosession::fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf)
    {
        pf->height = height;
        pf->width = width;
        pf->margintop = margintop;
        pf->marginleft = marginleft;
        pf->marginright = marginright;
        pf->marginbottom = marginbottom;
    }
    
    GuidoErrCode guidosession::getmap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap)
    {
        GuidoErrCode err;
        ARHandler arh;
        err = GuidoParseString (gmn.c_str(), &arh);
        if (err != guidoNoErr)
            return err;
        GRHandler grh;

        GuidoPageFormat pf;
        fillGuidoPageFormatUsingCurrentSettings(&pf);

        GuidoSetDefaultPageFormat(&pf);
        err = GuidoAR2GR (arh, 0, &grh);
        if (err != guidoNoErr)
            return err;
        
        /*
         In order to export a map, the score needs to be drawn on something.
         SVG export is a really quick way to do this drawing.
         It is a temporary solution
         */
        stringstream mystream;
        err = GuidoSVGExport(grh, page, mystream, "/Users/mikesolomon/Documents/guido/guido2.svg"); 
        
        switch(map){
            case PAGE : err = GuidoGetPageMap(grh, page, width, height, outmap); break;
            case STAFF : err = GuidoGetStaffMap(grh, page, width, height, aux, outmap); break;
            case VOICE : err = GuidoGetVoiceMap(grh, page, width, height, aux, outmap); break;
            case SYSTEM : err = GuidoGetSystemMap(grh, page, pf.width, pf.height, outmap); break;
            default : err = guidoErrActionFailed;
        }
        cout << "GMN " << gmn << " SIZE " << outmap.size () << endl;
        return err;
    }
} // end namespoace
