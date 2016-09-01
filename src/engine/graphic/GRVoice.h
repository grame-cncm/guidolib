#ifndef GRVoice_H
#define GRVoice_H

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

// #include "ARMusicalVoiceState.h"
#include "GRARCompositeNotationElement.h"

template <class T> class KF_IPointerList;
template <class T> class KF_IVector;

class ARMusicalVoice;
class GRSystem;
class GRVoice;
class GRStaffManager;
class GRVoiceManager;
class GRRod;
class GRSpring;
class GRPage;
class GRSpaceForceFunction2;
class GRSpacingMatrix;

#ifdef OLDSPFACTIVE
class GRSpaceForceFunction;
#endif

class SystemNode
{
		friend class GRVoice;
	private:
		GRSystem * grsystem;
		GuidoPos  pos;
};

// typedef KF_Vector<GuidoPos> SystemOffsetVector;
typedef KF_IPointerList<SystemNode> SysNodeList;
typedef KF_IVector<GRSpring> ISpringVector;

/** \brief Graphical representation of a voice.
*/
class GRVoice : public GRARCompositeNotationElement  
{
	friend class GRVoiceManager;

	public:
				GRVoice( ARMusicalObject * arobj, bool ownsar );
		virtual ~GRVoice();


				GRPage * getPageNum(int num,int denom);		// must match an existing element date.
				GRPage * getPageForTimePos( int num, int denom ) const;
				GRRod * getLastRod() const	{ return lastrod; }
				GRRod * getFirstRod() const	{ return firstrod; }


		virtual GuidoPos AddTail(GRNotationElement * el);

#ifdef OLDSPFACTIVE
	int updateQPMatrix(GRSpaceForceFunction * spf,
		ISpringVector * sprvect,
		int startspr, int endspr);

	int updateMSCMatrix(GRSpaceForceFunction * spf,
		int startspr, int endspr);
#endif
	int updateMSCMatrix(GRSpaceForceFunction2 * sff,
						GRSpacingMatrix * spm,	
						GRStaffManager * stfmgr,
						ISpringVector * sprvect,
						int startspr, int endspr);

	int createNewRods(GRStaffManager * stfmgr, int &startspr, int & endspr, float optForce);
	virtual void rememberNLinePosition(const TYPE_TIMEPOSITION & tp);
	virtual void setPossibleNLinePosition(const TYPE_TIMEPOSITION & tp);
//	void createRods(GRSystem *grsystem,int lastsprid);

	virtual void GGSOutput() const {}
	virtual void OnDraw( VGDevice & ) const {}
    
	ARMusicalVoice * getARMusicalVoice();

	void startNewSystem(GRSystem * grsystem);

	protected:

		GuidoPos getSystemStartPos(GRSystem * grsystem);

		GuidoPos 	firstPositionInLine;
		bool 		mIsNewLine;
		GuidoPos 	lastsprpos;
		GRRod * 	lastrod;
		GRRod * 	firstrod;
		TYPE_TIMEPOSITION nlinetp;
		GuidoPos 	nlinepos;
		SysNodeList * mSysNodeList;
};

#endif
