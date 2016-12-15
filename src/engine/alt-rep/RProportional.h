/*
 GUIDO Library
 Copyright (C) 2015 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#ifndef __RProportional__
#define __RProportional__

#include <stack>

#include "ARMusic.h"
#include "PianoRoll.h"
#include "GUIDOEngine.h"

/* \brief a class to create and configure a reduced proportional representation
*/
class RProportional : public PianoRoll {

	int	  fNumStaves;			// the number of staves
	float fLineHeight;			// a staff line height
	bool  fDrawDurationLine;	// duration lines control
	std::stack<VGColor> fFontColors;	// the saved font colors stack

	float	staffTopPos (int i) const;							// gives a staff top y position
	float	staffBottomPos (int i) const;							// gives a staff bottom y position
	int		pitch2staff (int midipitch) const;								// gives the staff num for a given pitch
	int		pitch2staff (int midipitch, int& halfspaces, int& alter) const;	// gives the staff + halspaces and alter
	int		diatonic (int midipitch, int& octave, int& alter) const;		// gives the diatonic pitch of a midi pitch + octave and alter
	float	halfspaces2ypos (int halfspaces, int staff) const;
	int		halfSpaces2LedgerLines (int halfspaces) const;
	float	noteWidth () const							{ return fLineHeight * 1.8;	}

	void	DrawHead(float x, float y, int alter, VGDevice* dev) const;			// draw note head
	void	DrawLedgerLines(float x, float y, int count, VGDevice* dev) const;	// draw ledger lines

	public:
				 RProportional(ARMusic *arMusic);
				 RProportional(const char *midiFileName);
		virtual ~RProportional() {}

		virtual void onDraw(int width, int height, VGDevice *dev);
				void SetDrawDurationLines(bool d)		{ fDrawDurationLine = d; }
				bool GetDrawDurationLines()				{ return fDrawDurationLine; }

	protected:

		virtual void  DrawGrid		(DrawParams &drawParams) const;
		virtual void  DrawNote		(int pitch, double date, double dur, const DrawParams& drawParams) const;
		virtual void  DrawMeasureBar(double date, const DrawParams& drawParams) const;
		virtual void  DrawStaff		(int n, int width, VGDevice* dev) const;
		virtual float pitch2ypos	(int midipitch, const DrawParams& drawParams) const;
		virtual float stepheight    (int height) const  { return fLineHeight / 3; }
		virtual void  setColor		(VGDevice* dev, const VGColor& color);
		virtual void  popColor		(VGDevice* dev);
};

#endif
