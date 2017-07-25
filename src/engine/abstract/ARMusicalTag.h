#ifndef ARMusicalTag_H
#define ARMusicalTag_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "ARMusicalObject.h"
#include "TagParameterMap.h"
#include "ARVisitable.h"
#include "ARVisitor.h"
#include "TagParameterStrings.h"
#include "TagParameterRGBColor.h"

class TagParameterString;
class TagParameterFloat;

/** \brief The base class for all musical tag classes.

	It stores common tag data and offers common tag functions.
*/
class ARMusicalTag : public ARMusicalObject
{
	static	TagParameterMap fCommonParams;

	public:
		// the tags order at the beginning of a voice
		enum ORDER { kDontMove=-1, kStartOrder, kStaffOrder, kInstrumentOrder, kClefOrder, kKeyOrder, kMeterOrder, kTitleOrder, kComposerOrder, kSystemFormatOrder, kAccoladeOrder,
					kDefaultOrder };

		enum ASSOCIATION { LA, RA, DC, EL, ER }; // Left, Right, Don't Care, Error Left, Error Right
		enum RANGE { NO, ONLY, RANGEDC };
	
						ARMusicalTag ( const TYPE_TIMEPOSITION & tp, const ARMusicalTag * copy = 0 );
						ARMusicalTag ( int pid = -1, const ARMusicalTag * copy = 0 );		
		virtual		   ~ARMusicalTag() {}


        virtual void accept(ARVisitor& visitor) { visitor.visitIn(this); visitor.visitOut(this); }

		// give the tag precedence, used to sort the elements at the beginning of a voice
		// and notably to get a correct clef, key meter order
		virtual int		getOrder() const				{ return kDefaultOrder; }
		virtual bool	MatchEndTag( const char * endstr );
				void	setAllowRange( int pallow );

		virtual const char*	getTagName () const		{ return "ARMusicalTag"; };
		virtual std::string getGMNName () const		{ return "\\musicalTag"; };
		virtual const char*	getParamsStr() const	{ return kCommonParams; };

				void	setIsAuto( bool isauto )	{ isAuto = isauto; }
				bool	getIsAuto() const			{ return isAuto; }
		        void	print          (std::ostream & os) const;

		// the association of a Tag
		// Left, Right, Don't Care, Error Left, Error Right
		// Error Left: It is associated to the left, but that is an error.
		// Error Right: It is associated to the right, but thati s an error
				void	setAssociation(ASSOCIATION p_assoc)		{ assoc = p_assoc; }
				ASSOCIATION getAssociation() const				{ return assoc; }

		virtual RANGE	getRangeSetting() const { return rangesetting; }
		virtual bool	IsStateTag() const		{ return false; }
		virtual void	setError(int i = 1)		{ error = i; }
		virtual int		getError() const		{ return error; }
				int		getID() const			{ return id; }
				void	setID(int pid)			{ id = pid; }

		virtual void	setRange(bool p)		{ mHasRange = p; }
		virtual bool	getRange() const		{ return mHasRange; }

				void	setColor( const char * cp );
                void	setRGBColor (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha=255);
				void	setSize(float newsize);
		virtual const	TagParameterFloat * getSize() const;

				const	TagParameterString *	getColor() const;
				const	TagParameterRGBColor*	getRGBColor() const;
				const	TagParameterFloat *		getDX() const;
				const	TagParameterFloat *		getDY() const;

    const ARMusicalObject *isARMusicalTag() const 			{ return this; }
 
	// -------------------------------------------------------------------
	// tag parameters management
	// -------------------------------------------------------------------
	// query a tag parameter and possibly gives the default one
	template<typename T> const T*	getParameter (const char* param, bool usedefault=false) const {
		const T* p = fParams.get<T> (param);
		if (usedefault && !p)
			p = fParamsTemplate.get<T>(param);
		return p;
	}
	// query a tag parameter that can have several names
	template<typename T> const T* getParameter (const char* name1, const char* name2) const {
		const T* p = fParams.get<T>(name1);
		return p ? p : fParams.get<T>(name2);
	}
	template<typename T> const T*	getDefaultParameter (const char* param) const { return fParamsTemplate.get<T>(param); }

	virtual void  setTagParameters				(TagParametersList& params);
	virtual void  clearTagDefaultParameter		(const char* param);
	// setTagParameters is intended to let subclasses process their specific parameters
	virtual void  setTagParameters				(const TagParameterMap& params)		{}
	virtual const TagParameterMap& getTagParameters() const				{ return fParams; }
	virtual const TagParameterMap& getSupportedTagParameters() const	{ return fParamsTemplate; }
	virtual void  addTagParameter (STagParameterPtr param)				{ fParams.Add (param); }
	virtual void  copyParameters(const TagParameterMap& map)			{ fParams = map; }

	// sets the template and default tag parameters
	virtual void  setupTagParameters(const TagParameterMap& map)		{ fParamsTemplate.Add(map); }

    virtual void	setIsInHeader(bool state) { mIsInHeader = state; }
    virtual bool	isInHeader() const        { return mIsInHeader; }

	protected: //  id(-1), isAuto(0), rangesetting(NO), error(0), hasRange(0)
		virtual TagParameterMap checkTagParameters	(TagParametersList& params, const std::string pTemplate);
		virtual void			checkUnitParameters	(TagParameterMap& map);

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

	private:
		void init(const ARMusicalTag * copy);
		TagParameterRGBColor fRgbColor;

		bool mIsInHeader;					// For proportional rendering
		TagParameterMap	fParams;			// the current tag parameters
		TagParameterMap	fParamsTemplate;	// the supported tag parameters
};

std::ostream & operator << ( std::ostream & os, const ARMusicalTag* tag );

#endif


 
