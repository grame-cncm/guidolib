#ifndef ARFeatheredBeam_H
#define ARFeatheredBeam_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "ARBeam.h"
// #include "ARMusicalEvent.h"

class TagParameterInt;
//class GRBeam;

/** \brief Abstract representation of a feathered beam.
*/
class ARFeatheredBeam :  public ARBeam
{
//	  friend class GRBeam;

  public:
					ARFeatheredBeam();
	virtual 		~ARFeatheredBeam();

	virtual bool MatchEndTag(const char * s);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void setTagParameterList(TagParameterList & tpl);

	bool isDurationsSet(){return durationsSet;}
	bool drawDuration(){return drawDur;}
	int  getFirstBeaming(){return beams.first;}
	int  getLastBeaming(){return beams.second;}
	void setBeginDuration( const TYPE_DURATION b){beginDur = b;}
	void setEndDuration( const TYPE_DURATION e){endDur = e;}
	void setBeginTimePosition( const TYPE_TIMEPOSITION bp){beginTimePos = bp;}
	void setEndTimePosition( const TYPE_TIMEPOSITION ep){endTimePos = ep;}
	TYPE_TIMEPOSITION getBeginTimePosition (){return beginTimePos;}
	TYPE_TIMEPOSITION getEndTimePosition (){return endTimePos;}
	void findDefaultPoints();

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARFeatheredBeam() { return this; }
    /*****************************************/

	protected:
		static ListOfTPLs ltpls;

		bool drawDur;
		bool durationsSet;
		std::pair<int,int> beams;
		TYPE_DURATION beginDur;
		TYPE_DURATION endDur;
		TYPE_TIMEPOSITION beginTimePos;
		TYPE_TIMEPOSITION endTimePos;

		void findPoints(std::string points); 
		void convertDurationToBeams(float valor, float valor2);
};

#endif
