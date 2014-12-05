#ifndef ARPositionTag_H
#define ARPositionTag_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "PrintVisitor.h"
#include "Visitable.h"
#include "GUIDOTypes.h"	// For GuidoPos

#define oldv 1 

class ARMusicalObject;

/** \brief The base class for all range tags.
*/

class ARPositionTag : public Visitable
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

                void	print(std::ostream & os) const { printName(os); os << ": "; printParameters(os); os << std::endl; }
		virtual void	printGMNName(std::ostream & os)    const = 0;
		virtual void	printName(std::ostream & os)       const = 0;
		virtual void	printParameters(std::ostream & os) const = 0;

        /**** Functions to avoid dynamic_cast ****/
        virtual ARMusicalObject *isARDisplayDuration() { return NULL; }
        virtual ARMusicalObject *isARChordTag()        { return NULL; }
        virtual ARMusicalObject *isARGrace()           { return NULL; }
        virtual ARMusicalObject *isARFeatheredBeam()   { return NULL; }
        /*****************************************/

		/* Visitor design pattern */
        virtual void accept(BaseVisitor *visitor) { visitor->visit(*this); }

protected:
		GuidoPos pos;
		GuidoPos ep;

		ARPositionTag * mPositionTag; // the correspondence.
#if !oldv
		ARPositionTag * mParentCorrespondence;
#endif
};

#endif

