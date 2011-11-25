#ifndef GRRod_H
#define GRRod_H

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

class GRNotationElement;
class GRSystem;
class GRStaff;
class GRVoice;
class GRStaffManager;
class GRSpring;

template <class T> class KF_IPointerList;
template <class T> class KF_IVector;

typedef KF_IPointerList<GRSpring> SpringList;
typedef KF_IVector<GRSpring> SpringVector;

/** \brief Used with springs and space-force functions 
*/
class GRRod  
{
	friend class GRSystem;
	friend class GRStaff;
	friend class GRVoice;
	friend class GRStaffManager;

public:

			GRRod( const GRRod & rod);
			GRRod( float inLength, int inSpring1, int inSpring2 );
			GRRod( const GRNotationElement * gr1, const GRNotationElement * gr2, 
																	int lastspringid);

			GRRod(const GRNotationElement * gr1, const GRNotationElement * gr2, 
										int lastspringid,float spacedistance);
	virtual ~GRRod() { }

	int 	resetForce(const GRRod & rd);
	int 	getSpr1() const				{ return mSpr1; }
	int 	getSpr2() const				{ return mSpr2; }
	bool 	getIsSpaceRod() const		{ return mIsSpaceRod; }
	void 	setIsSpaceRod(bool flag)	{ mIsSpaceRod = flag; }
	float 	getLength() const  			{ return mLength; }
	void	setLength( float in )		{ mLength = in; }

	GRStaff * mGrStaff;

	int 	operator < (const GRRod & r2) const;
	float 	stretchsprings(float inForce, SpringVector * sprvect);
	float 	calclength(float inForce ,SpringVector * sprvect);
	float 	calcforce(SpringVector * sprvect);
	int 	spansOne() const;
	void 	setSpringIDs(int id1, int id2)
							{ mSpr1 = id1; mSpr2 = id2; }
protected:

	float 	mForce;
	bool	mIsSpaceRod;
	int 	mSpr1;
	int 	mSpr2;
	float 	mLength;
};

inline int rodpcomp(const GRRod * r1, const GRRod * r2)
{
	return *r1 < *r2;
}

inline int GRRod::spansOne() const
{
	if (mSpr1 == mSpr2 - 1)
		return 1;
	return 0;
}

#endif
