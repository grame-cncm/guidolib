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

#include "guidoengine_guidosegment.h"
#include "javaIDs.h"
#include "GUIDOEngine.h"

jfieldID gStartID, gEndID;

// imported from guidoengine_guidoscore.cpp
void setDate (JNIEnv * env, jobject jdate, const GuidoDate& date);

// --------------------------------------------------------------------------------------------
void setSegment (JNIEnv * env, jobject segm, const GuidoDate& start, const GuidoDate& end)
{
	jobject ostart = env->GetObjectField (segm, gStartID);
	jobject oend   = env->GetObjectField (segm, gEndID);
	setDate (env, ostart, start);
	setDate (env, oend, end);
	env->SetObjectField (segm, gStartID, ostart);
	env->SetObjectField (segm, gEndID, oend);
}

/*
 * Class:     guidoengine_guidosegment
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidosegment_Init (JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gStartID, "start", "Lguidoengine/guidodate;")) return;
	if (!getID (env, cls, gEndID, "end", "Lguidoengine/guidodate;")) return;
}
