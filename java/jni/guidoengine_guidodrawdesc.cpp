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

#include "guidoengine_guidodrawdesc.h"
#include "javaIDs.h"

jfieldID gPageID, gscrollxID, gscrollyID;
jfieldID gsizexID, gsizeyID, gisprintID;

/*
 * Class:     guidoengine_guidodrawdesc
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidodrawdesc_Init (JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gPageID, "fPage", "I")) return;
	if (!getID (env, cls, gscrollxID, "fScrollx", "I")) return;
	if (!getID (env, cls, gscrollyID, "fScrolly", "I")) return;
	if (!getID (env, cls, gsizexID, "fWidth", "I")) return;
	if (!getID (env, cls, gsizeyID, "fHeight", "I")) return;
	if (!getID (env, cls, gisprintID, "fIsprint", "Z")) return;
}
