/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#ifndef __GuidoPianoRoll__
#define __GuidoPianoRoll__

#include <stack>

#include "ARMusicalVoice.h"
#include "ARNoteFormat.h"
#include "VGColor.h"
#include "GUIDOPianoRollAPI.h"

#ifdef MIDIEXPORT
#include "MidiShareLight.h"
#endif

#include "GUIDOEngine.h"

/* \brief a class to create and configure a piano roll
*/
class GuidoPianoRoll {
    #define kGoldenRatio      0.618033988749895

public:
             GuidoPianoRoll();
             GuidoPianoRoll(TYPE_TIMEPOSITION start, TYPE_TIMEPOSITION end, int width, int height, int minPitch, int maxPitch);
    virtual ~GuidoPianoRoll();

    virtual void setARMusic(ARMusic *arMusic);
    virtual void setMidiFile(const char *midiFileName);
    virtual void setCanvasDimensions(int width, int height);
    virtual void setLimitDates(GuidoDate start, GuidoDate end);
    virtual void setPitchRange(int minPitch, int maxPitch);
    virtual void enableDurationLines(bool enabled) { }
    virtual void enableKeyboard(bool enabled);
    virtual void enableRandomVoicesColor(bool enabled) { fVoicesAutoColored = enabled; }
    virtual void setColorToVoice(int voiceNum, int r, int g, int b, int a);
    virtual void enableMeasureBars(bool enabled) { fMeasureBarsEnabled = enabled; }
    virtual void setPitchLinesDisplayMode(PitchLinesDisplayMode mode) { fPitchLinesDisplayMode = mode; }
    virtual void getSize(int &width, int &height) { width = fWidth; height = fHeight; }

    bool ownsARMusic();
    bool ownsMidi();

    float getKeyboardWidth() { return fUntimedLeftElementWidth; }

    virtual void getRenderingFromAR(VGDevice *dev);
    virtual void getRenderingFromMidi(VGDevice *dev);

protected:
            void  computeNoteHeight     ();
            void  computeKeyboardWidth  ();
    virtual void  initRendering         ();
    virtual void  endRendering          ();

	virtual void  DrawGrid              () const;
	virtual void  DrawOctavesGrid       () const;
	virtual void  DrawTwoLinesGrid      () const;
	virtual void  DrawChromaticGrid     () const;
	virtual void  DrawDiatonicGrid      () const;

	virtual void  DrawKeyboard          () const;
	virtual void  DrawVoice             (ARMusicalVoice *v);
	virtual void  DrawMusicalObject     (ARMusicalObject *o, TYPE_TIMEPOSITION date, TYPE_DURATION dur);
	virtual void  DrawNote              (int pitch, double date, double dur);
	virtual void  DrawRect              (float x, float y, double dur) const;
	virtual void  DrawMeasureBar        (double date) const;

	virtual float pitch2ypos            (int midipitch) const;
	virtual bool  handleColor           (ARNoteFormat *e);
	//virtual void handleEmpty           (double date);

            void HSVtoRGB              (float h, float s, float v, int &r, int &g, int &b);

#ifdef MIDIEXPORT
    virtual void DrawFromMidi();
    virtual void DrawMidiSeq (MidiSeqPtr seq, int tpqn);
#endif
    
    virtual float date2xpos      (double pos) const;
    virtual float duration2width (double dur) const;
    virtual int   pitchRange     ()           const  { return fHighPitch - fLowPitch + 1; }
	virtual float stepheight     ()           const  { return (float) fHeight / (float) pitchRange(); }

    ARMusic    *fARMusic;
    const char *fMidiFileName;

    VGDevice *fDev;
    
    int fWidth;  // the pianoroll width
    int fHeight; // the pianoroll height

    TYPE_TIMEPOSITION fStartDate; // the score start date
    TYPE_TIMEPOSITION fEndDate;   // the score end date
    bool   fIsEndDateSet;         // is the end date set by user ?
    double fDuration;             // the time zone duration

    int  fLowPitch;               // the lower score pitch
    int  fHighPitch;              // the higher score pitch

    bool   fVoicesAutoColored; // does the user wants voices to be auto colored ?
    double fColorSeed;         // base random color

    std::vector<std::pair<int, VGColor> > *fVoicesColors; // voices colors that the user set himself
    
    std::stack<VGColor> *fColors;  // the colors stack (voice color, noteFormat color)

	bool fChord;                   // a flag to indicate that next note (or rest) is in a chord
    TYPE_DURATION fChordDuration;  // the chord duration (notes in a chord have a null duration)

    bool  fKeyboardEnabled;         // does the keyboard will be displayed ?
    float fNoteHeight;
    float fUntimedLeftElementWidth; // keyboard width if GuidoPianoRoll,

    bool fMeasureBarsEnabled;

    PitchLinesDisplayMode fPitchLinesDisplayMode;
};

#endif
