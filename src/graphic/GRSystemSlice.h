#ifndef GRSystemSlice_H
#define GRSystemSlice_H

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


#include "GREvent.h"

template <class T> class KF_IVector;
typedef KF_IVector<GRStaff> StaffVector;

class GRSystem;
class GRStaffManager;
class GRBeginSpaceForceFunction2;
class GRSpaceForceFunction2;
class GRPossibleBreakState;
class GRGlue;
class GRSliceHeight;
class GRFinishBar;
class GRDoubleBar;

/** \brief Form the basis for the optimum system-break-algorithm.

	 The system slice is a collection of staves that form non-breakable units they
	  are later glued together to build complete systems.

	GRSystemSlice is a part of a single line of music. A
	set of GRSystemSlices build a GRSystem. For a simple understanding,
	it is convenient to think of system slices as measures
	of a score. Class GRSystemSlice contains one or more
	instances of class GRStaff. Class GRSystemSlice is also responsible
	for graphical elements that belong to several staves;
	this might be, for example, a beam that begins and ends in
	different staves, as it is shown on the right.
*/

class GRSystemSlice : public GREvent
{
	friend class GRStaffManager;
	friend class GRSystem;
	friend class GRSliceHeight;

public:
	using GRNotationElement::getStaffNumber;

					GRSystemSlice(GRStaffManager * stmgr, const TYPE_TIMEPOSITION & tp);
	virtual 		~GRSystemSlice();

	virtual void	tellPosition(GObject * caller, const NVPoint & newPosition);
	virtual void	setHPosition( GCoord nx);
	virtual void 	OnDraw( VGDevice & hdc ) const;
	virtual void	GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;
	virtual void 	updateBoundingBox();


	void		Finish();
	void		FinishSlice();
	GRSystem *	getGRSystem() const			{ return mGrSystem; }
	GRGlue *	getStartGlue() const		{ return mStartGlue; }
	GRGlue *	getEndGlue() const			{ return mEndGlue; }
	int			getStaffNumber( const GRStaff * staff ) const;
	int			getNumber() const			{ return mNumber; }
    StaffVector * getStaves()   { return mStaffs; }
	GRBar*		getBarAt (const TYPE_TIMEPOSITION& pos) const;

	void		addSystemTag( GRNotationElement * tg );
	void		addStaff( GRStaff * newStaff, int num);
	void		addBar( GRBar * mybar, int btype, GRStaff * grstaff);
	void		addDoubleBar(GRDoubleBar * mybar,int bartype, GRStaff * grstaff);
	void		addFinishBar( GRFinishBar * mybar,int btype, GRStaff * grstaff);
	void		addPossibleBreakState( GRPossibleBreakState * in );

	void		setNumber( int num )		{ mNumber = num; }
	bool		hasSystemBars( )			{ return mHasSystemBars; }

protected:
	// this variable holds the current begin
	// sff: this space would be needed, if the
	// slice would be the first of a potential line of music.
	GRBeginSpaceForceFunction2 * mBeginForceFunc;	// was curbegsff
	GRSpaceForceFunction2 * mForceFunction;	// was sff

	StaffVector *	mStaffs;
	GRSystem *		mGrSystem;
	GRPossibleBreakState * mPossibleBreakState;

	int				mNumber;

	GRGlue *		mStartGlue;
	GRGlue *		mEndGlue;
	
	int				mStartSpringID;
	int				mEndSpringID;
	bool			mHasSystemBars;
	float			mFirstEvXPosition;  // used by the first slice to split the mapping consistently to the next systems
};

#endif
