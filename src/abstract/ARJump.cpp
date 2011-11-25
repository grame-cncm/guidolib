/*
	GUIDO Library
	Copyright (C) 2004  Grame

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

#include <string.h>
#include <iostream>
using namespace std;

#include "ARJump.h"

#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARJump::ltpls(1);

ARJump::ARJump(string mark) {
	FormatStringParser p;
	p.parse (mark.c_str(), mMark);
}

ARJump::~ARJump()   {}

void ARJump::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty()) {
		ListOfStrings lstrs;
		lstrs.AddTail( "S,m,,o;I,id,0,o" );
		CreateListOfTPLs(ltpls,lstrs);
	}
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if( ret >= 0 && rtpl ) {
		if( ret == 0 ) {
			TagParameterString * tps = TagParameterString::cast(rtpl->RemoveHead());
			if (tps && strlen(tps->getValue())) {
				FormatStringParser p;
				mMark.clear();
				p.parse (tps->getValue(), mMark);
			}
			delete tps;

			TagParameterInt * tpi = TagParameterInt::cast(rtpl->RemoveHead());
			if (tpi) mID = tpi->getValue();
			delete tpi;
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}


