#ifndef ARTuplet_H
#define ARTuplet_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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

// class GRNewTuplet;
class ARBase;

/** \brief Abstract representation of a tuplet.
*/
class ARTuplet : 
	// public ARMusicalObject,
	public ARMTParameter,
	public ARPositionTag
{
		// friend class GRNewTuplet;

	public:

							ARTuplet(); 
		virtual 			~ARTuplet()		{ }

				void 		setName(const char * inName);
		const NVstring & 	getName() const 	{ return mTupletFormat; }

		virtual void 		setTagParameterList(TagParameterList & tl);
		virtual void 		print() const	{ }
		virtual std::ostream & 	operator << (std::ostream & os) const;

				void		setupTuplet( ARBase * inBase );
				void		parseTupletFormatString();

				int			getNumerator() const { return mBaseNumerator; }
				int			getDenominator() const { return mBaseDenominator; }
				float		getDy1() const { return mDy1; }
				float		getDy2() const { return mDy2; }		
				bool		getLeftBrace() const { return mLeftBrace; }
				bool		getRightBrace() const { return mRightBrace; }

				bool		isDySet() const { return (mDy1Set || mDy2Set); }
				bool		isFormatSet() const { return mFormatSet; } 
	protected:

		NVstring mTupletFormat;	// format string <"-x:y-">

		float	mDy1;
		float	mDy2;
		int		mBaseNumerator;
		int		mBaseDenominator;
		bool	mLeftBrace;
		bool	mRightBrace;
	
		bool	mDy1Set;
		bool	mDy2Set;
		bool	mFormatSet;


		static ListOfTPLs ltpls;
		// a string ...
};


#endif
