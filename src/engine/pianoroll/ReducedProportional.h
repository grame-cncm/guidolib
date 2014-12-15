
/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ReducedProportional__
#define ReducedProportional__

#include "PianoRoll.h"

#define kStaffLines 5    // number of lines per staff

class ReducedProportional : public PianoRoll
{
public:
             ReducedProportional(ARMusic *arMusic);
             ReducedProportional(const char *midiFileName);
    virtual ~ReducedProportional() {}

    void enableDurationLines(bool enabled) { fDrawDurationLine = enabled; }

    void onDraw(int width, int height, VGDevice *dev);

protected:
    void init();

    DrawParams createDrawParamsStructure(int width, int height, VGDevice *dev) const;

    void DrawGrid        (DrawParams &drawParams) const;
    void DrawStaff       (int n, DrawParams &drawParams) const;
    void DrawVoice       (ARMusicalVoice* v, DrawParams &drawParams);
    void DrawLedgerLines (float x, float y, int count, DrawParams &drawParams) const;
    void DrawHead        (float x, float y, int alter, DrawParams &drawParams) const;
    void DrawNote        (int pitch, double date, double dur, DrawParams &drawParams);
	void DrawRect        (float x, float y, double dur, DrawParams &drawParams) const;

    void SetMusicFont(DrawParams &drawParams);
    
	void  handleColor(ARNoteFormat *e, DrawParams &drawParams) const;

    int   pitch2staff            (int midipitch) const;                              // gives a staff number for a given midi pitch
    int   pitch2staff            (int midipitch, int& halfspaces, int& alter) const; // gives a staff number for a given midi pitch + position and accidental
    float halfspaces2ypos        (int halfspaces, int staff, float noteHeight) const;
    int	  halfSpaces2LedgerLines (int halfspaces) const;
    float computeNoteHeight      (int height) const;

    int   diatonic       (int pitch, int& octave, int& alter) const; // converts a midi pitch in diatonic pitch class + octave and accidental
    float staffTopPos    (int i, float noteHeight) const;                              // gives a staff top y position
    float staffBottomPos (int i, float noteHeight) const { return staffTopPos(i, noteHeight) + ((kStaffLines - 1) * noteHeight); }  // gives a staff bottom y position
    float getNoteWidth   (float noteHeight) const { return noteHeight * 1.8f; }

    int	    fNumStaves;        // the number of staves
    bool    fDrawDurationLine; // duration lines control
    VGColor fFontSavedColor;   // the saved font color
};

#endif