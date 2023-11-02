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
				 ARTab( int string, const char* disp) : ARNote( "tab", 0, 0, 1, 4, 100 ),
					fString(string), fDisplay(disp) {
						setPitch (string2pitch (string));
						setOctave(string2octave(string));
					}
				 ARTab( const ARTab* tab, bool istied) : ARNote( *tab, istied ),
					fString(tab->getString()), fDisplay(tab->getDisplay()), fTied(istied) {}
		virtual ~ARTab() {}

		virtual ARNote*	Clone (bool istied = false ) const	{ return new ARTab(this, istied); }
		virtual ARMusicalObject * Copy() const 				{ return new ARTab(this, true); }

		virtual const char*	getTagName() const		{ return "ARTab"; };
		virtual std::string getGMNName() const		{
			std::stringstream sstr; sstr << "s" << fString << '"' << fDisplay << '"' << '*' << getDuration(); return sstr.str(); }
    
		virtual ARTab*			isARTab()           { return this; }
		virtual const ARTab*	isARTab() const     { return this; }
		
		int 		getString() const	{ return fString; }
		std::string getDisplay() const	{ return fDisplay; }
		bool 		isTied() const		{ return fTied; }
	
	private:
		int string2pitch (int string) {
			switch (string) {
				case 1 : return NOTE_A;
				case 2 : return NOTE_F;
				case 3 : return NOTE_D;
				case 4 : return NOTE_H;
				case 5 : return NOTE_G;
				case 6 : return NOTE_E;
			}
			return NOTE_E;
		}
		int string2octave (int string) {
			if (string > 2) return 1;
			return 2;
		}
		int		   fString;
		std::string fDisplay;
		bool		fTied = false;
};
