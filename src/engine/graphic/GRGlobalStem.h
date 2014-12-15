#ifndef GRGlobalStem_H
#define GRGlobalStem_H

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
#include "GRSystemTagInterface.h"

class ARShareStem;
class ARTStem;
class ARDisplayDuration;
class ARNoteFormat;

class GRStaff;
class GRFlag;
class GRPositionTag;
class GREvent;
class GRSingleNote;
class GRSystem;
class GRStdNoteHead;
class GRStem;


/** \brief Stem
*/
class GRGlobalStem : // public GRStem, 
	public GRPTagARNotationElement,
	public GRSystemTagInterface
{

	public:

        GRGlobalStem(GRStaff * inStaff,
            ARShareStem * pshare,
            ARTStem * curstemstate,
            ARDisplayDuration * curdispdur,
            ARNoteFormat * curnoteformat );

        virtual ~GRGlobalStem();

		int		getHighestAndLowestNoteHead( GRStdNoteHead ** highest, 
											 GRStdNoteHead ** lowest) const;

		virtual void setSize(float newsize);
        virtual void setMultiplicatedSize(float newMultiplicatedSize);
        virtual void setOffsetXY(float inOffsetX, float inOffsetY);
		virtual void setNoteStemLength( GREvent * ev, float inLen );
		virtual GRFlag * getGRFlag() const	{ return theFlag; }
		virtual GRStem * getGRStem() const	{ return theStem; }
		virtual int getNumFaehnchen() const;
		virtual void removeAssociation(GRNotationElement * grnot);

		virtual void checkPosition(const GRSystem * grsys);
		virtual GDirection getStemDir() const;

		virtual float changeStemLength( float inLen );
	    virtual void addAssociation(GRNotationElement * grnot);
        virtual void setHPosition( GCoord nx);

        virtual void OnDraw(VGDevice & hdc ) const;

        virtual void RangeEnd(GRStaff * inStaff);

        virtual GRNotationElement *  getFirstEl() 		{ return mFirstEl; }
        virtual void tellPosition(GObject *, const NVPoint &);

        virtual void setFlagOnOff(bool i);

        virtual void setStemDirection(GDirection dir);

        virtual NVPoint getStemStartPos() const;
        virtual NVPoint getStemEndPos() const;

        virtual bool getStemDirSet() const 		{ return stemdirset; }
        virtual bool getStemLengthSet() const 	{ return stemlengthset; }

        virtual const unsigned char * getColRef() const { return mColRef; }

        virtual float getStemLength() const;

	protected:

		void updateGlobalStem(const GRStaff * inStaff); 

		bool mFlagOnOff;
		bool stemdirset;
		bool stemlengthset;
		GDirection stemdir;

		GRStem * theStem;
		GRFlag * theFlag;
		ARTStem * stemstate;
		TYPE_DURATION dispdur;

		// mFirstEl is the first element in 
		// the \shareStem-Range. Only the
		// first event in the range gets a spring on its own ....
		GRNotationElement * mFirstEl;

		float mLowestY;
		float mHighestY;

//		unsigned char * colref;

		// - Lower note on the staff (to adjust stem length if it's a cross headnote)
		GRSingleNote *lowerNote;
		GRSingleNote *higherNote;

        float fStaffSize;
};

#endif
