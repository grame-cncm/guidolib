#ifndef ARMusicalTag_H
#define ARMusicalTag_H

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

#include <string>

#include "ARMusicalObject.h"
#include "ListOfStrings.h"

class TagParameterList;
class TagParameterString;
class TagParameterFloat;

class ListOfTPLs;

/** \brief The base class for all musical tag classes.

	It stores common tag data and offers common tag functions.
*/
class ARMusicalTag : public ARMusicalObject
{
	public:

		enum ASSOCIATION { LA, RA, DC, EL, ER }; // Left, Right, Don't Care, Error Left, Error Right
		enum RANGE { NO, ONLY, RANGEDC };
	
						ARMusicalTag ( const TYPE_TIMEPOSITION & tp, const ARMusicalTag * copy = 0 );
						ARMusicalTag ( int pid = -1, const ARMusicalTag * copy = 0 );		
		virtual			~ARMusicalTag();

		virtual int		MatchListOfTPLsWithTPL( const ListOfTPLs & ltpls, 
											TagParameterList & tpl, TagParameterList ** rtpl);

		virtual void	AddTagParametersList ( ListOfTPLs & ltpl, std::string& str) const;
		virtual void	CreateListOfTPLs( ListOfTPLs & ltpl, ListOfStrings & lstrs );
		virtual bool	MatchEndTag( const char * endstr );
				void	setAllowRange( int pallow );
		virtual void    print() const = 0;
		virtual void	PrintParameters( std::ostream & os ) const;
		virtual void	PrintName( std::ostream & os ) const;
				void	setIsAuto( bool isauto )	{ isAuto = isauto; }
				bool	getIsAuto() const			{ return isAuto; }

		// the association of a Tag
		// Left, Right, Don't Care, Error Left, Error Right
		// Error Left: It is associated to the left, but that is an error.
		// Error Right: It is associated to the right, but thati s an error

		virtual RANGE	getRangeSetting() const { return rangesetting; }
		virtual bool	IsStateTag() const		{ return false; }
		virtual void	setError(int i = 1)		{ error = i; }
		virtual int		getError() const		{ return error; }
				int		getID() const			{ return id; }
				void	setID(int pid)			{ id = pid; }

		// this can be used, to see, wether the tag has a range
		virtual void setRange(bool p)
		{
			if (p && (IsStateTag() || rangesetting == NO ))
			{
				// WARNING ..., State tags do not get
				// ranges ... are ignored!
			}
			// They have to be set anyway, so the the parser can deal with closing
			// Ranges correctly!
			mHasRange = p;
		} 

		virtual bool	getRange() const						{ return mHasRange; }
				void	setAssociation(ASSOCIATION p_assoc)		{ assoc = p_assoc; }
				ASSOCIATION getAssociation()					{ return assoc; }

		virtual std::ostream & operator << ( std::ostream & os ) const;

		const TagParameterString * getColor() const				{ return color; }

				void setColor( const char * cp );
		virtual void setDX( TagParameterFloat * tpf );
		virtual void setDY( TagParameterFloat * tpf );

				const	TagParameterFloat * getDX() const		{ return mDx; }
				const	TagParameterFloat * getDY() const		{ return mDy; }

				void	setSize(float newsize);
				const	TagParameterFloat * getSize() const		{ return size; }


  protected: //  id(-1), isAuto(0), rangesetting(NO), error(0), hasRange(0)

	  int id;
	  bool isAuto;
	  ASSOCIATION assoc;

	  // this is the RANGE setting -> NO Range, ONLY Range or DC Range meaning:
	  // No Range: The Tag must not have a range
	  // ONLY Range: The Tag must have a range
	  // DC Range: The Tag can either have or not have a range
	  RANGE rangesetting;
	  
	  int error;
	  // is 1, if the Tag has a valid range 0 otherwise.
	  bool mHasRange;

	  // these are the Tagparameters that can be optionally supplied-
	  TagParameterString * color;
	  TagParameterFloat * mDx;
	  TagParameterFloat * mDy;
	  TagParameterFloat * size;
};

#endif


 
