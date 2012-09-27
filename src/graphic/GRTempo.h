#ifndef GRTempo_H
#define GRTempo_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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
		virtual void print() const;
		virtual unsigned int getTextAlign() const;

	protected:
		TYPE_DURATION getDuration (const char * str) const;
		float	DrawNote( VGDevice & hdc, const TYPE_DURATION & noteDur, float xOffset, float yOffset ) const;
		void	DrawText( VGDevice & hdc,  const char * cp, float xOffset, float yOffset, float * outWidth = 0 ) const;

	private:
		static const VGFont* mFont;		
};

#endif
