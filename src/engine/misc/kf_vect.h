#ifndef KF_Vector_H
#define KF_Vector_H

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
/** \brief Implements a dynamic vector. The parameter 'maximum' shows which max index of 
		the vector has already be used.
 */

#include <climits> 
#include <cstdlib>	// for alloc, realloc, memmove...
#include <cstring>	// for memmove
// #include <cstddef>  // for memmove
// #include "defines.h"
// #include <cassert>

#include "kf_list.h"


#define KF_VECTOR_MININDEX INT_MIN
#define KF_VECTOR_MAXINDEX INT_MAX

template <class TYPE>
class KF_Vector
{
	public:
	  				KF_Vector(TYPE p_noelement);
	  				KF_Vector(TYPE p_noelement, TYPE * newdata, int newmemsize, int newindexoffset,
		  						int newminimum, int newmaximum, int newcount);
	  
		virtual		~KF_Vector()
		{
			free(data);
			  //delete mydatalist;
			  //delete myindexlist;
		}
	  
		// - STL-like interface
	 
		int		size() const { return GetCount(); }
		bool	empty() const { return size() == 0; }

	  //void SetMaximum(int index);
	  //void SetMinimum(int index);
	  
				void 	RemoveAll();

	  			int 	GetNextIndex(int index) const;
	  
	  			void 	Set(int index, TYPE mydata);
	  
		virtual void 	Cut(int index,KF_Vector<TYPE> **pnew);
		virtual void 	Delete(int index);
	  
	  TYPE Get(int index) const;	// replaced by at()
	  
	  int GetMaximum() const
	  {
		  assert(maximum - indexoffset + 1 <= memsize );
		  return maximum;
	  }
	  
	  int GetMinimum() const
	  {
		  assert( minimum - indexoffset >= 0 ) ; 
		  return minimum;
	  }

	  int GetCount() const // replaced by size()
	  {
		  assert( count <= memsize);
		  return count;
	  }
	  
  protected:

	  // this routine is needed to resize the memory area 
	  // index is the new index that is needed .....
	  void Resize(int index);

	  TYPE noelement;
	  int maximum;
	  int minimum;

	  int count;
	  int indexoffset;
	  
	  TYPE * data;
	  int  memsize;
};


template <class TYPE>
KF_Vector<TYPE>::KF_Vector(TYPE p_noelement)
{	
	minimum = 0;
	maximum = -1;
	memsize = 10;
	data = (TYPE *) malloc( memsize * sizeof(TYPE)); 
		  		  
	indexoffset = 0;
	noelement = p_noelement;
		  
	int i = 0;
	for (i=0;i<memsize;i++)
	{
		// initialize with zeroelement ....
		data[i] = noelement;
	}
	count = 0;
}

template <class TYPE>
KF_Vector<TYPE>::KF_Vector(TYPE p_noelement,TYPE * newdata,int newmemsize,
			int newindexoffset,int newminimum,int newmaximum,int newcount)
{
	noelement = p_noelement;
	data = newdata;
	memsize = newmemsize;
	count = newcount;
	indexoffset = newindexoffset;
	minimum = newminimum;
	maximum = newmaximum;

}

template <class TYPE>
void KF_Vector<TYPE>::Set(int index, TYPE mydata)
{
	int mymemindex = index - indexoffset;

	if (mymemindex < 0 || mymemindex >= memsize)
	{
		// we have to resize 
		do {
			Resize(index);
			mymemindex = index - indexoffset;
			if (mymemindex >= 0 && mymemindex < memsize)
			{
				if (data[mymemindex] == noelement && mydata != noelement)
					++ count;
				if (data[mymemindex] != noelement && mydata == noelement)
					-- count;
				
				data[mymemindex] = mydata;
				break;
			}
		} while (true);
	}
	else
	{
		if (data[mymemindex] == noelement && mydata != noelement)
			++ count;
		if (data[mymemindex] != noelement && mydata == noelement)
			-- count;
		// then the index is already in the data ....
		data[mymemindex] =  mydata;
	}


	if (mydata != noelement)
	{
		if (count == 1)
		{
			minimum = maximum = index;
		}
		else
		{
		if (index < minimum)
			minimum = index;
		if (index > maximum)
			maximum = index;
		}
	}
	else
	{
		if (count == 0)
		{
			minimum = 0;
			maximum = -1;
		}
		else // we have to find a new minimum and maximum 
		{
			//int newmin = minimum;
			//int newmax = maximum;
			int i;
			for (i=minimum;i<=maximum;++i)
			{
				if (data[i-indexoffset] != noelement)
				{
					minimum = i;
					break;
				}
			}
			for (i=maximum;i>=minimum;--i)
			{
				if (data[i-indexoffset] != noelement)
				{
					maximum = i;
					break;
				}
			}
		}

	}
	assert(maximum - minimum <= memsize);	
}	

/** \brief Deletes an Element at position index
*/
template <class TYPE>
void KF_Vector<TYPE>::Delete(int index)
{
	if (index < minimum)
	{
		assert(false);
		return;
	}
	if (index > maximum )
	{
		assert(false);
		return;
	}

	int mymemindex = index - indexoffset;

	assert(mymemindex < memsize);

	if (data[mymemindex] != noelement)
	{
		data[mymemindex] = noelement;
		--count;
	}

	if (count == 0)
	{
		minimum = 0;
		maximum = -1;

	}
	else if (count == 1)
	{
		if (index == minimum)
			minimum = maximum;
		else if (index == maximum)
			maximum = minimum;
	}
	else
	{
		if (index == minimum)
		{
		
			int newminimum = maximum;
			int i=0;
			for (i=minimum+1;i<=maximum;i++)
			{
				if (data[i - indexoffset] != noelement)
				{
					newminimum = i;
					break;
				}
			}
			minimum = newminimum;
		}
		else if (index == maximum)
		{
		
			int newmaximum = minimum;
			int i=0;
			for (i=maximum-1;i>=minimum;i--)
			{
				if (data[i-indexoffset] != noelement)
				{
					newmaximum = i;
					break;
				}
			}
			maximum = newmaximum;
		}
	}

	if (maximum < minimum)
	{
		minimum = 0;
		maximum = -1;
		assert(count == 0);
	}
}

/**\brief Returns element[index] if existing and 
	  noelement if element[index] doesn't exist 
*/
template <class TYPE>
TYPE KF_Vector<TYPE>::Get(int index) const
{
	if (index < minimum)	return noelement;
	if (index > maximum )	return noelement;

	const int mymemindex = index - indexoffset;

	assert(mymemindex < memsize);

	return data[mymemindex];
}

template <class TYPE>
int KF_Vector<TYPE>::GetNextIndex(int index) const
{

	if (index < minimum)
		return KF_VECTOR_MININDEX;
	if (index > maximum )
		return KF_VECTOR_MININDEX;

	int mymemindex = index - indexoffset;
	assert(mymemindex < memsize);


	int tmpindex;
	for (tmpindex=mymemindex+1;tmpindex<= maximum-indexoffset;tmpindex++)
	{
		if (data[tmpindex] != noelement)
			return tmpindex + indexoffset;
	}

	return KF_VECTOR_MININDEX;

}

template <class TYPE>
void KF_Vector<TYPE>::RemoveAll()
{
	// this removes all
	// just start a new. Because this is not a "owns"-element
	// situation, we can just get a new array ...

	int i;
	for (i=0;i<memsize;i++)
	{
		if (data[i] != noelement)
			Delete(i+indexoffset);
	}

	free(data);

	memsize = 10;
	data = (TYPE *) malloc(memsize * sizeof(TYPE));
	for (i=0;i<memsize;i++)
	{
		data[i] = noelement;
	}

	maximum = -1;
	minimum = 0;
	indexoffset = 0;
	count = 0;
}

/** \briefCuts a vector in two.

	The old vector reaches from minimum to index, the new vector
	(which is constructed within the function)
	ranges from index+1 till maximum.
*/
template <class TYPE>
void KF_Vector<TYPE>::Cut( int index, KF_Vector<TYPE> **pnew )
{
	*pnew = NULL;
	if (index< minimum)
		return;
	if (index>maximum)
		return;

 	int mymemindex = index - indexoffset;

	int newmin = INT_MAX;
	int newmax = INT_MIN;

	int newsize = maximum - index;
	if (newsize > 0)
	{
		// we make sure, that there are -10 left and +10 on the
		// right hand side ....
	   int newindexoffset = 10;

	   TYPE * newdata = (TYPE *) malloc((newsize + 2 * newindexoffset ) * sizeof( TYPE ) );

	   int i;
	   int newcount = 0;
	   for (i=0;i<newindexoffset;i++)
	   {
		   newdata[i] = noelement;
	   }
	   for (i=newindexoffset;i<newsize+newindexoffset;i++)
	   {
		   newdata[i] = data[mymemindex + 1 + i - newindexoffset] ;
		   if (newdata[i] != noelement)
		   {
			   int tmpindex = mymemindex + indexoffset + i - newindexoffset;
			   if (tmpindex < newmin)
				   newmin = tmpindex;
			   if (tmpindex > newmax)
				   newmax = tmpindex;
			   newcount++;
			   data[mymemindex + 1 + i - newindexoffset] = noelement;
		   }
	   }
	   for (i=newsize+newindexoffset;i<newsize+2*newindexoffset;i++)
	   {
		   newdata[i] = noelement;
	   }

	   if (newmin > newmax)
	   {
		   newmin = 0;
		   newmax = -1;
		   assert(newcount == 0);
	   }
		// create the new vector.
		*pnew  = new KF_Vector<TYPE>(noelement,newdata,newsize+2*newindexoffset,
			index + 1 - newindexoffset,newmin, newmax, newcount); // index+1,maximum,newcount);


		// we have to change the maximum and minimum and also the elements ....
		count = count - newcount;
		if (count == 0)
		{
			minimum = 0;
			maximum = -1;
		}
		else
		{
			int newmaximum = index;
			while (newmaximum >= minimum &&
				data[newmaximum - indexoffset] == noelement)
				--newmaximum;
			
			maximum = newmaximum;
		}
	}
	else
	{
		// the new vector has no size ....
		*pnew = new KF_Vector<TYPE>(noelement);
	}

	// first find the location of the cut.
}


// index is the index without indexoffset ...
template <class TYPE>
void KF_Vector<TYPE>::Resize(int index)
{
	int newmemsize = memsize;
	int offset = 0;
	int mymemindex = index - indexoffset;
	
	if (mymemindex < 0)
	{
		// then we have to increase the array to the left ....
		do // wow!
		{		
		    if (newmemsize <= 10)
			{
				newmemsize = 32;
				offset = 6; 
			}
			else if (newmemsize <=20)
			{
				newmemsize = 60;
				offset = 10;
			}
			else if (newmemsize <=100)
			{
				newmemsize = 240;
				offset = 20;
			}
			else if (newmemsize <= 500)
			{
				newmemsize = 560;
				offset = 30;
			}
			else 
			{
				newmemsize = newmemsize + 560;
				offset = 60;
			}
		}
		while( mymemindex < memsize - (newmemsize - (offset*2)) );
		
		int indexofnew = -index + offset;
/*		data = (TYPE *) realloc(data,(newmemsize * sizeof(TYPE))); */
		data = (TYPE *) realloc(data,(newmemsize * sizeof(TYPE)) + indexofnew);
		assert(data);

//		int indexofnew = -index + offset;
		// this moves the data to the right ...
		memmove(&(data[indexofnew]),data,memsize * sizeof(TYPE));

		int cnt;
		for (cnt=0;cnt<indexofnew;++cnt)
			data[cnt] = noelement;

		for (cnt=indexofnew + memsize;cnt<newmemsize;cnt++)
			data[cnt] = noelement;
		
		indexoffset = indexoffset + index - offset;
		memsize = newmemsize;
	}
	else if (mymemindex >= newmemsize ) {
		do {
		    if (newmemsize <= 10) {
				newmemsize = 32;
				offset = 6; 
			}
			else if (newmemsize <=20) {
				newmemsize = 60;
				offset = 10;
			}
			else if (newmemsize <=100) {
				newmemsize = 240;
				offset = 20;
			}
			else if (newmemsize <= 500) {
				newmemsize = 560;
				offset = 30;
			}
			else {
				newmemsize = newmemsize + 560;
				offset = 60;
			}
		} while( mymemindex >= newmemsize - (offset * 2) );
		
/*		data = (TYPE *) realloc(data,(newmemsize * sizeof(TYPE))); */
		data = (TYPE *) realloc(data,(newmemsize * sizeof(TYPE)) + offset);
		assert(data);		

		// this moves the data to the right ...
		memmove(&(data[offset]), data, memsize * sizeof(TYPE));

		int cnt;
		for (cnt=0;cnt<offset;++cnt)
			data[cnt] = noelement;
		for (cnt=offset + memsize;cnt<newmemsize;cnt++)
			data[cnt] = noelement;
		
		indexoffset = indexoffset - offset;
		memsize = newmemsize;
	}
}

#endif
