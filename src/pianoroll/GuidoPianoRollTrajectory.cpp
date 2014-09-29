/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <math.h>

#include "ARMusic.h"
#include "ARRest.h"
#include "ARNoteFormat.h"
#include "ARChordComma.h"
#include "ARBar.h"
#include "GUIDOEngine.h"
#include "MusicalSymbols.h"
#include "GuidoPianoRollTrajectory.h"
#include "VGDevice.h"
#include "VGSystem.h"
#include "VGFont.h"
#include "TagParameterString.h"

using namespace std;

//--------------------------------------------------------------------------
GuidoPianoRollTrajectory::GuidoPianoRollTrajectory() :
    GuidoPianoRoll(), fCurrentDate(0)
{
    previousEventInfos = new std::vector<EventInfos>;
    currentEventInfos  = new std::vector<EventInfos>;
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::DrawVoice(ARMusicalVoice* v)
{
    if (fVoicesColors != NULL) {
        int voiceNum = v->getVoiceNum();
        
        for (unsigned int i = 0; i < fVoicesColors->size() && fColors->empty(); i++) {
            std::pair<int, VGColor> pair = fVoicesColors->at(i);

            if (pair.first == voiceNum)
                fColors->push(pair.second);
        }
    }
    
    if (!fColors->empty() || fVoicesAutoColored) {
        if (fColors->empty()) {
            int r, g, b;

            fColorSeed += kGoldenRatio;
            fColorSeed  = fmod(fColorSeed, 1);

            HSVtoRGB((float) fColorSeed, 0.5f, 0.9f, r, g, b);

            fColors->push(VGColor(r, g, b, 255));
        }
    }

    fChord              = false;
	ObjectList   *ol    = (ObjectList *) v;
	GuidoPos      pos   = ol->GetHeadPosition();
    TYPE_DURATION finalDur;

	while(pos)
	{
		ARMusicalObject  *e    = ol->GetNext(pos);
		TYPE_DURATION     dur  = e->getDuration();
		TYPE_TIMEPOSITION date = e->getRelativeTimePosition();

        finalDur = (dur ? dur : finalDur); // REM: pas sur que ça soit le mieux à faire

        if (fChord) {
            dur   = fChordDuration;
            date -= dur;
        }

		TYPE_TIMEPOSITION end = date + dur;

		if (date >= fStartDate) {
            if (date < fEndDate) {
                if (end > fEndDate)
                    dur = fEndDate - date;

                DrawMusicalObject(e, date, dur);
            }
		}
		else if (end > fStartDate) { // to make the note end appear
			date = fStartDate;	
			
            if (end > fEndDate)
                dur = fEndDate - date;
			else
                dur = end - date;
			
            DrawMusicalObject(e, date, dur);
		}

		if (dynamic_cast<ARRest *>(e)) {
            handleRest(date);
			fChord = false;
        }
		else if (dynamic_cast<ARChordComma *>(e))
			fChord = true;
		else if (dynamic_cast<ARNoteFormat *>(e))
            handleColor(dynamic_cast<ARNoteFormat *>(e));
        else if (dynamic_cast<ARBar *>(e) && fMeasureBarsEnabled)
            DrawMeasureBar(date);
	}

    DrawLinks();              // Draws link to final event
    DrawFinalEvent(finalDur); // Draws link after final event

    while (!fColors->empty())
        fColors->pop();

    previousEventInfos->clear();
    currentEventInfos->clear();
    fCurrentDate = 0;
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::DrawNote(int pitch, double date, double dur)
{
    float   x     = date2xpos(date);
    float   y     = pitch2ypos(pitch);
    VGColor color = fColors->empty() ? NULL : fColors->top();

    if (fCurrentDate == date)
        currentEventInfos->push_back(createNoteInfos(x, y, color));
    else {
        DrawLinks();
        
        previousEventInfos = new std::vector<EventInfos>(*currentEventInfos);

        currentEventInfos->clear();
        currentEventInfos->push_back(createNoteInfos(x, y, color));

        fCurrentDate = date;
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::DrawLinks()
{
    if (!previousEventInfos->empty() && !currentEventInfos->empty())
        DrawAllLinksBetweenTwoEvents();
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::DrawFinalEvent(double dur)
{
    if (!currentEventInfos->empty()) {
        float w = duration2width(dur);

        for (unsigned int i = 0; i < currentEventInfos->size(); i++) {
            if (!currentEventInfos->at(i).isRest) {
                VGColor color = currentEventInfos->at(i).color;

                if (color != NULL)
                    fDev->PushFillColor(color);

                float xCoords[4] = {
                    (float) currentEventInfos->at(i).x,
                    (float) currentEventInfos->at(i).x + w,
                    (float) currentEventInfos->at(i).x + w,
                    (float) currentEventInfos->at(i).x
                };

                float yCoords[4] = {
                    (float) currentEventInfos->at(i).y - 0.5f * fNoteHeight,
                    (float) currentEventInfos->at(i).y - 0.5f * fNoteHeight,
                    (float) currentEventInfos->at(i).y + 0.5f * fNoteHeight,
                    (float) currentEventInfos->at(i).y + 0.5f * fNoteHeight
                };

                fDev->Polygon(xCoords, yCoords, 4);
                
                if (color != NULL)
                    fDev->PopFillColor();
            }
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::DrawAllLinksBetweenTwoEvents()
{
    for (unsigned int i = 0; i < previousEventInfos->size(); i++) {
        for (unsigned int j = 0; j < currentEventInfos->size(); j++) {
            if (!previousEventInfos->at(i).isRest)
                DrawLinkBetween(previousEventInfos->at(i), currentEventInfos->at(j));
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::DrawLinkBetween(GuidoPianoRollTrajectory::EventInfos leftEvent, GuidoPianoRollTrajectory::EventInfos rightEvent)
{
    VGColor color = leftEvent.color;

    if (color != NULL)
        fDev->PushFillColor(color);

    float xCoords[4] = {
        (float) leftEvent.x,
        (float) rightEvent.x,
        (float) rightEvent.x,
        (float) leftEvent.x
    };

    float y1 = (float) leftEvent.y - 0.5f * fNoteHeight;
    float y4 = (float) leftEvent.y + 0.5f * fNoteHeight;
    float y2 = y1;
    float y3 = y4;

    if (!rightEvent.isRest) {
        y2 = (float) rightEvent.y - 0.5f * fNoteHeight;
        y3 = (float) rightEvent.y + 0.5f * fNoteHeight;
    }

    float yCoords[4] = { y1, y2, y3, y4 };

    fDev->Polygon(xCoords, yCoords, 4);

    if (color != NULL)
        fDev->PopFillColor();
}

//--------------------------------------------------------------------------
bool GuidoPianoRollTrajectory::handleColor(ARNoteFormat* noteFormat)
{
	if (noteFormat) {
		const TagParameterString *tps = noteFormat->getColor();
		unsigned char colref[4];
		
        if (tps && tps->getRGB(colref))
            fColors->push(VGColor(colref[0], colref[1], colref[2], colref[3]));
        else if (fVoicesAutoColored && fColors->size() > 1
            || !fVoicesAutoColored && !fColors->empty())
            fColors->pop();

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
void GuidoPianoRollTrajectory::handleRest(double date)
{
    if (!fChord) {
        DrawLinks();

        float x = date2xpos(date);

        previousEventInfos = new std::vector<EventInfos>(*currentEventInfos);

        currentEventInfos->clear();
        currentEventInfos->push_back(createRestInfos(x)); 
    }
}

//--------------------------------------------------------------------------
/*void GuidoPianoRollTrajectory::handleEmpty(double date)
{
    handleRest(date);
}*/

//--------------------------------------------------------------------------
GuidoPianoRollTrajectory::EventInfos GuidoPianoRollTrajectory::createNoteInfos(float x, float y, VGColor color)
{
    EventInfos newNoteInfos;

    newNoteInfos.color  = color;
    newNoteInfos.x      = x;
    newNoteInfos.y      = y;
    newNoteInfos.isRest = false;
    
    return newNoteInfos;
}

//--------------------------------------------------------------------------
GuidoPianoRollTrajectory::EventInfos GuidoPianoRollTrajectory::createRestInfos(float x)
{
    EventInfos newRestInfos;

    newRestInfos.x      = x;
    newRestInfos.isRest = true;

    return newRestInfos;
}