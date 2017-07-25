/*
 GUIDO Library
 Copyright (C) 2015 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <cstdlib>
#include <time.h>
#include <math.h>
#include <sstream>

#include "ARMusic.h"
//#include "ARNoteName.h"
#include "ARRest.h"
#include "ARNote.h"
#include "ARBar.h"
#include "ARChordComma.h"
#include "ARNoteFormat.h"
#include "FontManager.h"
#include "HtmlColors.h"
#include "TagParameterString.h"
#include "TagParameterRGBColor.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "RProportional.h"

using namespace std;

#define kStaffLines			5		// number of lines per staff
#define kStaffSpacing		4		// number of spaces between staves
#define kStaffBorder		8		// top and bottom spaces reserved for high and low pitches
#define kStepsPerStaff		9		// number of diatonic steps on a staff
#define kHalSpacesPerOct	7		// number of half spaces per octave

#define kNWidthMin		4			// minimum width of a quarter
#define kNWidthMax		12			// maximum width of a quarter

static int kStaffUpPitch[]	= { 83, 59, 38 };	// pitch
static int kStaffTopDiat[]	= { 3, 3, 5, 5 };
static int kStaffTopOct[]	= { 8, 6, 4, 2 };

static const float kLineThickness = 0.5;


//--------------------------------------------------------------------------
RProportional::RProportional(ARMusic *arMusic) : PianoRoll (arMusic)
{
	fNumStaves = 4;			// 4 staves - hard coded for the moment
	fDrawDurationLine = true;
}
RProportional::RProportional(const char *midiFileName) : PianoRoll (midiFileName)
{
	fNumStaves = 4;			// 4 staves - hard coded for the moment
	fDrawDurationLine = true;
}


//--------------------------------------------------------------------------
void RProportional::onDraw(int width, int height, VGDevice *dev)
{
	fLineHeight = float(height) / ((fNumStaves * (kStaffLines - 1)) + ((fNumStaves-1) * kStaffSpacing) + (kStaffBorder * 2));
	int fontsize = int(fLineHeight * 3.5);		// value experimentaly defined
	string fname ("Guido2");
	const VGFont * font = FontManager::FindOrCreateFont( dev->getVGSystem(), fontsize, &fname);
	dev->SetMusicFont (font);
	PianoRoll::onDraw(width, height, dev);
}

//-------------------------------------------------------------------
float RProportional::staffTopPos (int i) const
{
	int offset = (kStaffSpacing + (kStaffLines-1)) * i;
	return (kStaffBorder + offset) * fLineHeight;
}

//-------------------------------------------------------------------
float RProportional::staffBottomPos (int i) const
{
	return staffTopPos(i) + ((kStaffLines-1) * fLineHeight);
}

//-------------------------------------------------------------------
void RProportional::DrawStaff (int n, int w, VGDevice* dev) const
{
	float y = staffTopPos (n);
	for (int i=0; i<kStaffLines; i++) {
		dev->Line( 0, y, float(w), y);
		y += fLineHeight;
	}
}

//--------------------------------------------------------------------------
void RProportional::DrawGrid(PianoRoll::DrawParams &drawParams) const
{
	VGDevice *dev = drawParams.dev;
	dev->PushPenWidth(kLineThickness);
	for (int i=0; i< 4; i++) {
		DrawStaff (i, drawParams.width, dev);
	}
	dev->PopPenWidth();
}


//-------------------------------------------------------------------
int	 RProportional::diatonic (int midipitch, int& octave, int& alter) const
{
	octave = midipitch / 12;
	int p = midipitch % 12;
	if (p < 5) {
		alter = p % 2;
		return p/2;
	}
	p += 1;
	alter = p % 2;
	return p/2;
}

//-------------------------------------------------------------------
int	 RProportional::pitch2staff (int midipitch, int& halfspaces, int& alter) const
{
	int oct;
	int p = diatonic (midipitch, oct, alter);

	int staff = pitch2staff (midipitch);
	int topOct	= kStaffTopOct[staff];
	int topDiat = kStaffTopDiat[staff];

	halfspaces = (topDiat - p) + ((topOct - oct) * kHalSpacesPerOct);
	return staff;
}

//--------------------------------------------------------------------------
int RProportional::pitch2staff (int midipitch) const
{
	for (int i=3; i > 0; i--) {
		if (midipitch < kStaffUpPitch[i-1]) return i;
	}
	return 0;
}

//-------------------------------------------------------------------
int	 RProportional::halfSpaces2LedgerLines (int halfspaces) const
{
	if (halfspaces < 0) return halfspaces / 2;
	halfspaces -= kHalSpacesPerOct + 1 ;
	if (halfspaces > 0) return halfspaces / 2;
	return 0;
}

//-------------------------------------------------------------------
float RProportional::halfspaces2ypos (int halfspaces, int staff) const
{
	float y = (halfspaces * fLineHeight) / 2;
	return staffTopPos(staff) + y;
}

//-------------------------------------------------------------------
void RProportional::DrawLedgerLines(float x, float y, int count, VGDevice* dev) const
{
	dev->PushPenWidth(kLineThickness);
	int n = (count > 0 ? count : -count);
	float step = (count > 0 ? fLineHeight : -fLineHeight);
	float w = noteWidth();
	x -= w/5;
	for (int i=0; i<n; i++) {
		y += step;
		dev->Line( x, y, x+w, y);
	}
	dev->PopPenWidth();
}

//--------------------------------------------------------------------------
void RProportional::DrawNote(int pitch, double date, double dur, const DrawParams& drawParams) const
{
	VGDevice *dev = drawParams.dev;

	float x = date2xpos (date, drawParams.width, drawParams.untimedLeftElementWidth);
	int alter, halfspaces;
	int staff = pitch2staff(pitch, halfspaces, alter);
	float y = halfspaces2ypos (halfspaces, staff);
	int ll = halfSpaces2LedgerLines (halfspaces);
	if (ll) {
		DrawLedgerLines(x, (ll > 0 ? staffBottomPos(staff) : staffTopPos(staff)), ll, dev);
	}
	if (fDrawDurationLine) DrawRect(x+1, y, dur-0.005, drawParams);
	DrawHead(x, y, alter, dev);
}

//--------------------------------------------------------------------------
void RProportional::setColor (VGDevice* dev, const VGColor& color)
{
	fFontColors.push (dev->GetFontColor());
	dev->SetFontColor(color);
	PianoRoll::setColor( dev, color);
}

//--------------------------------------------------------------------------
void RProportional::popColor (VGDevice* dev)
{
	PianoRoll::popColor(dev);
	if (fFontColors.size()) {
		VGColor color = fFontColors.top();
		dev->SetFontColor(color);
		fFontColors.pop();
	}
}

//-------------------------------------------------------------------
void RProportional::DrawHead(float x, float y, int alter, VGDevice* dev) const
{
	if (alter) {
		const VGFont * font = dev->GetMusicFont();
		char symbol = (alter > 0) ? kSharpSymbol : kFlatSymbol;
		float w, h;
		font->GetExtent( symbol, &w, &h, dev);
		dev->DrawMusicSymbol (x-w-(fLineHeight/4), y, symbol);
	}
	dev->DrawMusicSymbol (x, y, kFullHeadSymbol);
}

//--------------------------------------------------------------------------
void RProportional::DrawMeasureBar(double date,  const DrawParams& drawParams) const
{
    float x  = date2xpos(date, drawParams.width, drawParams.untimedLeftElementWidth);
	float y1 = staffTopPos (0);
	float y2 = staffBottomPos (3);
    drawParams.dev->PushPenWidth(0.3f);
    drawParams.dev->Line (roundFloat(x), roundFloat(y1), roundFloat(x), roundFloat(y2));
    drawParams.dev->PopPenWidth();
}

//--------------------------------------------------------------------------
float RProportional::pitch2ypos(int midipitch, const DrawParams& drawParams) const
{
	int   p = midipitch - fLowPitch;
    float h = ((float) (drawParams.height * p) / (float) pitchRange());

	return drawParams.height - (float) drawParams.noteHeight * 0.5f - h;
}

