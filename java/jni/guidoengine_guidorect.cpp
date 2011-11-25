/*
	GUIDO Library - JNI Interface
	Copyright (C) 2010 Sony CSL Paris
	All rights reserved.
	Author D. Fober

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

#include "guidoengine_guidorect.h"
#include "javaIDs.h"

jfieldID gRectTopID, gRectLeftID, gRectRightID, gRectBottomID;

/*
 * Class:     guidoengine_guidoscoremap
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidorect_Init (JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gRectTopID, "top", "I")) return;
	if (!getID (env, cls, gRectLeftID, "left", "I")) return;
	if (!getID (env, cls, gRectRightID, "right", "I")) return;
	if (!getID (env, cls, gRectBottomID, "bottom", "I")) return;
}

