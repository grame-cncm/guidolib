#ifndef GRNoteFactory_H
#define GRNoteFactory_H

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

/** It is checked, that n-notes can be displayed correct. 
	A current n-tuplet group will be created, which can be 
	react "intelligent" to incoming note durations.
	The GRNoteFactory will be called vom the GRStaffManager 
	for each single voice and closed after the construction of all notational elements
	A current GRStaff is managed.
*/

/*
#include "defines.h" 	// for TYPE_DURATION ...


class ARChord;
class ARNote;
class ARRest;

class GRStaff;
class GRRest;
class GRTuplet;
class GRTupletState;
class GRBeam;
class GRBeamState;
class GRNote;
class GRChord;

/** \brief Class GRNoteFactory creates GRNote objects. 
	This can be depending on the length - GRSingleNote or GRCompositeNote objects.


class GRNoteFactory
{
	public:
		void rememberLastNLinePosition(const TYPE_TIMEPOSITION &tp);
		void setPossibleNLinePosition(const TYPE_TIMEPOSITION &tp);
		GRChord * createChord(ARChord *,const TYPE_TIMEPOSITION &von,
			TYPE_TIMEPOSITION &bis);

		void EndChord(GRChord *grchord);
		void StartChord(GRChord *grchord);
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
		GRChord * curchord;
		int stemOnOff;
		GRStaff *grstaff;
		GRTuplet *curtuplet;
		GRBeam *curbeam;

		int stemdir;

};

*/
#endif
