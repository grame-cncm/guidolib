#ifndef GRSpacingMatrix_H
#define GRSpacingMatrix_H

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

class GRSpring;

template <class T> class KF_IVector;
template <class T> class KF_List;
template <class T> class KF_IPointerList;

typedef KF_List<int> intlist;
typedef KF_IPointerList<intlist> listofpointerstointlists;

typedef KF_IVector<GRSpring> ISpringVector;

/** \brief not yet documented
*/
class GRSpacingMatrix
{
	public:
					GRSpacingMatrix();
		virtual		~GRSpacingMatrix();

		int			getMSCMatrixRealSize();
		int			getMSCMatrixMemSize();
		double *	getMSCMatrix();

		double		getMSCMatrix(int i,int j) const;
		void		setMSCMatrix(int i,int j, double value);

		double *	resizeMSCMatrix( int newrealsize );

		void		AddNeighbourList(KF_List<int> * nl);

		void		CheckNeighbours(ISpringVector *sprvect, float spring);
 
	protected:

		int			mMatrixMemSize;
		int			mMatrixRealSize;
		double *	mMatrix;

		listofpointerstointlists * mNeighbours;
		listofpointerstointlists * mNeighboursRight;

		int mLastLeft;
		int mLastRight;

};

#endif
