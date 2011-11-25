#ifndef GRNote_H
#define GRNote_H

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

#include "GREvent.h"

class ARNote;
class ARNoteFormat;
class ARDotFormat;
class GRStaff;
class GRTie;
class GRTrill;

/** \brief Graphical representation of a note. 
*/

class GRNote : public GREvent
{
	public:
	  			 GRNote(GRStaff *, ARNote * abstractRepresentationOfNote,
						const TYPE_TIMEPOSITION & relativeTimePositionOfGRNote,
						const TYPE_DURATION & durationOfGRNote );

				 GRNote(GRStaff * inStaff, const TYPE_DURATION & inDuration );
				 GRNote(GRStaff * inStaff, ARNote * abstractRepresentationOfNote );
	virtual 	~GRNote();

	virtual void setNoteFormat( const ARNoteFormat * frmt );


		ARNote * 	getARNote();
		const ARNote * getARNote() const;

	virtual void 	OnDraw( VGDevice & hdc ) const		= 0;
	virtual void 	recalcVerticalPosition()			= 0;
	virtual void 	drawStemOnly( int flag )			= 0;
	virtual void 	setStemDirection( GDirection dir )	= 0;
	virtual float 	setStemLength( float inLen )		= 0;
	
	virtual	void	getPitchAndOctave( int * outPitch, int * outOctave, int * outAccidentals ) const;
	virtual void 	print() const;

	virtual GDirection	getDefaultThroatDirection() const;
	virtual GDirection	getThroatDirection() const;

	// Tell me: is a note splitted over several systems? 
	// should only occur for CompositeNotes.
	virtual bool isSplit(); // const;
	
protected :
		GRTrill * mOrnament;
};


#endif


