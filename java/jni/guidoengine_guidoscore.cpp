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

#include "guidoengine_guidoscore.h"
#include "guidoengine_bitmap_paint.h"

#define MIDIEXPORT
#include "GUIDOEngine.h"
#include "GUIDO2Midi.h"
#include "map_collectors.h"
#include "javaIDs.h"

#include <sstream>

// --------------------------------------------------------------------------------------------
// fields IDs declarations
// --------------------------------------------------------------------------------------------
// the score handlers ID
static jfieldID gARHandlerID, gGRHandlerID;
static jmethodID getRGBID;

// the guidodate IDs
extern jfieldID gDateNumID, gDateDenumID;

// the guidolayout IDs
extern jfieldID gSystemsDistanceID, gSystemsDistributionID, gOptimalPageFillID;
extern jfieldID gSystemsDistribLimitID, gForceID, gSpringID, gNeighborhoodSpacingID;

// the guidopageformat IDs
extern jfieldID gWidthID, gHeightID, gMLeftID, gMRightID, gMTopID, gMBottomID;

// the guidodrawdesc IDs
extern jfieldID gPageID, gscrollxID, gscrollyID, gsizexID, gsizeyID, gisprintID;

// the guidopaint IDs
extern jfieldID gEraseID, gLeftID, gRightID, gTopID, gBottomID;

// the guido2midiparams IDs
extern jfieldID gTempoID, gTicksID, gChanID;
extern jfieldID gIntensityID, gAccentFactorID, gMarcatoFactorID;
extern jfieldID gDFactorID, gStaccatoFactorID, gSlurFactorID, gTenutoFactorID, gFermataFactorID;
extern jfieldID gVoice2ChanID;

// --------------------------------------------------------------------------------------------
// methods imported from guidoengine_guidoscoremap.cpp
Time2GraphicMap* getMap (JNIEnv * env, jobject obj);


// --------------------------------------------------------------------------------------------
// methods to convert java objects to/from Guido structs
// --------------------------------------------------------------------------------------------
void setDate (JNIEnv * env, jobject jdate, const GuidoDate& date)
{
	env->SetIntField (jdate, gDateNumID, date.num);
	env->SetIntField (jdate, gDateDenumID, date.denom);
}

// --------------------------------------------------------------------------------------------
GuidoDate getDate (JNIEnv * env, jobject jdate)
{
	GuidoDate date;
	date.num	= env->GetIntField (jdate, gDateNumID);
	date.denom	= env->GetIntField (jdate, gDateDenumID);
	return date;
}

// --------------------------------------------------------------------------------------------
static void setPageFormat (JNIEnv * env, jobject jformat, const GuidoPageFormat& format)
{
	env->SetFloatField (jformat, gWidthID, format.width);
	env->SetFloatField (jformat, gHeightID, format.height);
	env->SetFloatField (jformat, gMLeftID, format.marginleft);
	env->SetFloatField (jformat, gMRightID, format.marginright);
	env->SetFloatField (jformat, gMTopID, format.margintop);
	env->SetFloatField (jformat, gMBottomID, format.marginbottom);
}

// --------------------------------------------------------------------------------------------
static GuidoLayoutSettings getLayout (JNIEnv * env, jobject jlayout)
{
	GuidoLayoutSettings layout;
	layout.systemsDistance		= env->GetFloatField (jlayout, gSystemsDistanceID);
	layout.systemsDistribution	= env->GetIntField (jlayout, gSystemsDistributionID);
	layout.systemsDistribLimit	= env->GetFloatField (jlayout, gSystemsDistribLimitID);
	layout.force				= env->GetFloatField (jlayout, gForceID);
	layout.spring				= env->GetFloatField (jlayout, gSpringID);
	layout.neighborhoodSpacing	= env->GetIntField (jlayout, gNeighborhoodSpacingID);
	layout.optimalPageFill		= env->GetBooleanField (jlayout, gOptimalPageFillID);
	return layout;
}

// --------------------------------------------------------------------------------------------
static GPaintStruct getPaintStruct (JNIEnv * env, jobject jpaint)
{
	GPaintStruct paint;
 	paint.erase		= env->GetBooleanField (jpaint, gEraseID) != 0;
 	paint.left		= env->GetIntField (jpaint, gLeftID);
 	paint.top		= env->GetIntField (jpaint, gTopID);
 	paint.right		= env->GetIntField (jpaint, gRightID);
 	paint.bottom	= env->GetIntField (jpaint, gBottomID);
	return paint;
}


// --------------------------------------------------------------------------------------------
// native methods implementation
// --------------------------------------------------------------------------------------------

/*
 * Class:     guidoengine_guidoscore
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscore_Init (JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gARHandlerID, "fARHandler", "J")) return;
	if (!getID (env, cls, gGRHandlerID, "fGRHandler", "J")) return;

	jclass 	colorClass = env->FindClass("java/awt/Color");
	if (colorClass == NULL) 
 		fprintf(stderr, "Java_guidoengine_guidoscore_Init got NULL color class\n");
	else {
		getRGBID = env->GetMethodID (colorClass, "getRGB", "()I");
		if (getRGBID == NULL)
			fprintf(stderr, "JavaTimeMapCollector::Time2TimeMap got NULL jmethodID\n");
		env->DeleteLocalRef(colorClass);		
	}
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    ParseFile
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_ParseFile (JNIEnv *env, jobject obj, jstring file)
{
	ARHandler ar;
	const char *str  = env->GetStringUTFChars(file, JNI_FALSE);

	int errcode = GuidoParseFile (str, &ar);
	if (errcode == guidoNoErr) {
		env->SetLongField (obj, gARHandlerID, (long)ar);
		env->SetLongField (obj, gGRHandlerID, 0);
	}
	env->ReleaseStringUTFChars(file, str);
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    ParseString
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_ParseString (JNIEnv * env, jobject obj, jstring gmn)
{
	ARHandler ar;
	const char *str  = env->GetStringUTFChars(gmn, JNI_FALSE);

	int errcode = GuidoParseString (str, &ar);
	if (errcode == guidoNoErr) {
		env->SetLongField (obj, gARHandlerID, (long)ar);
		env->SetLongField (obj, gGRHandlerID, 0);
	}
	env->ReleaseStringUTFChars(gmn, str);
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    AR2GR
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_AR2GR__  (JNIEnv * env, jobject obj)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	GRHandler gr;
	int errcode = GuidoAR2GR (ar, 0, &gr);
	if (errcode == guidoNoErr) {
		env->SetLongField (obj, gGRHandlerID, (long)gr);
	}
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    AR2GR
 * Signature: (Lguidoengine/guidolayout;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_AR2GR__Lguidoengine_guidolayout_2 (JNIEnv * env, jobject obj, jobject jlayout)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	GRHandler gr;
	GuidoLayoutSettings settings = getLayout (env, jlayout);
	int errcode = GuidoAR2GR (ar, &settings, &gr);
	if (errcode == guidoNoErr) {
		env->SetLongField (obj, gGRHandlerID, (long)gr);
	}
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    AR2MIDIFile
 * Signature: (Ljava/lang/String;Lguidoengine/guido2midiparams;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_AR2MIDIFile (JNIEnv * env, jobject obj, jstring file, jobject jparams)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	GuidoErrCode err = guidoErrInvalidHandle;
	if (ar) {
		Guido2MidiParams p;
		p.fTempo = env->GetIntField (jparams, gTempoID);
		p.fTicks = env->GetIntField (jparams, gTicksID);
		p.fChan  = env->GetIntField (jparams, gChanID);

		p.fIntensity	 = env->GetFloatField (jparams, gIntensityID);
		p.fAccentFactor  = env->GetFloatField (jparams, gAccentFactorID);
		p.fMarcatoFactor = env->GetFloatField (jparams, gMarcatoFactorID);

		p.fDFactor			= env->GetFloatField (jparams, gDFactorID);
		p.fStaccatoFactor	= env->GetFloatField (jparams, gStaccatoFactorID);
		p.fSlurFactor		= env->GetFloatField (jparams, gSlurFactorID);
		p.fTenutoFactor		= env->GetFloatField (jparams, gTenutoFactorID);
		p.fFermataFactor	= env->GetFloatField (jparams, gFermataFactorID);
		
		jintArray arr = (jintArray)env->GetObjectField(jparams, gVoice2ChanID);
		for (int i=0; i<128; i++) {
			jint val;
			env->GetIntArrayRegion (arr, i, 1, &val);
			if (val) p.fVChans[i] = val;
		}
		const char *str  = env->GetStringUTFChars(file, JNI_FALSE);
		err = GuidoAR2MIDIFile (ar, str, &p);
		env->ReleaseStringUTFChars(file, str);
	}
	return err;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    SVGExport
 * Signature: (ILjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_guidoengine_guidoscore_SVGExport (JNIEnv * env, jobject obj, jint page, jstring font)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	std::stringstream sstr;
	GuidoErrCode err = guidoErrInvalidHandle;
	if (gr) {
		const char *guidofont  = env->GetStringUTFChars(font, JNI_FALSE);
		err = GuidoSVGExport(gr, page, sstr, *guidofont ? guidofont : 0);
		env->ReleaseStringUTFChars(font, guidofont);
		if (err == guidoNoErr)
			return env->NewStringUTF(sstr.str().c_str());
	}
	return env->NewStringUTF(GuidoGetErrorString(err));
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    UpdateGR
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_UpdateGR__ (JNIEnv * env, jobject obj)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	return GuidoUpdateGR (gr, 0);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    UpdateGR
 * Signature: (Lguidoengine/guidolayout;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_UpdateGR__Lguidoengine_guidolayout_2 (JNIEnv *env, jobject obj, jobject jlayout)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoLayoutSettings settings = getLayout (env, jlayout);
	return GuidoUpdateGR (gr, &settings);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    ResizePageToMusic
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_ResizePageToMusic (JNIEnv * env, jobject obj)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	return GuidoResizePageToMusic (gr);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    FreeAR
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscore_FreeAR (JNIEnv * env, jobject obj)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	if (ar) {
		GuidoFreeAR (ar);
		env->SetLongField (obj, gARHandlerID, 0);
	}
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    FreeGR
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscore_FreeGR (JNIEnv * env, jobject obj)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	if (gr) {
		GuidoFreeGR (gr);
		env->SetLongField (obj, gGRHandlerID, 0);
	}
}

static VGColor jcolor2VGColor (JNIEnv * env, jobject jcolor)
{
	int color = env->CallIntMethod (jcolor, getRGBID);
	unsigned char r, g, b, a;
	b = (unsigned char)(color & 0xff);
	g = (unsigned char)((color & 0xff00) >> 8);
	r = (unsigned char)((color & 0xff0000) >> 16);
	a = (unsigned char)((color & 0xff000000) >> 24);
#ifdef WIN32
	return VGColor (r, g, b, a);
#else
	return VGColor (b, g, r, a);
#endif
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    getBitmap
 * Signature: ([ILguidoengine/guidodrawdesc;Lguidoengine/guidopaint;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetBitmap (JNIEnv * env, jobject obj, jintArray bitmapArray, jint w, jint h, jobject jdesc, jobject jpaint, jobject jcolor)
{
	GuidoOnDrawDesc desc;
	desc.handle		= (GRHandler)env->GetLongField (obj, gGRHandlerID);
	desc.page		= env->GetIntField (jdesc, gPageID);
	desc.scrollx	= env->GetIntField (jdesc, gscrollxID);
	desc.scrolly	= env->GetIntField (jdesc, gscrollyID);
	desc.sizex		= env->GetIntField (jdesc, gsizexID);
	desc.sizey		= env->GetIntField (jdesc, gsizeyID);
	desc.isprint	= env->GetBooleanField (jdesc, gisprintID);
	desc.updateRegion = getPaintStruct (env, jpaint);

	jint *dstBitmap = env->GetIntArrayElements(bitmapArray, 0);
	if (dstBitmap == 0) return guidoErrInvalidHandle;

	VGColor color(0,0,0);
	int result = getBitmap (dstBitmap, w, h, desc, jcolor2VGColor(env,jcolor));
	env->ReleaseIntArrayElements(bitmapArray, dstBitmap, 0);
	return result;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    DrawBoundingBoxes
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscore_DrawBoundingBoxes (JNIEnv *, jobject, jint bbmap)
{
	GuidoDrawBoundingBoxes (bbmap);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetDrawBoundingBoxes
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetDrawBoundingBoxes (JNIEnv *, jobject)
{
	return GuidoGetDrawBoundingBoxes();
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetPageFormat
 * Signature: (ILguidoengine/guidopageformat;)V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscore_GetPageFormat (JNIEnv * env, jobject obj, jint pagenum, jobject jformat)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoPageFormat format;
	GuidoGetPageFormat(	gr, pagenum, &format );
	setPageFormat (env, jformat, format);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    MarkVoice
 * Signature: (ILguidoengine/guidodate;Lguidoengine/guidodate;III)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_MarkVoice (JNIEnv * env, jobject obj, jint voicenum, 
	jobject jdate, jobject jduration, jint red, jint green, jint blue)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	GuidoDate date = getDate(env, jdate);
	GuidoDate duration = getDate(env, jduration);
	int errcode = GuidoMarkVoice( ar, voicenum, date, duration, (unsigned char)red, (unsigned char)green, (unsigned char)blue );
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetPageCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetPageCount (JNIEnv * env, jobject obj)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	return GuidoGetPageCount( gr );
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    CountVoices
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_CountVoices (JNIEnv * env, jobject obj)
{
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	return GuidoCountVoices (ar);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetDuration
 * Signature: (Lguidoengine/guidodate;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetDuration (JNIEnv * env, jobject obj, jobject jduration)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoDate date;
	int errcode = GuidoDuration( gr, &date );
	if (errcode == guidoNoErr)
		setDate (env, jduration, date);
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetPageDate
 * Signature: (ILguidoengine/guidodate;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetPageDate (JNIEnv * env, jobject obj, jint pagenum, jobject jdate)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoDate date;
	int errcode = GuidoGetPageDate( gr, pagenum, &date);
	if (errcode == guidoNoErr)
		setDate (env, jdate, date);
	return errcode;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    FindEventPage
 * Signature: (Lguidoengine/guidodate;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_FindEventPage (JNIEnv * env, jobject obj, jobject jdate)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoDate date = getDate (env, jdate);
	return GuidoFindEventPage(gr, date );
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    FindPageAt
 * Signature: (Lguidoengine/guidodate;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_FindPageAt (JNIEnv * env, jobject obj, jobject jdate)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoDate date = getDate (env, jdate);
	return GuidoFindPageAt( gr, date );
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetMap
 * Signature: (IFFLguidoengine/guidoscoremap/selector;Lguidoengine/mapcollector;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetMap (JNIEnv * env, jobject obj, jint pagenum, 
		jfloat width, jfloat height, jint jsel, jobject jcollector)
{
	JavaMapCollector collector (env, jcollector);
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	GuidoeElementSelector sel = GuidoeElementSelector(jsel);
	return GuidoGetMap( gr, pagenum, width, height, sel, collector);
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetTimeMap
 * Signature: (Lguidoengine/timemapcollector;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetTimeMap (JNIEnv * env, jobject obj, jobject jcollector)
{
	JavaTimeMapCollector collector (env, jcollector);
	ARHandler ar = (ARHandler)env->GetLongField (obj, gARHandlerID);
	return GuidoGetTimeMap ( ar, collector);
}


// -------------------------------------------------------------------------
// adjusted mappings methods - new in guido lib 1.47 and in guido java 1.20
// -------------------------------------------------------------------------
/*
 * Class:     guidoengine_guidoscore
 * Method:    GetPageMap
 * Signature: (IFFLguidoengine/guidoscoremap;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetPageMap (JNIEnv * env, jobject obj, jint page, jfloat w, jfloat h, jobject map)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	Time2GraphicMap * mapPtr = getMap(env, map);
	return mapPtr ? GuidoGetPageMap( gr, page, w, h, *mapPtr) : guidoErrActionFailed;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetStaffMap
 * Signature: (IFFILguidoengine/guidoscoremap;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetStaffMap (JNIEnv * env, jobject obj, jint page, jfloat w, jfloat h, jint staff, jobject map)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	Time2GraphicMap * mapPtr = getMap(env, map);
	return mapPtr ? GuidoGetStaffMap( gr, page, w, h, staff, *mapPtr) : guidoErrActionFailed;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetVoiceMap
 * Signature: (IFFILguidoengine/guidoscoremap;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetVoiceMap (JNIEnv * env, jobject obj, jint page, jfloat w, jfloat h, jint voice, jobject map)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	Time2GraphicMap * mapPtr = getMap(env, map);
	return mapPtr ? GuidoGetVoiceMap( gr, page, w, h, voice, *mapPtr) : guidoErrActionFailed;
}

/*
 * Class:     guidoengine_guidoscore
 * Method:    GetSystemMap
 * Signature: (IFFLguidoengine/guidoscoremap;)I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscore_GetSystemMap (JNIEnv * env, jobject obj, jint page, jfloat w, jfloat h, jobject map)
{
	GRHandler gr = (GRHandler)env->GetLongField (obj, gGRHandlerID);
	Time2GraphicMap * mapPtr = getMap(env, map);
	return mapPtr ? GuidoGetSystemMap( gr, page, w, h, *mapPtr) : guidoErrActionFailed;
}


