#ifndef GObject_H
#define GObject_H

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

#include "NVPoint.h"
#include "NVRect.h"
#include "MusicalSymbols.h"	// for kMaxMusical...
#include "GUIDOScoreMap.h"
#include "defines.h"

class NVstring;
class VGDevice;
class VGColor;
class VGDevive;

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
		virtual ~GObject();

		virtual void addToOffset(const NVPoint &)		{ }

		virtual const NVPoint & getOffset() const		{ return sRefposNone; }
		virtual const NVstring * getFont() const		{ return 0; }
		virtual const NVstring * getFontAttrib() const	{ return 0; }
		virtual const unsigned char * getColRef() const { return 0; }

		virtual int		getFontSize() const;
		virtual float	getSize() const { return 1.0f; }

		/** \brief Retrieves the Symbol-reference-position with respect to the GUIDO-reference-position.
		
			It is used for "generic" drawing of graphical symbols from a notation-font.
			Each symbol has to set the reference position according to font-symbol etc...
			the refpos is always specified using the "regular" fontsize; when the size
			parameter is different, the actual offset must be calculated during drawing.
		*/
		virtual const NVPoint & getReferencePosition() const { return sRefposNone; }
		
		virtual void setPosition( const NVPoint & newPosition );	
		virtual void setHPosition( float nx );	
		virtual void tellPosition( GObject * caller, const NVPoint & newPosition ); 
		
				const NVPoint & getPosition() const { return mPosition; }	
		
		// it could be, that we do not need this ...
		// we should have a bounding-polygon instead 
		// ((JB) or many rectangles)
		const NVRect & getBoundingBox() const	{ return mBoundingBox; }
				void	addToBoundingBox( const NVRect & in );

		// this is not constant! Can be changed!
		NVRect & getReferenceBoundingBox() { return mBoundingBox; }	
		
		virtual	bool	isGREventClass() const { return false; }

		static int		InstanceCount() { return sInstanceCount; }

		long getID() const 	{ return mGId; }		// Could it be void* instead of long ?
		void setID(long id) { mGId = id; }

		virtual void	OnDraw( VGDevice & hdc ) const = 0;
		virtual void	GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos) const {};
		virtual	void	DrawBoundingBox( VGDevice & hdc, const VGColor & inBrushColor ) const; // debug
		virtual char *	getGGSInfo(int) const    { return 0; }
		virtual void	GGSOutput() const;
		
		static bool		positionIsOnStaffLine( float inPositionY, float inLSpace );

		static float	GetSymbolExtent( unsigned int inSymbol );	// Rather put it in GRSpecial ?

	protected:
		
		NVPoint			mPosition;
		NVRect			mBoundingBox;
		NVRect			mMapping;

		long			mGId;

		// this is an abstract base class. No direct initialization.
		GObject();	
		GObject( const GObject & in );	

		static void		NotifyNewInstance();

		static float sSymbolExtentMap [ kMaxMusicalSymbolID ];
		static int	sInstanceCount;		// debug
		static NVPoint sRefposNone;
};

#endif
