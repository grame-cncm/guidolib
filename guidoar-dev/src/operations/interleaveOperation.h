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

#ifndef __interleaveOperation__
#define __interleaveOperation__

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
\brief	an operation to interleave 2 scores
*/
class gar_export interleaveOperation : public operation
{
    public:
		enum mode { kLeft, kRight };
		
				 interleaveOperation(mode m=kLeft) : fMode(m) {}
		virtual ~interleaveOperation() {}


		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );

    private:
		mode fMode;
		SARVoice interleave ( const Sguidoelement& score1, const Sguidoelement& score2, int offset=0 );
};

/*! @} */

} // namespace

#endif
