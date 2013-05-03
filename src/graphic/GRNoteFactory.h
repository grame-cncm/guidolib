#ifndef GRNoteFactory_H
#define GRNoteFactory_H

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

/** It is checked, that n-notes can be displayed correct. 
	A current n-tuplet group will be created, which can be 
	react "intelligent" to incoming note durations.
	The GRNoteFactory will be called vom the GRStaffManager 
	for each single voice and closed after the construction of all notational elements
	A current GRStaff is managed.
*/

/*
#include "defines.h" 	// for TYPE_DURATION ...


class ARNote;
class ARRest;

class GRStaff;
class GRRest;
class GRTuplet;
class GRTupletState;
class GRBeam;
class GRBeamState;
class GRNote;

/** \brief Class GRNoteFactory creates GRNote objects. 
	This can be depending on the length - GRSingleNote or GRCompositeNote objects.


class GRNoteFactory
{
	public:
		void rememberLastNLinePosition(const TYPE_TIMEPOSITION &tp);
		void setPossibleNLinePosition(const TYPE_TIMEPOSITION &tp);

		virtual void setStemOnOff(int p);
		GRNoteFactory(GRStaff *p_grstaff = NULL);
		virtual ~GRNoteFactory();

		GRNote * createNote(ARNote * arnote, const TYPE_TIMEPOSITION & von,
				const TYPE_DURATION &bis);
		
		GRRest * createRest(ARRest * arrest, const TYPE_TIMEPOSITION & von,
				const TYPE_DURATION &bis);

		int finish(GRTupletState &); // stops the note-creation, fills
			// if available the current TupletState class
			// if no current tuplet situation  available it returns 0
			// othwerwise 1.

		void setTupletState(GRTupletState &);

		virtual void setGRStaff(GRStaff *p_grstaff)
			{
				grstaff = p_grstaff;
			}

		virtual void setStemDirection(int dir)
			{
         	stemdir = dir;
			}

	protected:
		GRTupletState * nlineTupletState;
		int stemOnOff;
		GRStaff *grstaff;
		GRTuplet *curtuplet;
		GRBeam *curbeam;

		int stemdir;

};

*/
#endif
