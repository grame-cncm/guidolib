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

#include <iostream>
#include <sstream>
#include <string.h>

#include "ARMeter.h"
#include "GRMeter.h"
#include "GRDefine.h"
#include "GRMusic.h"
#include "GRStaff.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"
#include "secureio.h"

using namespace std;

GRMeter::GRMeter( ARMeter * abstractRepresentationOfMeter, GRStaff * curstaff, bool p_ownsAR )
  : GRTagARNotationElement(abstractRepresentationOfMeter, curstaff->getStaffLSPACE(), p_ownsAR),
	numerator(0), denominator(1)
{
	assert(abstractRepresentationOfMeter);
	assert(curstaff);

	curLSPACE = curstaff->getStaffLSPACE();
    mTagSize *= curstaff->getSizeRatio() * abstractRepresentationOfMeter->getSize();

	mtype = getARMeter()->getMeterType();
	if (mtype == ARMeter::NUMERIC)
	{
	  numeratorsVector = getARMeter()->getNumeratorsVector();
	  denominator      = getARMeter()->getDenominator();
    }

    numerator = getARMeter()->getNumerator();

	mNeedsSpring = 1;
	sconst = SCONST_METER;

	mBoundingBox.left = 0;
	mBoundingBox.top  = 0;

	float extent = 0;
	if (mtype == ARMeter::NUMERIC)
	{
        std::stringstream bufferDenominatorSStream;
        bufferDenominatorSStream << denominator;
        std::string bufferDenominator = bufferDenominatorSStream.str();
		
        std::vector<float> extentsNumeratorsVector;
		float extentDenominator = 0;
		float x = 0, y = 0;
        float extentPlus = 0;

        if (numeratorsVector.size() > 1)
        {
            char bufferPlus = '+';
            FontManager::gFontScriab->GetExtent(bufferPlus, &x, &y, gGlobalSettings.gDevice);
            x *= mTagSize;
            y *= mTagSize;
            extentPlus  = x;
        }

		if( gGlobalSettings.gDevice )
        {
            for(size_t i = 0; i < numeratorsVector.size(); i++)
            {
                if (i)
                    extentsNumeratorsVector.push_back(extentPlus);

                std::stringstream bufferNumeratorTmpSStream;

                bufferNumeratorTmpSStream << numeratorsVector[i];
                std::string bufferNumeratorTmp = bufferNumeratorTmpSStream.str();
                FontManager::gFontScriab->GetExtent(bufferNumeratorTmp.c_str(), bufferNumeratorTmp.size(), &x, &y, gGlobalSettings.gDevice);
                x *= mTagSize;
                y *= mTagSize;

                bufferNumeratorTmpSStream.clear();

                extentsNumeratorsVector.push_back(x);
            }
		
            FontManager::gFontScriab->GetExtent(bufferDenominator.c_str(), bufferDenominator.size(), &x, &y, gGlobalSettings.gDevice);
            x *= mTagSize;
            y *= mTagSize;
			extentDenominator = x;
		}

        totalNumeratorExtent = 0;

        for (size_t i = 0; i < extentsNumeratorsVector.size(); i++)
            totalNumeratorExtent += extentsNumeratorsVector[i];

		extent = totalNumeratorExtent > extentDenominator ? totalNumeratorExtent : extentDenominator;
		
		mBoundingBox.left  = (GCoord)(- extent * 0.5f);
		mBoundingBox.right = (GCoord)(  extent * 0.5f);
	}
	else if (mtype == ARMeter::C || mtype == ARMeter::C2)
	{
        extent = GetSymbolExtent(kCSymbol) * mTagSize;
		mBoundingBox.left  = (GCoord)(- extent * 0.5f);
		mBoundingBox.right = (GCoord)(  extent * 0.5f);
	}

	mBoundingBox.bottom = (GCoord)(5 * curLSPACE);

	// set leftSpace, rightSpace 
    mLeftSpace  = curLSPACE * 0.5f + (mTagSize - 1) * 2 * curLSPACE + (extent - (68 * mTagSize)) * 1.0f;
	mRightSpace = 2 * curLSPACE;

	// y position correction according to staff lines count - DF sept. 23 2009
	if (curstaff) {
		const float curLSPACE = curstaff->getStaffLSPACE();
		int linesOffset = curstaff->getNumlines() - 5;

		if (linesOffset)
			mPosition.y += curLSPACE * linesOffset / 2;
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
	if (error)
		return;
	if(!mDraw)
		return;

	if (mtype == ARMeter::NUMERIC)
	{
        /**** Numerator ****/

        float currentDx = 0;
        float totalExtent = mBoundingBox.right - mBoundingBox.left;
        float emptyNumeratorSpace;

        bool hastwo = false;

        const char charPlus = '+';
        float extentCharPlusx;
        float extentCharPlusy;
        FontManager::gFontScriab->GetExtent(charPlus, &extentCharPlusx, &extentCharPlusy, gGlobalSettings.gDevice);
        extentCharPlusx *= mTagSize;
        extentCharPlusy *= mTagSize;

        float dx2 = 0;

        for (size_t i = 0; i < numeratorsVector.size(); i++)
        {
            if (i == 0)
            {
                emptyNumeratorSpace = totalExtent - totalNumeratorExtent;
                currentDx = mBoundingBox.left + emptyNumeratorSpace / 2;
            }
            else
            {
                float dxOffsetPlus = (extentCharPlusx - extentCharPlusx * 0.9f) * 0.5f; // Offset to center the "+" between to numbers
                float dxOffsetNum  = (mTagSize - 1) * (- curLSPACE * 0.8f) - (totalExtent - (68 * mTagSize)) * 0.5f;
                float dyOffsetPlus = curLSPACE - (mTagSize - 1) * (curLSPACE * 0.9f);
                OnDrawSymbol(hdc, charPlus, currentDx + dxOffsetNum + dxOffsetPlus, dyOffsetPlus, mTagSize * 0.9f);
                currentDx += extentCharPlusx;
            }

            std::stringstream bufferSStream;
            bufferSStream << numeratorsVector[i];
            std::string buffer = bufferSStream.str();

            float extentBufferx;
            float extentBuffery;
            FontManager::gFontScriab->GetExtent(buffer.c_str(), buffer.size(), &extentBufferx, &extentBuffery, gGlobalSettings.gDevice);
            extentBufferx *= mTagSize;
            extentBuffery *= mTagSize;

            if (numerator > 9)
            {
                hastwo = true;

                float extentFirstCharx;
                float extentFirstChary;
                FontManager::gFontScriab->GetExtent(buffer.c_str()[0], &extentFirstCharx, &extentFirstChary, gGlobalSettings.gDevice);
                extentFirstCharx *= mTagSize;
                extentFirstChary *= mTagSize;

                dx2 = currentDx + extentFirstCharx;
            }
            
            float dxOffsetNum = (mTagSize - 1) * (- curLSPACE * 0.8f) - (totalExtent - (68 * mTagSize)) * 0.5f;
            float dyOffsetNum = curLSPACE - (mTagSize - 1) * (curLSPACE * 0.9f);

            OnDrawSymbol(hdc, buffer.c_str()[0], currentDx + dxOffsetNum, dyOffsetNum , mTagSize);
            
            if (buffer.c_str()[1])
                OnDrawSymbol(hdc, buffer.c_str()[1], dx2 + dxOffsetNum, dyOffsetNum, mTagSize);

            currentDx += extentBufferx;
        }

        /*******************/


        /**** Denominator ****/

        std::stringstream bufferSStream;
        bufferSStream << denominator;
        std::string buffer = bufferSStream.str();
        
        dx2 = 0;

        float extentBufferx;
        float extentBuffery;
        FontManager::gFontScriab->GetExtent(buffer.c_str(), buffer.size(), &extentBufferx, &extentBuffery, gGlobalSettings.gDevice);
        extentBufferx *= mTagSize;
        extentBuffery *= mTagSize;

        float emptyDenominatorSpace = totalExtent - extentBufferx;
        currentDx = mBoundingBox.left + emptyDenominatorSpace / 2;

		if (denominator > 9)
        {
			hastwo = true;

            float extentFirstCharx;
            float extentFirstChary;
            FontManager::gFontScriab->GetExtent(buffer.c_str()[0], &extentFirstCharx, &extentFirstChary, gGlobalSettings.gDevice);
            extentFirstCharx *= mTagSize;
            extentFirstChary *= mTagSize;

			dx2 = currentDx + extentFirstCharx;
		}
        
        float dxOffsetNum   = (mTagSize - 1) * (- curLSPACE * 0.8f) - (totalExtent - (68 * mTagSize)) * 0.5f;
        float dyOffsetDenom = (3 * curLSPACE) + (mTagSize - 1) * (curLSPACE * 0.9f);
		OnDrawSymbol(hdc, buffer.c_str()[0], currentDx + dxOffsetNum, dyOffsetDenom, mTagSize);

		if (buffer.c_str()[1])
			OnDrawSymbol(hdc, buffer.c_str()[1], dx2 + dxOffsetNum, dyOffsetDenom, mTagSize);

        /*********************/
	}
	else if (mtype == ARMeter::C)
	{
        float extentBufferx;
        float extentBuffery;
        FontManager::gFontScriab->GetExtent(kCSymbol, &extentBufferx, &extentBuffery, gGlobalSettings.gDevice);
        extentBufferx *= mTagSize;
        extentBuffery *= mTagSize;
        
        float dxOffsetNum   = - curLSPACE + (mTagSize - 1) * (- curLSPACE * 2.0f);
        float dyOffsetDenom = 2 * curLSPACE;
		OnDrawSymbol(hdc, kCSymbol, dxOffsetNum, dyOffsetDenom, mTagSize);
	}
	else if (mtype == ARMeter::C2)
	{
        float extentBufferx;
        float extentBuffery;
        FontManager::gFontScriab->GetExtent(kC2Symbol, &extentBufferx, &extentBuffery, gGlobalSettings.gDevice);
        extentBufferx *= mTagSize;
        extentBuffery *= mTagSize;
        
        float dxOffsetNum   = - curLSPACE + (mTagSize - 1) * (- curLSPACE * 2.0f);
        float dyOffsetDenom = 2 * curLSPACE;
		OnDrawSymbol(hdc, kC2Symbol, dxOffsetNum, dyOffsetDenom, mTagSize);
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

