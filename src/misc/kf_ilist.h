#ifndef KF_IPointerList_H
#define KF_IPointerList_H

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

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include "kf_list.h"

template <class TYPE>
class KF_IPointerList : public KF_List<TYPE *>
{
	public:
				 KF_IPointerList(int p_ownselements = 0)	{ ownselements = p_ownselements; }
				 KF_IPointerList(const KF_IPointerList<TYPE> &lst, int p_ownselements = 0);
		virtual ~KF_IPointerList()							{ RemoveAll(); }

		virtual GuidoPos GetElementPos(TYPE * data);

		virtual void	RemoveAll();
		virtual void	RemoveElementAt(GuidoPos pos);
		virtual int		RemoveElement(TYPE *data);

		virtual void	DumpListAtTail(KF_IPointerList<TYPE> * list );
		virtual KF_IPointerList<TYPE> * getCopy();
		virtual void	Cut(GuidoPos pos,KF_IPointerList<TYPE> **pnew);

		virtual void	setOwnership(int p_ownselements)	{ ownselements = p_ownselements; }
		virtual int		getOwnership() const				{ return ownselements; }

		virtual void	sort( int comp(const TYPE *, const TYPE *) );

		virtual void	AddSortedHead(TYPE *, int comp(const TYPE *, const TYPE *));
		virtual void	AddSortedTail(TYPE *, int comp(const TYPE *, const TYPE *));

	protected:
		int ownselements;	// 1: elements will be deleted by list

	private:
		void DeleteData(GuidoPos pos);
		void DeleteAllData();					// delete data

		using KF_List<TYPE *>::sort;
		using KF_List<TYPE *>::Cut;
};

template <class TYPE>
KF_IPointerList<TYPE>::KF_IPointerList(const KF_IPointerList<TYPE> &lst, int p_ownselements)
 : KF_List<TYPE *>()
{
	ownselements = p_ownselements;
	GuidoPos pos = lst.GetHeadPosition();
	TYPE * mytype;
	while (pos)
	{
		mytype = lst.GetNext(pos);
		KF_List<TYPE *>::AddTail(mytype);
	}
}

template <class TYPE>
void KF_IPointerList<TYPE>::DeleteAllData(void)
{
	if (ownselements) {
		GuidoPos pos = this->GetHeadPosition();
		while (pos) {
			DeleteData (pos);
			this->GetNext(pos);
		}
	}
}

template <class TYPE>
void KF_IPointerList<TYPE>::DeleteData(GuidoPos pos)
{
	if (ownselements) {
		delete this->GetAt(pos);
		this->SetAt (pos, 0);
	}
}

template <class TYPE>
void KF_IPointerList<TYPE>::RemoveAll()
{
	DeleteAllData();
	KF_List<TYPE *>::RemoveAll();
}

template <class TYPE>
GuidoPos KF_IPointerList<TYPE>::GetElementPos(TYPE *data)
{
	GuidoPos pos = this->GetHeadPosition();
	while (pos)
	{
		if ( data == this->GetAt(pos) )
			return pos;
		this->GetNext(pos);
	}
	return pos;
}

template <class TYPE>
int KF_IPointerList<TYPE>::RemoveElement(TYPE *data)
{
	GuidoPos pos = GetElementPos(data);
	if (pos)
	{
		RemoveElementAt(pos);
		return 1;
	}
	return 0;
}

template <class TYPE>
void KF_IPointerList<TYPE>::RemoveElementAt(GuidoPos pos)
{
	if (ownselements)
		DeleteData(pos);
	KF_List<TYPE *>::RemoveElementAt(pos);
}


template <class TYPE>
void KF_IPointerList<TYPE>::sort(int comp(const TYPE *, const TYPE *))
{
	// so we bubble agagin ...
	int needssort = 1;
	while (needssort)
	{
		needssort = 0;
		GuidoPos pos = this->GetHeadPosition();
		while (pos)
		{
			GuidoPos pos1 = pos;
			TYPE *t1 = this->GetNext(pos);
			if (pos)
			{
				TYPE *t2 = this->GetAt(pos);
				if ( comp(t1,t2) == 1)
				{
					KF_List<TYPE *>::SetAt(pos1,t2);
					KF_List<TYPE *>::SetAt(pos,t1);
					needssort = 1;
				}
			}
		}
	}
}

// - Stole the events of input list (was: AddListAtTail)
template<class TYPE>
void KF_IPointerList<TYPE>::DumpListAtTail( KF_IPointerList<TYPE> * list )
{
	if (this->fTail == 0)
	{
		this->fHead = list->fHead;
		this->fTail = list->fTail;
		this->fCount = list->fCount;
	}
	else if (list->fHead)
	{
		this->fTail->fNext = list->fHead;
		list->fHead->fPrev = this->fTail;
		this->fTail = list->fTail;
		this->fCount += list->fCount;
	}

	list->fHead = 0;
	list->fTail = 0;
	list->fCount = 0;
}

template <class TYPE>
void KF_IPointerList<TYPE>::Cut(GuidoPos pos, KF_IPointerList<TYPE> **pnew)
{
	// first, we create the new list:
	*pnew = new KF_IPointerList<TYPE>(ownselements);
	// this remembers the head-position for the new-list
	KF_ListNode<TYPE *> *node = (KF_ListNode<TYPE *> *) pos;

	if (node)
	{
		node = node->fNext;
		// then, we set the tail-position of the current list
		this->SetTailPosition(pos);
		// and the head-position (and tail-pos) of the new-list
		(*pnew)->SetHeadPosition((GuidoPos) node);
	}
	else
	{
		// cut at the very beginning
		(*pnew)->SetHeadPosition((GuidoPos) this->fHead);
		this->fHead = 0;
		this->fTail = 0;
		this->fCount = 0;
	}
}

// this routine copies the list ....
template <class TYPE>
KF_IPointerList<TYPE> * KF_IPointerList<TYPE>::getCopy()
{
	// if (ownselements) return 0;
	KF_IPointerList *myl = new KF_IPointerList(0);

	GuidoPos pos = this->GetHeadPosition();
	while (pos)
	{
		myl->AddTail(this->GetNext(pos));
	}
	return myl;
}

// These routines assume that the list is sorted
// already. a new element is placed at the correct position
// beginning at either the head or the tail ....
template <class TYPE>
void KF_IPointerList<TYPE>::AddSortedHead(TYPE * data, int comp(const TYPE *, const TYPE *))
{
	// iterate through the list from the beginning
	// and place the rod where it should go ...
	GuidoPos pos = this->GetHeadPosition();
	if (!pos)
	{
		//  there is no data in the list ....
		KF_List<TYPE *>::AddHead(data);
		return;
	}
	while (pos)
	{
		TYPE * tmp = this->GetAt(pos);
		int res = comp(tmp,data);
		// >= 0 means 0 or 1
		// 0: tmp is equal to data
		// 1: tmp is bigger then data
		if ( res >= 1)
		{
			// then we can place the element at the current position
			KF_List<TYPE *>::AddElementAt(pos,data);
			return;
		}
		this->GetNext(pos);
	}
	// then we have traversed the whole list without finding a suitable position ....
	KF_List<TYPE *>::AddTail(data);
}

template <class TYPE>
void KF_IPointerList<TYPE>::AddSortedTail(TYPE * data,int comp(const TYPE *, const TYPE *))
{
	// iterate through the list from the end and place the rod where it should go ...
	GuidoPos pos = this->GetTailPosition();
	if (!pos)
	{
		// then there is no data in the list ....
		KF_List<TYPE *>::AddTail(data);
		return;
	}
	while (pos)
	{
		TYPE * tmp = this->GetAt(pos);
		int res = comp(tmp,data);
		// <= means -1 or 0,
		// -1: tmp is smaller than data
		//  0 : tmp is equal to data
		if ( res < 0)
		{
			// then we can place the element at the current position
			KF_List<TYPE *>::AddElementAfter(pos,data);
			return;
		}
		this->GetPrev(pos);
	}
	// then we have traversed the whole list without finding a suitable position ....
	KF_List<TYPE *>::AddHead(data);
}

#endif
