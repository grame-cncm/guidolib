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
	virtual void print() const { }
	virtual void PrintName(std::ostream & os) const;
	virtual void setTagParameterList(TagParameterList & tpl);

	bool isDurationsSet(){return durationsSet;}
	bool drawDuration(){return drawDur;}
	int getFirstBeaming(){return beams.first;}
	int getLastBeaming(){return beams.second;}

	protected:

		static ListOfTPLs ltpls;

		bool drawDur;
		bool durationsSet;
		std::pair<int,int> beams;

		void findPoints(std::string points); 
};

#endif
