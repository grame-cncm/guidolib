#ifndef GRRepeatBegin_H
#define GRRepeatBegin_H

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

#include "ARBar.h"
#include "GRBar.h"
#include "GRDefine.h"
#include "GRVisitor.h"

class ARRepeatBegin;


/** \brief The right facing repeat bar sign.
*/
class GRRepeatBegin : public GRTagARNotationElement
    //class GRRepeatBegin : public GRBar
{
public:
              // GRRepeatBegin( ARRepeatBegin * ar, GRStaff * inStaff, const TYPE_TIMEPOSITION & inTimePos );
                 GRRepeatBegin ( const ARRepeatBegin * arrb, GRStaff * inStaff, bool p_ownsar = false);
    virtual 	~GRRepeatBegin () {}

	void accept (GRVisitor& visitor);

	void setPosFrom( float posy );
	void setPosTo( float posy );

	const ARRepeatBegin*	getARRepeatBegin() const;
	virtual unsigned int    getTextAlign() const;
    virtual const NVPoint & getReferencePosition() const { return refpos; }
    virtual void            OnDraw(VGDevice & hdc ) const;
    virtual void            GetMap(GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
    virtual void            updateBoundingBox();
    virtual void            tellPosition(GObject * caller, const NVPoint & newPosition);
    virtual void            setHPosition( float nx);

	const ARBar::TRanges	getRanges() const;
	void					setRanges(const GRBar::TRanges& r)	{ fRanges = r; }

protected:
	bool	isSystemSlice() const;
	void	DrawDots( VGDevice & hdc ) const;

    static NVPoint refpos;
	float   fBaseThickness;
	int 	fLinesCount;
	float   fSize;
	float   fStaffThickness;
	float 	fDx = 0;
	float 	fDy = 0;
private:
	float	getXOffset() const;
	GRBar::TRanges	fRanges;
};

#endif
