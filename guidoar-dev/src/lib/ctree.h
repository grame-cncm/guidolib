/*

  GUIDOEngine Library
  Copyright (C) 2007  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __ctree__
#define __ctree__

#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include <iostream>
#include <stack>
#include <vector>
#include <iterator>
#include "gar_smartpointer.h"
#include "visitable.h"

namespace guido 
{

//______________________________________________________________________________
template <typename T> class treeIterator : public std::iterator<std::input_iterator_tag, T>
{
	private:
		//________________________________________________________________________
		T getParent() const		{ return fStack.size() ? fStack.top().second : fRootElement; }
		
		//________________________________________________________________________
		// current element has sub-elements: go down to sub-elements first			
		void forward_down(const T& t) {
			fCurrentIterator = t->elements().begin();
			if (fCurrentIterator != t->elements().end())
				fStack.push( std::make_pair(fCurrentIterator+1, t));
		}

		//________________________________________________________________________
		// current element is empty: go up to parent element and possibly down to neighbor element
		void forward_up() {
			while (fStack.size()) {
				state s = fStack.top();
				fStack.pop();

				fCurrentIterator = s.first;
				if (fCurrentIterator != s.second->elements().end()) {
					fStack.push( std::make_pair(fCurrentIterator+1, s.second));
					return;
				}
			}
		}
		
		//________________________________________________________________________
		// move the iterator forward
		void forward() {
			if ((*fCurrentIterator)->size()) forward_down(*fCurrentIterator);
			else forward_up();
		}

	protected:
		typedef typename std::vector<T>::iterator nodes_iterator;
		typedef std::pair<nodes_iterator, T> state;

		std::stack<state>	fStack;
		T					fRootElement;
		nodes_iterator		fCurrentIterator;
		
	public:
				 treeIterator() {}
				 treeIterator(const T& t, bool end=false) {
					 fRootElement = t;
					 if (end) fCurrentIterator = t->elements().end();
					 else forward_down (t);
				 }
				 treeIterator(const treeIterator& a)  { *this = a; }
		virtual ~treeIterator() {}
		
		T operator  *() const	{ return *fCurrentIterator; }
		T operator ->() const	{ return *fCurrentIterator; } 
		treeIterator& operator ++()		{ forward(); return *this; }
		treeIterator& operator ++(int)	{ forward(); return *this; }

		//________________________________________________________________________
		// inc the iterator at the same vector level ie without forward down
		treeIterator& rightShift()		{ forward_up(); return *this; }

		//________________________________________________________________________
		treeIterator& erase() {
			T parent = getParent();
			fCurrentIterator = parent->elements().erase(fCurrentIterator);
			if (fStack.size()) fStack.pop();
			if (fCurrentIterator != parent->elements().end()) {
				fStack.push( std::make_pair(fCurrentIterator+1, parent));
			}
			else forward_up();
			return *this; 
		}

		//________________________________________________________________________
		treeIterator& insert(const T& value) {
			T parent = getParent();
			fCurrentIterator = parent->elements().insert(fCurrentIterator, value);
			if (fStack.size()) fStack.pop();
			fStack.push( std::make_pair(fCurrentIterator+1, parent));
			return *this;
		}

		//________________________________________________________________________
		bool operator ==(const treeIterator& i) const		{ 
			// we check that the iterators have the same parent (due to iterator compatibility issue with visual c++)
			return getParent() == i.getParent() ?  ( fCurrentIterator==i.fCurrentIterator ) : false;
		}
		bool operator !=(const treeIterator& i) const		{ return !(*this == i); }
};

/*!
\brief a simple tree representation
*/
//______________________________________________________________________________
template <typename T> class ctree : virtual public smartable
{
	public:
		typedef SMARTP<T>					treePtr;	///< the node sub elements type
		typedef std::vector<treePtr>		branchs;	///< the node sub elements container type
		typedef typename branchs::iterator	literator;	///< the current level iterator type
		typedef typename branchs::const_iterator const_literator;	///< the current level const_iterator type
		typedef treeIterator<treePtr>		iterator;	///< the top -> bottom iterator type

//		static treePtr new_tree() { ctree<T>* o = new ctree<T>; assert(o!=0); return o; }
		
		branchs& elements()						{ return fElements; }		
		const branchs& elements() const			{ return fElements; }		
		virtual void push (const treePtr& t)	{ fElements.push_back(t); }
		virtual void push (const branchs& b)	{ const_literator i;
			for (i = b.begin(); i != b.end(); i++)
				fElements.push_back(*i); 
		}
		virtual int  size  () const				{ return int(fElements.size()); }
		virtual bool empty () const				{ return fElements.size()==0; }
		virtual void clear ()					{ fElements.clear(); }

		iterator begin()			{ treePtr start=dynamic_cast<T*>(this); return iterator(start); }
		iterator end()				{ treePtr start=dynamic_cast<T*>(this); return iterator(start, true); }
		iterator erase(iterator i)	{ return i.erase(); }
		iterator insert(iterator before, const treePtr& value)	{ return before.insert(value); }
		
		literator lbegin() { return fElements.begin(); }
		literator lend()   { return fElements.end(); }
		const_literator lbegin() const { return fElements.begin(); }
		const_literator lend() const   { return fElements.end(); }

	protected:
				 ctree() {}
		virtual ~ctree() {}

	private:
		branchs	 fElements;
};


}

#endif
