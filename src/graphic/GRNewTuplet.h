#ifndef GRNewTuplet_H
#define GRNewTuplet_H

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

#include "NVPoint.h"
#include "GRPTagARNotationElement.h"

class ARTuplet;

class GRStaff;
class GRSystem;


/** \brief Graphical representation of a tuplet.
*/

class GRNewTuplet : public GRPTagARNotationElement
{

	public:
		// enum bracestate { BRACENONE = 0, BRACELEFT = 1, BRACERIGHT = 2 };

		class GRNewTupletSaveStruct : public GRPositionTag::GRSaveStruct
		{
			public:
						GRNewTupletSaveStruct()  {}
				virtual ~GRNewTupletSaveStruct() {}
				
				NVPoint p1;
				NVPoint p2;
				NVPoint textpos;
		};

						GRNewTuplet( GRStaff * inStaff, ARTuplet * artuplet );
		virtual 		~GRNewTuplet();

		virtual void 	OnDraw( VGDevice & hdc ) const;
		virtual void 	print() const;
		virtual void 	tellPosition( GObject * caller, const NVPoint & inPos );

	protected:

		const ARTuplet * getARTuplet() const;
		virtual GRPositionTag::GRSaveStruct * getNewGRSaveStruct() 
			{
				return new GRNewTupletSaveStruct; 
			}

			GDirection		mDirection;

	private:

			void	automaticPosition( GObject * caller, const NVPoint & inPos );
			void	manualPosition( GObject * caller, const NVPoint & inPos );
			
	//	bracestate mBraceState;
			bool	mShowLeftBrace;
			bool	mShowRightBrace;


};

#endif
