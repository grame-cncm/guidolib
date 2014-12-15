#ifndef __FontManager__
#define __FontManager__

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <vector>
#include <string>
#include "VGSystem.h"

class VGFont;
/** \brief Description of a font: name, font size and attributes.

	bold, underline, italic.
*/
class FontInfo 
{
	public:
				 FontInfo( const VGFont * fontRef, int size, const std::string& name, const std::string& attributes );
		virtual ~FontInfo();
		bool Compare( int size, const std::string& name, const std::string& attributes ) const;		

		const VGFont *	mFontRef;

	protected:
		int			mSize;
		std::string mName;
		std::string mAttrib;
};

typedef std::vector<FontInfo *> FontInfoList;
/** \brief Handles the list of all fonts used by the Guido Library.
*/
class FontManager
{
	static FontInfoList sFontList;

	public:
				 FontManager() {}
		virtual ~FontManager();

	static const VGFont * FindOrCreateFont(VGSystem* sys, int size, const std::string * iname = 0, const std::string * attributes = 0 );
	static const VGFont * FindOrCreateFont( int size, const std::string * iname = 0, const std::string * attributes = 0 );
	static void	 ReleaseAllFonts();

	static const VGFont * gFontScriab;
	static const VGFont * gFontText;
	static const char * kDefaultMusicFont;
	static const char * kDefaultTextFont;
};

#endif
