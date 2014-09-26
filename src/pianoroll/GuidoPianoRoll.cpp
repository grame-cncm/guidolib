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

#define kDefaultWidth     1024                    // the default canvas width
#define kDefaultHeight    400                     // the default canvas height
#define kDefaultLowPitch  0                       // the default canvas width
#define kDefaultHighPitch 127                     // the default canvas height
#define kDefaultStartDate TYPE_TIMEPOSITION(0, 1) // the default start date
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
GuidoPianoRoll::GuidoPianoRoll() :
    fWidth(kDefaultWidth), fHeight(kDefaultHeight),
    fLowPitch(kDefaultLowPitch), fHighPitch(kDefaultHighPitch),
    fKeyboardEnabled(false),
    fVoicesAutoColored(false), fVoicesColors(NULL),
    fMeasureBarsEnabled(false), fPitchLinesDisplayMode(-1),
    fARMusic(NULL), fDev(NULL), fIsEndDateSet(false)
{
    fColors = new std::stack<VGColor>();
    
    computeNoteHeight();
}

//--------------------------------------------------------------------------
GuidoPianoRoll::~GuidoPianoRoll() 
{
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setARMusic(ARMusic *arMusic) 
{
    fARMusic = arMusic;
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setMidiFile(const char *midiFileName)
{
    fMidiFileName = midiFileName;
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setCanvasDimensions(int width, int height)
{
    if (width == -1)
        fWidth = kDefaultWidth;
    else
        fWidth = width;

    if (height == -1)
        fHeight = kDefaultHeight;
    else
        fHeight = height;
    
    computeNoteHeight();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setLimitDates(GuidoDate start, GuidoDate end)
{
    if (start.num == 0 && start.denom == 0)
        fStartDate = kDefaultStartDate;
    else
	    fStartDate = TYPE_TIMEPOSITION(start.num, start.denom);

    if (end.num == 0 && end.denom == 0)
        fIsEndDateSet = false;
    else {
        fIsEndDateSet = true;
	    fEndDate = TYPE_TIMEPOSITION(end.num, end.denom);
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setPitchRange(int minPitch, int maxPitch)
{
    if (minPitch == -1)
        fLowPitch = kDefaultLowPitch;
    else
        fLowPitch = minPitch;

    if (maxPitch == -1)
        fHighPitch = kDefaultHighPitch;
    else
        fHighPitch = maxPitch;
    
    computeNoteHeight();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::enableKeyboard(bool enabled)
{
    fKeyboardEnabled = enabled;

    computeKeyboardWidth();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::setColorToVoice(int voiceNum, int r, int g, int b, int a)
{
    if (!fVoicesColors)
        fVoicesColors = new std::vector<std::pair<int, VGColor>>();
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
void GuidoPianoRoll::getRenderingFromAR(VGDevice *dev)
{
    fDev = dev;

    initRendering();

    if (!fIsEndDateSet)
        fEndDate = fARMusic->getDuration();

    fDuration = double(fEndDate - fStartDate);
    
    if (fKeyboardEnabled)
        DrawKeyboard();

    DrawGrid();

    GuidoPos pos = fARMusic->GetHeadPosition();

    while(pos) {
        ARMusicalVoice *e = fARMusic->GetNext(pos);
        DrawVoice(e);
    }

    endRendering();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::getRenderingFromMidi(VGDevice *dev)
{
    fDev = dev;

    initRendering();

    DrawFromMidi();

    endRendering();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::computeNoteHeight()
{
    fNoteHeight = (float) fHeight / (float) pitchRange();

    if (!fNoteHeight)
        fNoteHeight = 1;

    computeKeyboardWidth();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::computeKeyboardWidth()
{
    fUntimedLeftElementWidth = 0;

    if (fKeyboardEnabled)
        fUntimedLeftElementWidth = 6.0f * fNoteHeight;
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::initRendering()
{
    fWidth = fWidth + (int) floor(fUntimedLeftElementWidth);

    fDev->NotifySize(fWidth, fHeight);
    fDev->BeginDraw();
    fDev->PushPenColor(VGColor(100, 100, 100));
    fDev->PushFillColor(VGColor(0, 0, 0));

    if (fVoicesAutoColored) {
        fColorSeed = 0.5;
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::endRendering()
{
    fDev->PopFillColor();
    fDev->PopPenColor();
    fDev->EndDraw();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawGrid() const
{
    fDev->PushPenColor(VGColor(0, 0, 0));

    switch (fPitchLinesDisplayMode) {
    case -1:
        if (fNoteHeight < kLimitDist34Mode)
            DrawOctavesGrid();
        else if (fNoteHeight < kLimitDist23Mode)
            DrawTwoLinesGrid();
        else if (fNoteHeight < kLimitDist12)
            DrawChromaticGrid();
        else
            DrawDiatonicGrid();

        break;
    case 1:
        DrawOctavesGrid();
        break;
    case 2:
        DrawTwoLinesGrid();
        break;
    case 3:
        DrawChromaticGrid();
        break;
    case 4:
        DrawDiatonicGrid();
        break;
    }

    fDev->PopPenColor();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawOctavesGrid() const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i) + 0.5f * fNoteHeight;
		int step = i % 12; // the note in chromatic step

        if (step == 0) { // C notes are highlighted
            fDev->PushPenWidth((i == 60) ? kSubMainLineWidth : kNormalLineWidth);
            fDev->Line(fUntimedLeftElementWidth, y, (float) fWidth, y);
            fDev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawTwoLinesGrid() const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i) + 0.5f * fNoteHeight;
		int step = i % 12; // the note in chromatic step

        if (step == 0 || step == 7) {
            float width = kNormalLineWidth;

            if (i == 60)
                width = kMainLineWidth;
            else if (step == 0)
                width = kSubMainLineWidth;

            fDev->PushPenWidth(width);
            fDev->Line(fUntimedLeftElementWidth, y, (float) fWidth, y);
            fDev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawChromaticGrid() const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i) + 0.5f * fNoteHeight;
		int step = i % 12; // the note in chromatic step

        if (step == 0 || step == 2 || step == 4
            || step == 5 || step == 7 || step == 9
            || step == 11) {
                float width = kNormalLineWidth;

                if (i == 60)
                    width = kMainLineWidth;
                else if (step == 0)
                    width = kSubMainLineWidth;

                fDev->PushPenWidth(width);
                fDev->Line(fUntimedLeftElementWidth, y, (float) fWidth, y);
                fDev->PopPenWidth();
        }
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawDiatonicGrid() const
{
	for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        float y = pitch2ypos(i) + 0.5f * fNoteHeight;
		int step = i % 12; // the note in chromatic step
        
        float width = kNormalLineWidth;

        if (i == 60)
             width = kMainLineWidth;
        else if (step == 0)
             width = kSubMainLineWidth;

        fDev->PushPenWidth(width);
        fDev->Line(fUntimedLeftElementWidth, y, (float) fWidth, y);
        fDev->PopPenWidth();
    }
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawKeyboard() const
{
    float keyboardBlackNotesWidth = fUntimedLeftElementWidth / 1.5f;

    /* C notes indication settings */
    int oct;
    ostringstream octaveString;
    std::string cNoteString;
    
    NVstring *font = new NVstring("Arial");
    const VGFont *hTextFont;
	if (font && font->length() > 0)
		hTextFont = FontManager::FindOrCreateFont((int) floor(fNoteHeight * 0.8), font, new NVstring(""));

	fDev->SetTextFont(hTextFont);
    /******************************/

	fDev->PushPenWidth(0.8f);

    for (int i = fLowPitch; i <= fHighPitch + 1; i++) {
        int   step = i % 12;
        float y    = pitch2ypos(i) + 0.5f * fNoteHeight;
        
        switch (step) {
        case 0:
            if (i != fHighPitch + 1) {
                if (i == 60) { // Tint C4 note in grey
                    fDev->PushPenColor(VGColor(0, 0, 0));
                    fDev->PushFillColor(VGColor(200, 200, 200));

                    float xCoords[6] = {
                        0,
                        keyboardBlackNotesWidth,
                        keyboardBlackNotesWidth,
                        fUntimedLeftElementWidth,
                        fUntimedLeftElementWidth,
                        0
                    };

                    float yCoords[6] = {
                        y - fNoteHeight,
                        y - fNoteHeight,
                        y - 1.5f * fNoteHeight,
                        y - 1.5f * fNoteHeight,
                        y,
                        y
                    };

                    fDev->Polygon(xCoords, yCoords, 6);

                    fDev->PopFillColor();
                    fDev->PopPenColor();
                }

                oct = (i - 12) / 12;
                octaveString.str("");
                octaveString << oct;
                cNoteString = "C" + octaveString.str();

                fDev->DrawString(fUntimedLeftElementWidth * 0.75f, y - fNoteHeight * 0.25f, cNoteString.c_str(), 2);
            }

            fDev->Line(0, y, fUntimedLeftElementWidth, y);

            break;
        case 5:
            fDev->Line(0, y, fUntimedLeftElementWidth, y);
            break;
        case 2:
        case 4:
        case 7:
        case 9:
        case 11:
            if (i != fLowPitch)
                fDev->Line(keyboardBlackNotesWidth, y + 0.5f * fNoteHeight, fUntimedLeftElementWidth, y + 0.5f * fNoteHeight);
            
            break;
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
            if (i != fHighPitch + 1)
                fDev->Rectangle(0, y - fNoteHeight, keyboardBlackNotesWidth, y);

            break;
        }
    }
    
    float yMin = pitch2ypos(fLowPitch)  + 0.5f * fNoteHeight;
    float yMax = pitch2ypos(fHighPitch) - 0.5f * fNoteHeight;
    fDev->Line(fUntimedLeftElementWidth, yMin, fUntimedLeftElementWidth, yMax);

	fDev->PopPenWidth();
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawVoice(ARMusicalVoice* v)
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

            fColorSeed += kGoldenRatio;
            fColorSeed  = fmod(fColorSeed, 1);

            HSVtoRGB((float) fColorSeed, 0.5f, 0.9f, r, g, b);

            fColors->push(VGColor(r, g, b, 255));
        }
        
        fDev->PushFillColor(fColors->top());
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

                DrawMusicalObject(e, date, dur);
            }
		}
		else if (end > fStartDate) { // to make the note end appear
			date = fStartDate;	
			
            if (end > fEndDate)
                dur = fEndDate - date;
			else
                dur = end - date;
			
            DrawMusicalObject(e, date, dur);
		}

		if (dynamic_cast<ARRest *>(e))
			fChord = false;
		else if (dynamic_cast<ARChordComma *>(e))
			fChord = true;
		else if (dynamic_cast<ARNoteFormat *>(e))
            handleColor(dynamic_cast<ARNoteFormat *>(e));
        else if (dynamic_cast<ARBar *>(e) && fMeasureBarsEnabled)
            DrawMeasureBar(date);
	}

    while (!fColors->empty()) {
		fDev->PopFillColor();

        fColors->pop();
	}
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawMusicalObject(ARMusicalObject *e, TYPE_TIMEPOSITION date, TYPE_DURATION dur)
{
	ARNote *note = dynamic_cast<ARNote *>(e);

	if (note) {
		int pitch = note->midiPitch();

        if (pitch < 0)
            fChordDuration = dur; // prepare for potential chord
        else {
            if (pitch >= fLowPitch && pitch <= fHighPitch) {
                if (note->getName() != ARNoteName::empty)
                    DrawNote(pitch, double(date), double(dur));
            }

            fChord = false;
        }
	}
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawNote(int pitch, double date, double dur)
{
	float x = date2xpos (date);
	float y = pitch2ypos(pitch);
	DrawRect(x, y, dur);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawRect(float x, float y, double dur) const
{
	float w        = duration2width(dur);
	float halfstep = stepheight() / 2.0f;

	if (!halfstep)
        halfstep = 1;

	fDev->Rectangle(x, y - halfstep, x + (w ? w : 1), y + halfstep);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawMeasureBar(double date) const
{
    float x    = date2xpos(date);
	float yMin = pitch2ypos(fLowPitch)  + 0.5f * fNoteHeight;
	float yMax = pitch2ypos(fHighPitch) - 0.5f * fNoteHeight;
    
    fDev->PushPenColor(VGColor(0, 0, 0));
    fDev->PushFillColor(VGColor(0, 0, 0));

    fDev->PushPenWidth(0.3f);
    fDev->Line(x, yMin, x, yMax);
    fDev->PopPenWidth();

    fDev->PopFillColor();
    fDev->PopPenColor();
}

//--------------------------------------------------------------------------
bool GuidoPianoRoll::handleColor(ARNoteFormat* noteFormat)
{
    if (noteFormat) {
		const TagParameterString *tps = noteFormat->getColor();
		unsigned char colref[4];
		
        if (tps && tps->getRGB(colref)) {
            fColors->push(VGColor(colref[0], colref[1], colref[2], colref[3]));
            
            fDev->PushFillColor(fColors->top());
		}

		return true;
	}

	return false;
}

//--------------------------------------------------------------------------
/*void GuidoPianoRoll::handleEmpty(double date)
{
    // REM: TODO ?
}*/

//--------------------------------------------------------------------------
float GuidoPianoRoll::pitch2ypos(int midipitch) const
{
	int   p = midipitch - fLowPitch;
    float h = ((float) (fHeight * p) / (float) pitchRange());

	return fHeight - h;
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::date2xpos(double pos) const
{
    float  fTimedWidth = fWidth - fUntimedLeftElementWidth;
    double posDiff     = pos - double(fStartDate);

    return (float) (fTimedWidth * posDiff / fDuration + fUntimedLeftElementWidth);
}

//--------------------------------------------------------------------------
float GuidoPianoRoll::duration2width(double dur) const
{
    float  fTimedWidth = fWidth - fUntimedLeftElementWidth;
    return (float) (fTimedWidth * dur / fDuration);
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::HSVtoRGB(float h, float s, float v, int &r, int &g, int &b)
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

    r = (int) floor((float) rTmp * 256);
    g = (int) floor((float) gTmp * 256);
    b = (int) floor((float) bTmp * 256);
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
void GuidoPianoRoll::DrawMidiSeq(MidiSeqPtr seq, int tpqn)
{
	MidiEvPtr ev = FirstEv(seq);
	int tpwn     = tpqn * 4;
	double start = double(fStartDate);
	double end   = double(fEndDate);

	while (ev) {
		if (EvType(ev) == typeNote) {
			double date = double(Date(ev)) / tpwn;
			double dur  = double(Dur(ev))  / tpwn * 1.25; // REM: I don't know why it works but it does !

			if (date >= start) {
                if (date < end) {
                    double remain = end - date;
                    DrawNote(Pitch(ev), date, (dur > remain ? remain : dur));
                }
			}
			else if ((date + dur) > start) {
				dur -= (start - date);
				double remain = end - start;
				DrawNote(Pitch(ev), start, (dur > remain ? remain : dur));
			}
		}

		ev = Link(ev);
	}
}

//--------------------------------------------------------------------------
void GuidoPianoRoll::DrawFromMidi()
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

    if (!fIsEndDateSet)
        fEndDate = TYPE_TIMEPOSITION (int(double(maxTime) / (tpqn * 4) * 256), 256);

    fDuration = double(fEndDate - fStartDate);

    for (unsigned int i = 0; i < vseq.size(); i++) {
        DrawMidiSeq(vseq[i], tpqn);
        mf.midi()->FreeSeq(vseq[i]);
    }

    mf.Close();

    DrawKeyboard();
    DrawGrid();
}

#endif