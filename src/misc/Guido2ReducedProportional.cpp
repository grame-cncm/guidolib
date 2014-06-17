/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "GUIDOEngine.h"
#include "MusicalSymbols.h"
#include "Guido2ReducedProportional.h"
#include "VGDevice.h"
#include "VGSystem.h"
#include "VGFont.h"

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

//-------------------------------------------------------------------
GuidoReducedProportional::GuidoReducedProportional(TYPE_TIMEPOSITION start, TYPE_TIMEPOSITION end, int width, int height, bool drawdur)
	: GuidoPianoRoll(start, end, width, height), fDrawDurationLine(drawdur)
{
	fNumStaves = 4;			// 4 staves - hard coded for the moment
	fLineHeight = float(height) / ((fNumStaves * (kStaffLines - 1)) + ((fNumStaves-1) * kStaffSpacing) + (kStaffBorder * 2));
}

//-------------------------------------------------------------------
void GuidoReducedProportional::SetMusicFont(VGDevice* dev)
{
	VGSystem * sys = dev->getVGSystem();
	int fontsize = fLineHeight * 3.5;		// value experimentaly defined
	dev->SetMusicFont (sys->CreateVGFont("Guido2", fontsize, 0));
	fFontSavedColor = dev->GetFontColor();
}

//-------------------------------------------------------------------
float GuidoReducedProportional::staffTopPos (int i) const
{
	float offset = (kStaffSpacing + (kStaffLines-1)) * i;
	return (kStaffBorder + offset) * fLineHeight;
}

//-------------------------------------------------------------------
float GuidoReducedProportional::staffBottomPos (int i) const
{
	return staffTopPos(i) + ((kStaffLines-1) * fLineHeight);
}

//-------------------------------------------------------------------
float GuidoReducedProportional::noteWidth () const						{ return fLineHeight * 1.8;	}

//-------------------------------------------------------------------
void GuidoReducedProportional::DrawStaff (int n, VGDevice* dev) const
{
	float y = staffTopPos (n);
	for (int i=0; i<kStaffLines; i++) {
		dev->Line( 0, y, fWidth, y);
		y += fLineHeight;
	}
}

//-------------------------------------------------------------------
void GuidoReducedProportional::DrawGrid (VGDevice* dev) const
{
	dev->PushPenWidth(kLineThickness);
	for (int i=0; i< 4; i++) {
		DrawStaff (i, dev);
	}
	dev->PopPenWidth();
}

//-------------------------------------------------------------------
void GuidoReducedProportional::DrawLedgerLines(float x, float y, int count, VGDevice* dev) const
{
	dev->PushPenWidth(kLineThickness);
	int n = (count > 0 ? count : -count);
	int step = (count > 0 ? fLineHeight : -fLineHeight);
	float w = noteWidth();
	x -= (w - fLineHeight) / 1.8;
	for (int i=0; i<n; i++) {
		y += step;
		dev->Line( x, y, x+w, y);
	}
	dev->PopPenWidth();
}

//-------------------------------------------------------------------
int	 GuidoReducedProportional::pitch2staff (int midipitch) const
{
	for (int i=3; i > 0; i--) {
		if (midipitch < kStaffUpPitch[i-1]) return i;
	}
	return 0;
}

//-------------------------------------------------------------------
int	 GuidoReducedProportional::diatonic (int midipitch, int& octave, int& alter) const
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
int	 GuidoReducedProportional::halfSpaces2LedgerLines (int halfspaces) const
{
	if (halfspaces < 0) return halfspaces / 2;
	halfspaces -= kHalSpacesPerOct + 1 ;
	if (halfspaces > 0) return halfspaces / 2;
	return 0;
}

//-------------------------------------------------------------------
int	 GuidoReducedProportional::pitch2staff (int midipitch, int& halfspaces, int& alter) const
{
	int oct;
	int p = diatonic (midipitch, oct, alter);

	int staff = pitch2staff (midipitch);
	int topOct	= kStaffTopOct[staff];
	int topDiat = kStaffTopDiat[staff];

	halfspaces = (topDiat - p) + ((topOct - oct) * kHalSpacesPerOct);
	return staff;
}

//-------------------------------------------------------------------
float GuidoReducedProportional::halfspaces2ypos (int halfspaces, int staff) const
{
	float y = (halfspaces * fLineHeight) / 2;
	return staffTopPos(staff) + y;
}

//-------------------------------------------------------------------
void GuidoReducedProportional::DrawHead(float x, float y, int alter, VGDevice* dev) const
{
	if (fColored) dev->SetFontColor(fColor);
	else dev->SetFontColor(fFontSavedColor);
	if (alter) {
		const VGFont * font = dev->GetMusicFont();
		char symbol = (alter > 0) ? kSharpSymbol : kFlatSymbol;
		float w, h;
		font->GetExtent( symbol, &w, &h, dev);
		dev->DrawMusicSymbol (x-w-(fLineHeight/4), y, symbol);
	}
	dev->DrawMusicSymbol (x, y, kFullHeadSymbol);
}

//-------------------------------------------------------------------
void GuidoReducedProportional::Draw(int pitch, double date, double dur, VGDevice* dev) const
{
	float x = date2xpos (date);
	int alter, halfspaces;
	int staff = pitch2staff(pitch, halfspaces, alter);
	float y = halfspaces2ypos (halfspaces, staff);
	int ll = halfSpaces2LedgerLines (halfspaces);
	if (ll) {
		DrawLedgerLines(x, (ll > 0 ? staffBottomPos(staff) : staffTopPos(staff)), ll, dev);
	}
	if (fDrawDurationLine) DrawRect(x, y, dur, dev);
	DrawHead(x-1, y, alter, dev);
}



