#ifndef GRSystemSlice_H
#define GRSystemSlice_H

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
	virtual void	GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
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

	void		setNumber(int num )	{ mNumber = num; }
	bool		hasSystemBars( )	{ return mHasSystemBars; }

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
	bool			mHasSystemBars;     // Totally useless for now, I guess
	float			mFirstEvXPosition;  // used by the first slice to split the mapping consistently to the next systems
};

#endif
