/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

#ifndef __mirrorOperation__
#define __mirrorOperation__

#include "arexport.h"
#include "ARTypes.h"
#include "ARTag.h"
#include "clonevisitor.h"
#include "guidoelement.h"
#include "operation.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to mirror pitches around a given pitch
*/
class gar_export mirrorOperation :
	public operation,
	public clonevisitor,
	public visitor<SARKey>
{
    private:
		int				fCurrentKey;
		int				fCurrentOctave;
		unsigned int	fFixedPoint;

	protected:
		void visitStart ( SARNote& elt );
		void visitStart ( SARVoice& elt );
		void visitStart ( SARKey& elt );

    public:
				 mirrorOperation();
		virtual ~mirrorOperation();

		/*! computes a mirrored score
			\param score the score to be mirrored
			\param midipitch the fixed midi note
			\return a new mirrored score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, int midipitch );

		/*! computes a mirrored score
			\param score the score to be mirrored
			\param a score which first note of the first voice is taken as fixed point
			\return a new mirrored score
		*/
		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );
};

/*! @} */

} // namespace

#endif
