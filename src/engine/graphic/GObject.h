#ifndef GObject_H
#define GObject_H

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

#include <map>

#include "NVPoint.h"
#include "NVRect.h"
#include "MusicalSymbols.h"
#include "GUIDOScoreMap.h"
#include "GuidoDefs.h"
#include "defines.h"

class NVstring;
class VGDevice;
class VGColor;

enum GDirection 	// was STEMDIR
{
	dirOFF = 0,
	dirUP = 1,
	dirDOWN = -1,
	dirAUTO = 10
};


/** \brief A class to propagate device related information.
*/
class MapInfos 
{
	public:
		NVPoint fPos;
		NVPoint fScale;
		 
				 MapInfos() : fPos(0,0), fScale(1,1) {}
		virtual ~MapInfos() {}
};

/** \brief The base class for all graphic objects.
*/
class GObject 
{
	public:
		virtual ~GObject() {}

		virtual void addToOffset(const NVPoint &)		{ }

		virtual const NVPoint & getOffset() const		{ return sRefposNone; }
		virtual const char * 	getFont() const			{ return 0; }
		virtual const char * 	getFontAttrib() const	{ return 0; }
		virtual const unsigned char * getColRef() const { return 0; }

		virtual int		getFontSize() const				{ return (int)(1.5f * LSPACE); } // default
		virtual float	getSize() const 				{ return 1.0f; }

		/** \brief Retrieves the Symbol-reference-position with respect to the GUIDO-reference-position.
		
			It is used for "generic" drawing of graphical symbols from a notation-font.
			Each symbol has to set the reference position according to font-symbol etc...
			the refpos is always specified using the "regular" fontsize; when the size
			parameter is different, the actual offset must be calculated during drawing.
		*/
		virtual const NVPoint & getReferencePosition() const { return sRefposNone; }
		
		virtual void setPosition( const NVPoint & pos )					{ mPosition = pos; }
		virtual void setHPosition( float nx )							{ mPosition.x = nx; }
				const NVPoint & getPosition() const 					{ return mPosition; }
		virtual void tellPosition( GObject * caller, const NVPoint & newPosition );

		// it could be, that we do not need this ...
		// we should have a bounding-polygon instead 
		// ((JB) or many rectangles)
		const NVRect & getBoundingBox() const	{ return mBoundingBox; }
				void	addToBoundingBox( const NVRect & in );

		// this is not constant! Can be changed!
		NVRect & getReferenceBoundingBox() { return mBoundingBox; }
		const NVRect&   getMapping() const { return mMapping; }
		
		virtual	bool	isGREventClass() const { return false; }

		virtual void	OnDraw( VGDevice & hdc ) const = 0;
		virtual void	GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos) const {};
		virtual	void	DrawBoundingBox( VGDevice & hdc, const VGColor & inBrushColor ) const; // debug
		
		static bool		positionIsOnStaffLine( float inPositionY, float inLSpace );
		static float    GetSymbolExtent( unsigned int inSymbol );

	protected:
		NVPoint			mPosition;
		NVRect			mBoundingBox;
		NVRect			mMapping;

		// this is an abstract base class. No direct initialization.
		GObject() {}
		GObject( const GObject & in );	

		static std::map<ConstMusicalSymbolID, float> sSymbolExtentMap;
		static NVPoint sRefposNone;
};

#endif
