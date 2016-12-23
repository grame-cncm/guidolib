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


#include "guidoengine_guidofactory.h"
#include "javaIDs.h"

#define __UseOldGUIDOFactory__
#include "GUIDOFactory.h"

// --------------------------------------------------------------------------------------------
// fields IDs declarations
// --------------------------------------------------------------------------------------------
// the factory handler ID
static jfieldID gFactoryHandlerID;

/*
 * Class:     guidoengine_guidofactory
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidofactory_Init (JNIEnv * env, jclass cls)
{
	getID (env, cls, gFactoryHandlerID, "fFactoryHandler", "J");
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    Open
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_Open (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh;
	int errcode = GuidoFactoryOpen(&fh);
	if (errcode == guidoNoErr) 
		env->SetLongField (obj, gFactoryHandlerID, (long)fh);
	return errcode;
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    Close
 * Signature: ()I
 */
JNIEXPORT void JNICALL Java_guidoengine_guidofactory_Close (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	if (fh) GuidoFactoryClose (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    OpenMusic
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_OpenMusic (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryOpenMusic (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    CloseMusic
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_guidoengine_guidofactory_CloseMusic (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return jlong(GuidoFactoryCloseMusic (fh));
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    OpenVoice
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_OpenVoice (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryOpenVoice (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    CloseVoice
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_CloseVoice (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryCloseVoice (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    OpenChord
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_OpenChord (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryOpenChord (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    CloseChord
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_CloseChord (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryCloseChord (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    InsertCommata
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_InsertCommata (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryInsertCommata (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    OpenEvent
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_OpenEvent (JNIEnv * env, jobject obj, jstring jstr)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	const char *str  = env->GetStringUTFChars(jstr, JNI_FALSE);
	int errcode = GuidoFactoryOpenEvent (fh, str);
	env->ReleaseStringUTFChars(jstr, str);
	return errcode;
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    CloseEvent
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_CloseEvent (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryCloseEvent (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    AddSharp
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_AddSharp (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryAddSharp (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    AddFlat
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_AddFlat (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryAddFlat (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    SetEventDots
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_SetEventDots (JNIEnv * env, jobject obj, jint ndots)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactorySetEventDots (fh, ndots);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    SetEventAccidentals
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_SetEventAccidentals (JNIEnv * env, jobject obj, jint accident)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactorySetEventAccidentals (fh, accident);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    SetOctave
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_SetOctave (JNIEnv * env, jobject obj, jint octave)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactorySetOctave (fh, octave);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    SetDuration
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_SetDuration (JNIEnv * env, jobject obj, jint n, jint d)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactorySetDuration (fh, n, d);
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    OpenTag
 * Signature: (Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_OpenTag (JNIEnv * env, jobject obj, jstring jstr, jlong id)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	const char *str  = env->GetStringUTFChars(jstr, JNI_FALSE);
	jint result = GuidoFactoryOpenTag (fh, str, long(id));
	env->ReleaseStringUTFChars(jstr, str);
	return result;
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    OpenRangeTag
 * Signature: (Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_OpenRangeTag
  (JNIEnv * env, jobject obj, jstring jstr, jlong id)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	const char *str  = env->GetStringUTFChars(jstr, JNI_FALSE);
	jint result = GuidoFactoryOpenRangeTag (fh, str, long(id));
	env->ReleaseStringUTFChars(jstr, str);
	return result;
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    IsRangeTag
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_IsRangeTag (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryIsRangeTag (fh);
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    EndTag
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_EndTag (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryEndTag (fh);
}

/*
 * Class:     guidoengine_guidofactory
 * Method:    CloseTag
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_CloseTag (JNIEnv * env, jobject obj)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryCloseTag (fh);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    AddTagParameterString
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_AddTagParameterString (JNIEnv * env, jobject obj, jstring jstr)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	const char *str  = env->GetStringUTFChars(jstr, JNI_FALSE);
	int errcode = GuidoFactoryAddTagParameterString (fh, str);
	env->ReleaseStringUTFChars(jstr, str);
	return errcode;
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    AddTagParameterInt
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_AddTagParameterInt (JNIEnv * env, jobject obj, jint val)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryAddTagParameterInt (fh, val);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    AddTagParameterFloat
 * Signature: (D)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_AddTagParameterFloat (JNIEnv * env, jobject obj, jdouble val)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	return GuidoFactoryAddTagParameterFloat (fh, val);
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    SetParameterName
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_SetParameterName (JNIEnv * env, jobject obj, jstring jstr)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	const char *str  = env->GetStringUTFChars(jstr, JNI_FALSE);
	int errcode = GuidoFactorySetParameterName (fh, str);
	env->ReleaseStringUTFChars(jstr, str);
	return errcode;
}


/*
 * Class:     guidoengine_guidofactory
 * Method:    SetParameterUnit
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidofactory_SetParameterUnit (JNIEnv * env, jobject obj, jstring jstr)
{
	ARFactoryHandler fh = (ARFactoryHandler)env->GetLongField (obj, gFactoryHandlerID);
	const char *str  = env->GetStringUTFChars(jstr, JNI_FALSE);
	int errcode = GuidoFactorySetParameterUnit (fh, str);
	env->ReleaseStringUTFChars(jstr, str);
	return errcode;
}

