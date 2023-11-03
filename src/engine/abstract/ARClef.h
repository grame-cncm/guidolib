#ifndef ARClef_H
#define ARClef_H

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

#include <string>
#include <map>

#include "TagParameterStrings.h"
#include "ARMTParameter.h"

/*@mkdoc

@group:Clef Key Meter

@tagname:\clef
@tagalias:
@tagtype:P
@tagnotation:clef signs
@tagdesc
@tagend

@params:
@param:type:string:bass, basso, f, f5, f4, f3, f2, f1, <br />tenor, c, c5, c4, c3, c2, c1, c0, <br />alto, violin, treble, <br />g, g5, g4, g3, g2, g1, <br />gg, gg5, gg4, gg3, gg2, gg1, <br />perc, perc5, perc4, perc3, perc2, perc1, <br />TAB, <br />none or off:treble:false
@paramdesc
- bass, basso, f, f4 are equivalent and represent a standard F clef on the 4th staff line
- f1, f2, f3, f5 represent a f clef on the 1st, 2nd and 5th staff line
- alto, c, c3 are equivalent and represent a bratsche clef on the 3rd staff line
- c4 and tenor are equivalent and represent a bratsche clef on the 4th staff line
- c1, c2, c3, c5 represent a bratsche clef on the 1st, 2nd, 3rd and 5th staff line
- c0 represent a bratsche clef on the 1st ledger line under the staff
- g, g2, treble, violin are equivalent and represent a G clef on the 2nd staff line
- g1, g3, g4, g5 represent a G clef on the 1st, 3rd, 4th and 5th staff line
- gg, gg2 are equivalent and represent a double G clef on the 2nd staff line
- gg1, gg3, gg4, gg5 represent a double G clef on the 1st, 3rd, 4th and 5th staff line
- perc, perc3 are equivalent and represent a percussion clef on the 3rd staff line
- perc1, perc2, perc4, perc5 represent a percussion clef on the 1st, 2nd, 4th and 5th staff line
- TAB for guitar tablatures (expects a 'TAB' [staff format](../Layout/#staffformat))
- none, off prevents the clef to be displayed

See the [Clefs](@EXAMPLES/clefkeymeter/) example.
@paramend

*/

/** \brief Abstract representation of a staff clef.
*/
class ARClef : public ARMTParameter
{
	public:
		enum cleftype  { UNDEFINED, OFF, AUTO, VIOLIN, BASS, BRATSCHE, PERC, DOUBLEG, TAB };
		enum octavatype  { LOW15 = -2, LOW8 = -1, NONE = 0, HIGH8 = 1, HIGH15 = 2 };

						 ARClef();
						 ARClef(const ARClef & clef);
						 ARClef(const TYPE_TIMEPOSITION & timeposition);
						 ARClef(const std::string& theName);

		virtual 		~ARClef() {}

		virtual bool 	IsStateTag() const			{ return true;}

		virtual int	 		getOrder() const		{ return kClefOrder; }
		virtual const char*	getParamsStr() const	{ return kARClefParams; };
		virtual const char*	getTagName() const		{ return "ARClef"; };
		virtual std::string getGMNName() const		{ return "\\clef"; };

		virtual bool operator==(const ARClef & clef) const;

		virtual void setTagParameters (const TagParameterMap& params);
		void				setName(const std::string& theName);
		const std::string&	getName() const	{ return fName; }

	
		// staffline where the clef starts
		int			getLine() const 		{ return fStaffLine; }
		cleftype	getClefType() const 	{ return fClef;  }
		octavatype	getOctavaType() const 	{ return fOctava; }
		ARMusicalObject *isARClef()			{ return this; }

	private:
		void		buildMap ();
		std::string decodeOctava (const std::string& name);

		std::string	fName;

		cleftype 	fClef;
		int 		fStaffLine; // Die Notenlinie
		octavatype 	fOctava;

		static std::map<std::string, std::pair<cleftype,int> > fClefsMap;
};

#endif
