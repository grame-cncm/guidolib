#ifndef GRSpring_H
#define GRSpring_H

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

#include "kf_ilist.h"
#include <typeinfo>

// #include "ARMusicalObject.h"
#include "GRNotationElement.h"
#include "GRVoice.h"

typedef KF_IPointerList<GRNotationElement> GROList;	// TODO: replace by std::vector (easy)
typedef KF_IPointerList<GRVoice> GRVList;// TODO: replace by std::vector (easy)

class GRRod;
class GRSystem;
class GRStaff;
#ifdef OLDSPFACTIVE
class GRSpaceForceFunction;
#endif

class GRSpaceForceFunction2;
class GRSpringCollider;


/** \brief Used with rods and space fore functions
*/


class GRSpring  
{
#ifdef OLDSPFACTIVE
	friend class GRSpaceForceFunction;
#endif
	friend class GRSpaceForceFunction2;
	friend class GRRod;
	friend class GRSystem;
public:

			GRSpring( const TYPE_TIMEPOSITION  & vtp,
						const TYPE_DURATION & vdur );
			
			GRSpring( GRNotationElement *, GRVoice * );

	virtual ~GRSpring();

	void OnDraw( VGDevice & hdc ) const;

	bool containsBar() const;

	// A routine to check local collisions
	virtual void checkLocalCollisions();

	// a routine to check the accidental-collisions ...
	virtual void checkAccidentalCollisions();
	
	// this routine looks at the GRSpringCollider
	// of this instance of GRSpring and
	// at the GRSpringCollider of nextspring.
	// It finds matching poslists and
	// returns the maximum needed distance ...
	float GetMaxDistance( GRSpring * nextspring );
	float stretchWithForce( float newforce );
	bool hasStaffAndType( const GRStaff * grstaff, const std::type_info & ti );

	float set_const( float dc );
	static float funcpar;
	static float getFunctionParameter();
	float recalcConstant();
	static void setFunctionParameter( float npar );
	void addElement( GRNotationElement * el,GRVoice * vce );
	bool hasType( const std::type_info & ti );
	void setNoExtent();
	int removeElement( GRNotationElement * el );
	int setGRPositionX( float p_posx );
	void setGRSpringID();


	float getExtent() const 	{ return x; } 

	void setID(int _id);
	int getID();
	virtual const GRNotationElement * getFirstGRO() const;
	virtual GuidoPos getFirstGROPosition();
	virtual const GRNotationElement * getNextGRO(GuidoPos & pos) const;
	virtual float apply_force(float df);
	virtual void addSameTP(GRNotationElement * el,GRVoice * vce);
	virtual float change_x(float dx);
	virtual float change_force(float df);
	virtual float change_const(float dc);
	virtual float change_dur(const TYPE_DURATION & ndur );
	virtual float setlength(float dx);

			float getForce() const { return force; }

	virtual const TYPE_DURATION & getTimePosition() const
	{ return tp; }

	virtual const TYPE_DURATION & getDuration() const
	{ return dur; }

	int operator<(const GRSpring & spr) const;

	virtual float getConstant() const	{ return sconst; }

	static float onlycalcconst(const GRNotationElement *grn);
	static float defconst(const TYPE_DURATION &);
	static float defconst(float );

	int isCommonSpring(int numvoices) const;

	// this flag is set to true, if the spring
	// really has an element of its duration
	// this helps in spacing-issues.
	// (if the spring has no element of this type
	//  the average building process during
	//  the neighbourhoodcheck is made easier)
	bool hasDurElement;

	int isfrozen;
	float posx;	// (JB) was int

protected:
	float calcconst(GRNotationElement * grn);
	TYPE_TIMEPOSITION tp;
	TYPE_DURATION     dur;
	int				  id;      // a definite ID

	// A list of GObjects, that this spring includes
	float			  force;
	float			  x;
	float			  sconst;  // spring-constants

	GROList			  grolst;  //
	GRVList		      grvlst;


	GRSpringCollider * sprcol;

};

int sprcomp(const GRSpring &gr1,const GRSpring &gr2);
int sprpcomp(const GRSpring *gr1,const GRSpring *gr2);

#endif
