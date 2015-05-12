/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "guidoengine_rectangle.h"
#include "javaIDs.h"

jfieldID gLeft;
jfieldID gTop;
jfieldID gRight;
jfieldID gBottom;

/*
 * Class:     guidoengine_rectangle
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_rectangle_Init(JNIEnv *env, jclass cls)
{
	if (!getID (env, cls, gLeft, "left", "I")) return;
	if (!getID (env, cls, gTop, "top", "I")) return;
	if (!getID (env, cls, gRight, "right", "I")) return;
	if (!getID (env, cls, gBottom, "bottom", "I")) return;
}

