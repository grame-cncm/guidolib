
/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef GuidoReducedProportional__
#define GuidoReducedProportional__

#include "GuidoPianoRoll.h"

#define kStaffLines 5    // number of lines per staff

class GuidoReducedProportional : public GuidoPianoRoll
{
public:
             GuidoReducedProportional();
    virtual ~GuidoReducedProportional() {}

    void setCanvasDimensions(int width, int height);
    void enableDurationLines(bool enabled) { fDrawDurationLine = enabled; }

    void getRenderingFromAR(VGDevice *dev);
    void getRenderingFromMidi(VGDevice* dev);

protected:
    void DrawGrid        () const;
    void DrawStaff       (int i) const;
    void DrawVoice       (ARMusicalVoice* v);
    void DrawLedgerLines (float x, float y, int n) const;
    void DrawHead        (float x, float y, int alter) const;
    void DrawNote        (int pitch, double date, double dur);
	void DrawRect        (int x, int y, double dur) const;
    void DrawMeter       (ARMeter *meter, int voiceNum, double date) const;

    void SetMusicFont();

    int   pitch2staff            (int midipitch) const;                              // gives a staff number for a given midi pitch
    int   pitch2staff            (int midipitch, int& halfspaces, int& alter) const; // gives a staff number for a given midi pitch + position and accidental
    float halfspaces2ypos        (int halfspaces, int staff) const;
    int	  halfSpaces2LedgerLines (int halfspaces) const;

    int   diatonic       (int pitch, int& octave, int& alter) const; // converts a midi pitch in diatonic pitch class + octave and accidental
    float staffTopPos    (int i) const;                              // gives a staff top y position
    float staffBottomPos (int i) const { return staffTopPos(i) + ((kStaffLines - 1) * fLineHeight); }  // gives a staff bottom y position
    float getNoteWidth   ()      const { return fLineHeight * 1.8f; }

    int	    fNumStaves;        // the number of staves
    float   fLineHeight;       // a staff line height
    bool    fDrawDurationLine; // duration lines control
    VGColor fFontSavedColor;   // the saved font color
};

#endif