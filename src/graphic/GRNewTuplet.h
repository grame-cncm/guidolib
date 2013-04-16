#ifndef GRNewTuplet_H
#define GRNewTuplet_H

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
