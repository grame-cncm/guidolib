#ifndef GRDiminuendo_H
#define GRDiminuendo_H

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

class ARDiminuendo;
class GRStaff;
class GRStdNoteHead;


class GRDiminuendo : public GRPTagARNotationElement
{
public:

    class GRDiminuendoSaveStruct : public GRPositionTag::GRSaveStruct
    {
    public:
        GRDiminuendoSaveStruct() : numPoints(3), thickness(4) { }

        virtual ~GRDiminuendoSaveStruct() { }
        int numPoints;

        /* Params here */
        NVPoint points[3];
        float thickness;
        unsigned int fMarkingSymbol;
    };

    class GRDiminuendoContext
	{
		public:
				GRDiminuendoContext() : staff(0), leftHead(0), rightHead(0), leftNoteDX(0), 
									rightNoteDX(0) { }

			GRStaff * staff;
			bool openRight;
			bool openLeft;
			GRStdNoteHead *leftHead;
			GRStdNoteHead *rightHead;
			float leftNoteDX;
			float rightNoteDX;
	};

    GRDiminuendo(GRStaff * grstaff = 0);
    GRDiminuendo(GRStaff * grstaff, ARDiminuendo * abstractRepresentationOfDiminuendo);
    virtual			~GRDiminuendo();

    virtual void	OnDraw(VGDevice & hdc) const;

	virtual void    addAssociation(GRNotationElement * el);

    virtual void    tellPosition(GObject * caller, const NVPoint & newPosition);

protected:
    virtual void updateDiminuendo(GRStaff *grstaff);
    virtual	void getDiminuendoBeginingContext(GRDiminuendoContext *ioContext, GRSystemStartEndStruct * sse);
	virtual void getDiminuendoEndingContext(GRDiminuendoContext *ioContext, GRSystemStartEndStruct * sse);

    GRNotationElement * flaststartElement;

private:
    GRSystemStartEndStruct *initGRDiminuendo(GRStaff *grstaff);

    GRDiminuendoSaveStruct * fDimInfos;
    GRDiminuendoContext fDimContext;
};

#endif
