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

#ifndef __KR_HASHTABLE__
#define __KR_HASHTABLE__

// this saves the entries ...
#include "kf_ilist.h"

template<class KEY,class TYPE>
class KR_HashTable
{
public:
	class KeyType
	{
	public:
		KeyType(const KEY &parkey,
			const TYPE &partype)
			{
				key = parkey;
				val = partype;
			}
		KEY key;
		TYPE val;
		int ValGreater(const KeyType &kt) const
		{
			return val>kt.val;
		}
		int KeyGreater(const KeyType &kt) const
		{
			return key>kt.key;
		}
	};

private:

	KF_IPointerList<KeyType> *entrylist;

public:
	static int compVal(const KeyType *t1, const KeyType *t2);
	static int compKey(const KeyType *t1, const KeyType *t2);
	

public:
	KR_HashTable()
	{
		entrylist = new KF_IPointerList<KeyType>(1);
	}

	virtual ~KR_HashTable()
	{
		delete entrylist;
	}


	GuidoPos Lookup(const KEY &key,
		TYPE **value) 
	{
		GuidoPos pos = entrylist->GetHeadPosition();
		while (pos)
		{
			KeyType *kt = entrylist->GetAt(pos);
			if (kt->key == key)
			{
				*value = &kt->val;
				return pos;
			}
			entrylist->GetNext(pos);
		}
		*value = NULL;
		return NULL;
	}
	// returns 
	// pos in the hashtable
	// NULL -> not successfull
	GuidoPos Lookup(const KEY &key,
		TYPE &value) const
	{
		
		GuidoPos pos = entrylist->GetHeadPosition();
		while (pos)
		{
			KeyType *kt = entrylist->GetAt(pos);
			if (kt->key == key)
			{
				value = kt->val;
				return pos;
			}
			entrylist->GetNext(pos);
		}
		return NULL;
	}

	// returns 
	// 1: if replaced with something else
	// 0: if new entry
	int Set(const KEY &key,const TYPE &value)
	{
		GuidoPos pos = entrylist->GetHeadPosition();
		while (pos)
		{
			KeyType *kt = entrylist->GetAt(pos);
			if (kt->key == key)
			{
				kt->val = value;
				return 1;
			}
			entrylist->GetNext(pos);
		}

		// now we have to create a new entry
		KeyType *kt = new KeyType(key,value);
		entrylist->AddTail(kt);
		return 0;
	}

	int Set(GuidoPos pos,const KEY &key,const TYPE &value)
	{

		KeyType *kt = entrylist->GetAt(pos);
		kt->key = key;
		kt->val = value;

		// alway replacing something ...
		return 1;
	}

	void Delete(const KEY &key)
	{
		TYPE *val = NULL;
		GuidoPos pos = NULL;
		if ((pos = Lookup(key,&val) ) != NULL)
		{
			// We found it ... then
			// we remove it ....

			entrylist->RemoveElementAt(pos);

		}
	}

	void DeleteAll()
	{
		entrylist->RemoveAll();
	}

	int GetCount()
	{
		return entrylist->GetCount();
	}
	GuidoPos GetHeadPosition()
	{
		return entrylist->GetHeadPosition();
	}

	// 1 if successfull
	// 0 if not.
	int GetNext(GuidoPos &pos,TYPE &val)
	{
		if (pos)
		{
			KeyType *kt = entrylist->GetAt(pos);
			val = kt->val;
			entrylist->GetNext(pos);
			return 1;
		}
		return 0;
	}

	// 1 if successfull
	// 0 if not.
	int GetNext(GuidoPos &pos,KeyType  **kt)
	{
		if (pos)
		{
			*kt = entrylist->GetAt(pos);
			entrylist->GetNext(pos);
			return 1;
		}
		return 0;
	}

	// sorts the Entries in the
	// HashTable according to the VALUES
	void sortValues()
	{
		// the sort-function for
		// the entries.... this
		// sorts hash-table according
		// to the entries ...
		if (entrylist->GetCount()>1)
			entrylist->sort( &(KR_HashTable<KEY,TYPE>::compVal) );

	}

	// sorts the Entries of the
	// HashTable accoring to the Keys
	void sortKeys()
	{
		if (entrylist->GetCount()>1)
			entrylist->sort( &(KR_HashTable<KEY,TYPE>::compKey) );
	}
	

};

template<class KEY,class TYPE>
int KR_HashTable<KEY,TYPE>::compVal(const KeyType *t1, const KeyType *t2)
{
	if (t1->ValGreater(*t2) ) return 1;
	return 0;
}

template<class KEY,class TYPE>
int KR_HashTable<KEY,TYPE>::compKey(const KeyType *t1, const KeyType *t2)
{
	if (t1->KeyGreater(*t2) ) return 1;
	return 0;
}


#endif // __KR_HASHTABLE__
