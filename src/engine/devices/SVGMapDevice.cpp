/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "SVGMapDevice.h"

#include "GUIDOScoreMap.h"

//______________________________________________________________________________
// SVGMapDevice
//______________________________________________________________________________
SVGMapDevice::SVGMapDevice(std::ostream& outstream, SVGSystem* system, const char* guidofont, int inMappingMode) :
	SVGDevice(outstream, system, guidofont)
{
    voicesMapsVector = new std::vector<Time2GraphicMap>();
    stavesMapsVector = new std::vector<Time2GraphicMap>();
    systemMapsVector = new std::vector<Time2GraphicMap>();

    mappingMode = inMappingMode;
}

void SVGMapDevice::EndDraw()
{
	if (fPushedPen) closegroup();
	if (fPushedPenColor) closegroup();
	if (fPushedPenWidth) closegroup();
	if (fPushedFill) closegroup();
    if (fScaled) closegroup();
	if (fOffset) closegroup();
	if (fCurrFont) closegroup();
	fPushedPen = false;
	fPushedFill = false;
    fScaled = false;
	fOffset = false;

    drawMaps();

	fEndl--; fStream << fEndl << "</svg>" << fEndl;
}

void SVGMapDevice::drawMap(std::vector<Time2GraphicMap> *mapsVector)
{
    VGColor fillColors[2] = { VGColor(0, 0, 200, 100), VGColor(200, 0, 0, 100) };

	for (unsigned int i = 0; i < mapsVector->size(); i++) {
        Time2GraphicMap map = mapsVector->at(i);

        for (unsigned int j = 0; j < map.size(); j++) {
            PushFillColor(fillColors[j % 2]);

            FloatRect elem = map.at(j).second;
            Rectangle(elem.left, elem.top, elem.right, elem.bottom);

            PopFillColor();
        }
    }
}

void SVGMapDevice::drawMaps()
{
    int  currentMode = mappingMode;
    bool drawMapTable[3] = { false, false, false }; // Order : voice, staff, system

    for (int i = 2; i >= 0; i--) {
        if (currentMode - (1<<i) >= 0) {
            currentMode -= 1<<i;
            drawMapTable[i] = true;
        }
    }
    
    PushPenWidth(0.3f);
    PushPenColor(VGColor(255,255, 255, 255));
    
    if (drawMapTable[2] == true)
        drawMap(systemMapsVector);
    
    if (drawMapTable[1] == true)
        drawMap(stavesMapsVector);

    if (drawMapTable[0] == true)
        drawMap(voicesMapsVector);

    PopPenColor();
    PopPenWidth();
}
