/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef PianoRollTrajectory__
#define PianoRollTrajectory__

#include "PianoRoll.h"

class PianoRollTrajectory : public PianoRoll
{
public:
             PianoRollTrajectory(ARMusic *arMusic);
             PianoRollTrajectory(const char *midiFileName);
    virtual ~PianoRollTrajectory();

private:
    typedef struct {
        float x;
        float y;
        VGColor color;
        bool isRest;
    } EventInfos;

    EventInfos createNoteInfos(float x, float y, VGColor color) const;
    EventInfos createRestInfos(float x) const;

    /**** Chords ****/
    /* Computes if some rights chords points have several links towards them */
    bool isTherePointOverlap(int x, int y);
    void sortPoints();
    std::vector<EventInfos> *sortList(std::vector<EventInfos> *listToSort);
    /****************/

    std::vector<EventInfos> *previousEventInfos;
    std::vector<EventInfos> *currentEventInfos;

protected:
    void init();

	void DrawVoice(ARMusicalVoice* v, DrawParams &drawParams);
    void DrawNote (int pitch, double date, double dur, DrawParams &drawParams);
    void DrawLinks(DrawParams &drawParams) const;
    void DrawFinalEvent(double dur, DrawParams &drawParams);
    void DrawAllLinksBetweenTwoEvents(DrawParams &drawParams) const;
    void DrawLinkBetween(PianoRollTrajectory::EventInfos leftEvent, PianoRollTrajectory::EventInfos rightEvent, DrawParams &drawParams) const;

	void handleRest (double date, DrawParams &drawParams);
	//void handleEmpty (double date);

#ifdef MIDIEXPORT
    void DrawMidiSeq(MidiSeqPtr seq, int tpqn, DrawParams &drawParams);
#endif

    double fCurrentDate;
};

#endif