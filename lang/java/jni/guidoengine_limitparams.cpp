/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "guidoengine_limitparams.h"

static jfieldID gStartDate;
static jfieldID gEndDate;
static jfieldID gLowPitch;
static jfieldID gHighPitch;

/*
 * Class:     guidoengine_limitparams
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_limitparams_Init(JNIEnv *env, jclass cls)
{
	if (!getID (env, cls, gStartDate, "startDate", "Lguidoengine/guidodate;")) return;
	if (!getID (env, cls, gEndDate, "endDate", "Lguidoengine/guidodate;")) return;
	if (!getID (env, cls, gLowPitch, "lowPitch", "I")) return;
	if (!getID (env, cls, gHighPitch, "highPitch", "I")) return;
}

