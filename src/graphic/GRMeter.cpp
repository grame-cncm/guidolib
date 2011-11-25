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

#include <string.h>

#include "GRMeter.h"
#include "GRDefine.h"
#include "GRMusic.h"
#include "GRStaff.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"
#include "secureio.h"

#include <iostream>
using namespace std;

NVPoint GRMeter::refposC;
NVPoint GRMeter::refposC2;


GRMeter::GRMeter( ARMeter * abstractRepresentationOfMeter, GRStaff * curstaff, bool p_ownsAR )
  : GRTagARNotationElement(abstractRepresentationOfMeter, curstaff->getStaffLSPACE(),p_ownsAR),
	numerator(0), denominator(1)
{
	assert(abstractRepresentationOfMeter);
	assert(curstaff);

	curLSPACE = curstaff->getStaffLSPACE();

	mtype = getARMeter()->getMeterType();
	if (mtype == ARMeter::NUMERIC)
	{
	  numerator = getARMeter()->getNumerator();
	  denominator = getARMeter()->getDenominator();
	}

	//spacing= LSPACE;
	mNeedsSpring = 1;
	sconst = SCONST_METER;
	// obsolete
	// spacing = 0;

	mBoundingBox.left = 0;
	mBoundingBox.top  = 0;

	float extent = 0;
	if (mtype == ARMeter::NUMERIC)
	{
		char buffer1[10];
		char buffer2[10];
		
		snprintf(buffer1,10,"%d",numerator);
		snprintf(buffer2,10,"%d",denominator);
		
		float extent1 = 0;
		float extent2 = 0;
		float x = 0, y = 0;
		
		if( gGlobalSettings.gDevice )
		{
			FontManager::gFontScriab->GetExtent( buffer1, (int)strlen(buffer1), &x, &y, gGlobalSettings.gDevice ); 
			extent1 = x;
		
			FontManager::gFontScriab->GetExtent( buffer2, (int)strlen(buffer2), &x, &y, gGlobalSettings.gDevice ); 
			extent2 = x;
		}

		extent = extent1 > extent2 ? extent1 : extent2;
		
		mBoundingBox.left  = (GCoord)(-extent * 0.5f - LSPACE / 5);
		mBoundingBox.right = (GCoord)(extent * 0.5f + LSPACE);
	}
	else if (mtype == ARMeter::C || mtype == ARMeter::C2)
	{
		extent = GetSymbolExtent( kCSymbol);
		mBoundingBox.left = (GCoord)(-extent * 0.5f - LSPACE / 5);
		mBoundingBox.right = (GCoord)(extent * 0.5f + LSPACE);
	}

	mTagSize *= curstaff->getSizeRatio();
	mBoundingBox.bottom = (GCoord)(5*curLSPACE);

	// set leftSpace, rightSpace 

	mLeftSpace = (GCoord)(- mBoundingBox.left * mTagSize);
	mRightSpace = (GCoord)(mBoundingBox.right * mTagSize);

	switch (mtype)
	{
		case ARMeter::C :
			refposC.x  = (GCoord)(- extent * 0.5f - LSPACE / 5.0);
			refposC.y  = (GCoord)(2*LSPACE);								// no break ?
		case ARMeter::C2:
			refposC2.x  = (GCoord)(- extent * 0.5f - LSPACE / 5.0);
			refposC2.y  = (GCoord)(2*LSPACE);								// no break
		case ARMeter::NUMERIC:
			refpos.x  = (GCoord)(- extent * 0.5f - LSPACE / 5.0);
			refpos.y  = 0;
			break;
			
		default:	
			break;
	}

	// y position correction according to staff lines count - DF sept. 23 2009
	if( curstaff ) {
		const float curLSPACE = curstaff->getStaffLSPACE();
		int linesOffset = curstaff->getNumlines() - 5;
		if (linesOffset) {
			mPosition.y += curLSPACE * linesOffset / 2;
		}
	}

	// what about reference-position?
}

GRMeter::~GRMeter()
{
}

void GRMeter::GGSOutput() const
{
	char buffer[20];
	char buf[100];
	if (error) return;
	if (mtype == ARMeter::NUMERIC)
	  {
		
	  snprintf(buf,100,"\\draw_image<\"%d\",%ld,%d,%d>\n",
		  numerator,
		  getID(),
		  (int)(mPosition.x + ggsoffsetx),
		  (int)(LSPACE + ggsoffsety));
		  
	  AddGGSOutput(buf);
		   
	  
	  snprintf(buf,100,"\\draw_image<\"%d\",%ld,%d,%d>\n",
		denominator,
		  getID(),
		  (int)(mPosition.x + ggsoffsetx),
		  (int)(3*LSPACE + ggsoffsety));
	  AddGGSOutput(buf);
	  }
	 else if (mtype == ARMeter::C)
	  {
	  snprintf(buffer,20,"c" /*SCR_ALLABREVE*/ );
	  snprintf(buf,100,"\\draw_image<\"%s\",%ld,%d,%d>\n",
		buffer,
		  getID(),
		  (int)(mPosition.x + ggsoffsetx),
		  (int)(mPosition.y + 2 * LSPACE + ggsoffsety));

	  AddGGSOutput(buf);
	  }
	 else if (mtype == ARMeter::C2)
	  {
	  snprintf(buffer,20,"C" /* SCR_ALLABREVE2 */ );
	  snprintf(buf,100,"\\draw_image<\"%s\",%ld,%d,%d>\n",
		  buffer,
		  getID(),
		  (int)(mPosition.x + ggsoffsetx),
		  (int)(mPosition.y + 2 * LSPACE + ggsoffsety));
	  AddGGSOutput(buf);
	 }
}

void GRMeter::OnDraw(VGDevice & hdc) const
{
	if (error) return;

	if (mtype == ARMeter::NUMERIC)
	{
		char buffer[8];
		// wsprintf(buffer,"%d",numerator); <- windows only function, equiv sprintf ?
		snprintf( buffer, 8, "%d", numerator ); 
		
		// we need the size ....
		bool hastwo = false;
		if (numerator > 9)
			hastwo = true;
		
		float dx1 = 0;
		float dx2 = 0;
		if (hastwo)
		{
			dx1 = -30;	// hardcoded
			dx2 = 30;
		}
		else 
			dx1 = 0;

		OnDrawSymbol(hdc,buffer[0],dx1, curLSPACE, mTagSize);
		if (buffer[1])
			OnDrawSymbol(hdc,buffer[1],dx2, curLSPACE, mTagSize);

		// wsprintf(buffer,"%d",denominator); <- windows only
		snprintf( buffer, 8, "%d", denominator );
		
		hastwo = false;
		if (denominator > 9)
			hastwo = true;
		
		if (hastwo)
		{
			dx1 = -30;	// hardcoded
			dx2 = 30;
		}
		else {
			dx1 = 0;
			dx2 = 0;
        }

		OnDrawSymbol(hdc,buffer[0],dx1,(3 * curLSPACE), mTagSize);
		if (buffer[1])
			OnDrawSymbol(hdc,buffer[1],dx2,(3 * curLSPACE), mTagSize);

	}
	else if (mtype == ARMeter::C)
	{
		OnDrawSymbol( hdc, kCSymbol, 0, 0, mTagSize );
	}
	else if (mtype == ARMeter::C2)
	{
		OnDrawSymbol( hdc, kC2Symbol, 0, 0, mTagSize );
	}
}

void GRMeter::print() const
{
	fprintf(stderr,"(M%.2f,%.2f)",float(getRelativeTimePosition()),float(getDuration()));
}

ARMeter* GRMeter::getARMeter()
{
	return /*dynamic*/static_cast<ARMeter*>(getAbstractRepresentation());
}

const ARMeter* GRMeter::getARMeter() const 
{
	return /*dynamic*/static_cast<const ARMeter*>(getAbstractRepresentation());
 }

bool GRMeter::operator==(const GRMeter & meter) const
{
	return (mtype == meter.mtype
		&& numerator == meter.numerator
		&& denominator == meter.denominator);
}

bool GRMeter::operator==(const GRTag  &tag) const
{
	const GRMeter * meter = dynamic_cast<const GRMeter *>(&tag);
	if (meter)
		return this->operator ==(*meter);

	return false;
}

const NVPoint & GRMeter::getReferencePosition() const
{
	const ARMeter::metertype mtype = getARMeter()->getMeterType();
	switch (mtype)
	{
		case ARMeter::C:		return refposC;
		case ARMeter::C2:		return refposC2;
		case ARMeter::NUMERIC:	return refpos;
		default:	
			break;
	}
	return refpos;
}

