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

#ifndef __event2timeVisitor__
#define __event2timeVisitor__

#include "arexport.h"
#include "durationvisitor.h"
#include "counteventsvisitor.h"
#include "guidoelement.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//_______________________________________________________________________________
/*!
\brief  a visitor to convert events indexes to time and time to event index

	 \\empty events are ignored when looking for the target event or position.
	 Chords account for one event.
*/
class gar_export event2timevisitor : public durationvisitor
{
    public: 
				 event2timevisitor() {}
		virtual ~event2timevisitor() {}

		/*!
			\brief computes the duration of a score at a given event location
			\param score an input score
			\param evIndex the index of the target event
			\param voice the voice where to look for the target event
			\return the duration of the input score at the target event location, expressed as a rational (where 1 is a whole note)
			\note \\empty events are ignored when looking for the target event
		*/
		rational	event2time (const Sguidoelement& score, unsigned int evIndex, unsigned int voiceIndex=0);

		/*!
			\brief computes the duration of a score at a given event location
			\param score an input score
			\param time a time position
			\param voice the voice where to look for the target event
			\return the index of the event at the target time position
			\note \\empty events are ignored when looking for the target event
		*/
		int			time2event (const Sguidoelement& score, const rational& time, unsigned int voiceIndex=0);

	protected:
		rational		fTargetDate;
		int				fTargetEvent;
		unsigned int	fTargetVoice;
		unsigned int	fCurrentVoice;
		bool			fDone;
		counteventsvisitor fCountVisitor;

		bool done ();
		void init ();

		virtual void visitStart ( SARVoice& elt );
		virtual void visitStart ( SARNote& elt );
		virtual void visitStart ( SARChord& elt );

		virtual void visitEnd   ( SARVoice& elt );
		virtual void visitEnd   ( SARChord& elt );
};

/*! @} */

} // namespace

#endif
