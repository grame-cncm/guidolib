#ifndef ARBeam_H
#define ARBeam_H

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

/*@mkdoc

@group:Beaming

@tagname:\beam
@tagalias:\bm<br/>\b<br/>\beamBegin beamEnd
@tagtype:R
@tagnotation:notes beaming
@tagdesc
Beaming is automatically computed according to the current meter. Manual beaming is always possible.
@tagend

@params:
@param:dy1:unit:adjust the left beaming position:*none*:true
@param:dy2:unit:adjust the right beaming position:*none*:true
@param:dy:unit:similar to dy1=dy and dy2=dy:*none*:true
@paramdesc
*dy* parameters adjust the position of the beams relative to the note head.

Known issue: *dy* fails when the stems are down.

See the [Beaming](/examples/beaming/) example.

@paramend

*/

/** \brief Abstract representation of a beam.
*/
class ARBeam :  public ARMTParameter, public ARPositionTag
{
	public:
				 ARBeam();
		virtual ~ARBeam() {}

		bool isGuidoSpecBeam() const;
		bool isFeatheredBeam() const		{return fFeathered;}

		virtual bool isFullBeaming() const	{ return true;}
		virtual bool MatchEndTag(const char * s);

		virtual const char*	getParamsStr() const	{ return kARBeamParams; };
		virtual const char*	getTagName() const		{ return "ARBeam"; };
		virtual std::string getGMNName() const		{ return "\\beam"; };

		virtual void setTagParameters (const TagParameterMap& params);

		const TagParameterFloat * getDx1() const	{ return dx1; }
		const TagParameterFloat * getDy1() const	{ return dy1; }
		const TagParameterFloat * getDx2() const	{ return dx2; }
		const TagParameterFloat * getDy2() const	{ return dy2; }
		const TagParameterFloat * getDx3() const	{ return dx3; }
		const TagParameterFloat * getDy3() const	{ return dy3; }
		const TagParameterFloat * getDx4() const	{ return dx4; }
		const TagParameterFloat * getDy4() const	{ return dy4; }

	protected:
		bool fFeathered;
	
	private:
		const TagParameterFloat * dx1;
		const TagParameterFloat * dy1;
		const TagParameterFloat * dx2;
		const TagParameterFloat * dy2;
		const TagParameterFloat * dx3;
		const TagParameterFloat * dy3;
		const TagParameterFloat * dx4;
		const TagParameterFloat * dy4;
};

#endif

