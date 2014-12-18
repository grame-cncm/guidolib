/*
  GUIDO Library
  Copyright (C) 2014  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "guidoengine_guidopianoroll.h"
#include "guidoengine_bitmap_paint.h"

#include "GUIDOPianoRoll.h"
#include "javaIDs.h"
#include "VGColor.h"

// Abstract Handler of guidopianoroll java class
static jfieldID gARHandlerPianoRollID;
// Pianoroll reference of guidopianoroll java class
static jfieldID gPianoRollID;

extern jfieldID gStartDate;
extern jfieldID gEndDate;
extern jfieldID gLowPitch;
extern jfieldID gHighPitch;

// imported from guidoengine_guidoscore.cpp
GuidoDate getDate (JNIEnv * env, jobject jdate);
// imported from guidoengine_guidoscoremap.cpp
Time2GraphicMap* getMap (JNIEnv * env, jobject obj);
// imported from guidoengine_guidoscore.cpp
VGColor jcolor2VGColor (JNIEnv * env, jobject jcolor);

LimitParams getLimitParams(JNIEnv * env, jobject limitParams) {
	LimitParams params;
	params.lowPitch = env->GetIntField(limitParams, gLowPitch);
	params.highPitch = env->GetIntField(limitParams, gHighPitch);

	jobject startDate = env->GetObjectField(limitParams, gStartDate);
	params.startDate = getDate(env, startDate);

	jobject endDate = env->GetObjectField(limitParams, gEndDate);
	params.endDate = getDate(env, endDate);
	return params;
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    AR2PianoRoll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidopianoroll_AR2PianoRoll(JNIEnv *env, jobject obj, jint type)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerPianoRollID);
	PianoRollType enumtype = kSimplePianoRoll;
	if(type != 0) {
		enumtype = kTrajectoryPianoRoll;
	}
	PianoRoll * pr = GuidoAR2PianoRoll(enumtype, ar);
	env->SetLongField(obj, gPianoRollID, (long)pr);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    Midi2PianoRoll
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidopianoroll_Midi2PianoRoll(JNIEnv *env, jobject obj, jint type, jstring filename)
{
	PianoRollType enumtype = kSimplePianoRoll;
	if(type != 0) {
		enumtype = kTrajectoryPianoRoll;
	}

	PianoRoll * pr = GuidoMidi2PianoRoll(enumtype, env->GetStringUTFChars(filename, JNI_FALSE));
	env->SetLongField (obj, gPianoRollID, (long)pr);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    DestroyPianoRoll
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_DestroyPianoRoll(JNIEnv * env, jobject obj)
{
	return GuidoDestroyPianoRoll((PianoRoll *)env->GetLongField(obj, gPianoRollID));
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    SetLimits
 * Signature: (Lguidoengine/limitparams;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_SetLimits(JNIEnv *env, jobject obj, jobject limitparams)
{
	return GuidoPianoRollSetLimits(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				getLimitParams(env, limitparams));
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    EnableKeyboard
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_EnableKeyboard(JNIEnv *env, jobject obj, jboolean enabled)
{
	return GuidoPianoRollEnableKeyboard(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				enabled);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    GetKeyboardWidth
 * Signature: (IF)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_GetKeyboardWidth
	(JNIEnv *env, jobject obj, jint height, jfloat keyboardWidth)
{
	return GuidoPianoRollGetKeyboardWidth(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				height,
				keyboardWidth);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    EnableAutoVoicesColoration
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_EnableAutoVoicesColoration
	(JNIEnv *env, jobject obj, jboolean enabled)
{
	return GuidoPianoRollEnableAutoVoicesColoration(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				enabled);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    SetRGBColorToVoice
 * Signature: (IIIII)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_SetRGBColorToVoice
	(JNIEnv *env, jobject obj, jint voiceNum, jint r, jint g, jint b, jint a)
{
	return GuidoPianoRollSetRGBColorToVoice(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				voiceNum,
				r, g, b, a);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    SetHtmlColorToVoice
 * Signature: (IJ)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_SetHtmlColorToVoice
	(JNIEnv *env, jobject obj, jint voiceNum, jlong htmlColor)
{
	return GuidoPianoRollSetHtmlColorToVoice(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				voiceNum,
				htmlColor);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    EnableMeasureBars
 * Signature: (Z)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_EnableMeasureBars
  (JNIEnv *env, jobject obj, jboolean enabled)
{
	return GuidoPianoRollEnableMeasureBars(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				enabled);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    SetPitchLinesDisplayMode
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_SetPitchLinesDisplayMode
  (JNIEnv *env, jobject obj, jint mode)
{
	GuidoPianoRollSetPitchLinesDisplayMode(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				mode);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    GetMap
 * Signature: (IILguidoengine/guidoscoremap;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_GetMap
  (JNIEnv *env, jobject obj, jint width, jint height, jobject timemap)
{
	Time2GraphicMap * mapPtr = getMap(env, timemap);

	return GuidoPianoRollGetMap(
				(PianoRoll *)env->GetLongField(obj, gPianoRollID),
				width,
				height,
				*mapPtr);
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    GetBitmap
 * Signature: ([IIILguidoengine/guidopaint;Ljava/awt/Color;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidopianoroll_GetBitmap
  (JNIEnv *env, jobject obj, jintArray bitmapArray, jint w, jint h, jobject jpaint, jobject jcolor)
{
	jint *dstBitmap = env->GetIntArrayElements(bitmapArray, 0);
	if (dstBitmap == 0) return guidoErrInvalidHandle;
	PianoRoll * pr = (PianoRoll *)env->GetLongField(obj, gPianoRollID);
	int result = getBitmapPianoRoll (dstBitmap, w, h, pr, jcolor2VGColor(env,jcolor));
	env->ReleaseIntArrayElements(bitmapArray, dstBitmap, 0);
	return result;
}

/*
 * Class:     guidoengine_guidopianoroll
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidopianoroll_Init(JNIEnv *env , jclass cls)
{
	if (!getID (env, cls, gPianoRollID, "fPianoRoll", "J")) return;
	if (!getID (env, cls, gARHandlerPianoRollID, "fARHandler", "J")) return;
}

