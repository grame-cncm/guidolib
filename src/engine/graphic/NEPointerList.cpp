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

#include <climits>
#include <sstream>

#include "GRSingleNote.h"
#include "NEPointerList.h"

NEPointerList::NEPointerList(int ownselements) :  NEList(ownselements)
{
}

NEPointerList::NEPointerList(const NEPointerList & lst,
							 int ownselements)
							 : NEList(lst,ownselements)
{
}

NEPointerList::~NEPointerList()
{
}

std::ostream & operator << ( std::ostream & os, const NEPointerList* l)
{
	l->print(os);
	return os;
}

void NEPointerList::print(std::ostream& os) const
{
	GuidoPos pos = GetHeadPosition();
	while (pos) {
		GRNotationElement* elt = GetNext(pos);
		os << elt << ", ";
	}
}

// a more compact textual representation
std::string NEPointerList::reduce_print() const
{
	std::stringstream sstr;
	GuidoPos pos = GetHeadPosition();
	const char * sep = "";
	while (pos) {
		const GRNotationElement* elt = GetNext(pos);
		const ARMusicalObject * ar = elt->getAbstractRepresentation();
//		const ARNote * note = ar->isARNote();
//		if (note) sstr << sep << note;
//		else
		sstr << sep << ar->getGMNName();
		sep = ", ";
	}
	return sstr.str();
}

int NEPointerList::getMaxSpacing()
{


	assert(false);
#ifdef BLABLA
	GuidoPos pos=GetHeadPosition();
	GRNotationElement* e;
	TYPE_SPACING maxSpacing=0;
	while(pos)
	{
		e=GetNext(pos);
	}
	assert(maxSpacing>0);
	return maxSpacing;
#endif
	return 0;
}

GRNotationElement* NEPointerList::getElementWithMaxSpacing()
{
	assert(false);
#ifdef BLABLA
	assert(!empty());
	GuidoPos pos=GetHeadPosition();
	GRNotationElement* e= 0;
	GRNotationElement* eMax=GetNext(pos);
	while(pos)
	{
		e=GetNext(pos);
	}
	assert(eMax);
	return eMax;
#endif
	return 0;
}

NEPointerList* NEPointerList::getElementsWithShortestDurationNotNull()
{
	assert(!empty());
	NEPointerList* list=new NEPointerList;
	GRNotationElement * e = NULL;
	GuidoPos pos=GetHeadPosition();
	TYPE_DURATION dur(INT_MAX,1);
	while (pos)
	  {
	  e = GetNext(pos);
	  if (e->getDuration() < dur
		&& e->getDuration() > DURATION_0)
		 dur = e->getDuration();
	  }
	pos = GetHeadPosition();
	while (pos)
	  {
	  e = GetNext(pos);
	  if (e->getDuration() == dur)
		 {
		 list->push_back(e);
		 }
	  }
	return list;
}

NEPointerList* NEPointerList::getElementsWithShortestDuration()
{
	assert(!empty());
	NEPointerList * list = new NEPointerList;
	GRNotationElement *e = NULL;
	GuidoPos pos=GetHeadPosition();
	TYPE_DURATION dur(INT_MAX,1);
	while (pos)
	  {
	  e = GetNext(pos);
	  if (e->getDuration() < dur)
		 dur = e->getDuration();
	  }
	pos = GetHeadPosition();
	while (pos)
	  {
	  e = GetNext(pos);
	  if (e->getDuration() == dur)
		 {
		 list->push_back(e);
		 }
	  }
	return list;
}

// ATTENTION, aend timePosition!
void NEPointerList::setSpacing(const TYPE_TIMEPOSITION & timePosition, 
							   NVPoint& position)
{
	// this is obolete!
	assert(false);
#ifdef BALBAL
	if (empty()) return;
	assert(!empty());
	GRNotationElement * criticalElement = 0;
	NEPointerList * list= this; // getElementsWithTimePosition(timePosition);
	
	// think about it.
	TYPE_TIMEPOSITION nextTimePosition (getNextTimePosition(timePosition));
	TYPE_SPACING spacing = 0;
	if(!list->empty())
	{
		NEPointerList* shortyList=list->getElementsWithShortestDuration();
		criticalElement=shortyList->getElementWithMaxSpacing();
		float criticalElementSize=criticalElement->getBoundingBox().right;
		assert(criticalElementSize>=0); // get size of real element
		if(criticalElement->getRelativeEndTimePosition()>nextTimePosition) // is there an Overlap?
		{
			criticalElement = new GRSingleNote(nextTimePosition-timePosition);
			// Dummy element for spacing calculation
			// I Believe abstractRepresentation will not be created?
			delete criticalElement;
		}
		else
		{
		}

		GuidoPos pos = GetHeadPosition();
		GRNotationElement * e;
		float min = 0;
		while (pos)
		{
			e = GetNext(pos);
			if (e->getRelativeTimePosition()==timePosition
			  && e->getBoundingBox().left<min)
				 min = e->getBoundingBox().left;
		}
		pos = GetHeadPosition();
		while(pos)
		{
			e = GetNext(pos);
			if (tmp)
			{
				tmp->setHPosition(position.x-min);
			}
			else
			{
			}
		}
		position.x += spacing+criticalElementSize;
		position.x -= min;

		delete shortyList;
	}
	else
	{
		criticalElement = new GRSingleNote(nextTimePosition-timePosition); // special Dummy-Object??
		delete criticalElement;

		GRNotationElement * e;
		GuidoPos pos=GetHeadPosition();
		while(pos)
		{
			e=GetNext(pos);
		}
		position.x += spacing;
	}
#endif
}

// spacing for all elements without duration
void NEPointerList::setZeroSpacing(const TYPE_TIMEPOSITION & timePosition, 
								   NVPoint& position,
								   int addspace)
{
	// obsolete.
	assert(false);

}

NEPointerList* NEPointerList::getElementsWithTimePosition(const TYPE_TIMEPOSITION & timePosition) const
{
	NEPointerList* list=new NEPointerList;
	GRNotationElement * e = NULL;
	GuidoPos pos=GetHeadPosition();
	while (pos)
	  {
	  e = GetNext(pos);
	  if (e->getRelativeTimePosition() == timePosition)
		  list->AddHead(e);
	  }
	return list;

#ifdef BLABLA

  //## begin NEPointerList::getElementsWithTimePosition
	NEPointerList * list = new NEPointerList;
	GRNotationElement * e;
	GuidoPos pos=GetHeadPosition();
	while(pos)
	{
		e = GetNext(pos);
		GRNotationElement * tmp = e->getGRNotationElementAtTimePosition(timePosition);
		if (tmp)
			list->push_back(tmp);
	}
	return list;
  //## end NEPointerList::getElementsWithTimePosition
#endif
	return NULL;
}

TYPE_TIMEPOSITION NEPointerList::getNextTimePosition(const TYPE_TIMEPOSITION & timePosition)
{
	assert(false);
#ifdef BLABAL
  //## begin NEPointerList::getNextTimePosition
	TYPE_TIMEPOSITION nextTimePosition(MAXLONG,1);
	TYPE_TIMEPOSITION nt(0,1);
	GRNotationElement* e;
	GuidoPos pos=GetHeadPosition();
	while(pos) // naechste TimePosition suchen
	{
		e=GetNext(pos);
		if (e->getNextTimePosition(timePosition) < nextTimePosition )
			nextTimePosition = e->getNextTimePosition(timePosition);
	}
	assert(nextTimePosition>timePosition);
	return nextTimePosition;
  //## end NEPointerList::getNextTimePosition%856729266.body
#endif
	return DURATION_0;
}

void NEPointerList::OnDraw( VGDevice & hdc ) const
{
	//GRNotationElement* e;
	GuidoPos pos = GetHeadPosition();
	while(pos) GetNext(pos)->OnDraw(hdc);
}

void NEPointerList::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	//GRNotationElement* e;
	GuidoPos pos = GetHeadPosition();
	while(pos) GetNext(pos)->GetMap(sel, f, infos);
}

void NEPointerList::setPosition(const NVPoint & newPosition)
{
	GuidoPos pos=GetHeadPosition();
	while(pos) GetNext(pos)->setPosition(newPosition);
}

void NEPointerList::tellPosition(GObject * caller,const NVPoint & newPosition)
{
	GRNotationElement * e;
	GuidoPos pos=GetHeadPosition();
	while(pos) 
	{
		e = GetNext(pos);
		e->tellPosition(caller,newPosition);
	}
}



// ATTENTION: this is EXPENSIVE! Depending on the duration
// of the piece n*n!!! O(n**2)
// Inserts a graphical object sorted by time into the datastream
GuidoPos NEPointerList::AddAtCorrectTimePosition(GRNotationElement * grne)
{
	return AddAtTimePosition(grne->getRelativeTimePosition(),grne);
}

// Inserts a grafical object sorted by time into the datastream
GuidoPos NEPointerList::AddAtTimePosition(const TYPE_TIMEPOSITION & tp,
		GRNotationElement * grne)
{
	GuidoPos pos = GetHeadPosition();
	GRNotationElement * tmp;
	// this loop can be improved!
	
	// the element will be inserted at a position so that
	// time(position) < time(element) 
	while (pos)
	{
		tmp = GetAt(pos);
		if (tmp->getRelativeTimePosition() > tp)
		{
			return AddElementAt(pos, grne);
		}
		GetNext(pos);
	}
	return push_back(grne);
}



void NEPointerList::setHPosition( float nx )
{
	GuidoPos pos = GetHeadPosition();
	while(pos) GetNext(pos)->setHPosition(nx);
}
