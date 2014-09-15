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

#include "ARMusicalVoice.h"
#include "ARNoteFormat.h"
#include "VGColor.h"

#ifdef MIDIEXPORT
#include "MidiShareLight.h"
#endif

#include "GUIDOEngine.h"

/* \brief a class to create and configure a piano roll
*/
class GuidoPianoRoll {
public:
             GuidoPianoRoll();
             GuidoPianoRoll(TYPE_TIMEPOSITION start, TYPE_TIMEPOSITION end, int width, int height, int minPitch, int maxPitch);
    virtual ~GuidoPianoRoll();

    virtual void setARMusic(ARMusic *arMusic);
    virtual void setMidiFile(const char *midiFileName);
    virtual void setCanvasDimensions(int width, int height);
    virtual void setLimitDates(GuidoDate start, GuidoDate end);
    virtual void setPitchRange(int minPitch, int maxPitch);
    virtual void setDurationEnabled(bool enabled) { }

    bool ownsARMusic();
    bool ownsMidi();

    virtual void getRenderingFromAR(VGDevice *dev);
    virtual void getRenderingFromMidi(VGDevice *dev);

protected:
	virtual void DrawGrid         () const;
	virtual void DrawVoice        (ARMusicalVoice *v);
	virtual void DrawMusicalObject(ARMusicalObject *o, TYPE_TIMEPOSITION date, TYPE_DURATION dur);
	virtual void DrawNote         (int pitch, double date, double dur) const;
	virtual void DrawRect         (int x, int y, double dur) const;

	virtual int	 pitch2ypos       (int midipitch) const;
	virtual bool handleColor      (ARNoteFormat *e);

#ifdef MIDIEXPORT
    virtual void DrawFromMidi();
    virtual void DrawMidiSeq (MidiSeqPtr seq, int tpqn);
#endif
    
    virtual int	pitchRange     ()           const    { return fHighPitch - fLowPitch + 1;                          }
    virtual int date2xpos      (double pos) const    { return int(fWidth * (pos - double(fStartDate)) / fDuration); }
    virtual int duration2width (double dur) const	 { return int(fWidth * dur / fDuration);                       }
	virtual int stepheight     ()           const    { return fHeight / pitchRange();                              }

    ARMusic    *fARMusic;
    const char *fMidiFileName;

    VGDevice *fDev;
    
    int  fWidth;
    int  fHeight;

    TYPE_TIMEPOSITION fStartDate;
    TYPE_TIMEPOSITION fEndDate;
    bool fIsEndDateSet;

    int  fLowPitch;
    int  fHighPitch;

    int  fNoteHeight;
    
	bool fColored;  // a flag to indicate coloring (due to noteFormat tag)
	bool fChord;    // a flag to indicate that next note (or rest) is in a chord

	VGColor       fColor;         // the current color when colored
	TYPE_DURATION fChordDuration; // the chord duration (notes in a chord have a null duration)
	double	      fDuration;      // the time zone duration
};

#endif
