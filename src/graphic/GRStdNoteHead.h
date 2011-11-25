#ifndef GRStdNoteHead_H
#define GRStdNoteHead_H

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

#include "GRNoteHead.h"

class GREvent;
class GRChord;

/** \brief Graphical represenation of a note head.
*/
class GRStdNoteHead : public GRNoteHead 
{
  public:
	  friend class GRChord;

					GRStdNoteHead(GREvent * sngnote, const TYPE_DURATION & inDur);
		virtual	 	~GRStdNoteHead();

	  virtual NVPoint getNoteHeadPosition() const;
	  virtual const NVPoint & getPosition() const;

	  // static long getNoteBreite();

		virtual const NVPoint & getReferencePosition() const;
		virtual void OnDraw( VGDevice & hdc ) const;

		virtual void GGSOutput() const;

		virtual void print() const;


	/*	enum HEADTYPES { NOHEAD = kNoneSymbol, 
			WHOLENOTEHEAD = kWholeNoteHeadSymbol, // SCR_WHOLENOTEHEAD,
			HALFNOTEHEAD = kHalfNoteHeadSymbol, // SCR_HALFNOTEHEAD,
			FULLHEAD = kFullHeadSymbol, // SCR_FULLHEAD,
			ERRORHEAD = kErrorHeadSymbol // SCR_ERRORHEAD
		};
		*/

  protected:

		virtual unsigned int durationToHeadSymbol( const TYPE_DURATION & inDur ) const;
		
		static NVPoint sRefposFull;
		static NVPoint sRefposHalf;
		static NVPoint sRefposWhole;
 		static NVPoint sRefposLonga;
		static NVPoint sRefposDiamond;
		static NVPoint sRefposCrossHead;

		NVstring mStyle;
};

#endif


