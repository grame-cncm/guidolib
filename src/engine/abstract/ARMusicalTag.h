#ifndef ARMusicalTag_H
#define ARMusicalTag_H

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

#include <string>

#include "ARMusicalObject.h"
#include "ListOfStrings.h"

class TagParameterList;
class TagParameterString;
class TagParameterFloat;
class TagParameterRGBColor;

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
		virtual		   ~ARMusicalTag();

		virtual int		MatchListOfTPLsWithTPL( const ListOfTPLs & ltpls, 
											TagParameterList & tpl, TagParameterList ** rtpl);

		virtual void	AddTagParametersList ( ListOfTPLs & ltpl, std::string& str) const;
		virtual void	CreateListOfTPLs( ListOfTPLs & ltpl, ListOfStrings & lstrs );
		virtual bool	MatchEndTag( const char * endstr );
				void	setAllowRange( int pallow );

		        void	print          (std::ostream & os) const;
		virtual void	printName      (std::ostream & os) const { os << "printName() needs to be implemented in subclasses; "; };
		virtual void	printGMNName   (std::ostream & os) const { os << "printGMNName() needs to be implemented in subclasses; "; };
		        void	printAttributes(std::ostream & os) const;
        virtual void	printParameters(std::ostream & os) const;

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
        const TagParameterRGBColor * getRGBColor() const        { return rgbColor; }

				void setColor( const char * cp );
                void setRGBColor (unsigned char red, unsigned char green, unsigned char blue);
		virtual void setDX( TagParameterFloat * tpf );
		virtual void setDY( TagParameterFloat * tpf );

				const	TagParameterFloat * getDX() const		{ return mDx; }
				const	TagParameterFloat * getDY() const		{ return mDy; }

				void	setSize(float newsize);
				const	TagParameterFloat * getSize() const		{ return size; }

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARMusicalTag() { return this; }
    /*****************************************/

    virtual void setIsInHeader(bool state) { mIsInHeader = state; }
    virtual bool isInHeader() const        { return mIsInHeader; }

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
      TagParameterRGBColor * rgbColor;
	  TagParameterFloat * mDx;
	  TagParameterFloat * mDy;
	  TagParameterFloat * size;

      bool mIsInHeader; // For proportional rendering
};

#endif


 
