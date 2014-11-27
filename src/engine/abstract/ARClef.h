#ifndef ARClef_H
#define ARClef_H

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

#include <string>

#include "ARMTParameter.h"

/** \brief Abstract representation of a staff clef.
*/
class ARClef : 
// public ARMusicalObject,
	public ARMTParameter
{
public:
	enum cleftype  { OFF, AUTO, VIOLIN, BASS, BRATSCHE, PERC, DOUBLEG };
	enum octavatype  { LOW15 = -2, LOW8 = -1, NONE = 0, HIGH8 = 1, HIGH15 = 2 };

					 ARClef();
					 ARClef(const ARClef & clef);
					 ARClef(const TYPE_TIMEPOSITION & timeposition);
					 ARClef(const std::string& theName);

	virtual 		~ARClef();

	virtual bool 	IsStateTag() const;

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual bool operator==(const ARClef & clef);

	virtual void setTagParameterList(TagParameterList & theTagParameterList);

	void				setName(const std::string& theName);
	const std::string&	getName() const	{ return fName; }

	
	// staffline where the clef starts
	int			getLine() const 		{ return mStaffLine; }
	cleftype	getClefType() const 	{ return mClef;  }
	octavatype	getOctavaType() const 	{ return fOctava; }
        

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARClef() { return this; }
    /****************************************/

protected:
		static ListOfTPLs ltpls;

private:
	std::string	fName;

	cleftype 	mClef;
	int 		mStaffLine; // Die Notenlinie
	octavatype 	fOctava;
};

#endif
