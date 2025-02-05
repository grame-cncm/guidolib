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

#ifndef __gmnreader__
#define __gmnreader__

#include <string>

#include "ARTypes.h"
#include "guidorational.h"

//class rational;

namespace guido 
{

//class Sguidoelement;

//______________________________________________________________________________
/*!
\brief	An abstract class supporting the parser interface.
*/
class gmnreader   // : public singleton
{ 
	public:
		virtual ~gmnreader() {}
		virtual Sguidoelement* newComment(const std::string&, bool multiline) = 0;
		virtual Sguidoelement* newScore() = 0;
		virtual Sguidoelement* newVoice() = 0;
		virtual Sguidoelement* newChord() = 0;
		virtual Sguidoelement* newRest(const rational * r, long dots) = 0;
		virtual Sguidoelement* newNote(const std::string& name, long accidentals, long octave, const rational * r, long dots) = 0;
		virtual Sguidoelement* newTag(const std::string&, long id) = 0;
		virtual Sguidoattribute* newAttribute(long value) = 0;
		virtual Sguidoattribute* newAttribute(float value) = 0;
		virtual Sguidoattribute* newAttribute(const std::string& value, bool quote) = 0;
		virtual int error(const char * msg, int lineno) = 0;
};

} // namespace

#endif
