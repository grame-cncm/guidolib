/*

  guidoar Library
  Copyright (C) 2008-2009  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/
#ifndef __libguidoar__
#define __libguidoar__

#ifdef WIN32
# pragma warning (disable : 4786 4190)
#endif

#include <iostream>
#include <string>
#include <stdio.h>
#include "guidorational.h"
#include "arexport.h"

namespace guido 
{

/*!
\addtogroup interface C Interface
@{
*/

enum garErr { kNoErr, kInvalidFile, kInvalidArgument, kOperationFailed };


#ifdef __cplusplus
extern "C" {
#endif

/// \brief gives the library version number as a float number
gar_export float			guidoarVersion();
/// \brief gives the library version string
gar_export const char*		guidoarVersionStr();

//--------------------------------------------------------------------------------
// operations on scores
//--------------------------------------------------------------------------------
/*! \brief creates an unrolled version of a score

	An unrolled score is a score where repeats, jumps to coda, dal segno etc... are removed by producing 
	an explicit score where the structure is absent but explicitily written.
	\param gmn a string containing the gmn code
	\param out a stream to output the unrolled version
	\return an error code
*/
gar_export garErr			guido2unrolled(const char* gmn, std::ostream& out);

/*! \brief transpose a score

	Transposition of a score affects notes but also key signature when present. Between similar enharmonic
	key signatures, the simplest one is choosen.
	\param gmn a string containing the gmn code
	\param interval the transposing interval
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVTranpose(const char* gmn, int interval, std::ostream& out);

/*! \brief transpose a score using another score

	The transposing interval is computed as the difference between the 2 score's pitches: 
	the pitch of a score is the pitch of the first note of the first voice. In case the first voice 
	starts with a chord, the lowest pitch is taken. More intuitively, the operation forces the pitch of
	the first score to the pitch of the second one.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the transposing interval is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGTranpose(const char* gmn, const char* gmnSpec, std::ostream& out);

/*! \brief cut voices of a score

	The \c top operation preserves \c n voices of the score, starting from the first voice and drops
	the remaining voices.
	\param gmn a string containing the gmn code
	\param nvoices the number of voices to preserve
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVTop(const char* gmn, int nvoices, std::ostream& out);

/*! \brief cut voices of a score using another score

	The number of voices to preserve is taken from the second score voices number.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the number of voices is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGTop(const char* gmn, const char* gmnSpec, std::ostream& out);


/*! \brief cut voices of a score

	The \c bottom operation drops \c n voices of the score, starting the first voice and preserves
	the remaining voices.
	\param gmn a string containing the gmn code
	\param nvoices the number of voices to drop
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVBottom(const char* gmn, int nvoices, std::ostream& out);

/*! \brief cut voices of a score using another score

	The number of voices to drop is taken from the second score voices number.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the number of voices is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGBottom(const char* gmn, const char* gmnSpec, std::ostream& out);


/*! \brief takes the head of a score

	The \c head operation preserves a duration \c d  of the score, starting from the beginning and drops
	the remaining parts.
	\param gmn a string containing the gmn code
	\param duration a duration expressed as a rational, where 1/1 is a whole note.
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVHead(const char* gmn, rational duration, std::ostream& out);

/*! \brief takes the head of a score using another score

	The duration to preserve is taken from the second score duration.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the duration is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGHead(const char* gmn, const char* gmnSpec, std::ostream& out);

/*! \brief takes the head of a score on an event basis

	The \c ehead operation preserves \c n events of the score, starting from the beginning and drops
	the remaining events.
	\param gmn a string containing the gmn code
	\param n the number of events to preserve.
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVEHead(const char* gmn, int n, std::ostream& out);

/*! \brief takes the head of a score on an event basis using another score

	The number of events to preserve is taken from the second score events count.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the number of events is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGEHead(const char* gmn, const char* gmnSpec, std::ostream& out);


/*! \brief takes the tail of a score

	The \c tail operation drops a duration \c d  of the score, starting from the beginning and preserves
	the remaining parts.
	\param gmn a string containing the gmn code
	\param duration a duration expressed as a rational, where 1/1 is a whole note.
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVTail(const char* gmn, rational duration, std::ostream& out);

/*! \brief takes the tail of a score using another score

	The duration to drop is taken from the second score duration.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the duration is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGTail(const char* gmn, const char* gmnSpec, std::ostream& out);

/*! \brief takes the tail of a score on an event basis

	The \c etail operation drops \c n events of the score, starting from the beginning and preserves
	the remaining events.
	\param gmn a string containing the gmn code
	\param n the number of events to preserve.
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVETail(const char* gmn, int n, std::ostream& out);

/*! \brief takes the tail of a score on an event basis using another score

	The number of events to drop is taken from the second score events count.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the number of events is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGETail(const char* gmn, const char* gmnSpec, std::ostream& out);

/*! \brief takes the tail of a score on an event basis

	The \c etail operation drops \c n events of the score, starting from the beginning and preserves
	the remaining events.
	\param gmn a string containing the gmn code
	\param n the number of events to preserve.
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVETail(const char* gmn, int n, std::ostream& out);

/*! \brief takes the tail of a score on an event basis using another score

	The number of events to preserve is taken from the second score events count.
	\param gmn a string containing the gmn code
	\param gmnSpec a string containing a gmn score where the number of events is taken
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGETail(const char* gmn, const char* gmnSpec, std::ostream& out);


/*! \brief put 2 scores in sequence

	The new score is the sequence of the \c gmn1 and \c gmn2 scores.
	\param gmn1 a string containing gmn code
	\param gmn2 a string containing gmn code
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGSeq(const char* gmn1, const char* gmn2, std::ostream& out);

/*! \brief put 2 scores in parallel

	The new score is composed of the \c gmn1 voices followed by the \c gmn2 voices.
	\param gmn1 a string containing gmn code
	\param gmn2 a string containing gmn code
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGPar		(const char* gmn1, const char* gmn2, std::ostream& out);

/*! \brief put 2 scores in parallel with right justification

	Right justification means that rest may be inserted at the beginning of a voice,
	so that the longest voices of each score end a the same time.
	\param gmn1 a string containing gmn code
	\param gmn2 a string containing gmn code
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGRPar(const char* gmn1, const char* gmn2, std::ostream& out);

/*! \brief mirror a score using a second score as fixed pitch point

	\param gmn1 a string containing gmn code
	\param gmn2 a string containing gmn code which first note of first voice is used as fixed pitch
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGMirror(const char* gmn1, const char* gmn2, std::ostream& out);


/*! \brief set the duration of one score to the duration of the second one
	\param gmn a string containing gmn code
	\param gmnSpec a string containing gmn code
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoGSetDuration(const char* gmn, const char* gmnSpec, std::ostream& out);

enum TApplyMode { kApplyOnce, kApplyForwardLoop, kApplyForwardBackwardLoop};
enum chordPitchMode { kUseLowest, kUseHighest };

/*! \brief applies the rythmic structure of a score to another score
	\param gmn a string containing gmn code
	\param gmnSpec a string containing gmn code where rythmic structure is extracted
	\param mode		specifies how the rythmic structure should be applied
	\param out		the output stream
	\return an error code
	The rythmic structure is extracted from the first voice of the second score. It is applied to the first
	score without realignment at the beginning of each voice.
*/
gar_export garErr			guidoApplyRythm(const char* gmn, const char* gmnSpec, TApplyMode mode, std::ostream& out);

/*! \brief applies the pitch structure of a score to another score
	\param gmn a string containing gmn code
	\param gmnSpec a string containing gmn code where pitch structure is extracted
	\param mode		specifies how the pitch structure should be applied
	\param pmode	specifies how a chord pitch is computed
	\param out		the output stream
	\return an error code
	The pitch structure is extracted from the first voice of the second score. It is applied to the first
	score without realignment at the beginning of each voice.
*/
gar_export garErr			guidoApplyPitch(const char* gmn, const char* gmnSpec, TApplyMode mode, chordPitchMode pmode, std::ostream& out);

/*! \brief set the duration of a score to a given duration
	\param gmn a string containing gmn code
	\param duration the target duration
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVSetDuration(const char* gmn, rational duration, std::ostream& out);

/*! \brief multiply the duration of a score
	\param gmn a string containing gmn code
	\param mult the multiplication factor
	\param out		the output stream
	\return an error code
*/
gar_export garErr			guidoVMultDuration(const char* gmn, float mult, std::ostream& out);

/*! \brief gives an event index at a given date
	\param gmn a string containing gmn code
	\param date a date expressed as a rational (1 is a whole note)
	\param voice the target voice index
	\return an event index
*/
gar_export int				guidoTime2Ev(const char* gmn, const rational& date, unsigned int voice);

/*! \brief export to midifile
	\param gmn a string containing gmn code
	\param file the midi file name
	\return an error code
*/
gar_export garErr			guido2midifile(const char* gmn, const char* file);

/*! \brief check gmn code correctness
	\return a boolean value
*/
gar_export bool				guidocheck(const char* gmn);


#ifdef __cplusplus
}
#endif


/*! \brief gives a score duration
	\param gmn a string containing gmn code
	\return a rational value (1 is a whole note), negative in case of error
*/
gar_export rational			guidoDuration(const char* gmn);

/*! \brief gives an event date
	\param gmn a string containing gmn code
	\param index the target event index
	\param voice the target voice index
	\return a date in musical time expressed as a rational
*/
gar_export rational			guidoEv2Time(const char* gmn, unsigned int index, unsigned int voice);


/*! @} */


}

#endif
