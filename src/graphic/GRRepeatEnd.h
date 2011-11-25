#ifndef GRRepeatEnd_H
#define GRRepeatEnd_H

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
#include "GUIDOScoreMap.h"

class ARRepeatEndRangeEnd;
class ARRepeatEnd;


/** \brief The left facing repeat bar sign.
*/
// (JB) It's actually a range tag, so it must derive from Position Tag.
//class GRRepeatEnd : public GRPTagARNotationElement // was GRTagARNotationElement
//class GRRepeatEnd : public GRTagARNotationElement
class GRRepeatEnd : public GRBar
{
	public:

				 GRRepeatEnd( ARRepeatEnd * ar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos );
		virtual	~GRRepeatEnd();
	
		virtual void	print() const { }
		virtual unsigned int getTextAlign() const;
		virtual void	updateBoundingBox();
		virtual void	setHPosition( float nx);
		virtual void	GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void	OnDraw( VGDevice & hdc ) const;

		virtual const NVPoint & getReferencePosition() const { return refpos; }

	protected:
		static NVPoint refpos;

	private:
				void	InitRepeatEnd();
};

#endif
