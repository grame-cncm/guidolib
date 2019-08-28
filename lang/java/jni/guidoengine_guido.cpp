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

#include "guidoengine_guido.h"
#include "GUIDOEngine.h"
#include "VGSystem.h"
#include "musicxml.h"
#include "device_specific_functions.h"

#if WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

jmethodID getRGBID;

VGSystem * gSystem;
bool gAntiAliasing = false;
bool gMusicXML = false;

static jint guidoengine_guido_Init (JNIEnv * env, jstring guidofont, jstring textfont, bool antialiasing)
{
	static bool done = false;
	if (!done) {
		GuidoInitDesc desc;
		desc.graphicDevice = device_specific::getInitDevice(antialiasing);
		desc.musicFont = env->GetStringUTFChars(guidofont, JNI_FALSE);
		desc.textFont  = env->GetStringUTFChars(textfont, JNI_FALSE);;
		GuidoErrCode errcode = GuidoInit (&desc);
		env->ReleaseStringUTFChars(guidofont, desc.musicFont);
		env->ReleaseStringUTFChars(textfont, desc.textFont);
		if (errcode != guidoNoErr) return errcode;
		done = true;

		gMusicXML = loadMusicxml();
	}

# ifndef android
	jclass 	colorClass = env->FindClass("java/awt/Color");
	if (colorClass == NULL)
		fprintf(stderr, "Java_guidoengine_guidoscore_Init got NULL color class\n");
	else {
		getRGBID = env->GetMethodID (colorClass, "getRGB", "()I");
		if (getRGBID == NULL)
			fprintf(stderr, "JavaTimeMapCollector::Time2TimeMap got NULL jmethodID\n");
		env->DeleteLocalRef(colorClass);
	}
# endif

	return guidoNoErr;
}

/*
 * Class:     guidoengine_guido
 * Method:    shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guido_shutdown(JNIEnv *, jclass)
{
	GuidoShutdown();
}

/*
 * Class:     guidoengine_guido
 * Method:    xml2gmn
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_guidoengine_guido_xml2gmn__
  (JNIEnv *, jclass)
{
	return gMusicXML;
}

/*
 * Class:     guidoengine_guido
 * Method:    msucixmlversion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guido_musicxmlversion
  (JNIEnv * env, jclass)
{
	return env->NewStringUTF(musicxmllibVersionStr());
}

/*
 * Class:     guidoengine_guido
 * Method:    msucixml2guidoversion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guido_musicxml2guidoversion
  (JNIEnv * env, jclass)
{
	return env->NewStringUTF(musicxml2guidoVersionStr());
}

/*
 * Class:     guidoengine_guido
 * Method:    xml2gmn
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guido_xml2gmn__Ljava_lang_String_2
  (JNIEnv * env, jclass, jstring file)
{
	const char *str  = env->GetStringUTFChars(file, JNI_FALSE);
	char * guidoCode = musicxml2guido ( str );
	jstring guido = env->NewStringUTF(guidoCode ? guidoCode : "");
	env->ReleaseStringUTFChars(file, str);
	delete guidoCode;
	return guido;
}

/*
 * Class:     guidoengine_guido
 * Method:    Init
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guido_Init
	(JNIEnv *env, jclass, jstring gfont, jstring tfont)
{
	return guidoengine_guido_Init (env, gfont, tfont, true);
}

/*
 * Class:     guidoengine_guido
 * Method:    GetErrorString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guido_GetErrorString (JNIEnv * env, jclass, jint errcode)
{
	const char* str = GuidoGetErrorString( (GuidoErrCode)errcode );
	return env->NewStringUTF(str);
}

/*
 * Class:     guidoengine_guido
 * Method:    GetParseErrorLine
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guido_GetParseErrorLine (JNIEnv *, jclass)
{
	return GuidoGetParseErrorLine();
}

/*
 * Class:     guidoengine_guido
 * Method:    Unit2CM
 * Signature: (F)F
 */
JNIEXPORT jfloat JNICALL Java_guidoengine_guido_Unit2CM (JNIEnv *, jclass, jfloat u)
{
	return GuidoUnit2CM (u);
}

/*
 * Class:     guidoengine_guido
 * Method:    CM2Unit
 * Signature: (F)F
 */
JNIEXPORT jfloat JNICALL Java_guidoengine_guido_CM2Unit (JNIEnv *, jclass, jfloat cm)
{
	return GuidoCM2Unit (cm);
}

/*
 * Class:     guidoengine_guido
 * Method:    Unit2Inches
 * Signature: (F)F
 */
JNIEXPORT jfloat JNICALL Java_guidoengine_guido_Unit2Inches (JNIEnv *, jclass, jfloat u)
{
	return GuidoUnit2Inches (u);
}

/*
 * Class:     guidoengine_guido
 * Method:    Inches2Unit
 * Signature: (F)F
 */
JNIEXPORT jfloat JNICALL Java_guidoengine_guido_Inches2Unit (JNIEnv *, jclass, jfloat inches)
{
	return GuidoInches2Unit(inches);
}

/*
 * Class:     guidoengine_guido
 * Method:    GetVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guido_GetVersion (JNIEnv * env, jclass)
{
	char buff[256];
	int major, minor, sub;
	GuidoGetVersionNums (&major, &minor, &sub);
	sprintf (buff, "%d.%d.%d", major, minor, sub);
	return env->NewStringUTF(buff);
}


/*
 * Class:     guidoengine_guido
 * Method:    GetJNIVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guido_GetJNIVersion (JNIEnv * env, jclass)
{
	return env->NewStringUTF("1.2.1");
}

/*
 * Class:     guidoengine_guido
 * Method:    CheckVersionNums
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guido_CheckVersionNums (JNIEnv *, jclass, jint major, jint minor, jint sub)
{
	return GuidoCheckVersionNums(major, minor, sub);
}

/*
 * Class:     guidoengine_guido
 * Method:    GetLineSpace
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL Java_guidoengine_guido_GetLineSpace (JNIEnv *, jclass)
{
	return GuidoGetLineSpace();
}
