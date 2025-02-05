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

#ifndef __durationOperation__
#define __durationOperation__

#include "arexport.h"
#include "clonevisitor.h"
#include "operation.h"
#include "guidorational.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief A visitor to stretch a score.
*/
class gar_export durationOperation :
	public operation,
	public clonevisitor
{		
    public:
 				 durationOperation();
		virtual ~durationOperation()	{}

		/*! stretch a score
			\param score the score to be stretched
			\param fact the stretching factor
			\return a new score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, float factor );

		/*! stretch a score
			\param score the score to be stretched
			\param targetDuration the target duration
			\return a new score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, const rational& targetDuration );

		/*! stretch a score to the duration of another score
			\param score1 the score to be stretched
			\param score2 a score to specify the target duration 
			\return a new score
		*/
		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );
 
     protected:
		enum { kMaxDivider = 200 };
		bool fPrimeNumbers[kMaxDivider];
		
		rational	fFactor;
		rational	fCurrentDurationIn;
		rational	fCurrentDurationOut;

		rational		float2rational ( float val ) const;
		rational		safeDiv ( const rational& dur1, const rational& dur2 ) const;
		rational		boundedMult ( const rational& dur1, const rational& dur2, int maxDenom=kMaxDivider ) const;
		virtual rational stretch ( const rational& duration );
		virtual void visitStart ( SARNote& elt );
		virtual void visitStart ( SARVoice& elt );

		virtual Sguidoelement stretch ( const Sguidoelement& score, const rational& stretchFactor );
};

/*! @} */

} // namespace MusicXML


#endif
