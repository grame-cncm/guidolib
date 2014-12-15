#ifndef GRSpringCollider_H
#define GRSpringCollider_H

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

#include "GRNotationElement.h"
#include "GUIDOTypes.h"

#include "kf_vect.h"
#include "kf_ivect.h"

/*
template <class T> class KF_List;
template <class T> class KF_IVector;
*/

typedef KF_List<GuidoPos> poslist;
typedef KF_IVector<poslist> PosListVector;


/** \brief not yet documented
*/
class GRSpringCollider
{

public:
			GRSpringCollider();
	virtual ~GRSpringCollider();

	virtual void AddElement(GRNotationElement * el, GuidoPos posel);

	virtual void RemoveElement(GRNotationElement * el, GuidoPos posel);

	/** Called by a spring, which owns this 
		instance of GRSpringCollider and knows the 
		SpringCollider of the next spring. On the first
		run, count equals -1, after that, count is the index
		into the poslistvector of this instance.
	*/
	virtual int GetMatchingPoslists( int & count,const GRSpringCollider * sprcol,
									poslist **pl1,poslist **pl2);


//	virtual poslistvector * GetPosListVector() const { return mPosListVector; }

private:

	PosListVector mPosListVector;
};

#endif
