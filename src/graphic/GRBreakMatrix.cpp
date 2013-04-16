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

#include "GRBreakMatrix.h"
#include "kf_ilist.h"

GRPenaltyEntry::GRPenaltyEntry(GRPenaltyEntry * ptpe1,
	GRPenaltyEntry * ptpe2)
{
	
	if (ptpe1 && ptpe2)
	{
		ispointerentry = 1;
		penalty = ptpe1->penalty + ptpe2->penalty;
		entry1.pe = ptpe1;
		entry2.pe = ptpe2;
	}
	else
	{
		ispointerentry = 0;
		entry1.index = -1;
		entry2.index = -1;
		penalty = 0;
	}
}

GRPenaltyEntry::GRPenaltyEntry( float fpenalty,
		int pindex1,int pindex2)
{
	ispointerentry = 0;
	penalty = fpenalty;
	entry1.index = pindex1;
	entry2.index = pindex2;
}


intlist * GRPenaltyEntry::GetBreakList(intlist * mybreaklist)
{
	if (!ispointerentry)
	{
		if (mybreaklist->GetCount() == 0)
			mybreaklist->AddTail(entry1.index);
		mybreaklist->AddTail(entry2.index);
	}
	else
	{
		mybreaklist = entry1.pe->GetBreakList(mybreaklist);
		mybreaklist = entry2.pe->GetBreakList(mybreaklist);

	}
	return mybreaklist;
}


GRBreakMatrix::GRBreakMatrix()
	: rows(1)
{
	
}

pentrylist * GRBreakMatrix::GetAt(int row,int column)
{
	if (rows.GetMinimum() <= row &&
		row <= rows.GetMaximum() )
	{
		pentryvector * myrow = rows.Get(row);
		if (myrow)
		{
			if (myrow->GetMinimum() <= column
				&& column <= myrow->GetMaximum())
			{
				pentrylist *mylist = myrow->Get(column);
				return mylist;

			}
		}
	}
	return NULL;
}

void GRBreakMatrix::AddAt(int row, int column, GRPenaltyEntry * pe)
{
	pentryvector * myrow = rows.Get(row);
	if (!myrow)
	{
		// then we have to first create a row ...
		myrow = new pentryvector(1); // I own the elemnts ....
		rows.Set(row,myrow);
	}
	pentrylist * mylist = myrow->Get(column);
	if (!mylist)
	{
		mylist = new pentrylist(1);
		myrow->Set(column,mylist);
		mylist->AddTail(pe);
		pe = NULL;
	}
	else
	{
		// we check, wether the penalty is 
		// smaller ---- this is "dynamic" programming
		// at work: only the best way is saved. all
		// others are discarded ....
		GuidoPos pos = mylist->GetHeadPosition();
		while (pos)
		{
			GRPenaltyEntry * tmppe = mylist->GetNext(pos);
			if (tmppe && tmppe->penalty > pe->penalty)
			{

				mylist->RemoveAll();
				mylist->AddTail(pe);

				pe = NULL;
                break;
			}
		}
	}
	
	// this will be no longer needed ....
	delete pe;	
}

void GRBreakMatrix::ClearAt(int row,int column)
{
	pentryvector * myrow = rows.Get(row);
	if (myrow)
	{
		pentrylist  *mylist = myrow->Get(column);
		if (mylist)
		{
			// then I have to work on the list 
			// and throw away the ones that
			// are double .....
		}
	}
}
