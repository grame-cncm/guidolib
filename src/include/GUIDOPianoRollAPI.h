#ifndef GUIDOPianoRollAPI_H
#define GUIDOPianoRollAPI_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004, 1013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"
#include "GUIDOExport.h"

class GuidoPianoRoll;
class GuidoReducedProportional;

#ifdef __cplusplus
extern "C" {
#endif

/*!
\addtogroup PianoRoll Create a piano roll
@{
*/

	//---------------------------------------------------------------------------
	// the new API
	//---------------------------------------------------------------------------

	/*!
		\brief Creates a new piano roll
		\param type the piano roll type : 0 for basic piano roll
                                          1 for reduced proportional
                                          2 for trajectory piano roll
		\return a guido piano roll.
	*/
	GUIDOAPI(GuidoPianoRoll *)  GuidoCreatePianoRoll(int type);

	/*!
		\brief Destroys a guido piano roll and releases all the associated ressources
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)	    GuidoDestroyPianoRoll(GuidoPianoRoll *pr);

	/*!
		\brief Sets an AR to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param arh an AR handler
        \return a Guido error code
	*/
    GUIDOAPI(GuidoErrCode)      GuidoSetARToPianoRoll(GuidoPianoRoll *pr, ARHandler arh);
    
	/*!
		\brief Sets an midi file to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param midiFileName a midi file name
        \return a Guido error code
	*/
    GUIDOAPI(GuidoErrCode)      GuidoSetMidiToPianoRoll(GuidoPianoRoll *pr, const char *midiFileName);

	/*!
		\brief Sets dimensions to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param width the width of the canvas (-1 to set the default width)
        \param height the height of the canvas (-1 to set the default height)
        \return a Guido error code
	*/
    GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollCanvasDimensions(GuidoPianoRoll *pr, int width, int height);

	/*!
		\brief Sets time limits to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param start the start date (-0/0 to set the default start date)
        \param end the end date (-0/0 to set the default end date)
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollTimeLimits(GuidoPianoRoll *pr, GuidoDate start, GuidoDate end);

	/*!
		\brief Sets pitch limits to a piano roll (minimum 1 octave)
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param minPitch the minimal pitch (midi notation) (-1 to set the default minimal pitch)
        \param maxPitch the maximal pitch (midi notation) (-1 to set the default maximal pitch)
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollPitchLimits(GuidoPianoRoll *pr, int minPitch, int maxPitch);

    /*!
		\brief Sets if duration lines will de drawn (only for a GuidoReducedProportional)
		\param pr a pianoroll (GuidoReducedProportional) previously created with GuidoCreatePianoRoll
		\param enabled a boolean corresponding to the draw state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableDurationLines(GuidoPianoRoll *pr, bool enabled);
    
    /*!
		\brief Enables keyboard or not (not for GuidoReducedProportional)
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param enabled a boolean corresponding to the keyboard draw state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableKeyboard(GuidoPianoRoll *pr, bool enabled);
    
	/*!
		\brief Gets the piano roll keyboard width (not for GuidoReducedProportional)
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param keyboardWidth the keyboard width
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoGetPianoRollKeyboardWidth(GuidoPianoRoll *pr, int &keyboardWidth);

    /*!
		\brief Enables or not the automatic voices coloration (if a color is manually set with
            GuidoPianoRollSetColorToVoice, random color will not be applied for this voice)
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param enabled a boolean corresponding to the color state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableRandomVoicesColor(GuidoPianoRoll *pr, bool enabled);

    /*!
		\brief Sets a color to a voice
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param voiceNum the voice number
        \param r the red param of RGB color
        \param g the green param of RGB color
        \param b the blue param of RGB color
        \param a the alpha param of RGB color
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollSetColorToVoice(GuidoPianoRoll *pr, int voiceNum, int r, int g, int b, int a);
    
    /*!
		\brief Enables or not measure bars (not for a midi rendering)
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param enabled a boolean corresponding to the measure bars draw state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoPianoRollEnableMeasureBars(GuidoPianoRoll *pr, bool enabled);
    
    /*!
		\brief Sets the pitch lines display mode : -1 -> automatic
                                                    0 -> no pitch lines
                                                    1 -> only C lines
                                                    2 -> only C and G lines
                                                    3 -> only diatonic scale lines
                                                    4 -> all lines (chromatic scale) // REM: faire une enum pour ça
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param mode an int corresponding to the pitch lines display mode
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollPitchLinesDisplayMode(GuidoPianoRoll *pr, int mode);
    
	/*!
		\brief Gets a rendered piano roll from AR, writing it on a VGDevice
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param dev the device where the piano will be rendered
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoGetPianoRollRenderingFromAR(GuidoPianoRoll *pr, VGDevice* dev);

	/*!
		\brief Gets a rendered piano roll from midi, writing it on a VGDevice
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param dev the device where the piano will be rendered
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoGetPianoRollRenderingFromMidi(GuidoPianoRoll *pr, VGDevice* dev);

	/*!
		\brief Gets a rendered reduced proportional from AR, writing it on a VGDevice
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param dev the device where the piano will be rendered
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoGetRProportionalRenderingFromAR(GuidoPianoRoll *rp, VGDevice* dev);

	/*!
		\brief Gets a rendered reduced proportional from midi, writing it on a VGDevice
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param dev the device where the piano will be rendered
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoGetRProportionalRenderingFromMidi(GuidoPianoRoll *rp, VGDevice* dev);

/*! @} */


#ifdef __cplusplus
}
#endif

#endif	/* GUIDOPianoRollAPI_H */