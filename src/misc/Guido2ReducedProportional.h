
/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef GuidoReducedProportional_h
#define GuidoReducedProportional_h

#include "Guido2PianoRoll.h"

class GuidoReducedProportional : public GuidoPianoRoll
{
	int	  fNumStaves;			// the number of staves
	float fLineHeight;			// a staff line height
	bool  fDrawDurationLine;	// duration lines control
	VGColor fFontSavedColor;	// the saved font color

	int pitch2staff (int midipitch) const;						// gives a staff number for a given midi pitch
	int pitch2staff (int midipitch, int& halfspaces, int& alter) const;	// gives a staff number for a given midi pitch + position and accidental
	int diatonic (int pitch, int& octave, int& alter) const;	// converts a midi pitch in diatonic pitch class + octave and accidental
	float staffTopPos (int i) const;							// gives a staff top y position
	float staffBottomPos (int i) const;							// gives a staff bottom y position
	float noteWidth () const;									// gives a note width

	protected:
	
		virtual void DrawStaff			(int i, VGDevice* dev) const;
		virtual void DrawLedgerLines	(float x, float y, int n, VGDevice* dev) const;
		virtual void DrawHead			(float x, float y, int alter, VGDevice* dev) const;
		virtual void Draw				(int pitch, double date, double dur, VGDevice* dev) const;
		virtual float halfspaces2ypos	(int halfspaces, int staff) const;
		virtual int	 halfSpaces2LedgerLines (int halfspaces) const;
	
	public:
				 GuidoReducedProportional(TYPE_TIMEPOSITION start, TYPE_TIMEPOSITION end, int width, int height, bool drawdur=true);
		virtual ~GuidoReducedProportional() {}
		
		virtual void DrawGrid (VGDevice* dev) const;
		virtual void SetMusicFont(VGDevice* dev);
};


#endif
