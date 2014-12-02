#ifndef GRRepeatBegin_H
#define GRRepeatBegin_H

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

class ARRepeatBegin;


/** \brief The right facing repeat bar sign.
*/
class GRRepeatBegin : public GRTagARNotationElement
    //class GRRepeatBegin : public GRBar
{
public:
              // GRRepeatBegin( ARRepeatBegin * ar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos );
                 GRRepeatBegin(ARRepeatBegin * arrb, bool p_ownsar = false);
    virtual 	~GRRepeatBegin();

    virtual unsigned int    getTextAlign() const;
    virtual const NVPoint & getReferencePosition() const { return refpos; }
    virtual void            OnDraw(VGDevice & hdc ) const;
    virtual void            GetMap(GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
    virtual void            updateBoundingBox();
    virtual void            tellPosition(GObject * caller, const NVPoint & newPosition);
    virtual void            setHPosition( float nx);

protected:
    static NVPoint refpos;
           float   fBaseThickness;
           int     fLineNumber;
           float   fSize;
           float   fStaffThickness;
};

#endif
