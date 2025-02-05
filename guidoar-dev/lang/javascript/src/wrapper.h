/*
  GUIDO AR Library
  Copyright (C) 2016	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include "libguidoar.h"


typedef struct garOut {
	guido::garErr err;			// the error code
	std::string str;		// the output string
} garOut;


std::string	guidoarVersionString();

//--------------------------------------------------------------------------------
// operations on scores
//--------------------------------------------------------------------------------
/*! \brief a wrapper to guido2unrolled */
garOut			gmn2unrolled(const std::string& gmn);

/*! \brief a wrapper to guidoVTranpose */
garOut			gmnVTranpose(const std::string& gmn, int interval);

/*! \brief a wrapper to guidoGTranpose */
garOut			gmnGTranpose(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVTop */
garOut			gmnVTop(const std::string& gmn, int nvoices);

/*! \brief a wrapper to guidoGTop */
garOut			gmnGTop(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVBottom */
garOut			gmnVBottom(const std::string& gmn, int nvoices);

/*! \brief a wrapper to guidoGBottom */
garOut			gmnGBottom(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVHead */
garOut			gmnVHead(const std::string& gmn, rational duration);

/*! \brief a wrapper to guidoGHead */
garOut			gmnGHead(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVEHead */
garOut			gmnVEHead(const std::string& gmn, int n);

/*! \brief a wrapper to guidoGEHead */
garOut			gmnGEHead(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVTail */
garOut			gmnVTail(const std::string& gmn, rational duration);

/*! \brief a wrapper to guidoGTail */
garOut			gmnGTail(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVETail */
garOut			gmnVETail(const std::string& gmn, int n);

/*! \brief a wrapper to guidoGETail */
garOut			gmnGETail(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoVETail */
garOut			gmnVETail(const std::string& gmn, int n);

/*! \brief a wrapper to guidoGETail */
garOut			gmnGETail(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoGSeq */
garOut			gmnGSeq(const std::string& gmn1, const std::string& gmn2);

/*! \brief a wrapper to guidoGPar */
garOut			gmnGPar		(const std::string& gmn1, const std::string& gmn2);

/*! \brief a wrapper to guidoGRPar */
garOut			gmnGRPar(const std::string& gmn1, const std::string& gmn2);

/*! \brief a wrapper to guidoGMirror */
garOut			gmnGMirror(const std::string& gmn1, const std::string& gmn2);

/*! \brief a wrapper to guidoGSetDuration */
garOut			gmnGSetDuration(const std::string& gmn, const std::string& gmnSpec);

/*! \brief a wrapper to guidoApplyRythm */
garOut			gmnApplyRythm(const std::string& gmn, const std::string& gmnSpec, guido::TApplyMode mode);

/*! \brief a wrapper to guidoApplyPitch */
garOut			gmnApplyPitch(const std::string& gmn, const std::string& gmnSpec, guido::TApplyMode mode, guido::chordPitchMode pmode);

/*! \brief a wrapper to guidoVSetDuration */
garOut			gmnVSetDuration(const std::string& gmn, rational duration);

/*! \brief a wrapper to guidoVMultDuration */
garOut			gmnVMultDuration(const std::string& gmn, float mult);

/*! \brief a wrapper to guidocheck */
bool				gmncheck(const std::string& gmn);

/*! \brief a wrapper to guidoDuration */
rational			gmnDuration(const std::string& gmn);

/*! \brief a wrapper to guidoEv2Time */
rational			gmnEv2Time(const std::string& gmn, unsigned int index, unsigned int voice);


