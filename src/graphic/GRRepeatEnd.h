#ifndef GRRepeatEnd_H
#define GRRepeatEnd_H

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

#include "GRBar.h"
#include "GRDefine.h"

//class ARRepeatEndRangeEnd; REM: virer ?
class ARRepeatEnd;


/** \brief The left facing repeat bar sign.
*/
// (JB) It's actually a range tag, so it must derive from Position Tag.
//class GRRepeatEnd : public GRPTagARNotationElement // was GRTagARNotationElement
//class GRRepeatEnd : public GRTagARNotationElement
class GRRepeatEnd : public GRTagARNotationElement
{
	public:

				 GRRepeatEnd(ARRepeatEnd *arre, bool p_ownsar = false);
		virtual	~GRRepeatEnd();
	
		virtual void	print() const { }
		virtual unsigned int getTextAlign() const;
		virtual void	updateBoundingBox();
		virtual void	setHPosition( float nx);
		virtual void	GetMap(GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		virtual void	OnDraw( VGDevice & hdc ) const;
        virtual void    tellPosition(GObject * caller, const NVPoint & newPosition);

		virtual const NVPoint & getReferencePosition() const { return refpos; }

	protected:
		static NVPoint refpos;
               float   fBaseThickness;
               int     fLineNumber;
               float   fStaffThickness;
               float   fSize;
};

#endif
