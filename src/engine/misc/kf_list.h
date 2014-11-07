#ifndef KF_LIST_H
#define KF_LIST_H

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
#include "GuidoDefs.h"	// for GuidoPos ...
#include <cassert>

template <class TYPE>
class KF_ListNode
{
	public:
		KF_ListNode(TYPE n_data, KF_ListNode<TYPE> *n_prev = 0)
						: fData( n_data ), fNext( 0 ), fPrev( n_prev ) { }

		TYPE fData;
		KF_ListNode *fNext;
		KF_ListNode *fPrev;
};

template <class TYPE>
class KF_List
{
	public:
		typedef KF_ListNode<TYPE> mynode;

					 KF_List() : fHead(0), fTail(0), fCount(0) { }
		virtual 	~KF_List() { RemoveAll(); }

		// - STL-like interface
		int			size() const			{ return fCount; }
		bool		empty() const			{ return fCount == 0; }
		GuidoPos	push_back( TYPE data )	{ return AddTail( data ); }
		GuidoPos	insert( GuidoPos pos, TYPE data ) 
											{ return AddElementAt( pos, data ); }

		TYPE		back()	{ return GetTail(); }  // warning: currently return a copy
		TYPE		front() { return GetHead(); }  // idem

		// - Original interface
		virtual void ResetListNoDelete();
		virtual void RemoveAll();
		virtual void RemoveElementAt(GuidoPos pos);

		TYPE RemoveTail()
		{
			if (!fTail) return (TYPE) 0;

			mynode * myprev = fTail->fPrev;
			TYPE data = fTail->fData;
			if (fTail == fHead)
			{
				delete fTail;
				fTail = fHead = 0;
				fCount = 0;
			}
			else
			{
				delete fTail;
				fTail = myprev;
				fTail->fNext = 0;
				--fCount;
			}
			return data;
		}

		GuidoPos AddTail(TYPE data) // use push_back instead
		{
			mynode * node = new mynode(data,fTail);
			if (fTail)
				fTail->fNext = node;
			 else // there is no head therefore no tail
				fHead = node;

			fTail = node;
			++ fCount;
			return (GuidoPos) fTail;
		}

		GuidoPos AddElementAt(GuidoPos pos,TYPE data);
		GuidoPos AddElementAfter(GuidoPos pos, TYPE data);

		int GetCount() const { return fCount; }	// replaced by size()
		int IsEmpty() const { return fCount == 0; }	// replaced by empty() 

		GuidoPos SetTailPosition(GuidoPos pos);
		GuidoPos SetHeadPosition(GuidoPos pos);

		virtual void Cut(GuidoPos pos,KF_List<TYPE> **pnew);

		GuidoPos GetHeadPosition(void) const	{ return (GuidoPos) fHead; }
		GuidoPos GetTailPosition() const		{ return (GuidoPos) fTail; }

		TYPE GetNext(GuidoPos & pos) const
		{
			mynode *tmp = (mynode *) pos;
			pos = (GuidoPos) tmp->fNext;
			return tmp->fData;
		}

		TYPE GetPrev(GuidoPos &pos) const
		{
			mynode *tmp = (mynode *) pos;
			pos = (GuidoPos) tmp->fPrev;
			return tmp->fData;
		}

		TYPE GetAt(GuidoPos pos) const			{ return ((mynode *)pos)->fData; }
		void SetAt(GuidoPos pos, TYPE data)		{ ((mynode *)pos)->fData = data; }

		TYPE Get(int cnt) const;

		GuidoPos AddHead(TYPE data)
		{
			mynode *node = new mynode(data,0);
			if (fHead) {
				fHead->fPrev = node;
				node->fNext = fHead;
			}
			else {
				// there is no head therefore no tail
				fTail = node;
			}
			fHead = node;
			++fCount;
			return (GuidoPos) fHead;
		}

		TYPE RemoveHead()
		{
			if (!fHead) return (TYPE) 0;
			TYPE data = fHead->fData;

			mynode *tmp = fHead;
			if (fHead == fTail) {
				fHead = 0;
				fTail = 0;
			}
			else {
				fHead = fHead->fNext;
				fHead->fPrev = 0;
			}
			delete tmp;
			--fCount;
			return data;
		}

		TYPE GetHead() const		{ return fHead ? fHead->fData : 0; }
		TYPE GetTail() const		{ return fTail ? fTail->fData : 0; }

		// a sort-function, taking as an argument
		// a function, that compares to elements of
		// the given type and returns -1 for smaller,
		// 0 for equal and 1 for bigger.
		virtual void sort()
		{
			// bubble dir einene ...
			int needssort = 1;

			while (needssort)
			{
				needssort = 0;
				GuidoPos pos = GetHeadPosition();
				while (pos)
				{
					GuidoPos pos1 = pos;
					TYPE t1 = GetNext(pos);
					if (pos)
					{
						TYPE t2 = GetAt(pos);
						if (t1 > t2)
						{
							SetAt(pos1,t2);
							SetAt(pos,t1);
							needssort = 1;
						}
					}
					
				}
			}
		}

	protected:
		mynode *	fHead; 
		mynode *	fTail;	// at the end will be attached
		int			fCount;		// nr of elements

};

// inserts date BEFORE GuidoPos pos
template <class TYPE>
GuidoPos KF_List<TYPE>::AddElementAt(GuidoPos pos, TYPE data)
{
	if (((mynode *) pos) == 0 || ((mynode *) pos) == fHead)
		return AddHead(data);

	// pos is ptr to KG_ListNode
	mynode *thisnode = (mynode *) pos;
	mynode *node = new mynode(data,thisnode->fPrev);
	node->fNext = thisnode;
	thisnode->fPrev->fNext = node;
	thisnode->fPrev = node;

	fCount++;
	return (GuidoPos) node;
}

/* adds the date after the GuidoPos "pos".
   returns the GuidoPos of the added element"  */
template <class TYPE>
GuidoPos KF_List<TYPE>::AddElementAfter(GuidoPos pos, TYPE data)
{
	if (((mynode *) pos) == 0 || ((mynode *) pos) == fTail)
		return AddTail(data);

	// pos is a pointer to a KF_ListNode ...
	mynode *thisnode = (mynode *) pos;
	mynode *node = new mynode(data,thisnode);
	node->fNext = thisnode->fNext;
	node->fNext->fPrev = node;
	thisnode->fNext = node;
	fCount++;
	return (GuidoPos) node;
}

template <class TYPE> 
void KF_List<TYPE>::RemoveAll()
{
	mynode *cur = fHead;
	while (cur)
	{
		mynode * tmp = cur->fNext;
		delete cur;
		cur = tmp;
	}
	fHead = 0;
	fTail = 0;
	fCount = 0;
}

template <class TYPE>
void KF_List<TYPE>::ResetListNoDelete()
{
	fHead = 0;
	fTail = 0;
	fCount = 0;
}

template <class TYPE>
void KF_List<TYPE>::RemoveElementAt(GuidoPos pos)
{
	if (!pos) return;
	mynode *cur = (mynode *) pos;

	if (cur->fPrev)
	  cur->fPrev->fNext = cur->fNext;

	if (cur->fNext)

	  cur->fNext->fPrev = cur->fPrev;

	if (cur == fHead)
		fHead = cur->fNext;
	if (cur == fTail)
		fTail = cur->fPrev;
	delete cur;
	fCount--;
}


template <class TYPE>
GuidoPos KF_List<TYPE>::SetTailPosition(GuidoPos pos)
{
	// we don't care, what happens with the
	// rest of the list ... it is not
	// deleted by this function!

	fTail = (mynode *) pos;
	if (fTail)
		fTail->fNext = 0;

	// then we have to recount!
	fCount = 0;
	mynode *cur = fHead;
	while (cur)
	{
		fCount++;
		if (!cur->fNext)
		{
			// this ensure, that the given tail
			// was somewhere in the list already
			assert(cur == fTail);
		}
		cur = cur->fNext;
	}

	return (GuidoPos) fTail;
}

template <class TYPE>
TYPE KF_List<TYPE>::Get(int cnt) const
{
	int tmpcnt = 1;
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		TYPE mydata = GetNext(pos);
		if (tmpcnt == cnt)
			return mydata;
		tmpcnt++;
	}
	return (TYPE) 0;
}

template <class TYPE>
GuidoPos KF_List<TYPE>::SetHeadPosition(GuidoPos pos)
{

	assert(!fHead);
	assert(!fTail);

	fHead = (mynode *) pos;
	if (fHead)
		fHead->fPrev = 0;

	// then we have to recount!
	fCount = 0;
	mynode *cur = fHead;
	while (cur)
	{
		fCount++;
		if (!cur->fNext)
			fTail = cur;
		cur = cur->fNext;
	}

	return (GuidoPos) fHead;
}


template <class TYPE>
void KF_List<TYPE>::Cut(GuidoPos pos,KF_List<TYPE> **pnew)
{
	// first, we create the new list:
	*pnew = new KF_List<TYPE>;

	// this remembers the head-GuidoPos for the new-list
	mynode *node = (mynode *) pos;

	if (node)
	{
		node = node->fNext;
		// then, we set the tail-GuidoPos of the
		// current list
		SetTailPosition(pos);

		// and the head-GuidoPos (and tail-pos)
		// of the new-list

		(*pnew)->SetHeadPosition((GuidoPos) node);

	}
	else
	{
		// then, we try to cut at the
		// very beginning!
		(*pnew)->SetHeadPosition((GuidoPos) fHead);

		fHead = 0;
		fTail = 0;
		fCount = 0;
	}
}

#endif
