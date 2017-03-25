#ifndef GRTextHarmony_H
#define GRTextHarmony_H

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

#include <string>

#include "GRPTagARNotationElement.h"

class ARTextHarmony;
class GRStaff;
class GRRod;

/** \brief A chord symbol string.

	Can represent standard text, lyrics, fingering, label, cue-text, marks...

*/
class GRTextHarmony : public GRPTagARNotationElement
{
public:
    class GRTextSaveStruct : public GRPositionTag::GRSaveStruct
    {
    public:
        GRTextSaveStruct()  {}
        virtual ~GRTextSaveStruct() {};

        NVPoint position;
        NVRect boundingBox;
        std::string text;
    };

    GRTextHarmony( GRStaff *, ARTextHarmony * abstractRepresentationOfText );
    virtual 		~GRTextHarmony();

    virtual void 	removeAssociation( GRNotationElement * el );
    virtual void 	tellPosition( GObject * caller, const NVPoint & inPosition );
    virtual void 	addAssociation( GRNotationElement * el );

    virtual void 	OnDraw( VGDevice & hdc ) const;

    const ARTextHarmony * 	getARTextHarmony() const;

    virtual unsigned int getTextAlign() const { return mTextAlign; }

    virtual float 	getLeftSpace() const;
    virtual float 	getRightSpace() const;

    virtual void 	setPosition(const NVPoint & inPosition );
    virtual void 	setHPosition( float nx );
				void	mustFollowPitch( bool flag ) { mMustFollowPitch = flag; }

protected:

    virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct()
    { return new GRTextSaveStruct; }

    unsigned int mTextAlign;
    bool	mMustFollowPitch; // (when the text tag has a range)
};

#endif

