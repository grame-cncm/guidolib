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

#include <cmath>

#include "kf_ivect.h"
#include "GRSpacingMatrix.h"
#include "GRSpring.h"
const int NVALUESINMSCMATRIX = 6;

GRSpacingMatrix::GRSpacingMatrix()
{
	mMatrixMemSize = 50;
	mMatrixRealSize = 0;

//	mMatrix = (double *) malloc(mscmatmemsize * NVALUESINMSCMATRIX * 
//		sizeof(double));
	mMatrix = new double [ mMatrixMemSize * NVALUESINMSCMATRIX ];
	
	memset(mMatrix,0,mMatrixMemSize * NVALUESINMSCMATRIX * sizeof(double));

	mNeighbours = NULL;
	mNeighboursRight = NULL;

	mLastLeft = -1;
	mLastRight = -1;
}

GRSpacingMatrix::~GRSpacingMatrix()
{
	delete [] mMatrix;
	delete mNeighbours;
	delete mNeighboursRight;
}

// this routine resizes the matrix if necessary
double * GRSpacingMatrix::resizeMSCMatrix(int newrealsize)
{
	if (newrealsize > mMatrixMemSize)
	{
		int newmemsize = newrealsize + 10;
		double * newmat = new double [ newmemsize * NVALUESINMSCMATRIX ];

		memset(newmat,0,sizeof(double)*newmemsize*NVALUESINMSCMATRIX);
		// copy old matrix entries 
		// that's is wrong ....
		// why?
		int i;
		int j;
		for (i=0;i<mMatrixMemSize;i++)
		{
			for (j=0;j<NVALUESINMSCMATRIX;j++)
			{
				*(newmat + i + j*newmemsize) = *(mMatrix + i +
					j*mMatrixMemSize);
			}
		}

		mMatrixMemSize = newmemsize;

		double * oldmat = mMatrix;
		mMatrix = newmat;

		delete [] oldmat;
	}

	mMatrixRealSize = newrealsize;

	return mMatrix;
}

double GRSpacingMatrix::getMSCMatrix(int i,int j) const
{
	if (i>=0 && i< mMatrixRealSize &&
		j>=0 && j< NVALUESINMSCMATRIX)
	{
		return *(mMatrix + i + j*mMatrixMemSize); 
	}
	return 0;
}

void GRSpacingMatrix::setMSCMatrix(int i,int j,double value)
{
	if (i>=0 && i< mMatrixRealSize && j>=0 && j< NVALUESINMSCMATRIX)
	{
		*(mMatrix + i + j*mMatrixMemSize) = value;
	}

}

double * GRSpacingMatrix::getMSCMatrix()
{
	return mMatrix;
}

int GRSpacingMatrix::getMSCMatrixMemSize()
{
	return mMatrixMemSize;
}

int GRSpacingMatrix::getMSCMatrixRealSize()
{
	return mMatrixRealSize;
}

void GRSpacingMatrix::AddNeighbourList(KF_List<int> *nl)
{
	// the neighbourlist ....
	if (!mNeighbours)
	{
		// owns elements ....
		mNeighbours = new listofpointerstointlists(1);
		mNeighbours->AddTail(nl);
		mLastLeft = nl->GetHead();

		mNeighboursRight = new listofpointerstointlists(0);
		mNeighboursRight->AddTail(nl);
		mLastRight = nl->GetTail();
		return;
	}

	GuidoPos pos,prevpos;

	int newleft = nl->GetHead();
	int newright = nl->GetTail();

	if (newleft >= mLastLeft)
	{
		mLastLeft = newleft;
		mNeighbours->AddTail(nl);
		goto addright;
	}

	// find the right position to put the thing in;
	// only the first value matters ....
	pos = mNeighbours->GetHeadPosition();
	while (pos)
	{
		prevpos = pos;
		intlist * curlist = mNeighbours->GetNext(pos);
		if (curlist)
		{
			int left = curlist->GetHead();

			if (newleft <= left)
			{
				// then we are in for it ....
				mNeighbours->AddElementAt(prevpos,nl);
				goto addright;
			}
		}
	}
	
addright:

	// now we look where to put the right element ...
	if (newright <= mLastRight)
	{
		mLastRight = newright;
		mNeighboursRight->AddTail(nl);
		return;
	}

	// find the right position to put the thing in;
	// only the first value matters ....
	pos = mNeighboursRight->GetHeadPosition();
	while (pos)
	{
		prevpos = pos;
		intlist * curlist = mNeighboursRight->GetNext(pos);
		if (curlist)
		{
			int right = curlist->GetTail();

			if (newright >= right)
			{
				// then we are in for it ....
				mNeighboursRight->AddElementAt(prevpos,nl);
				break;
			}
		}
	}
}

/** \brief Checks wether any neighbours have "funny" spacing; this means that the 
	spring-constants do not match.
*/
// (JB) There were lots of double types mixed with float types. This generated 
// a huge amount of conversions and warnings. I've changed everything to double. 
// 'Hope I did not hurt the algo.
void GRSpacingMatrix::CheckNeighbours(ISpringVector *sprvect, float spring)
{
	if (!mNeighbours) 
		return;

	// create a new list does not own the pointers ...
	//listofpointerstointlists * errlist = new listofpointerstointlists(0);
	listofpointerstointlists errlist (0); // (JB) replaced by this stack allocation.

	const double oneOverLogTwo = 1.0 / log( 2.0 );
	
	// additionally, we check the variance (difference from the 
	// mean/average-value)
	GuidoPos pos = mNeighbours->GetHeadPosition();
	while (pos)
	{
		double value = 0.0f;
		double logvalue = 0;
		double diffpartial = 0;
		double checkval = 0.0f;
		double logcheckval = 0;
		double average = 0;
		double numvalues = 0;	// (JB) was an int, but the code will be faster with double.
		double theMin = 1e10;
		double theMax = 0;
		bool haserror = 0;
		//int allHaveDurElements = 1;
		intlist * curlist = mNeighbours->GetNext(pos);
		if (curlist)
		{
			GuidoPos pos2 = curlist->GetHeadPosition();
			int i1 = -1;
			int i2 = -1;
			bool first = true;
			while (pos2)
			{
				i1 = i2;
				if (i1 == -1)
				{	
					i1 = curlist->GetNext(pos2);
					assert( pos2 );
				}
				i2 = curlist->GetNext(pos2);

				checkval = 0.0f;
				// now we check 
				for( int cnt = i1; cnt < i2; cnt++ )
				{
					double val = *(mMatrix + cnt + mMatrixMemSize * 3);
					if (val < theMin)	theMin = val;
					if (val > theMax)	theMax = val;
					
					checkval += val;
					average += val;
					++ numvalues;
				}

				checkval /= (i2 - i1);

				// now we build the log-value of the partial average

				logcheckval = log(checkval) * oneOverLogTwo;

				// now we check the 
				if (first)
				{
					value = checkval;
					logvalue = logcheckval;
					first = false;
				}
				else
				{
					if (value != checkval)
					{
						// then we have an error in this neighbourhood!
						double tmpdiff = logcheckval - logvalue;
						if (tmpdiff < 0)
							tmpdiff = - tmpdiff;
						if (tmpdiff > diffpartial)
							diffpartial = tmpdiff;

						// then we determine the difference 
						haserror = true;

						// errlist->AddTail(curlist);
						
						// break;
					}
				}
			}

			if (haserror) //  && !allHaveDurElements) 
			{
				// now we have to check  the variance
//				average /= numvalues;  never used

				//float log2 = log(2.0);

				//float logaverage = log(average) / log2;
				//float logmax = log(max) / log2;
				//float logmin = log(min) / log2;

				//float diff1 = logmax - logaverage;
				//float diff2 = logaverage - logmin;
				//float diff3 = logmax - logmin;
				
				errlist.AddTail( curlist );
			}
		}
	}

	if (errlist.GetCount() == 0)
	{
		// we have not found a neighbourhoodviolation ...
		//delete errlist;
		return;
	}

	// else, we have an error-spacing somewhere.
	// now, we have to find out, which ranges to 
	// to adjust ...
	// this is done by looking if the range is already
	// extended enough .... // the BRUTE FORCE approach
	// is to compare each of the errlist entries to 
	// all of the neighbourhoodlists and to
	// to extend the entries, if they match ....
	// in order for this to work, the 
	// list must be sorted .....

	// listofpointerstointlists * errlist2 = new listofpointerstointlists(1); // (JB), replaced by...
	listofpointerstointlists errlist2 (1);		// ...a stack one.

	pos = errlist.GetHeadPosition();
	int lastleft = -1;
	while (pos)
	{
		// we iterate through the errorlist ...
		intlist * curlist = errlist.GetNext(pos);
		if (curlist)
		{
			int left = curlist->GetHead();
			int right = curlist->GetTail();

			GuidoPos pos2 = mNeighbours->GetHeadPosition();
			while (pos2)
			{
				intlist * complist = mNeighbours->GetNext(pos2);
				if (complist)
				{
					int compleft = complist->GetHead();
					int compright = complist->GetTail();

					if (compright > left && compleft < right)
					{
						//if (compleft < left)
						//	left = compleft;
						if (compright > right)
							right = compright;
					}
				}
			}

			pos2 = mNeighboursRight->GetHeadPosition();
			while (pos2)
			{
				intlist * complist = mNeighboursRight->GetNext(pos2);
				if (complist)
				{
					int compleft = complist->GetHead();
					int compright = complist->GetTail();

					if (compright > left && compleft < right)
					{
						if (compleft < left)
							left = compleft;
						//if (compright > right)
						//	right = compright;
					}
				}
			}

			if (left == lastleft)
			{
				// in this case, the last addition was similar
				// to the one that was being added ....
			}
			else
			{
			  // then we add that element ....
			  intlist * newlist = new intlist;
			  newlist->AddTail(left);
			  newlist->AddTail(right);
			  errlist2.AddTail(newlist);
			  mLastLeft = left;
			}
		}
	}

	// we don't need errlist any longer
//	delete errlist;

	// now we need to sort the errlist2 and
	// then we have what we want ....

	// we should have a complete errlist2 ....

	// we could do something with "partial" averages
	// have thought about how to exactly calculate this
	// Nevertheless, it seems to be sufficient to just
	// build the averages that are present at the springs
	// at this time ....


	// we can compare the effect of the "old" gourlay-
	// spring-constants and of the new, average (or maximum)
	// spacing. 
	// then we can compare the effect of a "normal" force
	// and see the difference.


	pos = errlist2.GetHeadPosition();
	while (pos)
	{
		intlist * curlist = errlist2.GetNext(pos);

		int left = curlist->GetHead();
		int right = curlist->GetTail();

		double average = 0;
		double max = 0;
		double oldconst = 0;
		double sumsprdur = 0;
		double newmaxconst = 0;
		double newaverageconst = 0;
		bool first = true;
		for( int cnt = left; cnt < right; cnt++ )
		{
			double val = *(mMatrix + cnt + mMatrixMemSize*3); 
			GRSpring * spr = sprvect->Get(cnt);
			if (spr)
			{
				TYPE_DURATION myfrac(val);
				const TYPE_DURATION & dur = spr->getDuration();
				myfrac.invert();
				double sconst = GRSpring::defconst(myfrac, spring);
				sconst *= (double)myfrac / (double)dur;

				if (first)
				{
					oldconst = sconst;
					first = false;
				}
				else
				{
					// oldconst = 1.0 / (1.0/oldconst + 1.0/sconst);			// <- was
					oldconst = (oldconst * sconst) / (oldconst + sconst);		// <- (JB) replaced by
				}																

				sumsprdur += (double)dur;
			}
			// we just take the values from the matrix
			// and build the average ...
			average += *(mMatrix + cnt + mMatrixMemSize*3);
			if (*(mMatrix + cnt + mMatrixMemSize*3) > max)
			{
				max = *(mMatrix	+ cnt + mMatrixMemSize*3);
			}
		}

		average /= (right-left);

		{
		TYPE_DURATION myfrac(max);
		myfrac.invert();
		float sconst = GRSpring::defconst(myfrac, spring);
		
		//newmaxconst = 1.0 / (sumsprdur / sconst * max ); // <- (JB) was
		newmaxconst = sconst / (sumsprdur * max);

		}

		{
		TYPE_DURATION myfrac(average);
		myfrac.invert();
		float sconst = GRSpring::defconst(myfrac, spring);
		
			// newaverageconst = 1.0 / (sumsprdur / sconst * average ); // <- (JB) was
			newaverageconst = sconst / (sumsprdur * average);
	
		}
		// now I have the oldconst and the newconst
		// and I can compare them ....

		double diff = oldconst - newaverageconst;
		if (diff < 0)
			diff = - diff;
		
		bool doit = false;
		double value;
		if (diff< 0.05 )
		{
			doit = true;
			value = average;
		}
		else
		{
			diff = oldconst - newmaxconst;
			if (diff < 0)
				diff = -diff;
			if (diff < 0.17)
			{
				doit = true;
				value = max;
			}
		}
		
		if (doit)
		{
			for (int cnt=left; cnt < right; cnt++)
			{
#ifdef _DEBUG
				GRSpring * spr = sprvect->Get(cnt);
#endif
				// we just take the values from the matrix
				// and build the average ...
				*(mMatrix + cnt + mMatrixMemSize*3) = value; // max; // average;
			}
		}

	}
//	delete errlist2;
}

