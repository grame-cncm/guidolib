#ifndef ARBarFormat_H
#define ARBarFormat_H

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

#include <vector>

#include "ARMTParameter.h"
#include "ARBar.h"

/*@mkdoc

@group:Barlines

@tagname:\barFormat
@tagalias:
@tagtype:P
@tagnotation:describes the span of a barline.
@tagdesc
@tagend

@params:
@param:style:string:system or staff:staff:true
@param:range:string:only for system style - indicates the range of staves covered by the bar:*none*:true
@paramdesc
- the **range** string must be in the form *"n-m"* where 'n' and 'm' are integer numbers denoting staff indexes (starting from 1)

See the [Barlines](@EXAMPLES/barlines/) example.
@paramend

*/

/** \brief the bar format tag parameter.
*/
class ARBarFormat : public ARMTParameter
{
	public:
		typedef std::vector<std::pair<int, int> >	TRanges;
		enum style { kStyleStaff, kStyleSystem, kStyleRange };

				 ARBarFormat();
				 ARBarFormat(const ARBarFormat&);
		virtual ~ARBarFormat() {}

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARBarFormatParams; };
		virtual const char*	getTagName() const		{ return "ARBarFormat"; };
		virtual std::string getGMNName() const		{ return "\\barFormat"; };

		style					getStyle() const	{ return fStyle; }
		const ARBar::TRanges&	getRanges() const	{ return fRanges; }
		virtual bool			IsStateTag() const	{ return true; }

	protected:
		ARBar::TRanges getRanges (const NVstring &str);

		style			fStyle;
		ARBar::TRanges	fRanges;
};

#endif 
