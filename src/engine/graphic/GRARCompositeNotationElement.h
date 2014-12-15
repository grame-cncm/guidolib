#ifndef GRARCompositeNotationElement_H
#define GRARCompositeNotationElement_H

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

#include "GUIDOTypes.h"
#include "GRARNotationElement.h"

#include "NEPointerList.h"

// class GRCompositeNotationElement;
// class NEPointerList;

/** \brief not yet documented
*/
class GRARCompositeNotationElement : public GRARNotationElement
{
	public:
						GRARCompositeNotationElement( ARMusicalObject * ar,
														bool ownsAR = false );

		virtual			~GRARCompositeNotationElement();


		virtual void	setGRStaff( GRStaff * inStaff );

		virtual void	setPosition( const NVPoint & inPos );
		virtual void	setHPosition( float inX );
		virtual void	addToOffset( const NVPoint & inOfset ); 
		virtual void	updateBoundingBox();

		// - Sub elements 

				GuidoPos First() const; // was GetHeadPosition() 
				GuidoPos Last() const; // was GetTailPosition
				
				GRNotationElement * GetNext( GuidoPos & ioPos ) const;

				GRNotationElement * GetTail() const;

				GuidoPos	AddTail( GRNotationElement * el );
				void		RemoveElement(GRNotationElement * el );
				
				void		DrawSubElements( VGDevice & hdc ) const;
				void		GetSubElementsMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const;

				void		RemoveAllSubElements();

				GuidoPos	SetTailPosition(GuidoPos pos);

			NEPointerList & GetCompositeElements() { return mCompositeElements; }	
			const NEPointerList & GetCompositeElements() const  { return mCompositeElements; }	

	private:

	  NEPointerList mCompositeElements; // the list of sub-elements, should be private

};


#endif


