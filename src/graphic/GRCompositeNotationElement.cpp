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

#include "GRCompositeNotationElement.h"
#include "GREvent.h"
#include "NEPointerList.h"

GRCompositeNotationElement::GRCompositeNotationElement()
  : mCompElements( 1 ) // (new NEPointerList(1)) // better init with NULL first?
{
}

GRCompositeNotationElement::~GRCompositeNotationElement()
{

/* ATTENTION -> either or -- not both versions of delete !

	GuidoPos pos=elements->GetHeadPosition();
	while(pos)
	{
		delete elements->GetNext(pos);
	} */

//	delete elements;
//	elements = 0;
}

GuidoPos GRCompositeNotationElement::AddTail(GRNotationElement * e)
{
	return mCompElements.AddTail(e);
}

void GRCompositeNotationElement::setGRStaff(GRStaff * stf)
{

	GRNotationElement::setGRStaff(stf);

	GuidoPos pos = mCompElements.GetHeadPosition();
	while (pos)
	{
		GRNotationElement * el = mCompElements.GetNext( pos );
		el->setGRStaff(stf);
	}
}
