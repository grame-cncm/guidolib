
/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef AR2PianoRoll_h
#define AR2PianoRoll_h

#include "ARMusicalVoice.h"
#ifdef MIDIEXPORT
#include "MidiShareLight.h"
#endif
#include "VGColor.h"

class ARTie;
class ARMusicalObject;
class ARNoteFormat;
class VGDevice;

class GuidoPianoRoll
{
	int		duration2width (double dur) const;
	int		pitchrange () const			{ return fHighPitch - fLowPitch + 1; }
	bool	handleColor (ARNoteFormat* e, VGDevice* dev);

	protected:

	int		fWidth;						// the image width
	int		fHeight;					// the image height
	
	TYPE_TIMEPOSITION	fStartDate;		// start of time zone to be displayed
	TYPE_TIMEPOSITION	fEndDate;		// end of time zone to be displayed
	
	double	fDuration;					// the time zone duration
	int		fLowPitch;					// the lowest pitch
	int		fHighPitch;
	int		fNoteHeight;
	
	bool			fColored;			// a flag to indicate coloring (due to noteFormat tag)
	bool			fChord;				// a flag to indicate that next note (or rest) is in a chord
	TYPE_DURATION	fChordDuration;		// the chord duration (notes in a chord have a null duration)
	
	VGColor			fColor;				// the current color when colored
	
				int	 date2xpos (double pos) const;
		virtual int	 stepheight () const			{ return fHeight / pitchrange(); }
		virtual void Draw(int pitch, double date, double dur, VGDevice* dev) const;
		virtual void DrawRect(int x, int y, double dur, VGDevice* dev) const;
		virtual void Draw (ARMusicalObject* o, TYPE_TIMEPOSITION date, TYPE_DURATION dur, VGDevice* dev);
#ifdef MIDIEXPORT
		virtual void Draw (MidiSeqPtr seq, int tpqn, VGDevice* dev);
#endif
		virtual int	 pitch2ypos (int midipitch) const;
	
	public:
				 GuidoPianoRoll(TYPE_TIMEPOSITION start, TYPE_TIMEPOSITION end, int width, int height, int minPitch, int maxPitch);
		virtual ~GuidoPianoRoll() {}
		
#ifdef MIDIEXPORT
		virtual GuidoErrCode Draw (const char* midifile, VGDevice* dev);
#endif
		virtual void Draw (ARMusicalVoice* v, VGDevice* dev);
		virtual void DrawGrid (VGDevice* dev) const;
};


#endif
