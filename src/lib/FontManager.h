#ifndef __FontManager__
#define __FontManager__

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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
