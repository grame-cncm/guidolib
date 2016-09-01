#ifndef GRSystem_H
#define GRSystem_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

template <class T> class KF_IVector;

// #include <vector>

#include "kf_ilist.h"	// required

#include "NEPointerList.h"
#include "GRRod.h"
#include "GRSystemSlice.h"
#include "GREvent.h"

#ifdef OLDSPFACTIVE
	class GRSpaceForceFunction;
#endif

class ARSystemFormat;
class ARMusic;
class ARBar;
class ARAccolade;

class GRPage;
class GRStaff;
class GRSpring;
class GRAccolade;
class GRSpaceForceFunction2;

typedef KF_IVector<GRStaff> StaffVector;
// typedef KF_IPointerList<GRSpring> SpringList;
typedef KF_IVector<GRSpring> ISpringVector;
typedef KF_IPointerList<GRRod> IRodList;
typedef KF_IPointerList<GRSystemSlice> SSliceList;

// typedef std::vector<GRAccolade *> GRAccoladeList;

extern GRStaff * gCurStaff;

/** \brief Manages a number of staffs and has a given start and end timeposition.
	GRSystem is the grafical representation of a part of ARMusic.
 
 	GRSystem is derived from GREvent, because GRSystem is really a musical 
	event and directly represents a part of ARMusic.
*/
class GRSystem : public GREvent
{
	// this is for curPosX and oldposx
	friend class GRStaffManager;
	
public:
	using GRNotationElement::getStaffNumber;

	enum BARTYPE { SYSTEM, ACCOLADE, STAFF, MENSUR };
/*
				GRSystem( 	GRPage * inPage, 
							const TYPE_TIMEPOSITION & relativeTimePositionOfSystem, float force);
			*/
							
				GRSystem(GRStaffManager * stfmgr,
							GRPage * inPage,
							const TYPE_TIMEPOSITION & relativeTimePositionOfSystem,
							SSliceList ** systemslices,
							int count,
							GRSystemSlice * beginslice,
							ISpringVector ** pvect,
							ARSystemFormat * sysform,
							float force, float spring, float proportionnalRender,
							bool islastsystem = false);
	
	virtual 			~GRSystem();

	virtual	float		getSystemWidthCm();
	virtual GRPage *	getGRPage()                             { return mPage; }
	GRSystemSlice *     getFirstGRSystemSlice();
	float               getDistance() const                     { return mDistance; }
	int                 getStaffNumber(const GRStaff * staff ) const;
	int                 getNewLinePage() const                  { return mNewLinePage; }
	float               getCurPosX() const                      { return mCurPosX; }							
	ARMusic *           getARMusic() const;
	GRSpring *          getSpring( int id ) const;
	const GRSpring *    getGRSpring( int id ) const;
    StaffVector *       getStaves() const   { return mStaffs; }
	SSliceList *        getSlices()         { return &mSystemSlices; }

	void        setSpringParameter(float nconst);
	void        setSystemFormat( ARSystemFormat * sysfrm );
	void        setDistance( float inDistance )  	{ mDistanceSet = true; mDistance = inDistance; }
	void        setGRPage( GRPage * inNewPage )		{ mPage = inNewPage; }
	void        setMeterOfStaffs( int num, int denom );

	virtual void		OnDraw( VGDevice & hdc ) const;
	virtual void		GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;
	virtual void		GGSOutput() const;
	virtual void		setPosition( class NVPoint const & );
	virtual void		updateBoundingBox();

	void	addSystemTag( GRNotationElement * mytag );
	void 	addBar( GRBar * mybar, enum GRSystem::BARTYPE btype, GRStaff * inStaff );
	void 	addToSpring(GRNotationElement * el, int id );
	void	addStaff( GRStaff * newStaff, int num );

	bool 	IsDistanceSet() const 				{ return mDistanceSet; }
	void	notifyAccoladeTag( ARAccolade * inAccoladeTag );

	void 	FinishSystem();
	void 	FinishSystem( ISpringVector * pvect, IRodList * prods1, IRodList * prods2,
#ifdef OLDSPFACTIVE
        GRSpaceForceFunction * pspf,
#endif
        GRSpaceForceFunction2 * psff, const TYPE_TIMEPOSITION & tp, int lastline = 0);

	TYPE_TIMEPOSITION mDebugSystemDate;
    static int sSystemID;


protected:

	void 		dospacing();
//	void		DrawAccolade(  VGDevice & hdc, const NVPoint & leftTop, const NVPoint & leftBottom, int id ) const;
	void 		DrawSystemSprings( VGDevice & hdc ) const;
	void 		DrawSystemForce( VGDevice & hdc ) const;	
	void		AddSystemSlice( GRSystemSlice * inSlice );
	void		InitForceFunction (GRStaffManager * staffmgr, SSliceList ** psystemslices, int count);
	void		AdjustForceFunction (GRSliceHeight &sliceheight, int &startspring, int &endspring, float optForce);
	GRStaff *	ComputeBoundingBox(GRSliceHeight &sliceheight);
								
	float 	mSystemforce; 		// The force used to stretch the system
	float 	mMarginLeft;		 // Margin to the Left (=indent)
	float 	mDistance;
	bool 	mDistanceSet;
	bool 	mIsLastLine;		// was int
	
//	int		mAccoladeType;	// (JB) new. Should be replaced by a list of accolades.

	// this is returned by GRStaffManager.
	ISpringVector * mSpringVector;
	
	
	// we have two rodlists, one simple one
	// with simple rods (spanning exactly one spring)
	// and another one with complex rods (spanning
	// more than one spring). These both are returned
	// by the StaffManager:
	IRodList * simplerods;
	IRodList * complexrods;
	
#ifdef OLDSPFACTIVE
	GRSpaceForceFunction * spf;
#endif

	GRSpaceForceFunction2 * mSpaceForceFunc;

	SSliceList 		mSystemSlices;
	
	float 			mOldPosX;
	int 			mNewLinePage;
	float 			mCurPosX;
	GRPage * 		mPage;
	NEPointerList 	mZeroSpaceList;
	NEPointerList 	mSpaceList;

	// GRAccoladeList	mAccolades; // TODO
	std::vector<GRAccolade *>	mAccolade;		// TEMP

private:  

	StaffVector * mStaffs;
	NVPoint mNextStaffPosition;
};

#endif


