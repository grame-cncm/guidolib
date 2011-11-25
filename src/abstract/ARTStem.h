#ifndef ARTStem_H
#define ARTStem_H

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

class TagParameterFloat;

/** \brief not yet documented
*/

class ARTStem : public ARMTParameter, public ARPositionTag
{
	public:

		enum STEMSTATE { UP ,DOWN ,AUTO, OFF };

		virtual bool IsStateTag() const { return true; }

		ARTStem( int st = ARTStem::AUTO,ARTStem * p_savestem = NULL,
												ARTStem * p_copystem = NULL);

		virtual ~ARTStem();

		ARTStem(const ARTStem * tstem);

		virtual ARMusicalObject * Copy() const;


		virtual void print() const		{ }
		virtual void PrintName(std::ostream & os) const;
		virtual void PrintParameters(std::ostream &os) const;

		virtual void setTagParameterList(TagParameterList & tpl);

		const TagParameterFloat * getLength() const
		{
			return mTpfLength;
		}

		virtual const STEMSTATE getStemState() const { return mStemState; }

		virtual ARMusicalObject * getEndTag() const
		{
			return new ARTStem(AUTO, NULL, mSaveStem);
		}

	protected:

		ARTStem * mSaveStem;

		STEMSTATE mStemState;

		static ListOfTPLs ltpls;

		TagParameterFloat * mTpfLength;

};

#endif
