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

#ifndef __gmnvisitor__
#define __gmnvisitor__

#include <ostream>

#include "arexport.h"
#include "ARTypes.h"
#include "streambeautifuller.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to print the gmn description
*/
class gar_export gmnvisitor :
	public visitor<Sguidotag>,
	public visitor<SARBar>,
	public visitor<SARRepeatBegin>,
	public visitor<SARRepeatEnd>,
	public visitor<SARNote>,
	public visitor<SARMusic>,
	public visitor<SARChord>,
	public visitor<SARVoice>
{
	private:
		streambeautifuller fOut;	///< the decorated output stream 
		long	fChordNotes;	///< count of notes, should always be 0 outside a chord, used for notes separator
		long	fVoicesCount;	///< count of voices, used for voices separator
		int		fInsideTag;		///< only used inside a chord for notes separator purpose
		bool	fVisitAuto;		///< control auto elements visite

		enum { kMaxLine=70 };	///< the maximum length of a line
		
		void barline ( Sguidotag& tag );

    public:
				gmnvisitor(std::ostream& stream, bool visitauto=false) 
					: fOut(stream, kMaxLine), fChordNotes(0), 
					  fVoicesCount(1), fInsideTag(0), fVisitAuto(visitauto) {}

		virtual void visitStart ( Sguidotag& tag );
		virtual void visitStart ( SARNote& tag );
		virtual void visitStart ( SARMusic& tag );
		virtual void visitStart ( SARChord& tag );
		virtual void visitStart ( SARVoice& tag );
		virtual void visitStart ( SARBar& tag );
		virtual void visitStart ( SARRepeatBegin& tag );
		virtual void visitStart ( SARRepeatEnd& tag );

		virtual void visitEnd ( Sguidotag& tag );
		virtual void visitEnd ( SARMusic& tag );
		virtual void visitEnd ( SARChord& tag );
		virtual void visitEnd ( SARVoice& tag );

};

/*! @} */

} // namespace

#endif
