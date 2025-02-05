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

#ifndef __rythmvisitor__
#define __rythmvisitor__

#include <ostream>

#include "arexport.h"
#include "guidoelement.h"
#include "ARTypes.h"
#include "guidorational.h"
#include "tree_browser.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to extract the rythm of a score voice
*/
class gar_export rythmvisitor :
	public visitor<SARVoice>,
	public visitor<SARChord>,
	public visitor<SARNote>
{
    public:
				 rythmvisitor() : fRythm(0) { fBrowser.set(this); }
       	virtual ~rythmvisitor() {}
              
		/*!
			\brief collects the rythm of a score voice
			\param score a score
			\param voice the voice index (0 based)
			\param outrythm a vector that collects the rythm values
		*/
		virtual void rythm(const Sguidoelement& score, int voice, std::vector<rational>* outrythm);

	protected: 
		virtual void visitStart( SARVoice& elt );
		virtual void visitStart( SARChord& elt );
		virtual void visitStart( SARNote& elt );
		virtual void visitEnd  ( SARVoice& elt );
		virtual void visitEnd  ( SARChord& elt );
		virtual void stop (bool state=true)	{ fBrowser.stop (state); }

		int			fTargetVoice, fCurrentVoice;
		std::vector<rational>*	fRythm;
		bool		fInChord;
		rational	fCurrentNoteDuration;
		int			fCurrentDots;

		tree_browser<guidoelement> fBrowser;
};

/*! @} */

} // namespace

#endif
