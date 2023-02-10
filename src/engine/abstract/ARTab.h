#pragma once

/*
  GUIDO Library
  Copyright (C) 2023 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "ARNote.h"
#include <sstream>

/** \brief Representation of a guitar tablature.
*/
class ARTab : public ARNote
{
	public:
				 ARTab( int string, const char* disp) :
					ARNote( "tab", 0, 0, 1, 4, 100 ),
					fString(string), fDisplay(disp) {}
		virtual ~ARTab() {}

		virtual const char*	getTagName() const		{ return "ARTab"; };
		virtual std::string getGMNName() const		{
			std::stringstream sstr; sstr << "s" << fString << '"' << fDisplay << '"' << '*' << getDuration(); return sstr.str(); }
    
		virtual ARTab*			isARTab()           { return this; }
		virtual const ARTab*	isARTab() const     { return this; }
		
		int 		getString() const	{ return fString; }
		std::string getDisplay() const	{ return fDisplay; }
	
	private:
		int		   fString;
		std::string fDisplay;
};
