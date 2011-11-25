/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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

#include "GRARCompositeNotationElement.h"

#include "NEPointerList.h"

// ----------------------------------------------------------------------------
GRARCompositeNotationElement::GRARCompositeNotationElement( ARMusicalObject * ar, bool ownsAR )
		: GRARNotationElement( ar ,ownsAR ),
		mCompositeElements( 1 ) // owns elements. Was: new NEPointerList(1))
{
}

// ----------------------------------------------------------------------------
GRARCompositeNotationElement::~GRARCompositeNotationElement()
{
// not needed:	mCompositeElements.RemoveAll();
	// delete mCompositeElements;
	// mCompositeElements = 0;
}

// ----------------------------------------------------------------------------
void 
GRARCompositeNotationElement::setGRStaff( GRStaff * inStaff )
{
	GRNotationElement::setGRStaff( inStaff );

	GuidoPos pos = First();
	while (pos)
		GetNext(pos)->setGRStaff( inStaff );

}

// ----------------------------------------------------------------------------
void 
GRARCompositeNotationElement::setPosition( const NVPoint & inPos )
{
	// - Call inherited
	GRARNotationElement::setPosition( inPos );

	// - Propagate
	GuidoPos pos = mCompositeElements.GetHeadPosition();
	while (pos)
		GetNext(pos)->setPosition( inPos );

	updateBoundingBox();
}

// ----------------------------------------------------------------------------
void 
GRARCompositeNotationElement::setHPosition( float inX )
{
	// - Propagate
	// First we set the position of the sub elements ...
	GuidoPos pos = mCompositeElements.GetHeadPosition();
	while (pos)
		GetNext(pos)->setHPosition( inX );

	// - Call inherited
	GRARNotationElement::setHPosition( inX );

	// first we set the position for the elements ...
	

	updateBoundingBox();
}

// ----------------------------------------------------------------------------
void 
GRARCompositeNotationElement::updateBoundingBox()
{
	mLeftSpace = 0;	// should probably not be done here
	mRightSpace = 0;

	mBoundingBox.Set( 0, 0, 0, 0 );

	// - Check note sub parts
	GuidoPos pos = First();
	while (pos)
	{
		// - TODO: take the position in account ! following code wrongly assumes
		// that the elements are exactly at the same position.
		GRNotationElement * e = GetNext(pos);
		if (e->getRightSpace() > mRightSpace)	mRightSpace = e->getRightSpace();
		if (e->getLeftSpace() > mLeftSpace)		mLeftSpace = e->getLeftSpace();

		// 
		
		addToBoundingBox( e );
		
		/* was ->
		const NVRect & eltBox  = e->getBoundingBox();
		if( eltBox.top < mBoundingBox.top)			mBoundingBox.top = eltBox.top;
		if( eltBox.bottom > mBoundingBox.bottom)	mBoundingBox.bottom = eltBox.bottom;
		*/
		//
	}
}


// ----------------------------------------------------------------------------
// was GetHeadPosition
GuidoPos 
GRARCompositeNotationElement::First() const
{
	return mCompositeElements.GetHeadPosition();

}

// ----------------------------------------------------------------------------
// was GetTailPosition
GuidoPos 
GRARCompositeNotationElement::Last() const
{
	return mCompositeElements.GetTailPosition();
}

// ----------------------------------------------------------------------------
GRNotationElement * 
GRARCompositeNotationElement::GetNext( GuidoPos & ioPos ) const
{
	return mCompositeElements.GetNext( ioPos );
}


// ----------------------------------------------------------------------------
void 
GRARCompositeNotationElement::addToOffset( const NVPoint & point )
{
	// first we set the position for the elements ...
	GuidoPos pos = mCompositeElements.GetHeadPosition();
	while (pos)
		GetNext(pos)->addToOffset( point );

	GRARNotationElement::addToOffset(point);
}

// ----------------------------------------------------------------------------
GRNotationElement * GRARCompositeNotationElement::GetTail() const
{ 
	return mCompositeElements.GetTail();
}

// ----------------------------------------------------------------------------
GuidoPos 
GRARCompositeNotationElement::AddTail( GRNotationElement * el )
{
	return mCompositeElements.AddTail(el);
}

// ----------------------------------------------------------------------------
GuidoPos GRARCompositeNotationElement::SetTailPosition( GuidoPos pos )
{
	return mCompositeElements.SetTailPosition( pos );
}

// ----------------------------------------------------------------------------
void GRARCompositeNotationElement::RemoveElement( GRNotationElement * el )
{
	mCompositeElements.RemoveElement( el );
}

// ----------------------------------------------------------------------------
void GRARCompositeNotationElement::DrawSubElements( VGDevice & hdc ) const
{
	mCompositeElements.OnDraw( hdc );
}

// ----------------------------------------------------------------------------
void GRARCompositeNotationElement::GetSubElementsMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	mCompositeElements.GetMap( sel, f, infos );
}

// ----------------------------------------------------------------------------
void 
GRARCompositeNotationElement::RemoveAllSubElements()
{
	mCompositeElements.RemoveAll();
}
