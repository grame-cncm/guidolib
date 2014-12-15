#ifndef GRClef_H
#define GRClef_H

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

#include "GRTagARNotationElement.h"
#include "GuidoDefs.h"

class ARClef;
class GRStaff;


/** \brief Graphial representation of a staff clef.
*/

class GRClef : public GRTagARNotationElement //,  public GRTag  
{
	
public:

/*	enum CLEFTYPE { NONE = 32, // SCR_NOCLEFT,
					VIOLIN = 38, // SCR_GCLEF, 
					BASS = 63, // SCR_FCLEF,
					BRATSCHE = 66 // SCR_BCLEF 
				};*/

					GRClef( ARClef * arClef, GRStaff * curstaff, bool ownsAR = false );
	
	virtual 		~GRClef();
	
	virtual bool operator ==(const GRTag & tag) const;
	virtual bool operator ==(const GRClef & clef) const;

	virtual void OnDraw( VGDevice & hdc ) const;
	virtual void GGSOutput() const;

	virtual const NVPoint & getReferencePosition() const;
	virtual unsigned int getTextAlign() const { return sClefTextAlign; }
		
	const ARClef * getARClef() const;
	
	
	TYPE_PITCH getBasePitch() const			{ return mClefBasePitch; }
	TYPE_REGISTER getBaseOct() const		{ return mClefBaseOctave; }
	int getBaseLine() const					{ return mClefStaffLine; }
	

	//virtual long getLeftSpace() const;
	// virtual long getRightSpace() const;
	
protected:

	TYPE_PITCH 		mClefBasePitch;
	TYPE_REGISTER 	mClefBaseOctave;
	int 			mClefStaffLine; // line count from the bottom of the staff.
	const char * 	mOctaveStr;
	NVPoint 		mOctaveOffset;
	
	bool			mDoubleTreble;
	
//	static char * cp8;
//	static char * cp15;

	static NVPoint refposPerc;
	static NVPoint refposTreble;
	static NVPoint refposAlto;
	static NVPoint refposBass;
	static unsigned int sClefTextAlign;
	
	
};


#endif


