#ifndef GRSliceHeight_H
#define GRSliceHeight_H

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

#include "kf_vect.h"

class GRSystemSlice;

typedef KF_Vector<float> HeightVector;

/** \brief This class manages the Height of Slices. 

This is important so that we know, how high a collection of 
slices will really be. The Idea is really simple : we just have to remember the maximum height of 
each staff within all the slices. Whenever a new slice is added, we check the individual staff-heights 
and check, wether they are bigger than the stored maximum in which case we update the height.
*/

class GRSliceHeight
{
	public:
					GRSliceHeight();
		virtual 	~GRSliceHeight();

		void 		AddSystemSlice(const GRSystemSlice * slc, bool doboundingrect = false );
		float 		getHeight( bool notoprect = false ) const;
		float 		getPosition(int i) const;

	protected:

		HeightVector mHVector;
		float boundingRectTop;
		float boundingRectBottom;
		float minHeight;
};

/* Revision Control

Revision 1.1  2002-09-21 17:11:04+02  renz
Needed more parameters in sliceheight; for once I need keep track
of the boundingRectTop of the first and boundingRectBottom of the
last staff. I also need a minHeight (which is set by a begin slice).
The original idea that I should just remember the maximum heights of the
staves did not work, because the height is determined by positions assigned
to the staves be GRSystemSlice. So now the height-vector is just a
position vector for the staves of the slice.

Revision 1.0  2002-09-18 09:08:28-07  renz
GRSliceHeight manages the height of a collection of slices. This is important for
tracking, where the systems of a page might be placed.

Revision 1.7  2002-09-13 12:13:15+02  renz
Default_system_Distance
and system_Distribution is now changeable by the user
The calculation for the Optimum break now not only gets one way
but the best way, even if the required force is not within the limit ....
One change in rod-handling (rods are now deleted correctly when
new systems are build (createBeginSlice)

Revision 1.6  2002-09-09 13:28:57+02  renz
Added stuff for Progress-dialog
changed some stuff in desctructor to free memory when a user abort is happening.
Changed the way, rods are added -> previously, the rods were resorted every time a new rod was entered - very 
inefficient. Now, rods are only sorted when necessary. Big decrease in running time!

Revision 1.5  2002-08-29 11:36:38+02  renz
Added RCS Headers
Very many changes in GRStaffManager.
This manager now has to build a list of systemslices
which are later combined to form systems.
This class also includes the new optimum-page-fill and
optimum-line-break algorithm.
Lots of old functions are commented out
*/

#endif
