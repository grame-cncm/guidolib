#ifndef GRKey_H
#define GRKey_H

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


