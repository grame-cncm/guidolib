
#ifndef GUIDOReducedProportional_H
#define GUIDOReducedProportional_H

/*
  GUIDO Library
  Copyright (C) 2015 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOEngine.h"
#include "GUIDOExport.h"
#include "GUIDOScoreMap.h"

class RProportional;

#ifdef __cplusplus
extern "C" {
#endif

/*!
\addtogroup Rproportional Reduced proportional representation
@{
*/

	/*!
		\brief Creates a new reduced proportional representation from AR
        \param arh an AR handler
		\return a guido reduced proportional representation.
	*/
	GUIDOAPI(RProportional *)  GuidoAR2RProportional(ARHandler arh);
    
	/*!
		\brief Creates a new reduced proportional representation from a midi file
        \param midiFileName a midi file name
		\return a guido reduced proportional representation.
	*/
	GUIDOAPI(RProportional *)  GuidoMidi2RProportional(const char *midiFileName);

	/*!
		\brief Destroys a reduced proportional representation and releases all the associated ressources
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
		\return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)	    GuidoDestroyRProportional(RProportional *pr);

	/*!
		\brief Sets limits to a reduced proportional representation (start/end date, lower/higher pitch)
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
		\param limitParams the structure containing limits :
                      start date    (GuidoDate)     (0/0 to adjust automatically start date to the score's start date)
                      end date      (GuidoDate)     (0/0 to adjust automatically end date to the score's end date)
                      minimal pitch (midi notation) (-1 to adjust automatically min pitch to the score's minimal pitch)
                      maximal pitch (midi notation) (-1 to adjust automatically max pitch to the score's maximal pitch)
                Remark : minimal range pitch accepted is 1 octave.
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalSetLimits(RProportional *pr, GuidoDate start, GuidoDate end, int lowpitch, int highpitch);

    /*!
		\brief Enables or disable duration lines (enabled by default)
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional
		\param enabled a boolean value
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalDrawDurationLines(RProportional *pr, bool enabled);

    /*!
		\brief Enables or not the automatic voices coloration (not enabled by default)
               If a color is manually set with GuidoRProportionalSetColorToVoice, automatic
               color will not be applied for this voice.
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional
		\param enabled a boolean corresponding to the color state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalEnableAutoVoicesColoration(RProportional *pr, bool enabled);

    /*!
		\brief Sets a RGB color to a voice (first voice is number 1) (black by default)
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
		\param voiceNum the voice number (first voice is number 1)
        \param r the red param of RGB color
        \param g the green param of RGB color
        \param b the blue param of RGB color
        \param a the alpha param of RGB color
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalSetRGBColorToVoice(RProportional *pr, int voiceNum, int r, int g, int b, int a=255);

    /*!
		\brief Sets a html color to a voice (first voice is number 1) (black by default)
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
		\param voiceNum the voice number (first voice is number 1)
        \param color the html color (constants are defined in Colors.h)
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalSetHtmlColorToVoice(RProportional *pr, int voiceNum, const char* color);

	/*!
		\brief remove a color to a voice (first voice is number 1) (black by default)
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
		\param voiceNum the voice number (first voice is number 1)
		\return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalRemoveColorToVoice(RProportional *pr, int voiceNum);

    /*!
		\brief Enables or disable measure bars (false by default)
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
		\param enabled a boolean corresponding to the measure bars draw state
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalEnableMeasureBars(RProportional *pr, bool enabled);

    /*!
		\brief Gets a reduced proportional representation map
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
        \param width the width of the piano roll (-1 to set the default width : 1024)
        \param height the height of the canvas (-1 to set the default height : 512)
        \return a Guido error code (returns guidoErrBadParameter if keyboard width is higher than width param)
	*/
    GUIDOAPI(GuidoErrCode)      GuidoRProportionalGetMap(const RProportional *pr, int width, int height, Time2GraphicMap &outmap);

	/*!
		\brief Draw the reduced proportional representation on a VGDevice
		\param pr a reduced proportional representation previously created with GuidoAR2RProportional or GuidoMidi2RProportional
        \param width the drawing area width (-1 to set the default width : 1024)
        \param height the drawing area height (-1 to set the default height : 512)
        \param dev the graphic device
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoRProportionalOnDraw(RProportional *pr, int width, int height, VGDevice* dev);

/*! @} */


#ifdef __cplusplus
}
#endif

#endif	/* GUIDOPianoRollAPI_H */
