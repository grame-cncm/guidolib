#ifndef ARKey_H
#define ARKey_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

		virtual 		~ARKey() {}

	  	virtual bool operator==(const ARKey & k) const;
	  	virtual bool IsStateTag() const;
		virtual void print() const;
		virtual void PrintName(std::ostream & os) const;
		virtual void PrintParameters(std::ostream & os) const;

		virtual void setTagParameterList(TagParameterList & theTagParameterList);


				int		getKeyNumber() const			{ return fKeyNumber; }
				void	setKeyNumber(int newnumber)		{ fKeyNumber = newnumber; }

		bool mIsFree; // True if accidental free specified

		virtual void getOctArray(int *) const;
		virtual void getFreeKeyArray(float *) const;


  protected:
	
	static ListOfTPLs ltpls;

		int		fKeyNumber; // >0 = nr of #, < 0 = nr of &
		float	fAccarray [NUMNOTES];
		int		fOctarray [NUMNOTES];
};

#endif

