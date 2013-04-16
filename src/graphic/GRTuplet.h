#ifndef GRTuplet_H
#define GRTuplet_H

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

// #include "Fraction.h"
// #include "defines.h" // for TYPE_TIMEPOSITION, TYPE_DURATION
#include "GRNotationElement.h"

class ARMusicalEvent;
class NEPointerList;
class GREvent;
class GRStaff;
class GRTuplet;

/** \brief not yet documented
*/
class GRTupletState
{
	friend class GRTuplet;

	public:

		GRTupletState(const GRTuplet & inTuplet );

		GRTupletState()
		{
			duration = DURATION_0;
			starttimepos = DURATION_0;
			endtimepos = DURATION_0;
			base = DURATION_0;
			relation = DURATION_0;
			elements = NULL;
		}
		
		GRTupletState & operator=(const GRTupletState & init)
		{
			duration = init.duration;
			starttimepos = init.starttimepos;
			endtimepos = init.endtimepos;
			base = init.base;
			relation = init.relation;
			return *this;
		}

		GRTupletState(const GRTupletState & init)
		{
			*this = init;
			elements = NULL;
		}

		virtual ~GRTupletState();

	protected:
		NEPointerList * elements;
		TYPE_DURATION duration;
		TYPE_TIMEPOSITION starttimepos;
		TYPE_TIMEPOSITION endtimepos;
		TYPE_DURATION base;
		Fraction relation;
};

/** \brief Graphical representation of a tuplet.
*/
class GRTuplet  : public GRNotationElement
{
	friend class GRTupletState;

	public:		
					GRTuplet( GRStaff * inStaff, const TYPE_DURATION & p_base, 
								Fraction p_relation = Fraction(3, 2));

       		 		GRTuplet( GRStaff * inStaff, const GRTupletState & tplstate);

		virtual 	~GRTuplet();

		// -- Heritage
		virtual void removeAssociation(GRNotationElement * el);
		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void print() const;
		virtual void setPosition(const NVPoint & point);
		virtual void tellPosition(GObject * caller, const NVPoint & newPosition);
	
		// -- Tuplet specific
		virtual void 	finish();

		virtual ARMusicalObject * getAbstractRepresentation() { return 0; }
		virtual ARMusicalObject * getAbstractRepresentation() const { return 0; }

		void 			removeEvent( GREvent * ev);
		void 			setTupletState( const GRTupletState & tpl);

		GREvent * 		addEvent( ARMusicalEvent * event, TYPE_TIMEPOSITION von,
									TYPE_DURATION & bis);

		TYPE_DURATION 	getDurTemplate(TYPE_DURATION d) const;

		bool 			isFull() const;
		void 			setHorizontalSpacing();

		const TYPE_DURATION & getDuration() const;
		const TYPE_TIMEPOSITION & getRelativeTimePosition() const;
		TYPE_TIMEPOSITION getRelativeEndTimePosition() const;


		const GRTupletState & getTupletState() const { return mTupletState; }

	protected:
		
		GRStaff  * staff;		// ! the class GRNotationElement has already a similar member: GRStaff * mGrStaff;
		GRTupletState mTupletState;

		 // - The content of the tuplet (notes, rests or chords):
		NEPointerList * mTupletElements;

		TYPE_DURATION calcRelation();

		// position for the char 
		NVPoint mTupletOffsets [5]; // 5 offset-Positions
			// Position= left upper edge
			// 1. offset: left lower edge  (position.x == offset[0].x)
			// 2. offset: right lower edge
			// 3. offset: right upper edge
			// 4. offset: left upper edge of textbox
			// 5. offset: right lower edge of textbox
};

#endif
