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

/** \brief Abstract representation of a staff clef.
*/
class ARClef : public ARMTParameter
{
	public:
		enum cleftype  { UNDEFINED, OFF, AUTO, VIOLIN, BASS, BRATSCHE, PERC, DOUBLEG };
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
