/*
  GUIDO AR Library
  Copyright (C) 2016	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <emscripten.h>
#include <bind.h>
#include "libguidoar.h"
#include "wrapper.h"

using namespace emscripten;
using namespace guido;

/*
 * Structure and enum binding.
 * This structures can be created and manipulated in javascript side like json object and passed to C++ method.
 */
EMSCRIPTEN_BINDINGS(CStruct) {
	emscripten::value_object<garOut>("garOut")
			.field("err", &garOut::err)
			.field("str", &garOut::str);

	emscripten::value_object<rational>("rational")
			.field("num", &rational::fNumerator)
			.field("denom", &rational::fDenominator);

	emscripten::enum_<garErr>("garErr")
			.value("kNoErr", garErr::kNoErr)
			.value("kInvalidFile", garErr::kInvalidFile)
			.value("kInvalidArgument", garErr::kInvalidArgument)
			.value("kOperationFailed", garErr::kOperationFailed);

	emscripten::enum_<TApplyMode>("TApplyMode")
			.value("kApplyOnce", TApplyMode::kApplyOnce)
			.value("kApplyForwardLoop", TApplyMode::kApplyForwardLoop)
			.value("kApplyForwardBackwardLoop", TApplyMode::kApplyForwardBackwardLoop);

	emscripten::enum_<chordPitchMode>("chordPitchMode")
			.value("kUseLowest", chordPitchMode::kUseLowest)
			.value("kUseHighest", chordPitchMode::kUseHighest);
}

/*
 * C interface bindings.
 * This classes can be used in javascript side.
 */
EMSCRIPTEN_BINDINGS(GuidoAR)
{
	function("guidoarVersion", 	guidoarVersion);
	function("guidoarVersionString", 	guidoarVersionString);

	function("gmn2unrolled", &gmn2unrolled);
	function("gmnVTranpose", &gmnVTranpose);
	function("gmnGTranpose", &gmnGTranpose);
	function("gmnVTop", 	&gmnVTop);
	function("gmnGTop", 	&gmnGTop);
	function("gmnVBottom", 	&gmnVBottom);
	function("gmnGBottom", 	&gmnGBottom);
	function("gmnVHead", 	&gmnVHead);
	function("gmnGHead", 	&gmnGHead);
	function("gmnVEHead", 	&gmnVEHead);
	function("gmnGEHead", 	&gmnGEHead);
	function("gmnVTail", 	&gmnVTail);
	function("gmnGTail", 	&gmnGTail);
	function("gmnVETail", 	&gmnVETail);
	function("gmnGETail", 	&gmnGETail);
	function("gmnVETail", 	&gmnVETail);
	function("gmnGETail", 	&gmnGETail);
	function("gmnGSeq", 	&gmnGSeq);
	function("gmnGPar", 	&gmnGPar);
	function("gmnGRPar", 	&gmnGRPar);
	function("gmnGMirror", 	&gmnGMirror);
	function("gmnGSetDuration", &gmnGSetDuration);
	function("gmnApplyRythm", 	&gmnApplyRythm);
	function("gmnApplyPitch", 	&gmnApplyPitch);
	function("gmnVSetDuration", &gmnVSetDuration);
	function("gmnVMultDuration",&gmnVMultDuration);
	
	function("gmncheck", 		&gmncheck);
	function("gmnDuration", 	&gmnDuration);
	function("gmnEv2Time",		&gmnEv2Time);

}
