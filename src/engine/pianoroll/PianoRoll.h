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
#include "ARMusic.h"
#include "VGColor.h"
#include "math.h"
#include "PianoRoll.h"
#include "GUIDOScoreMap.h"

#ifdef MIDIEXPORT
#include "MidiShareLight.h"
#endif

#include "GUIDOEngine.h"

#define kDefaultWidth          1024    // the default canvas width
#define kDefaultHeight         512     // the default canvas height
#define kDefaultLowPitch       -1      // the default min pitch
#define kDefaultHighPitch      -1      // the default max pitch
#define kDefaultStartDateNum    0      // the default start date num
#define kDefaultStartDateDenom  1      // the default start date num
#define kDefaultEndDateNum      0      // the default start date num
#define kDefaultEndDateDenom    0      // the default start date num

/* \brief a class to create and configure a piano roll
*/
class PianoRoll {
    #define kGoldenRatio 0.618033988749895 // To generate only different colors

public:
             PianoRoll(ARMusic *arMusic);
             PianoRoll(const char *midiFileName);
    virtual ~PianoRoll();

    virtual void  setLimitDates(GuidoDate start, GuidoDate end);
    virtual void  setPitchRange(int minPitch, int maxPitch);
    virtual void  enableDurationLines(bool enabled) { }
    virtual void  enableKeyboard(bool enabled) { fKeyboardEnabled = enabled; }
            float getKeyboardWidth(int height);
    virtual void  enableRandomVoicesColor(bool enabled) { fVoicesAutoColored = enabled; }
    virtual void  setColorToVoice(int voiceNum, int r, int g, int b, int a);
    virtual void  enableMeasureBars(bool enabled) { fMeasureBarsEnabled = enabled; }
    virtual void  setPitchLinesDisplayMode(int mode);
    virtual void  getMap(int w, int h, Time2GraphicMap &outmap);

    bool ownsARMusic();
    bool ownsMidi();

    virtual void onDraw(int width, int height, VGDevice *dev);

protected:
    struct DrawParams {
        DrawParams(int inWidth, int inHeight, float inNoteHeight, float inUntimedLeftElementWidth, VGDevice *inDev) {
            width                   = inWidth;
            height                  = inHeight;
            noteHeight              = inNoteHeight;
            untimedLeftElementWidth = inUntimedLeftElementWidth;
            dev                     = inDev;
            colorHue = 0.5;
        }

        int       width;
        int       height;
        float     noteHeight;
        float     untimedLeftElementWidth;
        double    colorHue;
        VGDevice *dev;
    };

    virtual void  init();
    
    DrawParams    createDrawParamsStructure(int width, int height, VGDevice *dev) const;

    virtual void  DrawFromAR(DrawParams &drawParams);

            float computeKeyboardWidth(float noteHeight) const;
    virtual float computeNoteHeight   (int height) const;
    virtual void  initRendering       (DrawParams &drawParams);
    virtual void  endRendering        (DrawParams &drawParams);

	        void  DrawGrid            (DrawParams &drawParams) const;
	        void  DrawOctavesGrid     (DrawParams &drawParams) const;
	        void  DrawTwoLinesGrid    (DrawParams &drawParams) const;
	        void  DrawDiatonicGrid    (DrawParams &drawParams) const;
	        void  DrawChromaticGrid   (DrawParams &drawParams, bool isUserDefined = false) const;

	virtual void  DrawKeyboard        (DrawParams &drawParams) const;
	virtual void  DrawVoice           (ARMusicalVoice* v, DrawParams &drawParams);
	virtual void  DrawMusicalObject   (ARMusicalObject *e, TYPE_TIMEPOSITION date, TYPE_DURATION dur, DrawParams &drawParams);
	virtual void  DrawNote            (int pitch, double date, double dur, DrawParams &drawParams);
	virtual void  DrawRect            (float x, float y, double dur, DrawParams &drawParams) const;
	virtual void  DrawMeasureBar      (double date, DrawParams &drawParams) const;

	        float pitch2ypos          (int midipitch, DrawParams &drawParams) const;
	virtual void  handleColor         (ARNoteFormat *e, DrawParams &drawParams);
	virtual void  popColor            (DrawParams &drawParams);
	//virtual void handleEmpty        (double date);

            void HSVtoRGB             (float h, float s, float v, int &r, int &g, int &b) const;
            
            int  detectARExtremePitch (bool detectLowerPitch);
            void autoAdjustPitchRange (int &lowerPitch, int &higherPitch); // in the case of pitch range lower than 12

#ifdef MIDIEXPORT
    virtual void  DrawFromMidi          (DrawParams &drawParams);
    virtual void  DrawMidiSeq           (MidiSeqPtr seq, int tpqn, DrawParams &drawParams);
			TYPE_TIMEPOSITION	getMidiEndDate        () const;
            int					detectMidiExtremePitch(bool detectLowerPitch);
#else
			TYPE_TIMEPOSITION	getMidiEndDate        () const			{ return Fraction(0);}
            int					detectMidiExtremePitch(bool)			{ return 0; }
#endif
    
    virtual float date2xpos      (double pos, int width, float untimedLeftElementWidth) const;
    virtual float duration2width (double dur, int width, float untimedLeftElementWidth) const;
    virtual int   pitchRange     ()             const  { return fHighPitch - fLowPitch + 1; }
	virtual float stepheight     (int height) const  { return (float) height / (float) pitchRange(); }

    float roundFloat(float numberToRound) const { return floor(numberToRound + 0.5f); }

    ARMusic    *fARMusic;
    const char *fMidiFileName;

    TYPE_TIMEPOSITION fStartDate; // the score start date
    TYPE_TIMEPOSITION fEndDate;   // the score end date
    double fDuration;             // the time zone duration

    int  fLowPitch;               // the lower score pitch
    int  fHighPitch;              // the higher score pitch

    bool fVoicesAutoColored; // does the user wants voices to be auto colored ?

    std::vector<std::pair<int, VGColor *> > *fVoicesColors; // voices colors that the user set himself
    
    std::stack<VGColor *> *fColors;  // the colors stack (voice color, noteFormat color)
    bool isAfterStateNoteFormatTag;

	bool fChord;                   // a flag to indicate that next note (or rest) is in a chord
    TYPE_DURATION fChordDuration;  // the chord duration (notes in a chord have a null duration)

    bool  fKeyboardEnabled;         // does the keyboard will be displayed ?

    bool fMeasureBarsEnabled;

    int  fPitchLinesDisplayMode;
    bool fBytes[12];
};

#endif
