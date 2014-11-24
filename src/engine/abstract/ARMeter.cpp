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
#include "secureio.h"

#include "ARMeter.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"


ListOfTPLs ARMeter::ltpls(1);

ARMeter::ARMeter()
{
	mtype = NONE;
	autoBarlines    = true;
	autoMeasuresNum = false;

    numerator = 0;
    denominator = 0;
}

ARMeter::ARMeter(int p_numerator, int p_denominator)
{
	numerator   = p_numerator;
	denominator = p_denominator;

    std::stringstream bufferSStream;
    bufferSStream << numerator << "/" << denominator;

	mMeterName = bufferSStream.str().c_str();
	mtype = NUMERIC;
	autoBarlines    = true;
	autoMeasuresNum = false;
}


ARMeter::~ARMeter()
{
}

void ARMeter::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,type,4/4,r;F,size,1.0,o;S,autoBarlines,on,o;S,autoMeasuresNum,off,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			mMeterName = tps->getValue();

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			assert(tpf);

			size = tpf->getValue();

			tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			std::string off("off");
			if (off == tps->getValue())
				autoBarlines = false;
			else
				autoBarlines = true;

			tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			std::string on("on");
			if (on == tps->getValue())
				autoMeasuresNum = true;
			else
				autoMeasuresNum = false;


            /**** Meter type analysis ****/

            if (mMeterName == "")
            {
                mtype = NONE;
                numeratorsVector.push_back(4);
                denominator = 4;
            }
            else if (mMeterName == "C")
            {
                mtype = C;
                numeratorsVector.push_back(4);
                denominator = 4;
            }
            else if (mMeterName == "C/")
            {
                mtype = C2;
                numeratorsVector.push_back(2);
                denominator = 2;
            }
            else
            {
                mtype       = NUMERIC;
                denominator = 1;

                //Meter string analysis to set numerator/denominator
                std::string meterStr(mMeterName);
                std::string delimiterSlash = "/";

                size_t posSlash = 0;
                std::string completeNumeratorStr;
                std::string denominatorStr = "";

                posSlash = meterStr.find(delimiterSlash);

                if (posSlash == std::string::npos)
                    completeNumeratorStr = meterStr;
                else {
                    completeNumeratorStr = meterStr.substr(0, posSlash);
                    denominatorStr       = meterStr;
                    denominatorStr.erase(0, posSlash + 1);
                }


                /* Numerator check */

                int tmpNumeratorX = 0;
                std::string delimiterPlus = "+";
                size_t posPlus = 0;
                size_t endStr;
                std::string tpmNumeratorXStr;

                do {
                    posPlus = completeNumeratorStr.find(delimiterPlus);
                    
                    if (posPlus == std::string::npos)
                        endStr = completeNumeratorStr.size();
                    else
                        endStr = posPlus;

                    tpmNumeratorXStr = completeNumeratorStr.substr(0, endStr);

                    if (sscanf(tpmNumeratorXStr.c_str(), "%d", &tmpNumeratorX) == 1)
                    {
                        /* We keep only the first 2 figures */
                        while(tmpNumeratorX > 99)
                            tmpNumeratorX = (int)(tmpNumeratorX / 10);

                        numeratorsVector.push_back(tmpNumeratorX);

                        completeNumeratorStr.erase(0, endStr + delimiterPlus.length());
                    }
                    else {
                        numeratorsVector.clear();
                        posPlus = std::string::npos;
                    }
                }
                while (posPlus != std::string::npos);

                if (!numeratorsVector.empty()) {
                    if (denominatorStr.size() > 0 && sscanf(denominatorStr.c_str(), "%d", &denominator) == 1)
                    {
                        while(denominator > 99)
                            denominator = (int)(denominator / 10);
                    }
                }
                else { // default
                    mtype = C;
                    numeratorsVector.push_back(4);
                    denominator = 4;
                }
            }
		}

		delete rtpl;
	}
	else
	{
		numeratorsVector.push_back(0);
		denominator = 1;
		mtype = NONE;
	}

    if (!numeratorsVector.empty())
    {
        for(size_t i = 0; i < numeratorsVector.size(); i++)
            numerator += numeratorsVector[i];
    }

    tpl.RemoveAll();
}

bool ARMeter::IsStateTag() const
{
	return true;
}

TYPE_TIMEPOSITION ARMeter::getMeterTime() const
{
	return TYPE_TIMEPOSITION(numerator,denominator);
}

void ARMeter::printName(std::ostream& os) const
{
    os << "ARMeter";
    ARMusicalTag::printName(os);
}

void ARMeter::printParameters(std::ostream& os) const
{
    os << "name: \"" << mMeterName << "\"; ";
    os << "autoBarlines: " << (autoBarlines ? "true" : "false") << "; ";
    os << "autoMeasuresNum: " << (autoMeasuresNum ? "true" : "false") << "; ";

    ARMusicalTag::printParameters(os);
}