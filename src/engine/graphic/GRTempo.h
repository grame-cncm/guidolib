#ifndef GRTempo_H
#define GRTempo_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRTagARNotationElement.h"

class ARTempo;
class GRStaff;
class VGDevice;
class VGFont;

/** \brief Graphical representation of the tempo.
*/

class GRTempo : public GRTagARNotationElement
{
	public:
		using GRNotationElement::getDuration;
		
					 GRTempo(GRStaff *, const ARTempo * );
		virtual 	~GRTempo() {}
		
		virtual void			OnDraw( VGDevice & dev ) const;
		virtual unsigned int	getTextAlign() const;
		virtual const GRTempo *	isGRTempo() const		{ return this; }

	private:
		TYPE_TIMEPOSITION fDate;

		float	DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const;
		float	DrawText( VGDevice & hdc,  const char * cp, float xOffset, float yOffset) const;
	
		unsigned int 	getSymbol	(const TYPE_DURATION & noteDur) const;
		unsigned int 	getFlags	(const TYPE_DURATION & noteDur) const;
		float 			getXPos		() const;
		float 			getYAlign	(float fsize) const;
		float 			getXAlign	() const;
	
		const VGFont* fFont = 0;
		const VGFont* fMusicFont = 0;
		std::string   fFormat;
		float   	  fNoteScale = 1.f;		// used to scale the note according to the font size
		float   	  fYAlign = 0.f;		// used as note Y offset acording to text vertical align
		float   	  fXAlign = 0.f;		// used as note X offset acording to text horizontal align

		unsigned int  fTextAlign;
};

#endif
