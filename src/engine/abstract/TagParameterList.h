#ifndef TagParameterList_H
#define TagParameterList_H

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

#include "kf_ilist.h"
#include "TagParameter.h"

class NVstring;
//class TagParameterList;

/** \brief A list of tag parameters.
*/

class TagParameterList : public KF_IPointerList<TagParameter>
{
	char * nextToken (char *ptr, char sep) const;
public:
	// this creates a new TagParameterList out of a definition string ...
			 TagParameterList(const NVstring & str);
			 TagParameterList(int powns = 0);
	virtual ~TagParameterList() {}

	int getMinimum() const;
	int getMaximum() const;

	TagParameterList *	Match			(const TagParameterList & tpl) const;
	bool				Match			(const TagParameterList* tpl, const TagParameter * tp) const;
	bool				checkRequired	(const TagParameterList* tpl) const;
	TagParameter*		getPosition		(const TagParameterList* tpl, int pos) const;
	void				copyTemplate	(TagParameterList *dst) const;

protected:
		mutable	int minimum;	// (JB) mutable because getMinimum might recalculate it.

};

class ListOfTPLs : public KF_IPointerList<TagParameterList> 
{ 
public:
	ListOfTPLs(int p_ownselements = 0) 
		: KF_IPointerList<TagParameterList>( p_ownselements ) { }
	ListOfTPLs(const KF_IPointerList<TagParameterList> & lst,int p_ownselements = 0)
		: KF_IPointerList<TagParameterList>( lst, p_ownselements ) { }

};

#endif
