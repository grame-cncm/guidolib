/*

  MusicXML Library
  Copyright (C) 2009  Grame

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

#ifndef __ringvector__
#define __ringvector__

#include <vector>

namespace guido 
{

//______________________________________________________________________________
template <typename T> class fwbwIterator : public std::iterator<std::input_iterator_tag, T>
{
	protected:
		typedef typename std::vector<T>::iterator stdIterator;
		stdIterator fIterator, fBegin, fEnd;
		bool fForward;

	public:
				 fwbwIterator() : fForward(true) {}
				 fwbwIterator(const stdIterator& current, const stdIterator& beg, const stdIterator& end) 
					: fIterator(current), fBegin(beg), fEnd(end), fForward(current != end) {}
		virtual ~fwbwIterator() {}
		
		T operator  *() const	{ return *fIterator; }
		T operator ->() const	{ return *fIterator; } 

		fwbwIterator& operator ++()	{ 
			stdIterator i = fIterator;
			if (fForward) {
				i++; 
				if (i == fEnd) {
					fIterator--;
					fForward = false;
				}
				else fIterator = i;
			}
			else {
				i--; 
				if (i == fBegin) {
					fIterator--;
					fForward = true;
				}
				else fIterator = i;
			}
			return *this; 
		}
		fwbwIterator& operator ++(int)	{ return operator ++(); }

		//________________________________________________________________________
		bool operator ==(const fwbwIterator& i) const		{ return fIterator == i.fIterator; }
		bool operator !=(const fwbwIterator& i) const		{ return !(*this == i); }
};

//______________________________________________________________________________
template <typename T> class ringvectorIterator : public std::iterator<std::input_iterator_tag, T>
{
	protected:
		typedef typename std::vector<T>::iterator stdIterator;
		stdIterator fIterator, fBegin, fEnd;

	public:
				 ringvectorIterator() {}
				 ringvectorIterator(const stdIterator& current, const stdIterator& beg, const stdIterator& end) 
					: fIterator(current), fBegin(beg), fEnd(end) {}
		virtual ~ringvectorIterator() {}
		
		T operator  *() const	{ return *fIterator; }
		T operator ->() const	{ return *fIterator; } 

		ringvectorIterator& operator ++()	{ 
			fIterator++; 
			if (fIterator== fEnd) fIterator= fBegin;
			return *this; 
		}
		ringvectorIterator& operator ++(int)	{ return operator ++(); }

		//________________________________________________________________________
		bool operator ==(const ringvectorIterator& i) const		{ return fIterator == i.fIterator; }
		bool operator !=(const ringvectorIterator& i) const		{ return !(*this == i); }
};

//______________________________________________________________________________
template <typename T> class ringvector : public std::vector<T> 
{
		typedef typename std::vector<T> stdvec;

	public:
		typedef ringvectorIterator<T>	iterator;
				 ringvector() {}
		virtual ~ringvector() {}
		
		iterator begin()	{ return ringvectorIterator<T>(stdvec::begin(), stdvec::begin(), stdvec::end()); }
		iterator end()		{ return ringvectorIterator<T>(stdvec::end(), stdvec::begin(), stdvec::end()); }
};

//______________________________________________________________________________
template <typename T> class fwbwvector : public std::vector<T> 
{
		typedef typename std::vector<T> stdvec;

	public:
		typedef fwbwIterator<T>	iterator;
				 fwbwvector() {}
		virtual ~fwbwvector() {}
		
		iterator begin()	{ return fwbwIterator<T>(stdvec::begin(), stdvec::begin(), stdvec::end()); }
		iterator end()		{ return fwbwIterator<T>(stdvec::end(), stdvec::begin(), stdvec::end()); }
};

}

#endif
