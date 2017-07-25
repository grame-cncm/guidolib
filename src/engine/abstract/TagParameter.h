#ifndef TagParameter_H
#define TagParameter_H

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

/** \brief The base class for all guido tags.
*/

#include <memory>
#include <string>
#include <vector>
#include <iostream>

class TagParameter 
{
	public:
	  		enum PARSEFLAG { SETBYPOSITION, SETBYNAME, SETBYAUTO, NOTSET, SET };
	
					 TagParameter() : fFlag( NOTSET ), fRequired(false) { }
		virtual 	~TagParameter() { }

		virtual void			set ( const TagParameter & in );

		virtual TagParameter *	getCopy() const						{ return 0; }
		virtual bool			copyValue(const TagParameter *tp)	{ return false; }

		virtual void 	print(std::ostream& out);

				void 	setRequired(bool i)			{ fRequired = i; }
	  			void 	setIsAuto()  		 		{ fFlag = SETBYAUTO; }
	  			void 	setByName()  		 		{ fFlag = SETBYNAME; }
	  			void 	setNoSet()  		 		{ fFlag = NOTSET; }
	  			void 	setByPos()  		 		{ fFlag = SETBYPOSITION; }
	  			void 	setBySet()  		 		{ fFlag = SET; }
	  	virtual void 	setValue(const char *)		{ }
	  			void 	setName(const std::string& p) { fName = p; }

				const std::string& 	getName() const { return fName; }
			  	bool TagIsRequired() const 			{ return fRequired; }
			  	bool TagIsSet() const  				{ return fFlag != NOTSET; };
			  	bool TagIsSetByPosition() const  	{ return fFlag == SETBYPOSITION; }
			  	bool TagIsSetByName() const 		{ return fFlag == SETBYNAME; }
			  	bool TagIsSetByAuto() const  		{ return fFlag == SETBYAUTO; }
			  	bool TagIsNotSet() const 			{ return fFlag == NOTSET; }

		virtual bool isString() const	{ return false; }
		virtual bool isInt() const		{ return false; }
		virtual bool isFloat() const	{ return false; }
        virtual bool isRGBValue() const { return false; }
	  	
	private:
		std::string fName;
		PARSEFLAG	fFlag;
		bool		fRequired;
};

inline std::ostream& operator << ( std::ostream & os, TagParameter* p) { p->print(os); return os; }

typedef std::shared_ptr<TagParameter>	STagParameterPtr;
typedef std::vector<STagParameterPtr>	TagParametersList;

#endif


