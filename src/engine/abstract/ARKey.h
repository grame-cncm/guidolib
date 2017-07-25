#ifndef ARKey_H
#define ARKey_H

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

#include "GuidoDefs.h"

#include "ARMTParameter.h"

/** \brief Key signature
*/
class ARKey : public ARMTParameter
{
	void	getKeyArray		(std::string inString);
	bool	getOctave		(const char*& ptr, int& oct) const;

	public:
					 ARKey(const TYPE_TIMEPOSITION & timeposition);
      				 ARKey(int p_keynumber);
					 ARKey();
					 ARKey(const ARKey & key);
		virtual 	~ARKey() {}

	  	virtual bool operator==(const ARKey & k) const;

	  	virtual bool IsStateTag() const				{ return true; }
		virtual int	 getOrder() const				{ return kKeyOrder; }
		virtual const char*	getParamsStr() const	{ return kARKeyParams; };
		virtual const char*	getTagName() const		{ return "ARKey"; };
		virtual std::string getGMNName() const		{ return "\\key"; };

		virtual void setTagParameters (const TagParameterMap& params);

				int	 getKeyNumber() const		 { return fKeyNumber; }
				void setKeyNumber(int newnumber) { fKeyNumber = newnumber; }
                bool hideAutoNaturals() const	 { return fHideAutoNaturals; }
                bool isHideAutoNaturalsSet() const	{ return fHideAutoNaturalsSet; }
                bool freeKey() const				{ return fIsFree; }

		virtual void getOctArray(int *) const;
		virtual void getFreeKeyArray(float *) const;
		ARMusicalObject *isARKey()					{ return this; }

	static float	getAccidental	(const char*& ptr);
	static int		getNote			(const char*& ptr);

	protected:
        ///  if true -> don't show auto generated naturals at key signature changes
        bool    fHideAutoNaturals;
		bool	fIsFree; // True if accidental free specified
        bool    fHideAutoNaturalsSet = false;
		int		fKeyNumber; // >0 = nr of #, < 0 = nr of &
		float	fAccarray [NUMNOTES];
		int		fOctarray [NUMNOTES];
	
	private:
		void	init();
		void	name2KeyNum (std::string name);
		void	getFreeKeyArray	(const std::string& inString);
};

#endif

