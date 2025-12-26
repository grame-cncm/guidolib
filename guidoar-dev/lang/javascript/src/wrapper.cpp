/*
  GUIDO AR Library
  Copyright (C) 2016	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <sstream>
#include "wrapper.h"

using namespace std;
using namespace guido;

string	guidoarVersionString()	{ return guidoarVersionStr(); }


//--------------------------------------------------------------------------------
// operations on scores
//--------------------------------------------------------------------------------
/*! \brief a wrapper to guido2unrolled */
garOut			gmn2unrolled(const string& gmn)
{
	stringstream stream;
	garOut out;
	out.err = guido2unrolled(gmn.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVTranpose */
garOut			gmnVTranpose(const string& gmn, int interval)
{
	stringstream stream;
	garOut out;
	out.err = guidoVTranpose(gmn.c_str(), interval, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGTranpose */
garOut			gmnGTranpose(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGTranpose(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVTop */
garOut			gmnVTop(const string& gmn, int nvoices)
{
	stringstream stream;
	garOut out;
	out.err = guidoVTop(gmn.c_str(), nvoices, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGTop */
garOut			gmnGTop(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGTop(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVBottom */
garOut			gmnVBottom(const string& gmn, int nvoices)
{
	stringstream stream;
	garOut out;
	out.err = guidoVBottom(gmn.c_str(), nvoices, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGBottom */
garOut			gmnGBottom(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGBottom(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVHead */
garOut			gmnVHead(const string& gmn, rational duration)
{
	stringstream stream;
	garOut out;
	out.err = guidoVHead(gmn.c_str(), duration, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGHead */
garOut			gmnGHead(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGHead(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVEHead */
garOut			gmnVEHead(const string& gmn, int n)
{
	stringstream stream;
	garOut out;
	out.err = guidoVEHead(gmn.c_str(), n, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGEHead */
garOut			gmnGEHead(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGEHead(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVTail */
garOut			gmnVTail(const string& gmn, rational duration)
{
	stringstream stream;
	garOut out;
	out.err = guidoVTail(gmn.c_str(), duration, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGTail */
garOut			gmnGTail(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGTail(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVETail */
garOut			gmnVETail(const string& gmn, int n)
{
	stringstream stream;
	garOut out;
	out.err = guidoVETail(gmn.c_str(), n, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGETail */
garOut			gmnGETail(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGETail(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGSeq */
garOut			gmnGSeq(const string& gmn1, const string& gmn2)
{
	stringstream stream;
	garOut out;
	out.err = guidoGSeq(gmn1.c_str(), gmn2.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGPar */
garOut			gmnGPar		(const string& gmn1, const string& gmn2)
{
	stringstream stream;
	garOut out;
	out.err = guidoGPar(gmn1.c_str(), gmn2.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGRPar */
garOut			gmnGRPar(const string& gmn1, const string& gmn2)
{
	stringstream stream;
	garOut out;
	out.err = guidoGRPar(gmn1.c_str(), gmn2.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGMirror */
garOut			gmnGMirror(const string& gmn1, const string& gmn2)
{
	stringstream stream;
	garOut out;
	out.err = guidoGMirror(gmn1.c_str(), gmn2.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoGSetDuration */
garOut			gmnGSetDuration(const string& gmn, const string& gmnSpec)
{
	stringstream stream;
	garOut out;
	out.err = guidoGSetDuration(gmn.c_str(), gmnSpec.c_str(), stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoApplyRythm */
garOut			gmnApplyRythm(const string& gmn, const string& gmnSpec, TApplyMode mode)
{
	stringstream stream;
	garOut out;
	out.err = guidoApplyRythm(gmn.c_str(), gmnSpec.c_str(), mode, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoApplyPitch */
garOut			gmnApplyPitch(const string& gmn, const string& gmnSpec, TApplyMode mode, chordPitchMode pmode)
{
	stringstream stream;
	garOut out;
	out.err = guidoApplyPitch(gmn.c_str(), gmnSpec.c_str(), mode, pmode, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVSetDuration */
garOut			gmnVSetDuration(const string& gmn, rational duration)
{
	stringstream stream;
	garOut out;
	out.err = guidoVSetDuration(gmn.c_str(), duration, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidoVMultDuration */
garOut			gmnVMultDuration(const string& gmn, float mult)
{
	stringstream stream;
	garOut out;
	out.err = guidoVMultDuration(gmn.c_str(), mult, stream);
	if (out.err == kNoErr) out.str = stream.str();
	return out;
}

/*! \brief a wrapper to guidocheck */
bool	gmncheck(const std::string& gmn)		{ return guidocheck(gmn.c_str()); }

/*! \brief a wrapper to guidoDuration */
rational gmnDuration(const std::string& gmn)		{ return guidoDuration(gmn.c_str()); }

/*! \brief a wrapper to guidoEv2Time */
rational gmnEv2Time(const std::string& gmn, unsigned int index, unsigned int voice)
{
	return guidoEv2Time (gmn.c_str(), index, voice);
}
