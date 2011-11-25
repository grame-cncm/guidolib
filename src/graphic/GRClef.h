#ifndef GRClef_H
#define GRClef_H

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
	virtual void print() const;
	
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


