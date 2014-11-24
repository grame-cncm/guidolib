#ifndef GRCrescendo_H
#define GRCrescendo_H

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

#include "GRPTagARNotationElement.h"

class ARCrescendo;
class GRStaff;
class GRStdNoteHead;


class GRCrescendo : public GRPTagARNotationElement
{
public:

    class GRCrescendoSaveStruct : public GRPositionTag::GRSaveStruct
    {
    public:
        GRCrescendoSaveStruct() : numPoints(3), thickness(4) { }

        virtual ~GRCrescendoSaveStruct() { }
        int numPoints;

        /* Params here */
        NVPoint points[3];
        float thickness;
        unsigned int fMarkingSymbol;
    };

    class GRCrescendoContext
	{
		public:
				GRCrescendoContext() : staff(0), leftHead(0), rightHead(0), leftNoteDX(0), 
									rightNoteDX(0) { }

			GRStaff * staff;
			bool openRight;
			bool openLeft;
			GRStdNoteHead *leftHead;
			GRStdNoteHead *rightHead;
			float leftNoteDX;
			float rightNoteDX;
	};

    GRCrescendo(GRStaff * grstaff = 0);
    GRCrescendo(GRStaff * grstaff, ARCrescendo * abstractRepresentationOfCrescendo);
    virtual			~GRCrescendo();

    virtual void	OnDraw(VGDevice & hdc) const;

	virtual void    addAssociation(GRNotationElement * el);

    virtual void    tellPosition(GObject * caller, const NVPoint & newPosition);

protected:
    virtual void updateCrescendo(GRStaff *grstaff);
    virtual	void getCrescendoBeginingContext(GRCrescendoContext *ioContext, GRSystemStartEndStruct * sse);
	virtual void getCrescendoEndingContext(GRCrescendoContext *ioContext, GRSystemStartEndStruct * sse);

    GRNotationElement * flaststartElement;

private:
    GRSystemStartEndStruct *initGRCrescendo(GRStaff *grstaff);

    GRCrescendoSaveStruct * fCrescInfos;
    GRCrescendoContext fCrescContext;
};

#endif
