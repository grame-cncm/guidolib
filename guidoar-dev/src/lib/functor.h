/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __functor__
#define __functor__

/*!
\addtogroup generic
@{
*/

namespace guido 
{

/*!
\brief basic functor templates
*/
template <typename R>
class rfunctor {
	public:
		virtual ~rfunctor() {}
		virtual R operator ()() = 0;
};

/*!
\brief A basic functor template	
*/
template <typename R, typename T>
class functor {
	public:
		virtual ~functor() {}
		virtual R operator ()(T) = 0;	///< the operator () supports a single parameter
};

/*!
\brief A 2 args functor template	
*/
template <typename R, typename T1, typename T2>
class functor2 {
	public:
		virtual ~functor2() {}
		virtual R operator ()(T1, T2) = 0;
};

} // namespace

/*! @} */

#endif
