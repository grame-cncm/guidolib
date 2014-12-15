#ifndef GRSingleRest_H
#define GRSingleRest_H

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

class ARRest;

#include "GRRest.h"

/** \brief Graphical representation of a rest.
*/
class GRSingleRest : public GRRest
{
	public:
	
		enum TYPES 
		{
		 	P0 		= kNoneSymbol,
			P1 		= kP1Symbol,
			P2		= kP2Symbol,
			P4 		= kP4Symbol,	// 165, // SCR_4REST, '\xA5', 
			P8 		= kP8Symbol,	//97, // SCR_8TREST, 'a', 
			P16 	= kP16Symbol,	// 64, // SCR_16REST,  
			P32 	= kP32Symbol,	//174, // SCR_32REST, '\xAE', 
			P64 	= kP64Symbol,	//108, // SCR_64REST, 'l',
			P128 	= kP128Symbol	// 47 // SCR_128REST '\x2F'
		};
	
				GRSingleRest(GRStaff * grstf, const TYPE_DURATION & theDuration);
				GRSingleRest(GRStaff * grstf, ARRest * abstractRepresentationOfRest);
				GRSingleRest(GRStaff * grstf, ARRest * abstractRepresentationOfRest,
								const TYPE_TIMEPOSITION & relativeTimePositionOfGRRest,
								const TYPE_DURATION & durationOfGRRest,
								const TYPE_DURATION p_durtemplate = DURATION_0);
	
		virtual ~GRSingleRest();

		virtual void setFillsBar(bool value,GRNotationElement * bar1, 
											GRNotationElement * bar2);
		
		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
		
		virtual void setPosition(const NVPoint & point);
		virtual void updateBoundingBox();
		
		virtual const TYPE_DURATION & getDurTemplate() const { return durtemplate; }
		
		virtual int getNumFaehnchen() const;
		virtual void tellPosition(GObject * caller,const NVPoint & newPosition);

		virtual const NVPoint & getReferencePosition() const;
		virtual unsigned int getTextAlign() const;

		virtual int adjustLength(const TYPE_DURATION & ndur);

		void setRestFormat(ARRestFormat * restfrmt);
	
	protected:
	
	void createRest(const TYPE_DURATION & duration);
	TYPES mType;
	TYPE_DURATION durtemplate;

//	GDirection mStemDir;
//	float  mStemLen;

	static NVPoint sRefpos;

	GRNotationElement  * firstbar;
	GRNotationElement  * secondbar;
};

#endif
