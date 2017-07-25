#ifndef GRTuplet_H
#define GRTuplet_H

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

#include "NVPoint.h"
#include "GRPTagARNotationElement.h"

class ARTuplet;

class GRStaff;
class GRSystem;
class GREvent;


/** \brief Graphical representation of a tuplet.
*/

class GRTuplet : public GRPTagARNotationElement
{
public:
    // enum bracestate { BRACENONE = 0, BRACELEFT = 1, BRACERIGHT = 2 };

    class GRTupletSaveStruct : public GRPositionTag::GRSaveStruct
    {
    public:
                 GRTupletSaveStruct()  {}
        virtual ~GRTupletSaveStruct() {}

        NVPoint  p1;
        NVPoint  p2;
        NVPoint  textpos;
    };

                 GRTuplet(GRStaff * inStaff, const ARTuplet * artuplet);
    virtual 	~GRTuplet();

    virtual void OnDraw( VGDevice & hdc ) const;
    virtual void tellPosition( GObject * caller, const NVPoint & inPos );

protected:
    const   ARTuplet * getARTuplet() const;
    virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct() { return new GRTupletSaveStruct; }

	NVstring	fText;
    GDirection	mDirection;

private:
	// computes the automatic tuplet position, returns true when done
	bool automaticPosition( GREvent * caller, const NVPoint & inPos, const GRStaff * staff, GRSystemStartEndStruct * sse );
    void manualPosition( GREvent * caller, const NVPoint & inPos, const GRStaff * staff, GRSystemStartEndStruct * sse);
	// gives the bounding box of the notes,
	// returns the preferred position (true for above) according to stems directions
	bool getElementsBoundingBox (NVRect& outRect) const;

    //	bracestate mBraceState;
    bool mShowLeftBrace;
    bool mShowRightBrace;
};

#endif
