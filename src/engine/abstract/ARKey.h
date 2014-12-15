#ifndef ARKey_H
#define ARKey_H

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

#include "GuidoDefs.h"

#include "ARMTParameter.h"

/** \brief Key signature
*/
class ARKey : public ARMTParameter
{
	void	getKeyArray		(std::string inString);
	void	newgetKeyArray	(const std::string& inString);
	int		getNote			(const char*& ptr) const;
	bool	getOctave		(const char*& ptr, int& oct) const;
	float	getAccidental	(const char*& ptr) const;

public:
					 ARKey(const TYPE_TIMEPOSITION & timeposition);
      				 ARKey(int p_keynumber);
					 ARKey();
					 ARKey(const ARKey & key);
		virtual 	~ARKey() {}

	  	virtual bool operator==(const ARKey & k) const;
	  	virtual bool IsStateTag() const;

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

				int	 getKeyNumber() const		 { return fKeyNumber; }
				void setKeyNumber(int newnumber) { fKeyNumber = newnumber; }

		bool mIsFree; // True if accidental free specified

		virtual void getOctArray(int *) const;
		virtual void getFreeKeyArray(float *) const;
        

        /**** Function to avoid dynamic_cast ****/
        ARMusicalObject *isARKey() { return this; }
        /****************************************/

  protected:
	
	static ListOfTPLs ltpls;

		int		fKeyNumber; // >0 = nr of #, < 0 = nr of &
		float	fAccarray [NUMNOTES];
		int		fOctarray [NUMNOTES];
};

#endif

