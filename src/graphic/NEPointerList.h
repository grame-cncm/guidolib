#ifndef NEPointerList_H
#define NEPointerList_H

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

/** \brief A list of notation elements
	
	TODO: replace by a std::vector

*/

#include "NEList.h"
#include "kf_ilist.h"	// for NEList
#include "defines.h"	// for TYPE_TIMEPOSITION
#include "GUIDOScoreMap.h"

class VGDevice;
class GObject;
class NVPoint;
class MapInfos;


class NEPointerList : public NEList  
{
  public:
	
	  			NEPointerList( const NEPointerList & lst, int ownselements = 0);
				NEPointerList( int ownselements = 0);

		virtual ~NEPointerList();

		void print();

	  virtual void setHPosition( float nx );
		int getMaxSpacing();

		GRNotationElement* getElementWithMaxSpacing();

		NEPointerList* getElementsWithShortestDurationNotNull();
		NEPointerList* getElementsWithShortestDuration();

		void setSpacing(const TYPE_TIMEPOSITION & timePosition, NVPoint & position);

		void setZeroSpacing(const TYPE_TIMEPOSITION & timePosition, 
			NVPoint &position, int addspace = 1);

		NEPointerList* getElementsWithTimePosition(const TYPE_TIMEPOSITION & timePosition);

		TYPE_TIMEPOSITION getNextTimePosition(const TYPE_TIMEPOSITION & timePosition);

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;

		virtual void setPosition(const NVPoint & newPosition);

		// tells all list elements the new position.
		void tellPosition(GObject * caller, const NVPoint & newPosition);

		// here some functions for time-sorted insert
		GuidoPos AddAtCorrectTimePosition(GRNotationElement * grne);
		GuidoPos AddAtTimePosition(const TYPE_TIMEPOSITION & tp, GRNotationElement * grne);
};

#endif


