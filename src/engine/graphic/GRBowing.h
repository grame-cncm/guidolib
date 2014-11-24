#ifndef GRBowing_H
#define GRBowing_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRPTagARNotationElement.h"

class ARBowing;

class GRStaff;
class GRSystem;
class GRGlobalStem;
class GRStdNoteHead;
class GRSystemStartEndStruct;


// - Graphic functions
void drawSlur( VGDevice & hdc, float x1, float y1, float x2, float y2, 
								float x3, float y3, float inflexion = 6 ); // was = 5

/** \brief Base class for ties and slurs.
*/
class GRBowing : public GRPTagARNotationElement
{
	
public:
	
	/** \brief Save informations needed to draw the bow 
			when different staves are needed, see GRPositionTag for comment.
	*/
	class GRBowingSaveStruct : public GRPositionTag::GRSaveStruct
	{
		public:
			GRBowingSaveStruct() : numoffsets(3), inflexion(6) { }
			
			virtual ~GRBowingSaveStruct() { }
			int numoffsets;
			
			NVPoint offsets[3];	
			NVPoint position;
			float inflexion;
	};
	
	class GRBowingContext	// (JB) new
	{
		public:
				GRBowingContext() : staff(0), openRight(false), openLeft(false),
									topLeftHead(0), bottomLeftHead(0),
									topRightHead(0), bottomRightHead(0),
									stemDirLeft(dirOFF), stemDirRight(dirOFF),
									leftChordStem(0), rightChordStem(0),
									curveDir(0)
									 { }
			GRStaff * staff;
			bool openRight;
			bool openLeft;
			GRStdNoteHead * topLeftHead;
			GRStdNoteHead * bottomLeftHead;
			GRStdNoteHead * topRightHead;
			GRStdNoteHead * bottomRightHead;
			int				stemDirLeft;
			int				stemDirRight;
			GRGlobalStem *  leftChordStem;
			GRGlobalStem *  rightChordStem;
			int				curveDir;	// -1: downward, 0: unspecified, 1: upward.
	};
	
	
	public:
					GRBowing( GRStaff * grstaff, GRNotationElement * startEl, GRNotationElement * endEl );
					GRBowing( GRStaff * grstaff = 0); // constructor without REAL abstract Element
					GRBowing( GRStaff * grstaff, ARBowing * abstractRepresentationOfBowing );
					// GRBowing( const GRBowing & inCopy );


		virtual 	~GRBowing();

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void updateBoundingBox();
										
		virtual void setOffset(int n_point, const NVPoint & p);
		virtual void removeAssociation(GRNotationElement * el );
		virtual void addAssociation(GRNotationElement * );

		virtual void tellPosition(GObject * caller, const NVPoint & newPosition);
				virtual GRNotationElement * getStartElement(GRStaff * grstaff) const;
		virtual GRNotationElement * getEndElement(GRStaff * grstaff) const;
		
		
	protected:

		virtual void drawSlur( VGDevice & hdc, NVPoint pstart, NVPoint pmid, NVPoint pend ) const;
		virtual void updateBow( GRStaff * grstaff );

		virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct();

		virtual GRSystemStartEndStruct * prepareSSEStructForBow( const GRStaff * inStaff );

		virtual	void getBowBeginingContext( GRBowingContext * ioContext, GRSystemStartEndStruct * sse );
		virtual void getBowEndingContext( GRBowingContext * ioContext, GRSystemStartEndStruct * sse );

		virtual	GRGlobalStem * findGlobalStem( GRSystemStartEndStruct * sse, 
												GRNotationElement * stemOwner );
		
		// - Positionning of the curve. 
		virtual void manualAnchorPoints( GRBowingContext * bowContext, 
										ARBowing * arBow, 
										GRSystemStartEndStruct * sse );

		virtual void automaticAnchorPoints( GRBowingContext * bowContext, 
										ARBowing * arBow, 
										GRSystemStartEndStruct * sse );

		virtual void automaticCurveDirection( GRBowingContext * bowContext, 
											  ARBowing * arBow, 
											  GRSystemStartEndStruct * sse );

		virtual void manualControlPoints( GRBowingContext * bowContext, 
										 ARBowing * arBow, 
										 GRSystemStartEndStruct * sse );


		virtual void automaticControlPoints( GRBowingContext * bowContext, 
										ARBowing * arBow, 
										GRSystemStartEndStruct * sse );

		virtual void applyAnchorPointsOffsets( GRBowingContext * bowContext, 
												ARBowing * arBow, 
												GRSystemStartEndStruct * sse );

	private:
		
			GRSystemStartEndStruct * initGRBowing( GRStaff * grstaff );
};

#endif

