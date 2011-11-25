#ifndef TagParameter_H
#define TagParameter_H

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
	  	
	  	
		NVstring name;
		PARSEFLAG pflag;
		bool required; 			// is 1, if the tagparameter is  required.
};


#endif


