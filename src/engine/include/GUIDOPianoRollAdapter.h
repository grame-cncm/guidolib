#ifndef GUIDOPIANOROLLADAPTER_H
#define GUIDOPIANOROLLADAPTER_H
/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOPianoRoll.h"
#include <string>

/*!
 * \addtogroup APICplusplus C++ interface
 * @{
 * \defgroup PianoRollAdapter Guido Piano Roll Adapter
 *	\brief A C++ interface to the GUIDOPianoRoll API
 *
 *	A C++ class to manupilate Piano roll.
 */
class_export GUIDOPianoRollAdapter
{
	public:
		GUIDOPianoRollAdapter();
		virtual ~GUIDOPianoRollAdapter();
		/*!
			\brief Creates a new piano roll from AR, corresponding to type :
								 simplePianoRoll     -> basic piano roll
								 trajectoryPianoRoll -> every event is graphically linked to the previous one
			\param type the piano roll type
			\param arh an AR handler
			\return a guido piano roll.
		*/
		PianoRoll * ar2PianoRoll(PianoRollType type, ARHandler arh);

		/*!
			\brief Creates a new piano roll from Midi, corresponding to type :
								 simplePianoRoll     -> basic piano roll
								 trajectoryPianoRoll -> every event is graphically linked to the previous one
			\param type the piano roll type
			\param midiFileName a midi file name
			\return a guido piano roll.
		*/
		PianoRoll * midi2PianoRoll(PianoRollType type, const std::string &midiFileName);

		/*!
			\brief Destroys a guido piano roll and releases all the associated ressources
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\return a Guido error code
		*/
		GuidoErrCode destroyPianoRoll(PianoRoll *pr);

		/*!
			\brief Sets limits to a piano roll (start/end date, lower/higher pitch)
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param limitParams the structure containing limits :
						  start date    (GuidoDate)     (0/0 to adjust automatically start date to the score's start date)
						  end date      (GuidoDate)     (0/0 to adjust automatically end date to the score's end date)
						  minimal pitch (midi notation) (-1 to adjust automatically min pitch to the score's minimal pitch)
						  maximal pitch (midi notation) (-1 to adjust automatically max pitch to the score's maximal pitch)
					Remark : minimal range pitch accepted is 1 octave.
			\return a Guido error code
		*/
		GuidoErrCode setLimits(PianoRoll *pr, LimitParams &limitParams);

		/*!
			\brief Enables keyboard or not (not enabled by default)
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param enabled a boolean corresponding to the keyboard draw state
			\return a Guido error code
		*/
		GuidoErrCode enableKeyboard(PianoRoll *pr, bool enabled);

		/*!
			\brief Gets the piano roll keyboard width
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param height the height of the canvas (-1 to set the default height : 512)
			\return the pianoroll keyboard width
		*/
		float getKeyboardWidth(PianoRoll *pr, int height);

		/*!
			\brief Enables or not the automatic voices coloration (not enabled by default) (not for a midi rendering) // REM: � voir
				   If a color is manually set with setColorToVoice, automatic
				   color will not be applied for this voice.
			\param pr a pianoroll previously created with ar2PianoRoll
			\param enabled a boolean corresponding to the color state
			\return a Guido error code
		*/
		GuidoErrCode enableAutoVoicesColoration(PianoRoll *pr, bool enabled);

		/*!
			\brief Sets a RGB color to a voice (first voice is number 1) (black by default)
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param voiceNum the voice number (first voice is number 1)
			\param r the red param of RGB color
			\param g the green param of RGB color
			\param b the blue param of RGB color
			\param a the alpha param of RGB color
			\return a Guido error code
		*/
		GuidoErrCode setRGBColorToVoice(PianoRoll *pr, int voiceNum, int r, int g, int b, int a);

		/*!
			\brief Sets a html color to a voice (first voice is number 1) (black by default)
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param voiceNum the voice number (first voice is number 1)
			\param color the html color (constants are defined in Colors.h)
			\return a Guido error code
		*/
		GuidoErrCode setHtmlColorToVoice(PianoRoll *pr, int voiceNum, long color);

		/*!
			\brief Enables or not measure bars (false by default)
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param enabled a boolean corresponding to the measure bars draw state
			\return a Guido error code
		*/
		GuidoErrCode enableMeasureBars(PianoRoll *pr, bool enabled);

		/*!
			\brief Sets the pitch lines display mode (automatic by default). Use Pitch lines display mode constants to pick lines
				   which will be be displayed.
				   Example : "kCLine + kGLine" will displayed C and G line. "kNoLine" doesn't display any line. "kAutoLines" adjust line display
							 according to piano roll pitch range (automatic possibilities : no line, C line, C and G line, chromatic scale, diatonic scale);
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param mode an int corresponding to the pitch lines display mode
			\return a Guido error code
		*/
		GuidoErrCode setPitchLinesDisplayMode(PianoRoll *pr, int mode);

		/*!
			\brief Gets the piano roll map
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param width the width of the piano roll (-1 to set the default width : 1024)
			\param height the height of the canvas (-1 to set the default height : 512)
			\return a Time2GraphicMap instance
		*/
		Time2GraphicMap getMap(PianoRoll *pr, int width, int height);

		/*!
			\brief Draw the piano roll on a VGDevice
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param width the width on which piano roll will be drawn (-1 to set the default width : 1024)
			\param height the height on which piano roll will be drawn (-1 to set the default height : 512)
			\param dev the device on which piano will be drawn
			\return a Guido error code (returns guidoErrBadParameter if keyboard width is higher than width param)
		*/
		GuidoErrCode onDraw(PianoRoll *pr, int width, int height, VGDevice* dev);

		/*!
		 * \brief svgExport Draw the piano roll on SVG device and return the xml string.
		 * \param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
		 * \param width the width on which piano roll will be drawn (-1 to set the default width : 1024)
		 * \param height the height on which piano roll will be drawn (-1 to set the default height : 512)
		 * \return the xml code of piano roll in svg.
		 */
		std::string svgExport(PianoRoll *pr, int width, int height);

#ifdef CANVASSYSTEM
		/** \brief Exports one page of score on html canvas.

			\param pr a pianoRoll.
			\param width
			\param height
			\return a Guido error code
		*/
		GuidoErrCode javascriptExport(PianoRoll *pr, int width, int height);
#endif
};
/*! @} */
#endif // GUIDOPIANOROLLADAPTER_H
