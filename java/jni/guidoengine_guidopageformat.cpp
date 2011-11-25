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

#include <stdio.h>
#include "guidoengine_guidopageformat.h"
#include "GUIDOEngine.h"
#include "javaIDs.h"


jfieldID gWidthID, gHeightID;
jfieldID gMLeftID, gMRightID, gMTopID, gMBottomID;


/*
 * Class:     guidoengine_guidopageformat
 * Method:    GetDefault
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidopageformat_GetDefault (JNIEnv * env, jobject obj)
{
	GuidoPageFormat format;
	GuidoGetDefaultPageFormat (&format);
	env->SetFloatField (obj, gWidthID, format.width);
	env->SetFloatField (obj, gHeightID, format.height);
	env->SetFloatField (obj, gMLeftID, format.marginleft);
	env->SetFloatField (obj, gMRightID, format.marginright);
	env->SetFloatField (obj, gMTopID, format.margintop);
	env->SetFloatField (obj, gMBottomID, format.marginbottom);	
}

/*
 * Class:     guidoengine_guidopageformat
 * Method:    SetDefault
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidopageformat_SetDefault (JNIEnv * env, jobject obj)
{
	GuidoPageFormat format;
	format.width		= env->GetFloatField (obj, gWidthID);
	format.height		= env->GetFloatField (obj, gHeightID);
	format.marginleft	= env->GetFloatField (obj, gMLeftID);
	format.marginright	= env->GetFloatField (obj, gMRightID);
	format.margintop	= env->GetFloatField (obj, gMTopID);
	format.marginbottom	= env->GetFloatField (obj, gMBottomID);	
	GuidoSetDefaultPageFormat(&format);
}

/*
 * Class:     guidoengine_guidopageformat
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidopageformat_Init (JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gWidthID, "fWidth", "F")) return;
	if (!getID (env, cls, gHeightID, "fHeight", "F")) return;
	if (!getID (env, cls, gMLeftID, "fMarginleft", "F")) return;
	if (!getID (env, cls, gMRightID, "fMarginright", "F")) return;
	if (!getID (env, cls, gMTopID, "fMargintop", "F")) return;
	if (!getID (env, cls, gMBottomID, "fMarginbottom", "F")) return;
}

