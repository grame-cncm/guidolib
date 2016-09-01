#ifndef __SVGMapDevice__
#define __SVGMapDevice__

/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "SVGDevice.h"

#include "GUIDOExport.h"
#include "GUIDOScoreMap.h"

class_export SVGMapDevice : public SVGDevice
{
public :
	SVGMapDevice(std::ostream& outstream, SVGSystem* system, const char* guidofont, int inMappingMode);

    void addVoiceMap (Time2GraphicMap inVoiceMap)  { voicesMapsVector->push_back(inVoiceMap); }
    void addStaffMap (Time2GraphicMap inStaffMap)  { stavesMapsVector->push_back(inStaffMap); }
    void addSystemMap(Time2GraphicMap inSystemMap) { systemMapsVector->push_back(inSystemMap); }

private:
    virtual void EndDraw();
    
    virtual void drawMap(std::vector<Time2GraphicMap> *mapsVector);
    virtual void drawMaps();

    std::vector<Time2GraphicMap> *voicesMapsVector;
    std::vector<Time2GraphicMap> *stavesMapsVector;
    std::vector<Time2GraphicMap> *systemMapsVector;
    
    int mappingMode;
};

#endif
