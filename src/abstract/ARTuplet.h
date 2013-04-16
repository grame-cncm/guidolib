#ifndef ARTuplet_H
#define ARTuplet_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
