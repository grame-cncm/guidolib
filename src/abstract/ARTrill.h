#ifndef ARTrill_H
#define ARTrill_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"
#include "ARMusicalVoice.h"

class TagParameterInt;
class TagParameterFloat;


/** \brief not yet documented
*/
class ARTrill : public ARMTParameter, public ARPositionTag
{		
	public:			

			enum TYPE { TRILL,TURN,MORD };

						ARTrill(TYPE typ);
						ARTrill(int pid, const ARTrill * copy);
						
		virtual		 	~ARTrill();

		virtual void 	setTagParameterList(TagParameterList & tpl);

		virtual void 	print() const;
		
		virtual void 	PrintName(std::ostream & os) const;
		virtual void 	PrintParameters(std::ostream & os) const;

				TYPE 	getType() const 		{ return mTrillType; }
				void 	setType(TYPE typ) 		{ mTrillType = typ; }
				void	setChordType(ARMusicalVoice::CHORD_TYPE param_type) {chordType = param_type;}
				ARMusicalVoice::CHORD_TYPE	getChordType() const	{return chordType;}
				void	setChordAccidental(ARMusicalVoice::CHORD_ACCIDENTAL param_accidental) {chordAccidental = param_accidental;}
				ARMusicalVoice::CHORD_ACCIDENTAL	getChordAccidental() const	{return chordAccidental;}
				bool	getCautionary()	const				{ return fShowCautionaryAccidentals;}
				void	setCautionary(bool showAcc)			{ fShowCautionaryAccidentals = showAcc;}
				float	getadx() const;
				float	getady() const;
				void setStatus(bool b, bool c, bool e);
				void setBegin(bool b);
				bool getBegin()		{return begin;}
				void setContine(bool c);
				bool getContinue()	{return cont;}
				void setEnd(bool e);
				bool getEnd()		{return end;}

	protected:

		TagParameterInt	  * mDur;
		TYPE 				mTrillType;
		ARMusicalVoice::CHORD_TYPE			chordType;
		ARMusicalVoice::CHORD_ACCIDENTAL	chordAccidental;
		bool	fShowCautionaryAccidentals;
		float adx;
		float ady;
		bool			begin;
		bool			cont;
		bool			end;

		static ListOfTPLs ltpls;
};

#endif
