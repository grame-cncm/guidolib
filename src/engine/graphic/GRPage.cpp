/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
using namespace std;

// - Guido Misc
#include "secureio.h"
#include "GUIDOInternal.h"
#include "VGDevice.h"

// - Guido AR
#include "ARMusic.h"
#include "ARPageFormat.h"
#include "ARMusicalVoice.h"

// - Guido GR
#include "GRPage.h"
#include "GRMusic.h"
#include "GRStaffManager.h"
#include "GRSystem.h"

// - Guido debug
#include "GObject.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRStaff.h"
#include "kf_ivect.h"

NVRect gClipRect;

GRSystem * gCurSystem;

// ----------------------------------------------------------------------------
/** \brief Used by the GRStaffManager to create Pages, to which
	Systems are added.
*/
GRPage::GRPage( GRMusic * grmusic, GRStaffManager * grstafmgr, 
					const TYPE_TIMEPOSITION & inDate, GRPage * prevpage )
		   :  GREvent( NULL, grmusic->getARMusic(), inDate, DURATION_0 ),
		 	  	m_totalsystemheight( 0 )
{
	// m_unit = UNIT_CM;

	if (prevpage == 0)
	{
		mWidth =		DF_SX * kCmToVirtual;
		mHeight =		DF_SY * kCmToVirtual;
		mLeftMargin =	DF_ML * kCmToVirtual;
		mTopMargin =	DF_MT * kCmToVirtual;
		mRightMargin =	DF_MR * kCmToVirtual;
		mBottomMargin =	DF_MB * kCmToVirtual;
	}
	else
	{
		mWidth = prevpage->mWidth;
		mHeight = prevpage->mHeight;
		mLeftMargin = prevpage->mLeftMargin;
		mTopMargin = prevpage->mTopMargin;
		mRightMargin = prevpage->mRightMargin;
		mBottomMargin = prevpage->mBottomMargin;
	}

	// TYPE_TIMEPOSITION starttime= von;
	m_staffmgr = grstafmgr;
	mCurMusic = grmusic;
}

// ----------------------------------------------------------------------------
GRPage::~GRPage()
{
	DeleteContent( &mSystems );
}

// ----------------------------------------------------------------------------
/** \brief Returns true if the system was added to the page false otherwise.
*/
bool GRPage::addSystem( GRSystem * inSystem, float * ioUsedSystemDistance )
{
	assert(inSystem->getGRPage() == this);
	GRSystem * lastSystem = mSystems.empty() ? 0 : mSystems.back(); // get the current last system
	const NVRect & newSystemBox = inSystem->getBoundingBox();
	
	NVPoint newPos;
	if( lastSystem ) {
		if (*ioUsedSystemDistance > 0) {
			newPos.y = lastSystem->getPosition().y + *ioUsedSystemDistance;
			*ioUsedSystemDistance = -1;
		}
		else {
			newPos.y = lastSystem->getPosition().y + lastSystem->getBoundingBox().bottom;
			// this should be handled by "springs" as well... and there should be a "minimum" distance...
			newPos.y -= newSystemBox.top;
			// the default distance ...
			// it is later distributed evenly between mSystems ...
			newPos.y += GRStaffManager::sDefaultSystemDistance;
		}
		m_totalsystemheight += newSystemBox.Height();
	}
	else // So this is the first system of the page
	{	
		newPos.y = - newSystemBox.top;
		m_totalsystemheight = newSystemBox.Height(); // TODO: bottom - newPos.y;
	}
	inSystem->setPosition( newPos );

	if( mSystems.empty())	mDebugPageDate = inSystem->mDebugSystemDate;
	mSystems.push_back( inSystem );
	updateBoundingBox();
	return true;
}

// ----------------------------------------------------------------------------
/** \brief Converts a Rectangle from device coordinades to logical (virtual) coordinates.
*/
bool GRPage::DPtoLPRect( VGDevice & hdc, float left, float top, 
						float right, float bottom, NVRect * outRect ) const
{
	hdc.DeviceToLogical( &left, &top );
	hdc.DeviceToLogical( &right, &bottom );

	outRect->left = left;
	outRect->top = top;
	outRect->right = right;
	outRect->bottom = bottom;

	return true;
}

// ----------------------------------------------------------------------------
void GRPage::GGSOutput() const
{
	char buffer[100];
	snprintf(buffer,100,"\\open_page<%d,%d>\n",(int)getPageWidth(),(int)getPageHeight());
	AddGGSOutput(buffer);

	GuidoPos pagepos = First();
	while (pagepos)
	{
		GetNext(pagepos)->GGSOutput();
	}

	const float tstx = -mLeftMargin;
	const float tsty = -mTopMargin;

	ggsoffsetx = (long)(-tstx);
	ggsoffsety = (long)(-tsty);

	ggsoffsetx = 0;
	ggsoffsety = 0;

	snprintf(buffer,100,"\\close_page\n");
	AddGGSOutput(buffer);
}

// ----------------------------------------------------------------------------
/** \brief Draws the score page.

	The GuidoEngine does not draw nor erase the background.
	Client applications must display it by themself. Usually, printing
	does not require to draw a background.

*/
void GRPage::OnDraw( VGDevice & hdc, const GuidoOnDrawDesc & inDrawInfos ) const
{

	setScaling( hdc, (float)inDrawInfos.sizex, (float)inDrawInfos.sizey );

	// if croll coords are in virtual units:
	hdc.SetOrigin( - (float)inDrawInfos.scrollx, - (float)inDrawInfos.scrolly ); // (JB) sign change


	// if scroll coords are in device units:
	//	hdc.SetOrigin( - (float)inDrawInfos.scrollx / hdc.GetXScale(), 
	//		- (float)inDrawInfos.scrolly / hdc.GetYScale());
	
	if ( /*fullredraw ||*/ inDrawInfos.updateRegion.erase )// (JB)   || c->ps.hdc == NULL 
	{
		gClipRect.Set( 0, 0, getPageWidth(), getPageHeight());
	}
	else
	{			
		/*	was: 
		// This is the update region
		const GCoord left = c->updateRegion.left;
 		const GCoord top = c->updateRegion.top;
		const GCoord right = c->updateRegion.right;
		const GCoord bottom = c->updateRegion.bottom;

		DPtoLPRect( hdc, left, top, right, bottom, &gClipRect ); // (JB) still ok ? */
	
		gClipRect.Set( float(inDrawInfos.updateRegion.left), float(inDrawInfos.updateRegion.top), 
						float(inDrawInfos.updateRegion.right), float(inDrawInfos.updateRegion.bottom) );
	}


#if (0)
	// Draw margins.
	const float x1 = getMarginLeft();
	const float x2 = getPageWidth() - getMarginRight();
	const float x3 = getPageWidth();
	const float y1 = getMarginTop();
	const float y2 = getPageHeight() - getMarginBottom();
	const float y3 = getPageHeight();

	hdc.PushPen( VGColor( 150, 150, 255 ), 5 );			// opaque
	hdc.PushFillColor( VGColor( 0, 0, 0, ALPHA_OPAQUE ));	
	
// DrawBoundingBox( hdc, GColor( 200, 255, 200 ));

	hdc.Rectangle( 0, 0, x3, y3 );
	hdc.Rectangle( x1, y1, x2, y2 );

	hdc.Line( 0, 0, x1, y1 );	// top left
	hdc.Line( 0, y3, x1, y2 );	// bottom left
	hdc.Line( x3, 0, x2, y1 );	// top right
	hdc.Line( x3, y3, x2, y2 );	// bottom right
	hdc.PopFillColor();
	hdc.PopPen();
#endif

	// - Draws elements of the page.
	OnDraw( hdc );
//    trace (hdc);
}

// ----------------------------------------------------------------------------
void GRPage::trace(VGDevice & hdc) const
{
    NVRect r;
    cout << "Page trace - page " << r << " " << getPosition() << endl;
    cout << "    => page num systems : " << getSystems()->size() << endl;
    SystemPointerList::const_iterator ptr;
    for( ptr = getSystems()->begin(); ptr != getSystems()->end(); ++ptr ) {
        GRSystem * system = *ptr;
        r = system->getBoundingBox();
        r += system->getPosition();
        cout << "    system " << r << endl;
        SSliceList * slices = system->getSlices();

        if (slices) {
            GuidoPos pos = slices->GetHeadPosition();
            while (pos) {
                GRSystemSlice * ss = slices->GetNext(pos);
                r = ss->getBoundingBox();
                r += ss->getPosition();
                cout << "    slice " << r << endl;

                StaffVector * sv = ss->getStaves();           // get the staves list
                if (sv) {
                    for( int i = sv->GetMinimum(); i <= sv->GetMaximum(); ++i) {
                        GRStaff * staff = sv->Get(i);
                        if (staff) {
                            cout << "      - staff " << staff->getBoundingBox() << " " << staff->getPosition()  << endl;
/*                            NEPointerList * selts = staff->getElements();
                            if (selts) {
                                GuidoPos pos = selts->GetHeadPosition();
                                while (pos) {
                                    GRNotationElement * nelt = selts->GetNext(pos);
                                    if (nelt) {
                                        cout << *nelt;
                                        nelt->DrawBoundingBox (hdc, GColor(0, 0, 255));
                                    }
                                    else cout << "==> GRStaff notation element is NULL" << endl;
                                }
                            }
                            else cout << "==> GRStaff elements is NULL" << endl;
*/
                        }
                        else cout << "==> GRStaff is NULL" << endl;
                    }								
                }
                else cout << "==> StaffVector is NULL" << endl;
            }
        }
        else cout << "==> no slices in system" << endl;
    }
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRPage::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	GuidoPos pagepos = First();
	while (pagepos)
	{
		GetNext(pagepos)->GetMap( sel, f, infos );
	}

	infos.fPos.x += mLeftMargin;
	infos.fPos.y += mTopMargin;
	if (sel == kGuidoPage)
		SendMap (f, getRelativeTimePosition(), getDuration(), kPage, infos);
	else {
		for( SystemPointerList::const_iterator i = mSystems.begin(); i != mSystems.end(); i++ )
		{
			(*i)->GetMap(sel, f, infos);
		}
	}
	infos.fPos.x -= mLeftMargin;
	infos.fPos.y -= mTopMargin;
}


// ----------------------------------------------------------------------------
/** \brief Draws the score page.
*/
void GRPage::OnDraw( VGDevice & hdc ) const
{	
// TODO: test if the element intersect with the clipping box
	GuidoPos pagepos = First();

	while (pagepos)
		GetNext(pagepos)->OnDraw(hdc);

	// - Convert from centimeter to logical.
	const float tstx = mLeftMargin;	// (JB) sign change
	const float tsty = mTopMargin;		// (JB) sign change

	// this resets the window-origin, so that left and top margins are correct ...
	// dont forget to set the clipping-rectangle!
	gClipRect.left -= tstx;
	gClipRect.top -= tsty;
	gClipRect.right -= tstx;
	gClipRect.bottom -= tsty;

	// The following sets the WindowOrigin so that
	// 0,0 is at the top left (including margins)
	//
	//  ---------------
	//  |    mt       |
	//  |  x-------   |
	//  |  |      | mr|
	//  |ml|      |   |
	//  |  --------   |
	//  |    mb       |
	//  |-------------|

	hdc.OffsetOrigin( tstx, tsty ); 

	GRSystem * theSystem;
	SystemPointerList::const_iterator ptr;
	for( ptr = mSystems.begin(); ptr != mSystems.end(); ++ ptr )
	{
		theSystem = *ptr;
		const NVPoint & pos = theSystem->getPosition();
		NVRect br (theSystem->getBoundingBox());
		br += pos;
			
		if( br.Collides( gClipRect ) == false )
			continue;

		gCurSystem = theSystem;
	    theSystem->OnDraw(hdc);
	}

	if (gBoundingBoxesMap & kPageBB)
		DrawBoundingBox( hdc, kPageBBColor);
	hdc.OffsetOrigin( -tstx, -tsty ); 
}

/** \brief Sets the scaling of input graphic device context, according to 
		input sizes.
	(was setMapping)
*/
void GRPage::setScaling( VGDevice & hdc, float vsizex, float vsizey ) const
{
	float newScaleX = vsizex;
	float newScaleY = vsizey;
	
	getScaling (newScaleX, newScaleY);
	hdc.SetScale( newScaleX, newScaleY ); // ok
	
//	const float sizex = getPageWidth();	// Get the logical (virtual) size
//	const float sizey = getPageHeight(); //	used internaly by Guido.
//
//	if( sizex <= 0 ) return;
//	if( sizey <= 0 ) return;
//
//	// - Calculate the new device context scaling factors.
//	float newScaleX = vsizex / sizex;
//	float newScaleY = vsizey / sizey;
//	
//	// - Force the page to be proportional. 
//	// (This could be a setting for GuidoSetSetting(): proportionnal or non-proportionnal)
//	if( newScaleX > newScaleY )		newScaleX = newScaleY;
//	if( newScaleY > newScaleX )		newScaleY = newScaleX;
//
//	// - Scale the device context to match desired size & zoom.
//	hdc.SetScale( newScaleX, newScaleY ); // ok
}

void GRPage::getScaling( float& vsizex, float& vsizey ) const
{
	const float sizex = getPageWidth();	// Get the logical (virtual) size
	const float sizey = getPageHeight(); //	used internaly by Guido.

	if( sizex <= 0 ) return;
	if( sizey <= 0 ) return;

	// - Calculate the new device context scaling factors.
	float newScaleX = vsizex / sizex;
	float newScaleY = vsizey / sizey;
	
	// - Force the page to be proportional. 
	// (This could be a setting for GuidoSetSetting(): proportionnal or non-proportionnal)
	if( newScaleX > newScaleY )		newScaleX = newScaleY;
	if( newScaleY > newScaleX )		newScaleY = newScaleX;
	
	vsizex = newScaleX;
	vsizey = newScaleY;
}

// ----------------------------------------------------------------------------
/** \brief Recalculates the bounding box of the page.
*/
void GRPage::updateBoundingBox()
{
	// - Start from a null rectangle
	mBoundingBox.Set( 0, 0, 0, 0 );
	GRNotationElement * el;

	// - Add systems boxes
	SystemPointerList::iterator ptr;
	TYPE_DURATION duration;
	for( ptr = mSystems.begin(); ptr != mSystems.end(); ++ ptr ) {
		el = *ptr;
		NVRect eltBox ( el->getBoundingBox());
		eltBox += el->getPosition();
		mBoundingBox.Merge( eltBox );
		duration += el->getDuration();
		//addToBoundingBox( el );
	}
	setDuration (duration);

	// - Add page and system-tags boxes
	GuidoPos mypos = First();
	if (mypos) {
		GRNotationElement * el = GetNext( mypos );
		NVRect eltBox ( el->getBoundingBox());
		eltBox += el->getPosition();
		mBoundingBox.Merge( eltBox );
		// todo: addToBoundingBox( el );
	}
	mMapping = mBoundingBox;

	// - Add the page margins
/*
	mBoundingBox.left -= getMarginLeft();
	mBoundingBox.top -= getMarginTop();
	mBoundingBox.right += getMarginRight();
	mBoundingBox.bottom += getMarginBottom();
*/
	// This adds some space to the right and at the bottom.
	// The NoteServer can display a nicer picture using this ....
	// maybe this needs to be changed later!?
	//mBoundingBox.right += 5* LSPACE;
	//mBoundingBox.bottom += 5* LSPACE;

}

// ----------------------------------------------------------------------------
/** \brief Trim the page size to the actual Music score. 
	
	This is done using the bounding box. (Note that the
	bounding box already contain margins)
*/
void GRPage::adjustPageSize()
{
	updateBoundingBox();
	mWidth = mBoundingBox.Width() + getMarginLeft() + getMarginRight();
	mHeight = mBoundingBox.Height() + getMarginTop() + getMarginBottom();
}

// ----------------------------------------------------------------------------
/** \brief not yet implemented
*/
void GRPage::setSpringParameter(float npar)
{
}

// ----------------------------------------------------------------------------
ARMusic * GRPage::getARMusic() const
{
	return /*dynamic*/static_cast<ARMusic*>(getAbstractRepresentation());
}

// ----------------------------------------------------------------------------
/** \brief Distributes the systems of the page
	evenly .... but only, if the distance will be less than
	(10 * LSPACE) between mSystems 

	\bug does not take account of the page title .
*/
void GRPage::finishPage( bool islastpage )
{
	if (GRStaffManager::sSystemDistribution == kNeverDistrib) {
		SystemPointerList::iterator ptr;
		for( ptr = mSystems.begin(); ptr != mSystems.end(); ++ ptr )
			(*ptr)->FinishSystem();
		return;
	}

	const size_t systemCount = mSystems.size();
	float pagesizey = getInnerHeight();
	float dist = pagesizey - m_totalsystemheight;
	if (systemCount > 1)
		dist = dist / (float(systemCount - 1));

	if (dist > 0) {
		if ((GRStaffManager::sSystemDistribution == kAlwaysDistrib) 
			|| (GRStaffManager::sSystemDistribution == kAutoDistrib) // DF added on Feb 13 2011 to force correct mapping
			|| GRStaffManager::sOptPageFill
			|| (!islastpage) || (dist <= (0.1f * pagesizey)))
		{
			const float distribLimit = GRStaffManager::sSystemDistribLimit * pagesizey;
			if(( GRStaffManager::sSystemDistribution == kAutoDistrib ) && ( dist > distribLimit ))
			{
				// We are here because the distance between systems is too large
				if( false ) // islastpage )
				{
					return;
				}
				else {
					// this is auto ...
					dist = 0.075f * pagesizey; // Hardcoded
				}
			}

			// then we put the mSystems at these distances ...
			float cury = 0;
			for(SystemPointerList::iterator i = mSystems.begin(); i != mSystems.end(); i++ ) {
				GRSystem * system = *i;
				if (cury > 0) {
					NVPoint newpos;
					newpos.y = cury - system->getBoundingBox().top;
					system->setPosition( newpos );
					cury += system->getBoundingBox().Height();
				}
				else // So this is the first system. Just get its bottom.
				{
					cury += system->getPosition().y + system->getBoundingBox().bottom;
				}				
				cury += dist;
                system->FinishSystem();
			}
		}
	}
	else {
		SystemPointerList::iterator ptr;
		for( ptr = mSystems.begin(); ptr != mSystems.end(); ++ ptr )
			(*ptr)->FinishSystem();
	}
	// hack to get correct time position for the page [DF - May 26 2010]
	setRelativeTimePosition ( (*mSystems.begin())->getRelativeTimePosition() );
}

// ==========================================================================
//		Page format: size and margins
// ==========================================================================

/** \brief Sets the page format (margins and size) of the Page.
	
	\param arp the new page format description.
*/
void GRPage::setPageFormat( ARPageFormat * arp )
{
	arp->getPageFormat( &mWidth, &mHeight, &mLeftMargin, &mTopMargin, &mRightMargin, &mBottomMargin );
}

// ----------------------------------------------------------------------------
/** \brief Gives the page format (size and margins) in Guido internal units.
	
*/
void 
GRPage::getPageFormat( GuidoPageFormat * outFormat ) const
						//float * width, float * height, 
						//	float * ml, float * mt, float * mr, float * mb )
{
	if( outFormat == 0 ) return;
	outFormat->width = mWidth;
	outFormat->height = mHeight;
	outFormat->marginleft = mLeftMargin;
	outFormat->margintop = mTopMargin;
	outFormat->marginright = mRightMargin;
	outFormat->marginbottom = mBottomMargin;
}

// ----------------------------------------------------------------------------
/** \brief Gives the margins of the score page, in centimeters.

	left, top, right and bottom pointers must be valid. The page size 
	includes margins.
*/
void GRPage::getMarginsCm(float * ml, float * mt, float * mr, float * mb)
{
	*ml = mLeftMargin * kVirtualToCm;
	*mt = mTopMargin * kVirtualToCm;
	*mr = mRightMargin * kVirtualToCm;
	*mb = mBottomMargin * kVirtualToCm;
}

/** \brief Width, internal units
*/
float GRPage::getPageWidth() const
{
	return mWidth;
}

/** \brief Height, internal units
*/
float GRPage::getPageHeight() const
{
	return mHeight;
}

/** \brief Inner width (without margins), internal units
*/
float GRPage::getInnerWidth() const
{
	return (mWidth - mLeftMargin - mRightMargin);
}

/** \brief Inner height (without margins), internal units
*/
float GRPage::getInnerHeight() const
{
	return (mHeight - mTopMargin - mBottomMargin);
}

/** \brief Width of the page, centimeters.
*/
float GRPage::getPageWidthCm() const
{
	return mWidth * kVirtualToCm;
}

/** \brief Height of the page, centimeters.
*/
float GRPage::getPageHeightCm() const
{
	return mHeight * kVirtualToCm;
}

/** \brief Inner width (without margins), centimeters.
*/
float GRPage::getInnerWidthCm() const
{
	return getInnerWidth() * kVirtualToCm;
}

/** \brief Inner height (without margins), centimeters.
*/
float GRPage::getInnerHeightCm() const
{
	return getInnerHeight() * kVirtualToCm;
}

/** \brief Gives the page margins
*/
float GRPage::getMarginLeft() const
{ 
	return mLeftMargin; 
}
	
float GRPage::getMarginTop() const
{ 
	return mTopMargin; 
}
		
float GRPage::getMarginRight() const
{ 
	return mRightMargin; 
}

float GRPage::getMarginBottom() const
{ 
	return mBottomMargin;	
}

// ==========================================================================
//		Attic
// ==========================================================================

/** \brief Called by the System, when it has been finished.
	The GRPage then decides if the system can fit on the page.

	 ! never called !?
*/ 
void GRPage::systemFinished(GRSystem * inSystem)
{
	assert(false);

	if (mSystems.size() == 1)
	{
		// then, do nothing ...
		updateBoundingBox();
		return;
	}

	// if we have more than one system ...
	assert(inSystem == mSystems.back());

	if (m_staffmgr->IsAutoPageBreak() && 
		inSystem->getPosition().y +
		inSystem->getBoundingBox().bottom
		> (getInnerHeight()))
	{
		// this just removes the tail from the list, DOES NOT DELETE!
		mSystems.pop_back();

		// then we have to build a new page ....
		GRPage * newpage = new GRPage( mCurMusic, m_staffmgr,
						inSystem->getRelativeTimePosition(), this);
		inSystem->setGRPage(newpage);
		float tmpf = 0; //(JB) this one was not initialized !
		newpage->addSystem(inSystem, &tmpf);
		m_staffmgr->NewPage(newpage);
	}
	updateBoundingBox();
}

void GRPage::setHorizontalSpacing()
{
	assert(false);
/* 	if (mSystems.empty()) return;

	GuidoPos possystem = mSystems->GetHeadPosition();
	while(possystem)
	{
		mSystems->GetNext(possystem);
	}
	updateBoundingBox();//mBoundingBox.right=maxpointx; // Korrektur der Musikgrenzen
	*/
}



