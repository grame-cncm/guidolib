/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef GuidoPianoRollTrajectory__
#define GuidoPianoRollTrajectory__

#include "GuidoPianoRoll.h"

class GuidoPianoRollTrajectory : public GuidoPianoRoll
{
public:
             GuidoPianoRollTrajectory();
    virtual ~GuidoPianoRollTrajectory() {}

private:
    typedef struct {
        int x;
        int y;
        VGColor color;
        bool isRest;
    } EventInfos;

    EventInfos createNoteInfos(int x, int y, VGColor color);
    EventInfos createRestInfos(int x);

    /**** Chords ****/
    /* Computes if some rights chords points have several links towards them */
    bool GuidoPianoRollTrajectory::isTherePointOverlap(int x, int y);
    void GuidoPianoRollTrajectory::sortPoints();
    std::vector<EventInfos> *sortList(std::vector<EventInfos> *listToSort);
    /****************/

    std::vector<EventInfos> *previousEventInfos;
    std::vector<EventInfos> *currentEventInfos;

protected:
	void DrawVoice(ARMusicalVoice *v);
    void DrawNote (int pitch, double date, double dur);
    void DrawLinks();
    void DrawFinalEvent(double dur);
    void DrawAllLinksBetweenTwoEvents();
    void DrawLinkBetween(EventInfos leftEvent, EventInfos rightEvent);

	bool handleColor (ARNoteFormat *e);
	void handleRest  (double date);
	//void handleEmpty (double date);

    double fCurrentDate;

    EventInfos test();
};

#endif