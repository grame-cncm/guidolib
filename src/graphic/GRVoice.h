#ifndef GRVoice_H
#define GRVoice_H

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
class GuidoFeedback;

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

	int createNewRods(GRStaffManager * stfmgr,int &startspr,int & endspr);
	virtual void rememberNLinePosition(const TYPE_TIMEPOSITION & tp);
	virtual void setPossibleNLinePosition(const TYPE_TIMEPOSITION & tp);
//	void createRods(GRSystem *grsystem,int lastsprid);

	virtual void GGSOutput() const {}
	virtual void OnDraw( VGDevice & ) const {}
    virtual void print() const {}

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
