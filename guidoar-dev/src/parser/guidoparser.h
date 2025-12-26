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

#ifndef __guidoparser__
#define __guidoparser__

#include <stdio.h>
#include "arexport.h"
#include "gmnreader.h"
#include "ARTypes.h"
#include "guidorational.h"

namespace guido 
{

class gar_export guidoparser : public gmnreader
{ 
	SARMusic fMusic;

	public:
				 guidoparser() {}
		virtual ~guidoparser() {}

		SARMusic parseFile  (FILE* fd);
		SARMusic parseFile  (const char* file);
		SARMusic parseString(const char* string);

		virtual Sguidoelement* newComment(const std::string&, bool multiline);
		virtual Sguidoelement* newScore();
		virtual Sguidoelement* newVoice();
		virtual Sguidoelement* newChord();
		virtual Sguidoelement* newRest(const rational * r, long dots);
		virtual Sguidoelement* newNote(const std::string& name, long accidentals, long octave, const rational * r, long dots);
		virtual Sguidoelement* newTag(const std::string&, long id);
		virtual Sguidoattribute* newAttribute(long value);
		virtual Sguidoattribute* newAttribute(float value);
		virtual Sguidoattribute* newAttribute(const std::string& value, bool quote);
		virtual int error(const char * msg, int lineno);
};

} // namespace

#endif
