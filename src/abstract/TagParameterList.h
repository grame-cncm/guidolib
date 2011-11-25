#ifndef TagParameterList_H
#define TagParameterList_H

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
