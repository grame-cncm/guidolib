#ifndef ARPositionTag_H
#define ARPositionTag_H

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

#include "GUIDOTypes.h"	// For GuidoPos

#define oldv 1 

class ARMusicalObject;

/** \brief The base class for all range tags.
*/

class ARPositionTag
{
	public:

		ARPositionTag() : pos(0), ep(0), mPositionTag(0)
		{
#if !oldv
			mParentCorrespondence=0;
#endif
		}

#if oldv
		virtual ~ARPositionTag()  { }
#else
		virtual ~ARPositionTag()  
		{
			if(mPositionTag) {
				mPositionTag->setParentCorrespondence(0);
				mPositionTag = 0;
			}
			if(mParentCorrespondence) {
				mParentCorrespondence->setCorrespondence(0);
				mParentCorrespondence = 0;
			}
		}
#endif
		virtual ARMusicalObject * Copy() const { return 0; }

		virtual void setPosition(GuidoPos p_pos)	{ pos = p_pos ;  }
		virtual GuidoPos getPosition() const		{ return pos; }
		virtual GuidoPos getStartPosition() const	{ return pos; }

		virtual void setStartPosition(GuidoPos p_pos) 
		{
			pos = p_pos;
		}

		virtual GuidoPos getEndPosition() const
		{
			if (mPositionTag)
				return mPositionTag->getPosition();
			else 
				return ep;
		}

		virtual void setEndPosition(GuidoPos p_pos)
		{
			if (mPositionTag)
				mPositionTag->setPosition(p_pos);
			else
				ep = p_pos;
		}


		virtual void setCorrespondence(ARPositionTag *p_cor)
		{
			mPositionTag = p_cor;
#if !oldv
			if(mPositionTag) {
				mPositionTag->setParentCorrespondence(this);
			}
#endif
		}

		virtual ARPositionTag * getCorrespondence()
		{
			return mPositionTag;
		}

#if !oldv
		virtual void setParentCorrespondence(ARPositionTag *parent)
		{
			mParentCorrespondence = parent;
		}
#endif
		virtual bool	isEndTagClass() const { return false; }

	protected:

		GuidoPos pos;
		GuidoPos ep;

		ARPositionTag * mPositionTag; // the correspondence.
#if !oldv
		ARPositionTag * mParentCorrespondence;
#endif
};

#endif

