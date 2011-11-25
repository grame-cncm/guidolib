#ifndef GRCompositeNote_H
#define GRCompositeNote_H

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

		virtual void print() const;

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

