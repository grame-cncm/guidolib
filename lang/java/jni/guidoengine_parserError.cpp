/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "guidoengine_parserError.h"
#include "javaIDs.h"

jfieldID gCol;
jfieldID gLigne;
jfieldID gMessage;

/*
 * Class:     guidoengine_parserError
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_parserError_Init(JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gCol, "col", "I")) return;
	if (!getID (env, cls, gLigne, "ligne", "I")) return;
	if (!getID (env, cls, gMessage, "message", "Ljava/lang/String;")) return;
}
