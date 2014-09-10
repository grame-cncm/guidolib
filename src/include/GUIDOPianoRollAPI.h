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
		\return a guido piano roll.
	*/
	GUIDOAPI(GuidoPianoRoll *)  GuidoCreatePianoRoll();

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
		\brief Sets dimensions to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param width the width of the canvas (-1 to let default width) // REM: à changer ?
        \param height the height of the canvas (-1 to let default height) // REM: à changer ?
        \return a Guido error code
	*/
    GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollCanvasDimensions(GuidoPianoRoll *pr, int width, int height);

	/*!
		\brief Sets time limits to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param start the start date (-1/1 to let default start date) // REM: à changer ?
        \param end the end date (-1/1 to let default date) // REM: à changer ?
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollTimeLimits(GuidoPianoRoll *pr, GuidoDate start, GuidoDate end);

	/*!
		\brief Sets pitch limits to a piano roll
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
		\param minPitch the minimal pitch (midi notation) (-1 to let default minimal pitch) // REM: à changer ?
        \param maxPitch the maximal pitch (midi notation) (-1 to let default maximal pitch) // REM: à changer ?
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoSetPianoRollPitchLimits(GuidoPianoRoll *pr, int minPitch, int maxPitch);

	/*!
		\brief Get a rendered piano roll, wrote on a VGDevice
		\param pr a pianoroll previously created with GuidoCreatePianoRoll
        \param dev the device where the piano will be rendered
        \return a Guido error code
	*/
	GUIDOAPI(GuidoErrCode)      GuidoGetPianoRollRendering(GuidoPianoRoll *pr, VGDevice* dev);

/*! @} */


#ifdef __cplusplus
}
#endif

#endif	/* GUIDOPianoRollAPI_H */