/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <math.h>

#include "ARMusic.h"
#include "ARRest.h"
#include "ARNoteFormat.h"
#include "ARChordComma.h"
#include "ARBar.h"
#include "GUIDOEngine.h"
#include "MusicalSymbols.h"
#include "GuidoReducedProportional.h"
#include "VGDevice.h"
#include "VGSystem.h"
#include "VGFont.h"
#include "TagParameterString.h"

using namespace std;

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
GuidoReducedProportional::GuidoReducedProportional(ARMusic *arMusic) :
    GuidoPianoRoll(arMusic), fDrawDurationLine(true)
{
	init();
}

//--------------------------------------------------------------------------
GuidoReducedProportional::GuidoReducedProportional(const char *midiFileName) :
    GuidoPianoRoll(midiFileName), fDrawDurationLine(true)
{
	init();
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::init()
{
	fNumStaves = 4; // REM: 4 staff - hard coded for the moment
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::onDraw(int width, int height, VGDevice* dev)
{
    DrawParams drawParams = createDrawParamsStructure(width, height, dev);
    
    initRendering(drawParams);
    
	DrawGrid(drawParams);
    SetMusicFont(drawParams);

    if (ownsARMusic())
        DrawFromAR(drawParams);
    else
        DrawFromMidi(drawParams);

    endRendering(drawParams);
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::SetMusicFont(DrawParams drawParams)
{
	VGSystem * sys = drawParams.dev->getVGSystem();
	int fontSize = (int) (drawParams.noteHeight * 3.5); // value experimentaly defined

	drawParams.dev->SetMusicFont(sys->CreateVGFont("Guido2", fontSize, 0));
	fFontSavedColor = drawParams.dev->GetFontColor();
}

//--------------------------------------------------------------------------
float GuidoReducedProportional::staffTopPos(int i, float noteHeight) const
{
	float offset = (kStaffSpacing + (kStaffLines - 1)) * (float) i;
	return (kStaffBorder + offset) * noteHeight;
}

//--------------------------------------------------------------------------
GuidoPianoRoll::DrawParams GuidoReducedProportional::createDrawParamsStructure(int width, int height, VGDevice *dev) const
{
    int currentWidth  = (width  == -1 ? kDefaultWidth  : width);
    int currentHeight = (height == -1 ? kDefaultHeight : height);
    
    float noteHeight              = computeNoteHeight(currentHeight);
    float untimedLeftElementWidth = 0;

    GuidoPianoRoll::DrawParams drawParams(currentWidth, currentHeight, noteHeight, untimedLeftElementWidth, dev);

    return drawParams;
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawStaff(int n, DrawParams drawParams) const
{
    float y = staffTopPos(n, drawParams.noteHeight);

	for (int i = 0; i < kStaffLines; i++) {
		drawParams.dev->Line(0, y, (float) drawParams.width, y);
		y += drawParams.noteHeight;
	}
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawGrid(DrawParams drawParams) const
{
	drawParams.dev->PushPenWidth(kLineThickness);

	for (int i = 0; i < 4; i++)
		DrawStaff(i, drawParams);

	drawParams.dev->PopPenWidth();
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawLedgerLines(float x, float y, int count, DrawParams drawParams) const
{
	drawParams.dev->PushPenWidth(kLineThickness);

	int n = (count > 0 ? count : - count);
	int step = (count > 0 ? (int) drawParams.noteHeight : (int) - drawParams.noteHeight);
	float w = getNoteWidth(drawParams.noteHeight);

	x -= (w - drawParams.noteHeight) / 1.8f;

	for (int i = 0; i < n; i++) {
		y += step;
		drawParams.dev->Line(x, y, x + w, y);
	}

	drawParams.dev->PopPenWidth();
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
	int p = diatonic(midipitch, oct, alter);

	int staff   = pitch2staff (midipitch);
	int topOct	= kStaffTopOct[staff];
	int topDiat = kStaffTopDiat[staff];

	halfspaces = (topDiat - p) + ((topOct - oct) * kHalSpacesPerOct);

	return 2;
}

//--------------------------------------------------------------------------
float GuidoReducedProportional::halfspaces2ypos(int halfspaces, int staff, float noteHeight) const
{
	float y = (halfspaces * noteHeight) / 2;

	return staffTopPos(staff, noteHeight) + y;
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawVoice(ARMusicalVoice* v, DrawParams drawParams)
{
    if (fVoicesColors != NULL) {
        int voiceNum = v->getVoiceNum();
        
        for (unsigned int i = 0; i < fVoicesColors->size() && fColors->empty(); i++) {
            std::pair<int, VGColor> pair = fVoicesColors->at(i);

            if (pair.first == voiceNum)
                fColors->push(pair.second);
        }
    }
    
    if (!fColors->empty() || fVoicesAutoColored) {
        if (fColors->empty()) {
            int r, g, b;
            
            drawParams.colorHue += kGoldenRatio;
            drawParams.colorHue  = fmod(drawParams.colorHue, 1);

            HSVtoRGB((float) drawParams.colorHue, 0.5f, 0.9f, r, g, b);

            fColors->push(VGColor(r, g, b, 255));
        }
        
        drawParams.dev->PushPenColor(fColors->top());
        drawParams.dev->PushFillColor(fColors->top());
    }

    fChord   = false;
	ObjectList *ol  = (ObjectList *)v;
	GuidoPos    pos = ol->GetHeadPosition();

	while (pos)
	{
		ARMusicalObject  *e    = ol->GetNext(pos);
		TYPE_DURATION     dur  = e->getDuration();
		TYPE_TIMEPOSITION date = e->getRelativeTimePosition();

        if (fChord) {
            dur   = fChordDuration;
            date -= dur;
        }

		TYPE_TIMEPOSITION end = date + dur;

		if (date >= fStartDate) {
            if (date < fEndDate) {
                if (end > fEndDate)
                    dur = fEndDate - date;

                DrawMusicalObject(e, date, dur, drawParams);
            }
		}
		else if (end > fStartDate) { // to make the note end appear
			date = fStartDate;	
			
            if (end > fEndDate)
                dur = fEndDate - date;
			else
                dur = end - date;
			
            DrawMusicalObject(e, date, dur, drawParams);
		}

		if (dynamic_cast<ARRest *>(e))
			fChord = false;
		else if (dynamic_cast<ARChordComma *>(e))
			fChord = true;
		else if (dynamic_cast<ARNoteFormat *>(e))
            handleColor(dynamic_cast<ARNoteFormat *>(e), drawParams);
        else if (dynamic_cast<ARBar *>(e) && fMeasureBarsEnabled)
            DrawMeasureBar(date, drawParams);
	}

    while (!fColors->empty()) {
		drawParams.dev->PopFillColor();
		drawParams.dev->PopPenColor();

        fColors->pop();
	}
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawHead(float x, float y, int alter, DrawParams drawParams) const
{
    if (!fColors->empty())
        drawParams.dev->SetFontColor(fColors->top());
	else
        drawParams.dev->SetFontColor(fFontSavedColor);

	if (alter) {
		const VGFont *font = drawParams.dev->GetMusicFont();
		char symbol = (alter > 0) ? kSharpSymbol : kFlatSymbol;
		float w, h;
		font->GetExtent(symbol, &w, &h, drawParams.dev);
		drawParams.dev->DrawMusicSymbol(x - w - (drawParams.noteHeight / 4), y, symbol);
	}

	drawParams.dev->DrawMusicSymbol(x, y, kFullHeadSymbol);
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawNote(int pitch, double date, double dur, DrawParams drawParams)
{
    if (pitch >= fLowPitch && pitch <= fHighPitch) {
        int   alter, halfspaces;
        float x     = (float) date2xpos(date, drawParams.width, drawParams.untimedLeftElementWidth) + drawParams.noteHeight / 5 /* for notes to not be cut */;
        int   staff = pitch2staff(pitch, halfspaces, alter);
        float y     = halfspaces2ypos(halfspaces, staff, drawParams.noteHeight);
        int   ll    = halfSpaces2LedgerLines(halfspaces);

        if (ll)
            DrawLedgerLines(x, (ll > 0 ? staffBottomPos(staff, drawParams.noteHeight) : staffTopPos(staff, drawParams.noteHeight)), ll, drawParams);

        if (fDrawDurationLine)
            DrawRect(x, y, dur, drawParams);

        DrawHead(x - 1, y, alter, drawParams);
    }
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::DrawRect(float x, float y, double dur, DrawParams drawParams) const
{
	float w              =   duration2width(dur, drawParams.width, drawParams.untimedLeftElementWidth);
	float rectHalfHeight =   drawParams.noteHeight / 4;
    float xLeftOffset    =   drawParams.noteHeight / 2;
    float xRightOffset   = - drawParams.noteHeight / 4;
    float yOffset        =   drawParams.noteHeight / 8;

	drawParams.dev->Rectangle((float) x + xLeftOffset, (float) (y - rectHalfHeight + yOffset), (float) (x + xRightOffset + (w ? w : 1)), (float) (y + rectHalfHeight + yOffset));
}

//--------------------------------------------------------------------------
void GuidoReducedProportional::handleColor(ARNoteFormat* noteFormat, DrawParams drawParams) const
{
    const TagParameterString *tps = noteFormat->getColor();
    unsigned char colref[4];

    if (tps && tps->getRGB(colref)) {
        fColors->push(VGColor(colref[0], colref[1], colref[2], colref[3]));

        drawParams.dev->PushPenColor(fColors->top());
        drawParams.dev->PushFillColor(fColors->top());
    }
    else {
        fColors->pop();

        drawParams.dev->PopFillColor();
        drawParams.dev->PopPenColor();
    }
}

//--------------------------------------------------------------------------
float GuidoReducedProportional::computeNoteHeight(int height) const
{
    float noteHeight = float(height) / ((fNumStaves * (kStaffLines - 1)) + ((fNumStaves - 1) * kStaffSpacing) + (kStaffBorder * 2));

    return noteHeight;
}