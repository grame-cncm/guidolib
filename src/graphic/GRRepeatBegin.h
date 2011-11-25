#ifndef GRRepeatBegin_H
#define GRRepeatBegin_H

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

#include "GRBar.h"
#include "GRDefine.h"

class ARRepeatBegin;


/** \brief not yet documented
*/
class GRRepeatBegin : public GRTagARNotationElement
//class GRRepeatBegin : public GRBar
{
	public:
//		GRRepeatBegin( ARRepeatBegin * ar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos );
					GRRepeatBegin(ARRepeatBegin * arrb, bool p_ownsar = false);
		virtual 	~GRRepeatBegin();

	
		virtual void	print() const {}
		virtual unsigned int getTextAlign() const;
		virtual const NVPoint & getReferencePosition() const { return refpos; }
		virtual void	OnDraw(VGDevice & hdc ) const;
		virtual void	GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void	updateBoundingBox();
		virtual void	tellPosition(GObject * caller, const NVPoint & newPosition);
		virtual void	setHPosition( float nx);

	protected:
		static NVPoint refpos;

};

#endif
