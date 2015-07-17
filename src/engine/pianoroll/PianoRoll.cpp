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
#include "ARNoteName.h"
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

#ifdef MIDIEXPORT
    #include "midifile.h"
#endif

#include "PianoRoll.h"
#include "GUIDOPianoRoll.h"

using namespace std;

#define kMainLineWidth    1.6f
#define kSubMainLineWidth 1.1f
#define kNormalLineWidth  0.6f

#define kLimitDist34Mode  6                       // the minimum distance between lines of the grid
                                                  //     (to switch between mode 4 and mode 3 of pitch line display)
#define kLimitDist23Mode  10                      // the medium distance between lines of the grid
                                                  //     (to switch between mode 3 and mode 2 of pitch line display)
#define kLimitDist12      14                      // the minimum distance between lines of the grid
                                                  //     (to switch between mode 2 and mode 1 of pitch line display)

//--------------------------------------------------------------------------
PianoRoll::PianoRoll(ARMusic *arMusic) :
	fARMusic(arMusic), fMidiFileName(NULL),
	fVoicesAutoColored(false),
	fKeyboardEnabled(false), fMeasureBarsEnabled(false),
	fPitchLinesDisplayMode(kAutoLines)
{
	init();
}

//--------------------------------------------------------------------------
PianoRoll::PianoRoll(const char *midiFileName) :
	fARMusic(NULL), fMidiFileName(midiFileName),
	fVoicesAutoColored(false),
	fKeyboardEnabled(false), fMeasureBarsEnabled(false),
	fPitchLinesDisplayMode(kAutoLines)
{
	init();
}

//--------------------------------------------------------------------------
void PianoRoll::initAutoVoiceColors()
{
	const int alpha = 255;
	unsigned char c[4];
	if (HtmlColor::get("Gray", c))			fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Blue", c))			fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Crimson", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("ForestGreen", c))	fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("DarkMagenta", c))	fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("GoldenRod", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Brown", c))			fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Orange", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Black", c))			fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("LimeGreen", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Magenta", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("BurlyWood", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("DarkCyan", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("DeepSkyBlue", c))	fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Gold", c))			fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("MediumSlateBlue",c)) fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("GreenYellow", c))	fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Silver", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("Salmon", c))		fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
	if (HtmlColor::get("MediumAquaMarine",c)) fAutoVoicesColors.push_back (VGColor(c[0], c[1], c[2], alpha));
}

//--------------------------------------------------------------------------
void PianoRoll::init()
{
	GuidoDate defaultStartDate = {kDefaultStartDateNum, kDefaultStartDateDenom};
	GuidoDate defaultEndDate   = {kDefaultEndDateNum, kDefaultEndDateDenom};

	setLimitDates(defaultStartDate, defaultEndDate);
	setPitchRange(kDefaultLowPitch, kDefaultHighPitch);
	fNoteColor = 0;
	initAutoVoiceColors ();
}

//--------------------------------------------------------------------------
void PianoRoll::setLimitDates(GuidoDate start, GuidoDate end)
{
    if (start.num == 0 && start.denom == 0)
        fStartDate = TYPE_TIMEPOSITION(kDefaultStartDateNum, kDefaultStartDateDenom);
    else
	    fStartDate = TYPE_TIMEPOSITION(start.num, start.denom);

    if (end.num == 0 && end.denom == 0)
        fEndDate = (fARMusic ? fARMusic->getDuration() : getMidiEndDate());
    else
	    fEndDate = TYPE_TIMEPOSITION(end.num, end.denom);

    fDuration = double(fEndDate - fStartDate);
}

//--------------------------------------------------------------------------
void PianoRoll::setPitchRange(int minPitch, int maxPitch)
{
    if (minPitch == -1)
        fLowPitch = (fARMusic ? detectARExtremePitch(true) : detectMidiExtremePitch(true));
    else
        fLowPitch = minPitch;

    if (maxPitch == -1)
        fHighPitch = (fARMusic ? detectARExtremePitch(false) : maxPitch = detectMidiExtremePitch(false));
    else
        fHighPitch = maxPitch;

    if (fHighPitch - fLowPitch < 11)
        autoAdjustPitchRange(fLowPitch, fHighPitch);
}

//--------------------------------------------------------------------------
float PianoRoll::getKeyboardWidth(int height) const
{
    int currentHeight = (height == -1 ? kDefaultHeight : height);

    float noteHeight              = computeNoteHeight(currentHeight);
    float untimedLeftElementWidth = computeKeyboardWidth(noteHeight);

    return untimedLeftElementWidth;
}

void PianoRoll::setPitchLinesDisplayMode(int mode)
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
void PianoRoll::setColorToVoice(int voiceNum, int r, int g, int b, int a)
{
	fVoicesColors[voiceNum] = VGColor(r, g, b, a);
}

//--------------------------------------------------------------------------
bool PianoRoll::removeColorToVoice(int voiceNum)
{
	map<int, VGColor>::iterator it = fVoicesColors.find(voiceNum);
	if(it != fVoicesColors.end()) {
		fVoicesColors.erase(it);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
void PianoRoll::getMap(int width, int height, Time2GraphicMap &outmap) const
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
PianoRoll::DrawParams PianoRoll::createDrawParamsStructure(int width, int height, VGDevice *dev) const
{
    int currentWidth  = (width  == -1 ? kDefaultWidth  : width);
    int currentHeight = (height == -1 ? kDefaultHeight : height);
    
    float noteHeight              = computeNoteHeight(currentHeight);
    float untimedLeftElementWidth = computeKeyboardWidth(noteHeight);

    PianoRoll::DrawParams drawParams(currentWidth, currentHeight, noteHeight, untimedLeftElementWidth, dev);

    return drawParams;
}

//--------------------------------------------------------------------------
void PianoRoll::onDraw(int width, int height, VGDevice *dev)
{
    DrawParams drawParams = createDrawParamsStructure(width, height, dev);
    initRendering(drawParams);
    DrawGrid(drawParams);

    if (fKeyboardEnabled)
        DrawKeyboard(drawParams);

    if (fARMusic)
        DrawFromAR(drawParams);
#ifdef MIDIEXPORT
	else if (fMidiFileName)
        DrawFromMidi(drawParams);
#endif
    
	endRendering(drawParams);
}

//--------------------------------------------------------------------------
// gives the color of a voice identifioed by index
//
bool PianoRoll::getVoiceColor  (unsigned int index, VGColor& color) const
{
	if (fVoicesAutoColored) {
		// when the auto colored flag is on, returns the predefined colors
		// and ignores the user settings
		color = fAutoVoicesColors[index % fAutoVoicesColors.size()];
		return true;
	}
	
	// look for user colors settings
	map<int, VGColor>::const_iterator i = fVoicesColors.find(index);
	if (i != fVoicesColors.end()) {
		color = i->second;
		return true;
	}

	// at this step, no color is indicated for the current voice
	return false;
}

//--------------------------------------------------------------------------
void PianoRoll::DrawFromAR(const DrawParams& drawParams)
{
    GuidoPos pos = fARMusic->GetHeadPosition();
	unsigned int i = 0;

    while(pos) {
        ARMusicalVoice *e = fARMusic->GetNext(pos);
  		VGColor color;
		bool colored = getVoiceColor (i++, color);
		if (colored) drawParams.dev->PushFillColor(color);
		DrawVoice(e, drawParams);
		if (colored) drawParams.dev->PopFillColor();
    }
}

//--------------------------------------------------------------------------
float PianoRoll::computeKeyboardWidth(float noteHeight) const
{
    float untimedLeftElementWidth = 0;

    if (fKeyboardEnabled)
        untimedLeftElementWidth = 6.0f * noteHeight;

    return untimedLeftElementWidth;
}

//--------------------------------------------------------------------------
float PianoRoll::computeNoteHeight(int height) const
{
    float noteHeight = (float) height / (float) pitchRange();

    if (!noteHeight)
        noteHeight = 1;

    return noteHeight;
}

//--------------------------------------------------------------------------
void PianoRoll::initRendering(PianoRoll::DrawParams &drawParams) const
{
	drawParams.dev->NotifySize(drawParams.width, drawParams.height);
	drawParams.dev->BeginDraw();
}

//--------------------------------------------------------------------------
void PianoRoll::endRendering(PianoRoll::DrawParams &drawParams) const
{
    drawParams.dev->EndDraw();
}

//--------------------------------------------------------------------------
void PianoRoll::DrawGrid(PianoRoll::DrawParams &drawParams) const
{
    if (fPitchLinesDisplayMode == kAutoLines) {
        /*if (drawParams.noteHeight < kLimitDist34Mode)
            DrawOctavesGrid(drawParams);
        else if (drawParams.noteHeight < kLimitDist23Mode)
            DrawTwoLinesGrid(drawParams);
        else if (drawParams.noteHeight < kLimitDist12)
            DrawDiatonicGrid(drawParams);
        else*/
            DrawChromaticGrid(drawParams);
    }
    else
        DrawChromaticGrid(drawParams, true);
}

//--------------------------------------------------------------------------
void PianoRoll::DrawOctavesGrid(PianoRoll::DrawParams &drawParams) const
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
void PianoRoll::DrawTwoLinesGrid(PianoRoll::DrawParams &drawParams) const
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
void PianoRoll::DrawDiatonicGrid(PianoRoll::DrawParams &drawParams) const
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
void PianoRoll::DrawChromaticGrid(PianoRoll::DrawParams &drawParams, bool isUserDefined) const
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
void PianoRoll::DrawKeyboard(PianoRoll::DrawParams &drawParams) const
{
    float keyboardBlackNotesWidth = drawParams.untimedLeftElementWidth / 1.5f;

    /* C notes indication settings */
    int oct;
    ostringstream octaveString;
    std::string cNoteString;
    
    NVstring font("Arial");
    const VGFont *hTextFont = 0;
	if (font.length() > 0)
		hTextFont = FontManager::FindOrCreateFont((int) floor(drawParams.noteHeight * 0.8), &font, new NVstring(""));

	drawParams.dev->SetTextFont(hTextFont);
	drawParams.dev->PushPenWidth(0.8f);

    for (int i = fHighPitch + 1; i >= fLowPitch; i--) {
        int   step = i % 12;
        float y    = pitch2ypos(i, drawParams) + 0.5f * drawParams.noteHeight;
        
        switch (step) {
        case 0:
            if (i != fHighPitch + 1) {
				// Octave with guido octave number
				oct = (i - 12 * 4) / 12;
                octaveString.str("");
                octaveString << oct;
                cNoteString = "C" + octaveString.str();

                drawParams.dev->DrawString(
                    drawParams.untimedLeftElementWidth * 0.75f,
                    y - drawParams.noteHeight * 0.25f, cNoteString.c_str(), 2);
            }

            drawParams.dev->Line( 0,
                roundFloat(y),
                roundFloat(drawParams.untimedLeftElementWidth),
                roundFloat(y));

            break;
        case 5:
            drawParams.dev->Line( 0,
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
                drawParams.dev->Line(roundFloat(keyboardBlackNotesWidth), roundFloat(y + 0.5f * drawParams.noteHeight),
					roundFloat(drawParams.untimedLeftElementWidth),
                    roundFloat(y + 0.5f * drawParams.noteHeight));
            
            break;
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
            if (i != fHighPitch + 1)
                drawParams.dev->Rectangle( 0,
                    roundFloat(y - drawParams.noteHeight),
                    roundFloat(keyboardBlackNotesWidth),
                    roundFloat(y));

            break;
        }
    }
    
    /**** Right and left vertical lines ***/
    float yMin = pitch2ypos(fLowPitch, drawParams)  + 0.5f * drawParams.noteHeight;
    float yMax = pitch2ypos(fHighPitch, drawParams) - 0.5f * drawParams.noteHeight;
    drawParams.dev->Line( 0, roundFloat(yMin), 0, roundFloat(yMax));
    drawParams.dev->Line( roundFloat(drawParams.untimedLeftElementWidth), roundFloat(yMin), roundFloat(drawParams.untimedLeftElementWidth), roundFloat(yMax));

	drawParams.dev->PopPenWidth();
}

//--------------------------------------------------------------------------
void PianoRoll::DrawVoice(ARMusicalVoice* v, const DrawParams& drawParams)
{
    fChord          = false;
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
            if (end > fEndDate)		dur = fEndDate - date;
			else					dur = end - date;
            DrawMusicalObject(e, date, dur, drawParams);
		}

        if (dynamic_cast<ARRest *>(e->isARRest()))
			fChord = false;
        else if (dynamic_cast<ARChordComma *>(e->isARChordComma()))
			fChord = true;
        else if (dynamic_cast<ARNoteFormat *>(e->isARNoteFormat()))
            handleColor(dynamic_cast<ARNoteFormat *>(e->isARNoteFormat()), drawParams);
        else if (dynamic_cast<ARBar *>(e->isARBar()) && fMeasureBarsEnabled)
            DrawMeasureBar(date, drawParams);
	}

	while (fNoteColor) {		// check for possible color from noteFormat tag
		drawParams.dev->PopFillColor();
		fNoteColor--;
	}
}

//--------------------------------------------------------------------------
void PianoRoll::DrawMusicalObject(ARMusicalObject *e, TYPE_TIMEPOSITION date, TYPE_DURATION dur, const DrawParams& drawParams)
{
    ARNote *note = static_cast<ARNote *>(e->isARNote());

	if (note) {
		int pitch = note->getMidiPitch();

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
void PianoRoll::DrawNote(int pitch, double date, double dur, const DrawParams& drawParams) const
{
	float x = date2xpos (date, drawParams.width, drawParams.untimedLeftElementWidth);
	float y = pitch2ypos(pitch, drawParams);
	DrawRect(x, y, dur, drawParams);
}

//--------------------------------------------------------------------------
void PianoRoll::DrawRect(float x, float y, double dur, const DrawParams& drawParams) const
{
	float w        = duration2width(dur, drawParams.width, drawParams.untimedLeftElementWidth);
	float halfstep = stepheight(drawParams.height) / 2.0f;
	if (!halfstep) halfstep = 1;

    drawParams.dev->Rectangle(roundFloat(x), roundFloat(y - halfstep), roundFloat(x + (w ? w : 1)), roundFloat(y + halfstep));
}

//--------------------------------------------------------------------------
void PianoRoll::DrawMeasureBar(double date,  const DrawParams& drawParams) const
{
    float x    = date2xpos(date, drawParams.width, drawParams.untimedLeftElementWidth);
	float yMin = pitch2ypos(fLowPitch, drawParams)  + 0.5f * drawParams.noteHeight;
	float yMax = pitch2ypos(fHighPitch, drawParams) - 0.5f * drawParams.noteHeight;
    
    drawParams.dev->PushPenWidth(0.3f);
    drawParams.dev->Line (roundFloat(x), roundFloat(yMin), roundFloat(x), roundFloat(yMax));
    drawParams.dev->PopPenWidth();
}

//--------------------------------------------------------------------------
void PianoRoll::handleColor(ARNoteFormat* noteFormat, const DrawParams& drawParams)
{
    const TagParameterString   *tps = noteFormat->getColor();
    const TagParameterRGBColor *tpc = noteFormat->getRGBColor();
    unsigned char colref[4];
	bool endRange = !noteFormat->getPosition() && noteFormat->getIsAuto();


	if ((tps && tps->getRGB(colref)) || (tpc && tpc->getRGBColor(colref))) {
        if (endRange && fNoteColor ) {				// Means it's a range noteFormat end tag and a color is already set
			drawParams.dev->PopFillColor();
			fNoteColor--;
		}
		VGColor color(colref[0], colref[1], colref[2], colref[3]);
		drawParams.dev->PushFillColor(color);
		fNoteColor++;
    }
    else if (endRange && fNoteColor) {			// noteFormat end tag and a color is already set
		drawParams.dev->PopFillColor();
		fNoteColor--;
	}
}

//--------------------------------------------------------------------------
float PianoRoll::pitch2ypos(int midipitch, const DrawParams& drawParams) const
{
	int   p = midipitch - fLowPitch;
    float h = ((float) (drawParams.height * p) / (float) pitchRange());

	return drawParams.height - (float) drawParams.noteHeight * 0.5f - h;
}

//--------------------------------------------------------------------------
float PianoRoll::date2xpos(double pos, int width, float untimedLeftElementWidth) const
{
    float  fTimedWidth = width - untimedLeftElementWidth;
    double posDiff     = pos - double(fStartDate);

    return (float) (fTimedWidth * posDiff / fDuration + untimedLeftElementWidth);
}

//--------------------------------------------------------------------------
float PianoRoll::duration2width(double dur, int width, float untimedLeftElementWidth) const
{
    float  fTimedWidth = width - untimedLeftElementWidth;

    return (float) (fTimedWidth * dur / fDuration);
}

//--------------------------------------------------------------------------
int PianoRoll::detectARExtremePitch(bool detectLowerPitch)
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

            ARNote *note = static_cast<ARNote *>(musicalObject->isARNote());

            if (note) {
                int pitch = note->getMidiPitch();

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
void PianoRoll::autoAdjustPitchRange(int &lowerPitch, int &higherPitch)
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
TYPE_TIMEPOSITION PianoRoll::getMidiEndDate() const
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
void PianoRoll::DrawMidiSeq(MidiSeqPtr seq, int tpqn, const DrawParams& drawParams) const
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
void PianoRoll::DrawFromMidi(const DrawParams& drawParams) const
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
		VGColor color;
		bool colored = getVoiceColor (i, color);
		if (colored) drawParams.dev->PushFillColor(color);
        DrawMidiSeq(vseq[i], tpqn, drawParams);
		if (colored) drawParams.dev->PopFillColor();
        mf.midi()->FreeSeq(vseq[i]);
    }

    mf.Close();
}

//--------------------------------------------------------------------------
int PianoRoll::detectMidiExtremePitch(bool detectLowerPitch)
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
