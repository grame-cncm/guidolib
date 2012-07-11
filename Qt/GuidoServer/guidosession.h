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
#include "utilities.h"
#include "guido2img.h"
#include "json.h"

using namespace std;

namespace guidohttpd
{
    enum GuidoSessionMapType {PAGE, STAFF, VOICE, SYSTEM, NO_TYPE};
    enum GuidoSessionParsingError { GUIDO_SESSION_PARSING_SUCCESS, GUIDO_SESSION_PARSING_FAILURE };
    enum GuidoWebApiFormat { GUIDO_WEB_API_PNG, GUIDO_WEB_API_JPEG, GUIDO_WEB_API_GIF, GUIDO_WEB_API_SVG,
        GUIDO_WEB_API_UNDEFINED };
    //--------------------------------------------------------------------------
    class guido2img;
    class guidosession
    {
        friend class guido2img;
        guido2img*			fConverter;
        
        private :
        bool resizeToPage_;
        string gmn_;
        string url_;
        GuidoWebApiFormat format_;
        int page_;
        int width_;
        int height_;
        float zoom_;
        float marginleft_;
        float margintop_;
        float marginbottom_;
        float marginright_;
        float systemsDistance_;
        int systemsDistribution_;
        float systemsDistribLimit_;
        float force_;
        float spring_;
        int neighborhoodSpacing_;
        int optimalPageFill_;
        const char* formatToMIMEType ();
        const char* formatToLayType ();
        const char* getStringRepresentationOf (string toget);
        void fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf);
        GuidoErrCode getmap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap);
        
        public :

        guidosession(guido2img* g2img);
		virtual ~guidosession();
        void initialize();
        
        typedef GuidoSessionParsingError(guidosession::*callback_function)(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        
        // ------- CALLBACKS -------
        GuidoSessionParsingError handleGet(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handlePage(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleWidth(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleHeight(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleMarginLeft(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleMarginRight(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleMarginTop(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleMarginBottom(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleResizePageToMusic(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleZoom(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleGMN(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleFormat(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        GuidoSessionParsingError handleFaultyInput(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        // -----------------------------
        GuidoSessionParsingError genericReturnImage(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance);
        GuidoSessionParsingError genericFailure(int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const char* errorstring);
        GuidoSessionParsingError simpleGet (int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, string thingToGet);
        GuidoSessionParsingError mapGet (int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n, string thingToGet);
        GuidoSessionParsingError pointGet (int* size, const char** data, string* format, string* errstring, unsigned int* argumentsToAdvance, const TArgs& args, unsigned int n);
        
        void setUrl(string url);
        static json_type swapTypeForName (string type);
    };
    
} // end namespoace

#endif