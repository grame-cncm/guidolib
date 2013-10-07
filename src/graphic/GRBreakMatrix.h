#ifndef GRBreakMatrix_H
#define GRBreakMatrix_H

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

#include "kf_ivect.h"	// required

//#include "kf_list.h"
//#include "kf_ilist.h"

template <class T> class KF_IPointerList;
template <class T> class KF_List;

class GRBreakMatrix;


typedef KF_List<int> intlist;

/** \brief Describes an entry in the Sparse-Break-Matrix.
	We have a (sorted) list of individual
	entries, which save a force and an individual break-list.
	the sorting is just be number of lines
	(which is just the length of the associated list)
*/

class GRPenaltyEntry
{
public:

	friend class GRBreakMatrix;

	GRPenaltyEntry(float fpenalty,
		int pindex1,int pindex2);

	
	// this constructs a new PenaltyEntry. This
	// is just made up from two previous entries.
	// the 		
	// and the combination of two previous breaklists
	GRPenaltyEntry(GRPenaltyEntry *ptpe1,
		GRPenaltyEntry *ptpe2);


	intlist * GetBreakList(intlist * mybreaklist);


protected:
	// this is the penalty-value
	float penalty;
	// this is a pointer to 
	union 
	{
		GRPenaltyEntry * pe;
		int index;
	} entry1, entry2;

	int ispointerentry;
};

typedef KF_IPointerList<GRPenaltyEntry> pentrylist;
typedef KF_IVector<pentrylist> pentryvector;
typedef KF_IVector<pentryvector> ppentryvector;

 
/** \brief Realises the "sparse"-matrix that operates on indexes and
	saves the penalties associated with a given piece.
*/
class GRBreakMatrix
{
public:
	GRBreakMatrix();

	pentrylist * GetAt(int row,int column);
	void AddAt(int row,int column,GRPenaltyEntry * pe);
	void ClearAt(int row,int column);

protected:

	ppentryvector rows;
};

#endif
