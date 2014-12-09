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
									secondLeftHead(0), firstLeftHead(0),
									secondRightHead(0), firstRightHead(0), accidentalRight(0),
									sizeLeft(1), sizeRight(1), leftNoteDX(0), 
									rightNoteDX(0), leftNoteDY(0), rightNoteDY(0)
									 { }
			GRStaff * staff;
			bool openRight;
			bool openLeft;
			GRStdNoteHead * secondLeftHead;
			GRStdNoteHead * firstLeftHead;
			GRStdNoteHead * secondRightHead;
			GRStdNoteHead * firstRightHead;
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
				virtual GRNotationElement * getStartElement(GRStaff * grstaff) const;
		virtual GRNotationElement * getEndElement(GRStaff * grstaff) const;
		virtual void BreakTag(GRStaff * grstaff, GuidoPos & assocpos);
		virtual void setPrevGlissando( GRGlissando * prev);
		virtual GRGlissando * getPrevGlissando(){return prevGRGlissando;}
		GRGlissandoContext * getContext(){return &fglissContext;}
		void	setHidden(){hidden = true;}
		bool	isFilled(){return filled;}
	
		
	protected:
		
		virtual void updateGlissando( GRStaff * grstaff );
		virtual	void getGlissandoBeginningContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse );
		virtual void getGlissandoEndingContext( GRGlissandoContext * ioContext, GRSystemStartEndStruct * sse );
		//bool wavy;
		bool fill;
		bool hidden;
		bool filled;
		GRNotationElement * flaststartElement;

	private:
		void compareAccidentals(GRSystemStartEndStruct * sse, bool * isUp, bool * isDown);
		GRSystemStartEndStruct * initGRGlissando( GRStaff * grstaff );
		GRGlissandoContext fglissContext;
		GRGlissandoSaveStruct * fglissInfos;
		GRGlissando * prevGRGlissando;
};

#endif
