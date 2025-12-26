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

#ifndef __parOperation__
#define __parOperation__

#include "arexport.h"
#include "guidoelement.h"
#include "operation.h"
#include "ARTypes.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to put scores in parallel
*/
class gar_export parOperation : public operation
{
    public:
		enum mode { kLeft, kRight };
		
				 parOperation(mode m=kLeft) : fMode(m) {}
		virtual ~parOperation() {}


		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );

    private:
		mode fMode;

		SARMusic parallel ( const SARMusic& score1, const SARMusic& score2 );
		SARMusic extend   ( SARMusic& score, const rational& duration );
};

//______________________________________________________________________________
/*!
\brief	A visitor to put scores in parallel, right justified
*/
class gar_export rparOperation : public parOperation
{
    public:
				 rparOperation() : parOperation(kRight) {}
		virtual ~rparOperation() {}
};

/*! @} */

} // namespace

#endif
