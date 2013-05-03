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
#ifndef __KF_IVECT_H
#define __KF_IVECT_H

#include "kf_vect.h"

/** class KF_IVector stores an array of pointer
  * the highest used index will be stored
  * if an element doesn't exist NULL will be returned
*/

template <class TYPE>
class KF_IVector : public KF_Vector<TYPE *>
{
	public:
		KF_IVector(int p_ownselements = 0)
			: KF_Vector<TYPE *>(NULL)
		{
			ownselements = p_ownselements;
		}
		
		KF_IVector(int p_ownselements,TYPE ** newdata,int newsize,
			int newindexoffset, int newminimum,int newmaximum,
			int newcount)
			: KF_Vector<TYPE *>(NULL,newdata,newsize,newindexoffset,newminimum,newmaximum,
			newcount)
		{
			ownselements = p_ownselements;
		}
		
		virtual ~KF_IVector();

		virtual void Delete(int index)
		{
			if (ownselements)
			{
				if (this->data[index - this->indexoffset ] != NULL)
				{
					assert( index >= this->minimum );
					assert( index <= this->maximum );
					delete this->data[ index - this->indexoffset ];

					KF_Vector<TYPE *>::Delete(index);
					// this does the adjustment of minimum
					// and maximum (hopefully)

				}
			}
			else
				KF_Vector<TYPE *>::Delete(index);
		}
		
		virtual void Cut(int index,KF_IVector<TYPE> **ppvect);
		virtual void CutBegin(int index,KF_IVector<TYPE> **ppvect);

		virtual void setOwnership(int p_ownselements)
			{ ownselements = p_ownselements; }
		
		virtual int getOwnership() const
			{ return ownselements; }

		using KF_Vector<TYPE *>::Cut;

	protected:
		int ownselements; // 1: delete the data elements of the array
};

template <class TYPE>
KF_IVector<TYPE>::~KF_IVector()
{
	if (ownselements)
	{
		int i;
		for (i=0;i<this->memsize;i++)
		{
			if (this->data[i] != NULL)
			{
				assert( i+this->indexoffset >= this->minimum);
				assert( i+this->indexoffset <= this->maximum);
				delete this->data[i];
			}
		}
	}
	// when calling the standard destruc´tor of KF_Vector, the
	// vector structures will also be deleted
}

// this operation cuts a vector in two
// the old vector reaches from
// minimum to index, the new vector
// (which is constructed within the function)
// ranges from index+1 till maximum.
template <class TYPE>
void KF_IVector<TYPE>::Cut(int index,KF_IVector<TYPE> **pnew)
{
	*pnew = NULL;
	if (index < this->minimum)		return;
	if (index > this->maximum)		return;

	int mymemindex = index - this->indexoffset;

	int newmin = INT_MAX;
	int newmax = INT_MIN;

	int newsize = this->maximum - index;
	if (newsize > 0)
	{
		// we make sure, that there are -10 left and +10 on the
		// right hand side ....
	   int newindexoffset = 10;

	   TYPE **newdata = (TYPE **) malloc((newsize + 2*newindexoffset ) * sizeof( TYPE *) );

	   int i;
	   int newcount = 0;
	   for (i=0;i<newindexoffset;i++)
	   {
		   newdata[i] = this->noelement;
	   }
	   for (i=newindexoffset;i<newsize+newindexoffset;i++)
	   {
		   newdata[i] = (TYPE *) this->data[mymemindex + 1 + i - newindexoffset] ;
		   if (newdata[i] != this->noelement)
		   {
			   int tmpindex = mymemindex + this->indexoffset + i - newindexoffset;
			   if (tmpindex < newmin)
				   newmin = tmpindex;
			   if (tmpindex > newmax)
				   newmax = tmpindex;

			   newcount++;
			   this->data[mymemindex + 1 + i - newindexoffset] = this->noelement;
		   }
	   }
	   for (i=newsize+newindexoffset;i<newsize+2*newindexoffset;i++)
	   {
		   newdata[i] = this->noelement;
	   }

	   if (newmin > newmax)
	   {
		   newmin = 0;
		   newmax = -1;
		   assert(newcount == 0);
	   }


		// create the new vector.
		*pnew  = new KF_IVector<TYPE>(ownselements,newdata,newsize+2*newindexoffset,
			newmin - newindexoffset,newmin, newmax, newcount); // index+1,maximum,newcount);


		// we have to change the maximum and minimum and also the elements ....
		this->count = this->count - newcount;
		if (this->count == 0)
		{
			this->minimum = 0;
			this->maximum = -1;
		}
		else
		{
			int newmaximum = index;
			while (newmaximum >= this->minimum &&
				this->data[newmaximum - this->indexoffset] == this->noelement)
				newmaximum--;
			this->maximum = newmaximum;
		}
	}
	else
	{
		// the new vector has no size ....
		*pnew = new KF_IVector<TYPE>(ownselements);
	}
}

// this operation cuts a vector in two
// the old vector reaches from
// index+1 to maximum and the new vector (pnew) goes
// from minimum to index.
template <class TYPE>
void KF_IVector<TYPE>::CutBegin(int index,KF_IVector<TYPE> **pnew)
{
	*pnew = 0;
	if (index < this->minimum)		return;
	if (index > this->maximum)		return;

	int mymemindex = this->minimum - this->indexoffset;

	int newmin = INT_MAX;
	int newmax = INT_MIN;
	int newsize = index - this->minimum + 1;
	if (newsize > 0)
	{
		// we make sure, that there are -10 left and +10 on the
		// right hand side ....
	   int newindexoffset = 10;

	   TYPE **newdata = (TYPE **) malloc((newsize + 2*newindexoffset ) * sizeof( TYPE *) );

	   int i;
	   int newcount = 0;
	   for (i=0;i<newindexoffset;i++)
	   {
		   newdata[i] = this->noelement;
	   }
	   for (i=newindexoffset;i<newsize+newindexoffset;i++)
	   {
		   newdata[i] = (TYPE *) this->data[mymemindex + i - newindexoffset] ;
		   if (newdata[i] != this->noelement)
		   {
			   int tmpindex = mymemindex + this->indexoffset + i - newindexoffset;
			   if (tmpindex < newmin)
				   newmin = tmpindex;
			   if (tmpindex > newmax)
				   newmax = tmpindex;
			   newcount++;
			   this->data[mymemindex + i - newindexoffset] = this->noelement;
		   }
	   }
	   for (i=newsize+newindexoffset;i<newsize+2*newindexoffset;i++)
	   {
		   newdata[i] = this->noelement;
	   }

	   if (newmin > newmax)
	   {
		   newmin = 0;
		   newmax = -1;
		   assert(newcount == 0);
	   }
		// create the new vector.
		*pnew  = new KF_IVector<TYPE>(ownselements,newdata,newsize+2*newindexoffset,
			newmin - newindexoffset,newmin,newmax,newcount); // minimum,index,newcount);


		// we have to change the maximum and minimum and also the elements ....
		// find the new minimum ....
		
		this->count = this->count - newcount;
		if (this->count == 0)
		{
			this->minimum = 0;
			this->maximum = -1;

		}
		else 
		{
			int newminimum = index + 1;
		    while (newminimum <= this->maximum &&
				this->data[newminimum - this->indexoffset] == this->noelement)
				newminimum++;
			this->minimum = newminimum; 
		}

	}
	else
	{
		// the new vector has no size ....
		*pnew = new KF_IVector<TYPE>(ownselements);
	}

}

#endif /* __KF_IVECT_H */
