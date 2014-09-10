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

    void setARMusic(ARMusic *arMusic);
    void setCanvasDimensions(int width, int height);
    void setLimitDates(GuidoDate start, GuidoDate end);
    void setPitchRange(int minPitch, int maxPitch);

    bool ownsARMusic();    

    void getRendering(VGDevice *dev);

protected:
	virtual void DrawGrid    (VGDevice *dev) const;
	virtual void Draw        (ARMusicalVoice *v, VGDevice *dev);
	virtual void Draw        (ARMusicalObject *o, TYPE_TIMEPOSITION date, TYPE_DURATION dur, VGDevice *dev); // REM: stocker dev comme membre, ça sera plus simple
	virtual void Draw        (int pitch, double date, double dur, VGDevice* dev) const;
	virtual void DrawRect    (int x, int y, double dur, VGDevice* dev) const;
	virtual int	 pitch2ypos  (int midipitch) const;
	bool	     handleColor (ARNoteFormat *e, VGDevice *dev);
    
    virtual int	pitchRange     ()           const    { return fHighPitch - fLowPitch + 1;                          }
    virtual int date2xpos      (double pos) const    { return int(fWidth * (pos- double(fStartDate)) / fDuration); }
    virtual int duration2width (double dur) const	 { return int(fWidth * dur / fDuration);                       }
	virtual int stepheight     ()           const    { return fHeight / pitchRange();                              }

    ARMusic *fARMusic;
    
    int  fWidth;
    int  fHeight;

    TYPE_TIMEPOSITION fStartDate;
    TYPE_TIMEPOSITION fEndDate;

    int  fLowPitch;
    int  fHighPitch;

    int  fNoteHeight;
    
	bool fColored;  // a flag to indicate coloring (due to noteFormat tag)
	bool fChord;    // a flag to indicate that next note (or rest) is in a chord

	VGColor       fColor;         // the current color when colored
	TYPE_DURATION fChordDuration; // the chord duration (notes in a chord have a null duration)
	double	fDuration;            // the time zone duration

    #ifdef MIDIEXPORT
		virtual GuidoErrCode Draw (const char* midifile, VGDevice* dev);
    #endif
};

#endif
