#ifndef GRBreakMatrix_H
#define GRBreakMatrix_H

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
