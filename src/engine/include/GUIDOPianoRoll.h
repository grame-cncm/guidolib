#ifndef GUIDOPianoRollAPI_H
#define GUIDOPianoRollAPI_H

/*
  GUIDO Library
  Copyright (C) 2014 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"
#include "GUIDOExport.h"
#include "GUIDOScoreMap.h"

class PianoRoll;

/** \brief PianoRollType
*/
enum PianoRollType {
	kSimplePianoRoll,
    kTrajectoryPianoRoll
};

/** \brief Pitch lines display mode
*/
const int kCLine      =  1;
const int kCSharpLine =  1<<1;
const int kDLine      =  1<<2;
const int kDSharpLine =  1<<3;
const int kELine      =  1<<4;
const int kFLine      =  1<<5;
const int kFSharpLine =  1<<6;
const int kGLine      =  1<<7;
const int kGSharpLine =  1<<8;
const int kALine      =  1<<9;
const int kASharpLine =  1<<10;
const int kBLine      =  1<<11;
const int kAutoLines  =  0;
const int kNoLine     = -1;

typedef struct LimitParams {
    GuidoDate startDate;
    GuidoDate endDate;
    int       lowPitch;
    int       highPitch;
} LimitParams;

#ifdef __cplusplus
extern "C" {
#endif

/*!
\addtogroup PianoRoll Piano API
@{
*/

	//---------------------------------------------------------------------------
	// the new API
	//---------------------------------------------------------------------------

	/*!
		\brief Creates a new piano roll from AR, corresponding to type :
                             simplePianoRoll     -> basic piano roll
                             trajectoryPianoRoll -> every event is graphically linked to the previous one
		\param PianoRollType the piano roll type
        \param arh an AR handler
		\return a guido piano roll.
	*/
	GUIDOAPI(PianoRoll *)  GuidoAR2PianoRoll(PianoRollType type, ARHandler arh);
    
	/*!
		\brief Creates a new piano roll from Midi, corresponding to type :
                             simplePianoRoll     -> basic piano roll
                             trajectoryPianoRoll -> every event is graphically linked to the previous one
		\param type the piano roll type
        \param midiFileName a midi file name
		\return a guido piano roll.
	*/
	GUIDOAPI(PianoRoll *)  GuidoMidi2PianoRoll(PianoRollType type, const char *midiFileName);

	/*!
		\brief Destroys a guido piano roll and releases all the associated ressources
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)	    GuidoDestroyPianoRoll(PianoRoll *pr);

	/*!
		\brief Sets limits to a piano roll (start/end date, lower/higher pitch)
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param limitParams the structure containing limits :
                      start date    (GuidoDate)     (0/0 to adjust automatically start date to the score's start date)
                      end date      (GuidoDate)     (0/0 to adjust automatically end date to the score's end date)
                      minimal pitch (midi notation) (-1 to adjust automatically min pitch to the score's minimal pitch)
                      maximal pitch (midi notation) (-1 to adjust automatically max pitch to the score's maximal pitch)
                Remark : minimal range pitch accepted is 1 octave.
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollSetLimits(PianoRoll *pr, LimitParams limitParams);

    /*!
		\brief Enables keyboard or not (not enabled by default)
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param enabled a boolean corresponding to the keyboard draw state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableKeyboard(PianoRoll *pr, bool enabled);
    
	/*!
		\brief Gets the piano roll keyboard width
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
        \param height the height of the canvas (-1 to set the default height : 512)
        \param keyboardWidth the pianoroll keyboard width
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollGetKeyboardWidth(PianoRoll *pr, int height, float &keyboardWidth);

    /*!
		\brief Enables or not the automatic voices coloration (not enabled by default) (not for a midi rendering) // REM: à voir
               If a color is manually set with GuidoPianoRollSetColorToVoice, automatic
               color will not be applied for this voice.
		\param pr a pianoroll previously created with GuidoAR2PianoRoll
		\param enabled a boolean corresponding to the color state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableAutoVoicesColoration(PianoRoll *pr, bool enabled);

    /*!
		\brief Sets a RGB color to a voice (first voice is number 1) (black by default)
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param voiceNum the voice number (first voice is number 1)
        \param r the red param of RGB color
        \param g the green param of RGB color
        \param b the blue param of RGB color
        \param a the alpha param of RGB color
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollSetRGBColorToVoice(PianoRoll *pr, int voiceNum, int r, int g, int b, int a);

    /*!
		\brief Sets a html color to a voice (first voice is number 1) (black by default)
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param voiceNum the voice number (first voice is number 1)
        \param color the html color (constants are defined in Colors.h)
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollSetHtmlColorToVoice(PianoRoll *pr, int voiceNum, long color);

	/*!
		\brief remove a color to a voice (first voice is number 1) (black by default)
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param voiceNum the voice number (first voice is number 1)
		\return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollRemoveColorToVoice(PianoRoll *pr, int voiceNum);

    /*!
		\brief Enables or not measure bars (false by default)
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param enabled a boolean corresponding to the measure bars draw state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableMeasureBars(PianoRoll *pr, bool enabled);
    
    /*!
		\brief Sets the pitch lines display mode (automatic by default). Use Pitch lines display mode constants to pick lines
               which will be be displayed.
               Example : "kCLine + kGLine" will displayed C and G line. "kNoLine" doesn't display any line. "kAutoLines" adjust line display
                         according to piano roll pitch range (automatic possibilities : no line, C line, C and G line, chromatic scale, diatonic scale);
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
		\param mode an int corresponding to the pitch lines display mode
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollSetPitchLinesDisplayMode(PianoRoll *pr, int mode);

    /*!
		\brief Gets the piano roll map
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
        \param width the width of the piano roll (-1 to set the default width : 1024)
        \param height the height of the canvas (-1 to set the default height : 512)
        \return a Guido error code (returns guidoErrBadParameter if keyboard width is higher than width param)
	*/
    GUIDOAPI(GuidoErrCode)      GuidoPianoRollGetMap(const PianoRoll *pr, int width, int height, Time2GraphicMap &outmap);

	/*!
		\brief Draw the piano roll on a VGDevice
		\param pr a pianoroll previously created with GuidoAR2PianoRoll or GuidoMidi2PianoRoll
        \param width the width on which piano roll will be drawn (-1 to set the default width : 1024)
        \param height the height on which piano roll will be drawn (-1 to set the default height : 512)
        \param dev the device on which piano will be drawn
        \return a Guido error code (returns guidoErrBadParameter if keyboard width is higher than width param)
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollOnDraw(PianoRoll *pr, int width, int height, VGDevice* dev);

/*! @} */


#ifdef __cplusplus
}
#endif

#endif	/* GUIDOPianoRollAPI_H */
