/*
  GUIDO Library
  Copyright (C) 2006-2008  Grame

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

#ifndef __guidoValueTypes__
#define __guidoValueTypes__

#include "gar_smartpointer.h"

namespace guidolang 
{
class guidoexpression;
typedef guido::SMARTP<guidoexpression>		Sguidoexpression;

class guidovalue;
class guidoSeqValue;
class guidoScoreValue;
class guidoMixValue;
class guidoClosureValue;
class guidoApplyValue;
class guidoErrValue;
class guidoEvalSusp;

typedef guido::SMARTP<guidovalue>			Sguidovalue;
typedef guido::SMARTP<guidoSeqValue>		SguidoSeqValue;
typedef guido::SMARTP<guidoScoreValue>		SguidoScoreValue;
typedef guido::SMARTP<guidoMixValue>		SguidoMixValue;
typedef guido::SMARTP<guidoClosureValue> 	SguidoClosureValue;
typedef guido::SMARTP<guidoApplyValue>		SguidoApplyValue;
typedef guido::SMARTP<guidoErrValue>		SguidoErrValue;
typedef guido::SMARTP<guidoEvalSusp>		SguidoEvalSusp;


} // namespace

#endif
