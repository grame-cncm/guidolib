/*
  Copyright � Grame 2008

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

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifndef __transposeOperation__
#define __transposeOperation__

#include <map>
#include <vector>

#include "arexport.h"
#include "ARTypes.h"
#include "guidoelement.h"
#include "operation.h"
#include "visitor.h"


namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief A visitor that computes a diatonic transposition of a score.

	The transposition visitor computes a diatonic transposition of
	a score. The transposition interval is specified as a number of chromatic
	steps, the simplest enharmonic diatonic transposition is automatically selected.
	The operation makes use of a fifth cycle table to compute transposed values.
	
\todo transposing ornaments elements
*/
class gar_export transposeOperation :
	public operation,
	public visitor<SARNote>,
	public visitor<SARKey>,
	public visitor<SARVoice> 
{		
    public:
		enum { kUndefinedKey = -99 };
		
		typedef int	Chromatic;
		typedef int	Diatonic;

 				 transposeOperation();
		virtual ~transposeOperation();

		/*! computes a score transposition
			\param score the score to transpose
			\param steps the chromatic transposition step
			\return a new transposed score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, int steps );

		/*! computes a score transposition
			\param score1 the score to transpose
			\param score2 a score as transposing interval specifier, 
				  actually the difference between the first notes of the scores (uses the lowest note of chords)
			\return a new transposed score
		*/
		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );

		/*! computes an octaves count from a chromatic interval
			\param steps a chromatic steps count
			\return the count of octaves
		*/
		static int  getOctave ( Chromatic steps )			{ return steps/12; }

		/*! computes a step number wihin an octave
			\param steps a chromatic steps count
			\return the step number within an octave
		*/
		static Chromatic  getOctaveStep ( Chromatic steps ) { return steps%12; }

		/*! computes a key sign corresponding to a chromatic degree 
			\param steps a chromatic step
			\return a numeric key signature representing a number of sharps when positive
			and a number of flats when negative
		*/
		static int  getKey ( Chromatic steps );

		/*! converts a key string specification to a n umeric value
			\param key a guido compliant key string
			\return a numeric key signature representing a number of sharps when positive
			and a number of flats when negative
		*/
		static int  convertKey ( const std::string& key );

		/*! Transpose a key expressed as a count of sharps (positive keys) or flats (negative key).
			The method may do enharmonic adjustments to choose the simplest key. 		
			\param key the key to be transposed
			\param steps chromatic transposition steps
			\param enharmonicChange on output, possible adjustment to enharmonic keys
			\return the transposed key
		*/
		static int transposeKey ( int key, Chromatic steps, int& enharmonicChange );
 
     protected:
		std::vector<std::pair<char,int> >	fFifthCycle;	// the fifth cycle table
		Chromatic	fChromaticSteps;						// the target transposing interval

		int		fTableShift;			// the current shift into the table of fifths
		int		fOctaveChange;			// the target octave change computed from fChromaticSteps
		int		fCurrentOctaveIn;		// the current octave of input notes (default to 1)
		int		fCurrentOctaveOut;		// the current octave of output notes

		void	initialize	();

		/*! Transpose a pitch expressed as a diatonic value + alteration + octave
			\param pitch on input a diatonic pitch value (where C=1), on output the new pitch value
			\param alter on input the accidental value, on output the new accidental value
			\param octave on input the octave number, on output the new octave number
			\param tableshift the current shift into the table of fifths
		*/
		void	transpose ( char& pitch, int& alter, int& octave, int tableshift ) const;

		virtual void visitStart ( SARNote& elt );
		virtual void visitStart ( SARKey& elt );
		virtual void visitStart ( SARVoice& elt ); 		
};

/*! @} */

} // namespace MusicXML


#endif
