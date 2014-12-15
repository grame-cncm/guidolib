/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json_value__
#define json_value__

#include <string>
#include <iostream>
#include "json_stream.h"
#include "json_printable.h"
#ifndef JSON_ONLY
#include "osc_stream.h"
#endif
#include "export.h"

namespace json
{

class json_object;
class json_array;
//--------------------------------------------------------------------------
/*!
	\brief base class of a json value 
	
	json values are as defined by the json spec (see http://json.org/)
	each kind of value is embedded in a specific class derived from json_value
*/
class jsonexport json_value : public json_printable
{
	public:
		virtual ~json_value() {}

		/*!
		 \brief utility for parameter type checking
		*/
		template<typename X> const X* value() const { return dynamic_cast<const X*> (this); }

		/*!
		 \brief abstract interface for printing
		 \param out the output strem
		*/
		virtual void	print(json_stream&) const {}
#ifndef JSON_ONLY
		virtual void	print(osc_stream&) const {}
#endif
};

/// \private
inline json_stream& operator << (json_stream& os, const json_value& val)	{ val.print(os); return os; }


//--------------------------------------------------------------------------
/*!
	\brief json object value
*/
class jsonexport json_object_value : public json_value
{
	const json_object*	fValue;
	public:
				 json_object_value(json_object* val)	: fValue(val)	{}
		virtual ~json_object_value();
		
		const json_object*	getValue() const	{ return fValue; }
		void	print(json_stream& out) const;
#ifndef JSON_ONLY
		void	print(osc_stream& out) const;
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json array value
*/
class jsonexport json_array_value : public json_value
{
	const json_array*	fValue;
	public:
				 json_array_value(json_array* val)	: fValue(val)	{}
		virtual ~json_array_value();
		
		const json_array*	getValue() const	{ return fValue; }
		void	print(json_stream& out) const;
#ifndef JSON_ONLY
		void	print(osc_stream& out) const;
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json numeric value
*/
class jsonexport json_int_value : public json_value
{
	long	fValue;
	public:
				 json_int_value(long val)	: fValue(val)	{}
		virtual ~json_int_value() {}
		
		int		getValue() const				{ return fValue; }
		void	print(json_stream& out) const	{ out << fValue; }
#ifndef JSON_ONLY
		void	print(osc_stream& out) const	{ out << fValue; }
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json numeric float value
*/
class jsonexport json_float_value : public json_value
{
	double	fValue;
	public:
				 json_float_value(double val)	: fValue(val)	{}
		virtual ~json_float_value() {}
		
		double	getValue() const				{ return fValue; }
		void	print(json_stream& out) const	{ out << fValue; }
#ifndef JSON_ONLY
		void	print(osc_stream& out) const	{ out << fValue; }
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json string value
	
	string values are stored with unescaped '"'. Other special characters remains escaped.
	see http://json.org/
*/
class jsonexport json_string_value : public json_value
{
	std::string fValue;
	public:
				 json_string_value(const char* val);
		virtual ~json_string_value() {}
		
		const std::string&	getValue() const		{ return fValue; }

		std::string	to_json() const;				///< converts the value to json string (with escape) 
		void	print(json_stream& out) const		{ out << '"' << to_json() << '"'; }
#ifndef JSON_ONLY
		void	print(osc_stream& out) const		{ out << fValue; }
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json null value
*/
class jsonexport json_null_value : public json_value
{
	public:
				 json_null_value() {}
		virtual ~json_null_value() {}
		
		int		getValue() const				{ return 0; }
		void	print(json_stream& out) const	{ out << "null"; }
#ifndef JSON_ONLY
		void	print(osc_stream& out) const	{ out << 0; }
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json true value
*/
class jsonexport json_true_value : public json_value
{
	public:
				 json_true_value() {}
		virtual ~json_true_value() {}
		
		bool	getValue() const				{ return true; }
		void	print(json_stream& out) const	{ out << "true"; }
#ifndef JSON_ONLY
		void	print(osc_stream& out) const	{ out << 1; }
#endif
};

//--------------------------------------------------------------------------
/*!
	\brief json false value
*/
class jsonexport json_false_value : public json_value
{
	public:
				 json_false_value() {}
		virtual ~json_false_value() {}
		
		bool	getValue() const				{ return false; }
		void	print(json_stream& out) const	{ out << "false"; }
#ifndef JSON_ONLY
		void	print(osc_stream& out) const	{ out << 0; }
#endif
};


} // end namespoace

#endif
