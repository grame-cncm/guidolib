/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMusic.h"
#include "GUIDOEngine.h"
#include "MusicalSymbols.h"
#include "GuidoReducedProportional.h"
#include "VGDevice.h"
#include "VGSystem.h"
#include "VGFont.h"

using namespace std;

#define kDefaultWidth    1024 // the default canvas width
#define kDefaultHeight   400  // the default canvas height
#define kStaffSpacing    4    // number of spaces between staves
#define kStaffBorder     8    // top and bottom spaces reserved for high and low pitches
#define kStepsPerStaff   9    // number of diatonic steps on a staff
#define kHalSpacesPerOct 7    // number of half spaces per octave

#define kNWidthMin       4    // minimum width of a quarter
#define kNWidthMax       12   // maximum width of a quarter


static int kStaffUpPitch[] = { 83, 59, 38 }; // pitch
static int kStaffTopDiat[] = { 3, 3, 5, 5 };
static int kStaffTopOct [] = { 8, 6, 4, 2 };

static const float kLineThickness = 0.5;

//--------------------------------------------------------------------------
GuidoReducedProportional::GuidoReducedProportional() :
    GuidoPianoRoll(), fDrawDurationLine(true)
{
	fNumStaves = 4; // REM: 4 staves - hard coded for the moment
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::setCanvasDimensions(int width, int height)
{
    if (width == -1)
        fWidth = kDefaultWidth;
    else
        fWidth = width;

    if (height == -1)
        fHeight = kDefaultHeight;
    else
        fHeight = height;
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::getRenderingFromAR(VGDevice* dev)
{
    fDev = dev;
    
    initRendering();

    if (!fIsEndDateSet)
        fEndDate = fARMusic->getDuration();

    fDuration = double(fEndDate - fStartDate);

    fLineHeight = float(fHeight) / ((fNumStaves * (kStaffLines - 1)) + ((fNumStaves - 1) * kStaffSpacing) + (kStaffBorder * 2));

	SetMusicFont();
	DrawGrid();

    GuidoPos pos = fARMusic->GetHeadPosition();

	while (pos) {
		ARMusicalVoice *e = fARMusic->GetNext(pos);
		DrawVoice(e);
	}

	endRendering();
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::getRenderingFromMidi(VGDevice* dev)
{
    fDev = dev;
    
    initRendering();

	SetMusicFont();

    DrawFromMidi();

    endRendering();
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::SetMusicFont()
{
	VGSystem * sys = fDev->getVGSystem();
	int fontSize = (int) (fLineHeight * 3.5); // value experimentaly defined

	fDev->SetMusicFont(sys->CreateVGFont("Guido2", fontSize, 0));
	fFontSavedColor = fDev->GetFontColor();
}

//--------------------------------------------------------------------------
float GuidoReducedProportional::staffTopPos(int i) const
{
	float offset = (kStaffSpacing + (kStaffLines - 1)) * (float) i;
	return (kStaffBorder + offset) * fLineHeight;
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawStaff(int n) const
{
	float y = staffTopPos (n);

	for (int i = 0; i < kStaffLines; i++) {
		fDev->Line(0, y, (float) fWidth, y);
		y += fLineHeight;
	}
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawGrid() const
{
	fDev->PushPenWidth(kLineThickness);

	for (int i = 0; i < 4; i++)
		DrawStaff(i);

	fDev->PopPenWidth();
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawLedgerLines(float x, float y, int count) const
{
	fDev->PushPenWidth(kLineThickness);

	int n = (count > 0 ? count : - count);
	int step = (count > 0 ? (int) fLineHeight : (int) - fLineHeight);
	float w = getNoteWidth();
	x -= (w - fLineHeight) / 1.8f;

	for (int i = 0; i < n; i++) {
		y += step;
		fDev->Line(x, y, x + w, y);
	}

	fDev->PopPenWidth();
}

//--------------------------------------------------------------------------
int	GuidoReducedProportional::pitch2staff(int midipitch) const
{
	for (int i = 3; i > 0; i--) {
		if (midipitch < kStaffUpPitch[i - 1])
            return i;
    }

	return 0;
}

//--------------------------------------------------------------------------
int	GuidoReducedProportional::diatonic(int midipitch, int& octave, int& alter) const
{
	octave = midipitch / 12;
	int p  = midipitch % 12;

	if (p < 5) {
		alter = p % 2;
		return p / 2;
	}

	p += 1;
	alter = p % 2;

	return p / 2;
}

//--------------------------------------------------------------------------
int	GuidoReducedProportional::halfSpaces2LedgerLines(int halfspaces) const
{
	if (halfspaces < 0)
        return halfspaces / 2;

	halfspaces -= kHalSpacesPerOct + 1 ;

	if (halfspaces > 0)
        return halfspaces / 2;

	return 0;
}

//--------------------------------------------------------------------------
int	GuidoReducedProportional::pitch2staff(int midipitch, int& halfspaces, int& alter) const
{
	int oct;
	int p = diatonic (midipitch, oct, alter);

	int staff   = pitch2staff (midipitch);
	int topOct	= kStaffTopOct[staff];
	int topDiat = kStaffTopDiat[staff];

	halfspaces = (topDiat - p) + ((topOct - oct) * kHalSpacesPerOct);

	return staff;
}

//--------------------------------------------------------------------------
float GuidoReducedProportional::halfspaces2ypos(int halfspaces, int staff) const
{
	float y = (halfspaces * fLineHeight) / 2;

	return staffTopPos(staff) + y;
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawHead(float x, float y, int alter) const
{
    if (!fColors->empty())
        fDev->SetFontColor(fColors->top());
	else
        fDev->SetFontColor(fFontSavedColor);

	if (alter) {
		const VGFont *font = fDev->GetMusicFont();
		char symbol = (alter > 0) ? kSharpSymbol : kFlatSymbol;
		float w, h;
		font->GetExtent(symbol, &w, &h, fDev);
		fDev->DrawMusicSymbol(x - w - (fLineHeight / 4), y, symbol);
	}

	fDev->DrawMusicSymbol(x, y, kFullHeadSymbol);
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawNote(int pitch, double date, double dur) const
{
    if (pitch >= fLowPitch && pitch <= fHighPitch) {
        int   alter, halfspaces;
        float x     = (float) date2xpos(date);
        int   staff = pitch2staff(pitch, halfspaces, alter);
        float y     = halfspaces2ypos(halfspaces, staff);
        int   ll    = halfSpaces2LedgerLines(halfspaces);

        if (ll)
            DrawLedgerLines(x, (ll > 0 ? staffBottomPos(staff) : staffTopPos(staff)), ll);

        if (fDrawDurationLine)
            DrawRect((int) x, (int) y, dur);

        DrawHead(x - 1, y, alter);
    }
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawRect(int x, int y, double dur) const
{
	int   w              =   duration2width(dur);
	float rectHalfHeight =   fLineHeight / 4;
    float xLeftOffset    =   fLineHeight / 2;
    float xRightOffset   = - fLineHeight / 4;
    float yOffset        =   fLineHeight / 8;

	fDev->Rectangle((float) x + xLeftOffset, (float) (y - rectHalfHeight + yOffset), (float) (x + xRightOffset + (w ? w : 1)), (float) (y + rectHalfHeight + yOffset));
}