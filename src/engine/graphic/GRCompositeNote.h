#ifndef GRCompositeNote_H
#define GRCompositeNote_H

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

#include "GRNote.h"

class ARNote;
class GRSingleNote;
class GRTie;


/** \brief This class describes an abstract note which consists of several 
	grafical notes which are linked by ties.
*/
class GRCompositeNote : public GRNote
{
  public:

				GRCompositeNote( GRStaff *, ARNote * abstractRepresentationOfNote );

		virtual ~GRCompositeNote();

		virtual void OnDraw( VGDevice & hdc ) const;

		
		virtual void updateBoundingBox();

		virtual void setPosition(const NVPoint & newPosition);

		virtual void recalcVerticalPosition();

		virtual NVPoint getTieStart() const;
		virtual NVPoint getTieEnd() const;

     	virtual TYPE_TIMEPOSITION getNextTimePosition(const TYPE_TIMEPOSITION tp) const;
		virtual GRNotationElement * getGRNotationElementAtTimePosition(
      	const TYPE_TIMEPOSITION tp);

		virtual void addNote(GRNote * note);
		virtual void finish();

		virtual void tellPosition(GObject * caller, const NVPoint & point);
			// this function will be called by "Child"-notes

		virtual void drawStemOnly(int flag);
		virtual void setStemDirection(GDirection dir);

      virtual float setStemLength( float inLen );

  private:

      GRNote * prevnote;
      GRTie * prevtie;
};



#endif

