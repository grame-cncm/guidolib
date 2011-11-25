#ifndef GRARCompositeNotationElement_H
#define GRARCompositeNotationElement_H

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
				void		GetSubElementsMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const;

				void		RemoveAllSubElements();

				GuidoPos	SetTailPosition(GuidoPos pos);

			NEPointerList & GetCompositeElements() { return mCompositeElements; }	
			const NEPointerList & GetCompositeElements() const  { return mCompositeElements; }	

	private:

	  NEPointerList mCompositeElements; // the list of sub-elements, should be private

};


#endif


