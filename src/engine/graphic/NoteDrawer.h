
#pragma once

/*
  GUIDO Library
  Copyright (C) 2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "NVPoint.h"
#include "defines.h"

class VGDevice;
class VGFont;

/** \brief Utility to draw a note.
*/

class NoteDrawer
{
	public:
					 NoteDrawer(const VGFont * font, const NVPoint& pos, float align );
		virtual 	~NoteDrawer() {}
		
			float			DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const;
	static 	float 			GetScaling (float textfontsize );
	static 	const VGFont* 	GetMusicFont (float scale );

	private:
		unsigned int 	getSymbol	(const TYPE_DURATION & noteDur) const;
		unsigned int 	getFlags	(const TYPE_DURATION & noteDur) const;
	
		const VGFont* 	fFont = 0;
		NVPoint			fPosition;
		float   	  	fYAlign = 0.f;			// used as Y offset acording to the context vertical align
};
