#ifndef NEPointerList_H
#define NEPointerList_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

		void print(std::ostream& os) const;
		std::string reduce_print() const;

	  virtual void setHPosition( float nx );
		int getMaxSpacing();

		GRNotationElement* getElementWithMaxSpacing();

		NEPointerList* getElementsWithShortestDurationNotNull();
		NEPointerList* getElementsWithShortestDuration();

		void setSpacing(const TYPE_TIMEPOSITION & timePosition, NVPoint & position);

		void setZeroSpacing(const TYPE_TIMEPOSITION & timePosition, 
			NVPoint &position, int addspace = 1);

		NEPointerList* getElementsWithTimePosition(const TYPE_TIMEPOSITION & timePosition) const;

		TYPE_TIMEPOSITION getNextTimePosition(const TYPE_TIMEPOSITION & timePosition);

		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;

		virtual void setPosition(const NVPoint & newPosition);

		// tells all list elements the new position.
		void tellPosition(GObject * caller, const NVPoint & newPosition);

		// here some functions for time-sorted insert
		GuidoPos AddAtCorrectTimePosition(GRNotationElement * grne);
		GuidoPos AddAtTimePosition(const TYPE_TIMEPOSITION & tp, GRNotationElement * grne);
};

std::ostream & operator << ( std::ostream & os, const NEPointerList* l);


#endif


