#ifndef ARFeatheredBeam_H
#define ARFeatheredBeam_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "ARBeam.h"


/*@mkdoc

@group:Beaming

@tagname:\fBeam
@tagalias:\fbeamBegin fbeamEnd
@tagtype:R
@tagnotation:feathered beaming
@tagdesc
Feathered beaming is provided to support contemporary notation.

Notes grouped under a feathered beam should express their duration normally.
The spacing algorithm will take these durations into account to position them like any other note.
The number of beams is computed according to the first and last notes durations.
@tagend

@params:
@param:durations:string:used to control the number of beams:*none*:true
@param:drawDuration:boolean:displays the effective duration of the beamed group:false:true
@paramdesc
- **duration** may be used to override the default strategy to compute the number of beams.
It must be a string in the form "n/d,m/e" that express the number of beams for the begin and end of the group,
under the form of two rationals denoting durations. For example, "1/8,1/32" indicates that the group starts with
- **drawDuration** is used to display the actual total duration of the beamed group under the form of a rational value.

See the [Beaming](/examples/beaming/) example.
@paramend

*/

/** \brief Abstract representation of a feathered beam.
*/
class ARFeatheredBeam :  public ARBeam
{
//	  friend class GRBeam;

	public:
						 ARFeatheredBeam();
		virtual 		~ARFeatheredBeam() {}

		virtual bool MatchEndTag(const char * s);

		virtual const char*	getParamsStr() const	{ return kARFeatheredBeamParams; };
		virtual const char*	getTagName() const		{ return "ARFeatheredBeam"; };
		virtual std::string getGMNName() const		{ return "\\featheredBeam"; };

		virtual void setTagParameters (const TagParameterMap& params);

		bool isDurationsSet() const			{ return fDurationsSet;}
		bool drawDuration() const			{ return fDrawDur;}
		int  getFirstBeaming() const		{ return fBeams.first;}
		int  getLastBeaming() const			{ return fBeams.second;}
		TYPE_TIMEPOSITION getBeginTimePosition () const	{ return fBeginTimePos;}
		TYPE_TIMEPOSITION getEndTimePosition () const	{ return fEndTimePos;}

		void setBeginDuration( const TYPE_DURATION b)			{ fBeginDur = b;}
		void setEndDuration( const TYPE_DURATION e)				{ fEndDur = e;}
		void setBeginTimePosition( const TYPE_TIMEPOSITION bp)	{ fBeginTimePos = bp;}
		void setEndTimePosition( const TYPE_TIMEPOSITION ep)	{ fEndTimePos = ep;}


		const ARMusicalObject *isARFeatheredBeam() const  { return this; }
	
		void findDefaultPoints();

	private:

		bool fDrawDur;
		bool fDurationsSet;
		std::pair<int,int>  fBeams;
		TYPE_DURATION		fBeginDur;
		TYPE_DURATION		fEndDur;
		TYPE_TIMEPOSITION	fBeginTimePos;
		TYPE_TIMEPOSITION	fEndTimePos;

		void findPoints(std::string points); 
		void convertDurationToBeams(float valor, float valor2);
};

#endif
