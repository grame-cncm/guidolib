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

#ifndef __seqOperation__
#define __seqOperation__

#include <map>

#include "arexport.h"
#include "ARTypes.h"
#include "ARTag.h"
#include "clonevisitor.h"
#include "guidoelement.h"
#include "operation.h"
#include "guidorational.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to put scores in sequence
*/
class gar_export seqOperation :
	public operation,
	public clonevisitor,
	public visitor<SAREndBar>
{
    private:
		std::map<std::string,Sguidotag> fRangeTags;
		std::map<std::string,Sguidotag> fPosTags;
		std::map<std::string,Sguidotag> fOpenedTags;
		Sguidotag fCurrentMatch;

		rational fCurrentDuration;
		int		 fCurrentOctave;

		void storeTag(Sguidotag tag);					///< stores the current tag
		void endTag(Sguidotag tag);						///< update the current tags list
		bool currentTag(Sguidotag tag, bool end=false);
		bool matchOpenedTag(Sguidotag tag, bool end=false);
		bool checkmatch(Sguidotag tag1, Sguidotag tag2);
		bool compareContent (Sguidotag tag1, Sguidotag tag2);


	protected:
		enum state { kInFirstScore, kInSecondScore, kRemainVoice };
		state	fState;		// the current operation state: copying the first score, the second score of the remaining of the second
		bool	fFirstInSecondScore;		// a flag for special handling of the seconf score first note
		

		void visitStart ( SARNote& elt );
		void visitStart ( SARVoice& elt );
		void visitStart ( Sguidotag& elt );
		void visitStart ( SAREndBar& elt );		// filers the end bar in the first score

		void visitEnd	( SARVoice& elt );
		void visitEnd	( Sguidotag& elt );

    public:
				 seqOperation() {}
		virtual ~seqOperation() {}

		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );
};

/*! @} */

} // namespace

#endif
