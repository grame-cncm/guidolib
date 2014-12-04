#ifndef GRTempo_H
#define GRTempo_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

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
		
					GRTempo(GRStaff *, ARTempo * );
		virtual 	~GRTempo();
		
		virtual void OnDraw( VGDevice & dev ) const;
				virtual unsigned int getTextAlign() const;

	protected:
		TYPE_DURATION getDuration (const char * str) const;
		float	DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const;
		void	DrawText( VGDevice & hdc,  const char * cp, float xOffset, float yOffset, float * outWidth = 0 ) const;

	private:
		static const VGFont* mFont;		
};

#endif
