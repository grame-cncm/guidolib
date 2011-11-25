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

#include "ARMusicalTag.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "GRDefine.h"
#include "GRTag.h"

// #include "GRStaff.h"

GRTag::GRTag( const ARMusicalTag * artag, float curLSPACE ) : 
			error(0), 
			tagtype(VOICETAG), 
			isautotag(0),
			sconst(SCONST_DEFAULT)
{
	
	if (artag && artag->IsStateTag())
		tagtype = STAFFTAG;
	if (artag && artag->getIsAuto())
		isautotag = 1;
	if (artag && artag->getError())
		error = artag->getError();
	
	mColRef = NULL;
	font = NULL;
	fontAttrib = NULL;
	mFontSize = (int) (1.5f * LSPACE); // default ...
	
	if (artag)
	{
		const TagParameterString * color = artag->getColor();
		if (color)
		{
			mColRef = new unsigned char [4];
			color->getRGB(mColRef);
		}
		const TagParameterFloat * dx = artag->getDX();
		const TagParameterFloat * dy = artag->getDY();
		if (dx)
		{
			mTagOffset.x = (GCoord) dx->getValue(curLSPACE);
		}
		if (dy)
		{
			mTagOffset.y -= (GCoord) dy->getValue(curLSPACE);
		}
		const TagParameterFloat * tps = artag->getSize();
		if (tps)
		{
			mTagSize = tps->getValue();
		}
		else 
			mTagSize = 1.0f;
	}
}

GRTag::~GRTag()
{
	delete [] mColRef;
	delete font;
	delete fontAttrib;
}

 void GRTag::RangeEnd(GRStaff * grstaff)
{

}

void GRTag::StaffFinished(GRStaff * grstaff)
{

}

void GRTag::StaffBegin(GRStaff * grstaff)
{
} 


int GRTag::getError()
{
	return error;
}

// changed: the abstract representation
// is no longer notified ....
// errors are not brought back from graphics ...
void GRTag::setError(int p_error)
{
	error = p_error;
	/* if (artag && error != 0)
	{
		artag->setError();
	} */
}

bool GRTag::IsStateTag() const
{
	return (tagtype == STAFFTAG);
}

int GRTag::getIsAuto() const
{
	return isautotag;
}

bool GRTag::operator==(const GRTag & tag) const
{
	return false;
} 
