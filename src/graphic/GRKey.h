#ifndef GRKey_H
#define GRKey_H

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

#include "GRARCompositeNotationElement.h"
#include "GRTag.h"
#include "GuidoDefs.h" // for NUMNOTES

class ARKey;
class GRStaff;


/** \brief Key notation tag
*/
class GRKey : public GRARCompositeNotationElement, public GRTag
{
  public:

	  virtual bool operator==(const GRKey & key) const;
	  virtual bool operator==(const GRTag & tag) const;

						GRKey( GRStaff * staff, ARKey * arkey, int p_natural, bool ownsAR );
		virtual 		~GRKey();

	  	virtual void 	setGRStaff(GRStaff * stf);
				int 	getKeyArray(float * KeyArray);
		static 	int 	getNonFreeKeyArray(int pnumkeys, float * KeyArray);
				void 	getOctArray(int * ctArray);

		virtual void 	updateBoundingBox();

		virtual void 	setHPosition( GCoord nx);
		virtual void 	setPosition(const NVPoint & point);


		virtual void 	print() const;

		virtual void 	GGSOutput() const;
		virtual void 	OnDraw(VGDevice & dev ) const;

      	virtual void 	recalcVerticalPosition();

  protected:

		int		mNatural;
		int		mNumKeys;
		float	mkarray[NUMNOTES];
		int		mOctarray[NUMNOTES]; // octave of accidentlas

		// GRStaff *grstaff;

		virtual void createAccidentals();

		float mCurLSPACE; // this parameter remembers the LSPACE that is
				// valid at the time of creation. We can later recalc
				// positions better (when a clef-change occurs for example)
				// if this parameter is known.
};


#endif


