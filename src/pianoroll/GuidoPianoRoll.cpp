/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
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
#include "VGDevice.h"
#include "ARNoteName.h"
#include "ARRest.h"
#include "ARNote.h"
#include "ARBar.h"
#include "ARChordComma.h"
#include "ARNoteFormat.h"
#include "TagParameterString.h"
#include "FontManager.h"

#ifdef MIDIEXPORT
    #include "midifile.h"
#endif

#include "GuidoPianoRoll.h"

using namespace std;

#define kMainLineWidth    1.6f
#define kSubMainLineWidth 1.1f
#define kNormalLineWidth  0.6f

#define kColorSeed        0.5f

#define kLimitDist34Mode  6                       // the minimum distance between lines of the grid
                                                  //     (to switch between mode 4 and mode 3 of pitch line display)
#define kLimitDist23Mode  10                      // the medium distance between lines of the grid
                                                  //     (to switch between mode 3 and mode 2 of pitch line display)
#define kLimitDist12      14                      // the minimum distance between lines of the grid
                                                  //     (to switch between mode 2 and mode 1 of pitch line display)

//--------------------------------------------------------------------------
GuidoPianoRoll::GuidoPianoRoll(ARMusic *arMusic) :
    fKeyboardEnabled(false),
    fVoicesAutoColored(false), fVoicesColors(NULL),
    fMeasureBarsEnabled(false),
    fPitchLinesDisplayMode(kAutoLines),
    fARMusic(NULL)
{
    fARMusic = arMusic;

    init();
}

//--------------------------------------------------------------------------
GuidoPianoRoll::GuidoPianoRoll(const char *midiFileName) :
    fKeyboardEnabled(false),
    fVoicesAutoColored(false), fVoicesColors(NULL),
    fMeasureBarsEnabled(false),
    fPitchLinesDisplayMode(kAutoLines),
    fARMusic(NULL)
{
    fMidiFileName = midiFileName;

    init();
}

//--------------------------------------------------------------------------
GuidoPianoRoll::~GuidoPianoRoll() 
{
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::init() 
{
    GuidoDate defaultStartDate = {kDefaultStartDateNum, kDefaultStartDateDenom};
    GuidoDate defaultEndDate   = {kDefaultEndDateNum, kDefaultEndDateDenom};

    setLimitDates(defaultStartDate, defaultEndDate);
    setPitchRange(kDefaultLowPitch, kDefaultHighPitch);

    fColors = new std::stack<VGColor>();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setLimitDates(GuidoDate start, GuidoDate end)
{
    if (start.num == 0 && start.denom == 0)
        fStartDate = TYPE_TIMEPOSITION(kDefaultStartDateNum, kDefaultStartDateDenom);
    else
	    fStartDate = TYPE_TIMEPOSITION(start.num, start.denom);

    if (end.num == 0 && end.denom == 0)
        fEndDate = (ownsARMusic() ? fARMusic->getDuration() : getMidiEndDate());
    else
	    fEndDate = TYPE_TIMEPOSITION(end.num, end.denom);

    fDuration = double(fEndDate - fStartDate);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setPitchRange(int minPitch, int maxPitch)
{
    if (minPitch == -1)
        fLowPitch = (ownsARMusic() ? detectARExtremePitch(true) : detectMidiExtremePitch(true));
    else
        fLowPitch = minPitch;

    if (maxPitch == -1)
        fHighPitch = (ownsARMusic() ? detectARExtremePitch(false) : maxPitch = detectMidiExtremePitch(false));
    else
        fHighPitch = fHighPitch;

    if (fHighPitch - fLowPitch < 11)
        autoAdjustPitchRange(fLowPitch, fHighPitch);
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::getKeyboardWidth(int width, int height)
{
    int currentWidth  = (width  == -1 ? kDefaultWidth  : width);
    int currentHeight = (height == -1 ? kDefaultHeight : height);

    float noteHeight              = computeNoteHeight(currentHeight);
    float untimedLeftElementWidth = computeKeyboardWidth(noteHeight);

    return untimedLeftElementWidth;
}

void GuidoPianoRoll::setPitchLinesDisplayMode(int mode)
{
    fPitchLinesDisplayMode = mode;

    int currentInt = mode;

    for (int i = 11; i >= 0; i--) {
        if (currentInt - (1<<i) >= 0) {
            currentInt -= 1<<i;
            fBytes[i] = true;
        }
        else
            fBytes[i] = false;
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setColorToVoice(int voiceNum, int r, int g, int b, int a)
{
    if (!fVoicesColors)
        fVoicesColors = new std::vector<std::pair<int, VGColor> >();
    else {
        for (unsigned int i = 0; i < fVoicesColors->size(); i++) {
            std::pair<int, VGColor> pair = fVoicesColors->at(i);

            if (pair.first == voiceNum) {
                fVoicesColors->erase(fVoicesColors->begin() + i);
                break;
            }
        }
    }

    VGColor color(r, g, b, a);
    std::pair<int, VGColor> newVoiceColor(voiceNum, color);

    fVoicesColors->push_back(newVoiceColor);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::getMap(int width, int height, Time2GraphicMap &outmap)
{
    int currentWidth  = (width  == -1 ? kDefaultWidth  : width);
    int currentHeight = (height == -1 ? kDefaultHeight : height);
    
    float noteHeight              = computeNoteHeight(currentHeight);
    float untimedLeftElementWidth = computeKeyboardWidth(noteHeight);

    FloatRect r(0 + untimedLeftElementWidth, 0, (float) currentWidth, (float) currentHeight);

    GuidoDate from	= { fStartDate.getNumerator(), fStartDate.getDenominator() };
    GuidoDate to	= { fEndDate.getNumerator(), fEndDate.getDenominator() }; // REM: est-ce fEndDate est vraiment la fin du pianoroll ?
    TimeSegment dates (from, to);

    outmap.push_back(make_pair(dates, r));
}

//--------------------------------------------------------------------------
bool GuidoPianoRoll::ownsARMusic() {
    if (fARMusic)
        return true;
    else
        return false;
}

//--------------------------------------------------------------------------
bool GuidoPianoRoll::ownsMidi() {
    if (fMidiFileName)
        return true;
    else
        return false;
}

//--------------------------------------------------------------------------
GuidoPianoRoll::DrawParams GuidoPianoRoll::createDrawParamsStructure(int width, int height, VGDevice *dev) const
{
    int currentWidth  = (width  == -1 ? kDefaultWidth  : width);
    int currentHeight = (height == -1 ? kDefaultHeight : height);
    
    float noteHeight              = computeNoteHeight(currentHeight);
    float untimedLeftElementWidth = computeKeyboardWidth(noteHeight);

    GuidoPianoRoll::DrawParams drawParams(currentWidth, currentHeight, noteHeight, untimedLeftElementWidth, dev);

    return drawParams;
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::onDraw(int width, int height, VGDevice *dev)
{
    DrawParams drawParams = createDrawParamsStructure(width, height, dev);

    initRendering(drawParams);
    
    DrawGrid(drawParams);

    if (fKeyboardEnabled)
        DrawKeyboard(drawParams);

    if (ownsARMusic())
        DrawFromAR(drawParams);
    else
        DrawFromMidi(drawParams);

    endRendering(drawParams);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawFromAR(GuidoPianoRoll::DrawParams drawParams)
{
    GuidoPos pos = fARMusic->GetHeadPosition();

    while(pos) {
        ARMusicalVoice *e = fARMusic->GetNext(pos);
        DrawVoice(e, drawParams);
    }
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::computeKeyboardWidth(float noteHeight) const
{
    float untimedLeftElementWidth = 0;

    if (fKeyboardEnabled)
        untimedLeftElementWidth = 6.0f * noteHeight;

    return untimedLeftElementWidth;
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::computeNoteHeight(int height) const
{
    float noteHeight = (float) height / (float) pitchRange();

    if (!noteHeight)
        noteHeight = 1;

    return noteHeight;
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::initRendering(GuidoPianoRoll::DrawParams drawParams)
{
    float currentWidth =(float) drawParams.width + (int) floor(drawParams.untimedLeftElementWidth);

    drawParams.dev->NotifySize(drawParams.width, drawParams.height);
    drawParams.dev->BeginDraw();

    drawParams.dev->PushPenColor(VGColor(100, 100, 100));
    drawParams.dev->PushFillColor(VGColor(0, 0, 0));
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::endRendering(GuidoPianoRoll::DrawParams drawParams)
{
    drawParams.dev->PopFillColor();
    drawParams.dev->PopPenColor();
    drawParams.dev->EndDraw();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawGrid(GuidoPianoRoll::DrawParams drawParams) const
{
    drawParams.dev->PushPenColor(VGColor(0, 0, 0));

    if (fPitchLinesDisplayMode == kAutoLines) {
        if (drawParams.noteHeight < kLimitDist34Mode)
            DrawOctavesGrid(drawParams);
        else if (drawParams.noteHeight < kLimitDist23Mode)
            DrawTwoLinesGrid(drawParams);
        else if (drawParams.noteHeight < kLimitDist12)
            DrawDiatonicGrid(drawParams);
        else
            DrawChromaticGrid(drawParams);
    }
    else
        DrawChromaticGrid(drawParams, true);

    drawParams.dev->PopPenColor();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawOctavesGrid(GuidoPianoRoll::DrawParams drawParams) const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i, drawParams) + 0.5f * drawParams.noteHeight;
		int step = i % 12; // the note in chromatic step

        if (step == 0) { // C notes are highlighted
            drawParams.dev->PushPenWidth((i == 60) ? kSubMainLineWidth : kNormalLineWidth);
            drawParams.dev->Line(
                roundFloat(drawParams.untimedLeftElementWidth),
                roundFloat(y),
                (float) drawParams.width,
                roundFloat(y));
            drawParams.dev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawTwoLinesGrid(GuidoPianoRoll::DrawParams drawParams) const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i, drawParams) + 0.5f * drawParams.noteHeight;
		int step = i % 12; // the note in chromatic step

        if (step == 0 || step == 7) {
            float width = kNormalLineWidth;

            if (i == 60)
                width = kMainLineWidth;
            else if (step == 0)
                width = kSubMainLineWidth;

            drawParams.dev->PushPenWidth(width);
            drawParams.dev->Line(
                roundFloat(drawParams.untimedLeftElementWidth),
                roundFloat(y),
                (float) drawParams.width,
                roundFloat(y));
            drawParams.dev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawDiatonicGrid(GuidoPianoRoll::DrawParams drawParams) const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i, drawParams) + 0.5f * drawParams.noteHeight;
		int step = i % 12; // the note in chromatic step

        if (step == 0 || step == 2 || step == 4
            || step == 5 || step == 7 || step == 9
            || step == 11) {
                float width = kNormalLineWidth;

                if (i == 60)
                    width = kMainLineWidth;
                else if (step == 0)
                    width = kSubMainLineWidth;

                drawParams.dev->PushPenWidth(width);
                drawParams.dev->Line(
                    roundFloat(drawParams.untimedLeftElementWidth),
                    roundFloat(y),
                    (float) drawParams.width,
                    roundFloat(y));
                drawParams.dev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawChromaticGrid(GuidoPianoRoll::DrawParams drawParams, bool isUserDefined) const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i, drawParams) + 0.5f * drawParams.noteHeight;
		int step = i % 12; // the note in chromatic step
        
        if (!isUserDefined ||fBytes[step] == true) {
            float width = kNormalLineWidth;

            if (i == 60)
                width = kMainLineWidth;
            else if (step == 0)
                width = kSubMainLineWidth;

            drawParams.dev->PushPenWidth(width);
            drawParams.dev->Line(
                roundFloat(drawParams.untimedLeftElementWidth),
                roundFloat(y),
                (float) drawParams.width,
                roundFloat(y));
            drawParams.dev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawKeyboard(GuidoPianoRoll::DrawParams drawParams) const
{
    float keyboardBlackNotesWidth = drawParams.untimedLeftElementWidth / 1.5f;

    /* C notes indication settings */
    int oct;
    ostringstream octaveString;
    std::string cNoteString;
    
    NVstring *font = new NVstring("Arial");
    const VGFont *hTextFont;
	if (font && font->length() > 0)
		hTextFont = FontManager::FindOrCreateFont((int) floor(drawParams.noteHeight * 0.8), font, new NVstring(""));

	drawParams.dev->SetTextFont(hTextFont);
    /******************************/

	drawParams.dev->PushPenWidth(0.8f);

    for (int i = fHighPitch + 1; i >= fLowPitch; i--) {
        int   step = i % 12;
        float y    = pitch2ypos(i, drawParams) + 0.5f * drawParams.noteHeight;
        
        switch (step) {
        case 0:
            if (i != fHighPitch + 1) {
                if (i == 60) { // Tint C4 note in grey
                    drawParams.dev->PushPenColor(VGColor(0, 0, 0));
                    drawParams.dev->PushFillColor(VGColor(200, 200, 200));

                    float xCoords[6] = {
                        0,
                        roundFloat(keyboardBlackNotesWidth),
                        roundFloat(keyboardBlackNotesWidth),
                        roundFloat(drawParams.untimedLeftElementWidth),
                        roundFloat(drawParams.untimedLeftElementWidth),
                        0
                    };

                    float yCoords[6] = {
                        roundFloat(y - drawParams.noteHeight),
                        roundFloat(y - drawParams.noteHeight),
                        roundFloat(y - 1.5f * drawParams.noteHeight),
                        roundFloat(y - 1.5f * drawParams.noteHeight),
                        roundFloat(y),
                        roundFloat(y)
                    };

                    drawParams.dev->Polygon(xCoords, yCoords, 6);

                    drawParams.dev->PopFillColor();
                    drawParams.dev->PopPenColor();
                }

                oct = (i - 12) / 12;
                octaveString.str("");
                octaveString << oct;
                cNoteString = "C" + octaveString.str();

                drawParams.dev->DrawString(
                    drawParams.untimedLeftElementWidth * 0.75f,
                    y - drawParams.noteHeight * 0.25f, cNoteString.c_str(), 2);
            }

            drawParams.dev->Line(
                0,
                roundFloat(y),
                roundFloat(drawParams.untimedLeftElementWidth),
                roundFloat(y));

            break;
        case 5:
            drawParams.dev->Line(
                0,
                roundFloat(y),
                roundFloat(drawParams.untimedLeftElementWidth),
                roundFloat(y));
            break;
        case 2:
        case 4:
        case 7:
        case 9:
        case 11:
            if (i != fLowPitch)
                drawParams.dev->Line(
                    roundFloat(keyboardBlackNotesWidth),
                    roundFloat(y + 0.5f * drawParams.noteHeight),
                    roundFloat(drawParams.untimedLeftElementWidth),
                    roundFloat(y + 0.5f * drawParams.noteHeight));
            
            break;
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
            if (i != fHighPitch + 1)
                drawParams.dev->Rectangle(
                    0,
                    roundFloat(y - drawParams.noteHeight),
                    roundFloat(keyboardBlackNotesWidth),
                    roundFloat(y));

            break;
        }
    }
    
    /**** Right and left vertical lines ***/

    float yMin = pitch2ypos(fLowPitch, drawParams)  + 0.5f * drawParams.noteHeight;
    float yMax = pitch2ypos(fHighPitch, drawParams) - 0.5f * drawParams.noteHeight;
    drawParams.dev->Line(
        0,
        roundFloat(yMin),
        0,
        roundFloat(yMax));

    drawParams.dev->Line(
        roundFloat(drawParams.untimedLeftElementWidth),
        roundFloat(yMin),
        roundFloat(drawParams.untimedLeftElementWidth),
        roundFloat(yMax));
    
    /**************************************/

	drawParams.dev->PopPenWidth();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawVoice(ARMusicalVoice* v, GuidoPianoRoll::DrawParams drawParams)
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

        fColors->pop();
	}
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawMusicalObject(ARMusicalObject *e, TYPE_TIMEPOSITION date, TYPE_DURATION dur, GuidoPianoRoll::DrawParams drawParams)
{
	ARNote *note = dynamic_cast<ARNote *>(e);

	if (note) {
		int pitch = note->midiPitch();

        if (pitch < 0)
            fChordDuration = dur; // prepare for potential chord
        else {
            if (pitch >= fLowPitch && pitch <= fHighPitch) {
                if (note->getName() != ARNoteName::empty)
                    DrawNote(pitch, double(date), double(dur), drawParams);
            }

            fChord = false;
        }
	}
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawNote(int pitch, double date, double dur, GuidoPianoRoll::DrawParams drawParams)
{
	float x = date2xpos (date, drawParams.width, drawParams.untimedLeftElementWidth);
	float y = pitch2ypos(pitch, drawParams);
	DrawRect(x, y, dur, drawParams);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawRect(float x, float y, double dur, GuidoPianoRoll::DrawParams drawParams) const
{
	float w        = duration2width(dur, drawParams.width, drawParams.untimedLeftElementWidth);
	float halfstep = stepheight(drawParams.height) / 2.0f;

	if (!halfstep)
        halfstep = 1;

    drawParams.dev->Rectangle(
        roundFloat(x),
        roundFloat(y - halfstep),
        roundFloat(x + (w ? w : 1)),
        roundFloat(y + halfstep));
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawMeasureBar(double date, GuidoPianoRoll::DrawParams drawParams) const
{
    float x    = date2xpos(date, drawParams.width, drawParams.untimedLeftElementWidth);
	float yMin = pitch2ypos(fLowPitch, drawParams)  + 0.5f * drawParams.noteHeight;
	float yMax = pitch2ypos(fHighPitch, drawParams) - 0.5f * drawParams.noteHeight;
    
    drawParams.dev->PushPenColor(VGColor(0, 0, 0));
    drawParams.dev->PushFillColor(VGColor(0, 0, 0));

    drawParams.dev->PushPenWidth(0.3f);
    drawParams.dev->Line(
        roundFloat(x),
        roundFloat(yMin),
        roundFloat(x),
        roundFloat(yMax));
    drawParams.dev->PopPenWidth();

    drawParams.dev->PopFillColor();
    drawParams.dev->PopPenColor();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::handleColor(ARNoteFormat* noteFormat, DrawParams drawParams) const
{
    const TagParameterString *tps = noteFormat->getColor();
    unsigned char colref[4];

    if (tps && tps->getRGB(colref)) {
        fColors->push(VGColor(colref[0], colref[1], colref[2], colref[3]));

        drawParams.dev->PushFillColor(fColors->top());
    }
    else {
        fColors->pop();

        drawParams.dev->PopFillColor();
    }
}

//--------------------------------------------------------------------------
/*void GuidoPianoRoll::handleEmpty(double date)
{
    // REM: TODO ?
}*/

//--------------------------------------------------------------------------
float GuidoPianoRoll::pitch2ypos(int midipitch, DrawParams drawParams) const
{
	int   p = midipitch - fLowPitch;
    float h = ((float) (drawParams.height * p) / (float) pitchRange());

	return drawParams.height - (float) drawParams.noteHeight * 0.5f - h;
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::date2xpos(double pos, int width, float untimedLeftElementWidth) const
{
    float  fTimedWidth = width - untimedLeftElementWidth;
    double posDiff     = pos - double(fStartDate);

    return (float) (fTimedWidth * posDiff / fDuration + untimedLeftElementWidth);
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::duration2width(double dur, int width, float untimedLeftElementWidth) const
{
    float  fTimedWidth = width - untimedLeftElementWidth;

    return (float) (fTimedWidth * dur / fDuration);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::HSVtoRGB(float h, float s, float v, int &r, int &g, int &b) const
{
    int   i = (int) floor((float) h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    float rTmp, gTmp, bTmp;

    switch (i % 6)
    {
        case 0:
            rTmp = v;
            gTmp = t;
            bTmp = p;
            break;
        case 1:
            rTmp = q;
            gTmp = v;
            bTmp = p;
            break;
        case 2:
            rTmp = p;
            gTmp = v;
            bTmp = t;
            break;
        case 3:
            rTmp = p;
            gTmp = q;
            bTmp = v;
            break;
        case 4:
            rTmp = t;
            gTmp = p;
            bTmp = v;
            break;
        case 5:
            rTmp = v;
            gTmp = p;
            bTmp = q;
            break;
    }

    r = (int) roundFloat((float) rTmp * 256);
    g = (int) roundFloat((float) gTmp * 256);
    b = (int) roundFloat((float) bTmp * 256);
}

//--------------------------------------------------------------------------
int GuidoPianoRoll::detectARExtremePitch(bool detectLowerPitch)
{
    bool containsNote = false;
    int  extremePitch;

    if (detectLowerPitch)
        extremePitch = 127;
    else
        extremePitch = 0;

    GuidoPos pos = fARMusic->GetHeadPosition();

    while(pos) {
        ARMusicalVoice *voice = fARMusic->GetNext(pos);

        ObjectList *ol  = (ObjectList *)voice;
        GuidoPos    pos = ol->GetHeadPosition();

        while (pos)
        {
            ARMusicalObject *musicalObject = ol->GetNext(pos);

            ARNote *note = dynamic_cast<ARNote *>(musicalObject);

            if (note) {
                int pitch = note->midiPitch();

                if (detectLowerPitch) {
                    if (pitch >= 0 && note->getOctave() >= -4 && pitch < extremePitch) {
                        extremePitch = pitch;
                        containsNote = true;
                    }
                }
                else {
                    if (pitch > extremePitch) {
                        extremePitch = pitch;
                        containsNote = true;
                    }
                }
            }
        }
    }

    if (detectLowerPitch) {
        if (containsNote)
        return extremePitch;
    else
        return kDefaultLowPitch;
    }
    else {
        if (containsNote)
        return extremePitch;
    else
        return kDefaultHighPitch;
    }    
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::autoAdjustPitchRange(int &lowerPitch, int &higherPitch)
{
    int difference = 11 - (higherPitch - lowerPitch);

    if (difference % 2 == 0) {
        lowerPitch  -= difference / 2;
        higherPitch += difference / 2;
    }
    else {
        lowerPitch  -= difference / 2;
        higherPitch += difference / 2 + 1;
    }
}

#ifdef MIDIEXPORT
//--------------------------------------------------------------------------
// MIDI Piano roll
//--------------------------------------------------------------------------

static MidiSeqPtr KeyOnOff2Note(MidiSeqPtr seq, MidiLight* midi)
{
	if (!seq)
        return 0;

	map<int, MidiEvPtr> pitchMap;
	MidiEvPtr ev = FirstEv(seq);
	MidiSeqPtr outseq = midi->NewSeq();

	while (ev) {
		switch (EvType(ev)) {
			case typeKeyOn:
				pitchMap[Pitch(ev)] = ev;
				break;
            case typeKeyOff: {
                    MidiEvPtr note = midi->CopyEv(pitchMap[Pitch(ev)]);
                    pitchMap[Pitch(ev)] = 0;

                    if (note) {
                        EvType(note) = typeNote;
                        Dur(note)    = Date(ev) - Date(note);
                        midi->AddSeq(outseq, note);
                    }
                    else
                        cerr << "KeyOnOff2Note: key off " << int(Pitch(ev)) << " without matching key on at date " << Date(ev) << endl;
                }

				break;
			default:
				midi->AddSeq(outseq, midi->CopyEv(ev));
		}

		ev = Link(ev);
	}

	midi->FreeSeq(seq);

	return outseq;
}

//--------------------------------------------------------------------------
TYPE_TIMEPOSITION GuidoPianoRoll::getMidiEndDate() const
{
    MIDIFile mf;

    mf.Open(fMidiFileName, MidiFileRead);

    int n    = mf.infos().ntrks; /* get the number of tracks */
    int tpqn = mf.infos().time;
    vector<MidiSeqPtr> vseq;
    long maxTime = 0;

    while (n--) {
        MidiSeqPtr seq = KeyOnOff2Note(mf.ReadTrack(), mf.midi()); /* read every track */

        if (seq) {
            vseq.push_back(seq);
            long t = Date(LastEv(seq));

            if (t > maxTime)
                maxTime = t;
        }
    }

    return TYPE_TIMEPOSITION (int(double(maxTime) / (tpqn * 4) * 256), 256);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawMidiSeq(MidiSeqPtr seq, int tpqn, GuidoPianoRoll::DrawParams drawParams)
{
	MidiEvPtr ev = FirstEv(seq);
	int tpwn     = tpqn * 4;
	double start = double(fStartDate);
	double end   = double(fEndDate);

	while (ev) {
		if (EvType(ev) == typeNote) {
			double date = double(Date(ev)) / tpwn;
			double dur  = double(Dur(ev))  / tpwn;

			if (date >= start) {
                if (date < end) {
                    double remain = end - date;
                    DrawNote(Pitch(ev), date, (dur > remain ? remain : dur), drawParams);
                }
			}
			else if ((date + dur) > start) {
				dur -= (start - date);
				double remain = end - start;
				DrawNote(Pitch(ev), start, (dur > remain ? remain : dur), drawParams);
			}
		}

		ev = Link(ev);
	}
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawFromMidi(GuidoPianoRoll::DrawParams drawParams)
{
    MIDIFile mf;

    mf.Open(fMidiFileName, MidiFileRead);
    
    int n    = mf.infos().ntrks; /* get the number of tracks */
    vector<MidiSeqPtr> vseq;
    int tpqn = mf.infos().time;
    long maxTime = 0;

    while (n--) {
        MidiSeqPtr seq = KeyOnOff2Note(mf.ReadTrack(), mf.midi()); /* read every track */

        if (seq) {
            vseq.push_back(seq);
            long t = Date(LastEv(seq));

            if (t > maxTime)
                maxTime = t;
        }
    }

    for (unsigned int i = 0; i < vseq.size(); i++) {
        DrawMidiSeq(vseq[i], tpqn, drawParams);
        mf.midi()->FreeSeq(vseq[i]);
    }

    mf.Close();
}

//--------------------------------------------------------------------------
int GuidoPianoRoll::detectMidiExtremePitch(bool detectLowerPitch)
{
    bool containsNote = false;
    int  extremePitch;

    if (detectLowerPitch)
        extremePitch = 127;
    else
        extremePitch = 0;

    MIDIFile mf;

    mf.Open(fMidiFileName, MidiFileRead);

    int n    = mf.infos().ntrks; /* get the number of tracks */
    int tpqn = mf.infos().time;
    vector<MidiSeqPtr> vseq;
    long maxTime = 0;

    while (n--) {
        MidiSeqPtr seq = KeyOnOff2Note(mf.ReadTrack(), mf.midi()); /* read every track */

        if (seq) {
            vseq.push_back(seq);
            long t = Date(LastEv(seq));

            if (t > maxTime)
                maxTime = t;
        }
    }

    for (unsigned int i = 0; i < vseq.size(); i++) {
        MidiSeqPtr seq = vseq[i];

        MidiEvPtr ev = FirstEv(seq);

        while (ev) {
            if (EvType(ev) == typeNote) {
                int pitch = Pitch(ev);

                if (detectLowerPitch) {
                    if (pitch >= 0 && pitch < extremePitch) {
                        extremePitch = pitch;
                        containsNote = true;
                    }
                }
                else {
                    if (pitch > extremePitch) {
                        extremePitch = pitch;
                        containsNote = true;
                    }
                }
            }

            ev = Link(ev);
        }

        mf.midi()->FreeSeq(vseq[i]);
    }

    mf.Close();

    if (detectLowerPitch) {
        if (containsNote)
        return extremePitch;
    else
        return kDefaultLowPitch;
    }
    else {
        if (containsNote)
        return extremePitch;
    else
        return kDefaultHighPitch;
    }  
}

#endif