#ifndef GRGlissando_H
#define GRGlissando_H

/*
  GUIDO Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRPTagARNotationElement.h"

class ARGlissando;
class GRStdNoteHead;

/*
class GRStaff;
class GRSystem;
class GRGlobalStem;
class GRSystemStartEndStruct;

*/

class GRGlissando : public GRPTagARNotationElement
{	
	public:
	
	class GRGlissandoSaveStruct : public GRPositionTag::GRSaveStruct
	{
		public:
			GRGlissandoSaveStruct() : numPoints(4) { }
			
			virtual ~GRGlissandoSaveStruct() { }
			int numPoints;
			
			NVPoint points[4];	
			NVPoint position;
	};
	
	class GRGlissandoContext
	{
		public:
				GRGlissandoContext() : staff(0), openRight(false), openLeft(false),
									topLeftHead(0), bottomLeftHead(0),
									topRightHead(0), bottomRightHead(0), accidentalRight(0),
									sizeLeft(1), sizeRight(1), leftNoteDX(0), rightNoteDX(0),
									leftNoteDY(0), rightNoteDY(0)
									 { }
			GRStaff * staff;
			bool openRight;
			bool openLeft;
			GRStdNoteHead * topLeftHead;
			GRStdNoteHead * bottomLeftHead;
			GRStdNoteHead * topRightHead;
			GRStdNoteHead * bottomRightHead;
			GRAccidental * accidentalRight;
			float sizeLeft;
			float sizeRight;
			float leftNoteDX;
			float rightNoteDX;
			float leftNoteDY;
			float rightNoteDY;
	};

	public:
					GRGlissando( GRStaff * grstaff = 0); 
					GRGlissando( GRStaff * grstaff, ARGlissando * abstractRepresentationOfGlissando );


		virtual 	~GRGlissando();

		virtual void OnDraw( VGDevice & hdc ) const;
										
		virtual void removeAssociation(GRNotationElement * el );
		virtual void addAssociation(GRNotationElement * el);

		virtual void tellPosition(GObject * caller, const NVPoint & newPosition);
		virtual void print() const;
		virtual GRNotationElement * getStartElement(GRStaff * grstaff) const;
		virtual GRNotationElement * getEndElement(GRStaff * grstaff) const;
		virtual void ResumeTag(GRStaff * grstaff, GuidoPos assocpos);
		virtual void BreakTag(GRStaff * grstaff, GuidoPos & assocpos);
	
		
	protected:
		
		virtual void updateGlissando( GRStaff * grstaff );

		//virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct();

		//virtual GRSystemStartEndStruct * prepareSSEStructForGlissando( const GRStaff * inStaff );

		virtual	void getGlissandoBeginingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse );
		virtual void getGlissandoEndingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse );
		//virtual	GRGlobalStem * findGlobalStem( GRSystemStartEndStruct * sse, GRNotationElement * stemOwner );
		bool wavy;

		GRNotationElement * laststartElement;
		GuidoPos laststartpos;

	private:
		
			GRSystemStartEndStruct * initGRGlissando( GRStaff * grstaff );
};

#endif
