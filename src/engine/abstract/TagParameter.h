#ifndef TagParameter_H
#define TagParameter_H

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

/** \brief The base class for all guido tags.
*/

#include "nvstring.h"

class TagParameter 
{
	public:
	  		enum PARSEFLAG { SETBYPOSITION, SETBYNAME, SETBYAUTO, NOTSET };
	
					 TagParameter() : pflag( NOTSET ), required(false) { }
					 TagParameter(const TagParameter & tp );
		virtual 	~TagParameter() { }

		virtual void		set( const TagParameter & in );

		virtual TagParameter *	getCopy() const = 0;
		virtual bool			copyValue(const TagParameter *tp) = 0;

				void 	setRequired(bool i)			{ required = i; }
	  			void 	setIsAuto()  		 		{ pflag = SETBYAUTO; }
	  	virtual void 	setValue(const char *)		{ }
	  			void 	setName(const char *p)  	{ name = p; }

			  	bool TagIsRequired() const 			{ return required; }
			  	bool TagIsSet() const  				{ return pflag != NOTSET; };
			  	bool TagIsSetByPosition() const  	{ return pflag == SETBYPOSITION; }
			  	bool TagIsSetByName() const 		{ return pflag == SETBYNAME; }
			  	bool TagIsSetByAuto() const  		{ return pflag == SETBYAUTO; }
			  	bool TagIsNotSet() const 			{ return pflag == NOTSET; }

		virtual bool isString() const	{ return false; }
		virtual bool isInt() const		{ return false; }
		virtual bool isFloat() const	{ return false; }
        virtual bool isRGBValue() const { return false; }
	  	
	  	
		NVstring name;
		PARSEFLAG pflag;
		bool required; 			// is 1, if the tagparameter is  required.
};


#endif


