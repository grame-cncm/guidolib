/*
	GUIDO Library - JNI Interface
	Copyright (C) 2012 Grame
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


#include "guidoengine_guidoscoremapbase.h"
#include "javaIDs.h"
#include "GUIDOScoreMap.h"

// imported from guidoengine_guidoscore.cpp
GuidoDate getDate (JNIEnv * env, jobject jdate);
// imported from guidoengine_guidosegment.cpp
void setSegment (JNIEnv * env, jobject segm, const GuidoDate& start, const GuidoDate& end);

// --------------------------------------------------------------------------------------------
// fields IDs declarations
// --------------------------------------------------------------------------------------------
// the score handlers ID
static jfieldID gMapID;

// --------------------------------------------------------------------------------------------
Time2GraphicMap* getMap (JNIEnv * env, jobject obj)
{
	Time2GraphicMap* map = (Time2GraphicMap*)env->GetLongField (obj, gMapID);
	if (!map) {
		map = new Time2GraphicMap;
		env->SetLongField (obj, gMapID, (long)map);
	}
	if (map) map->clear();
	else fprintf (stderr, "can't get a Time2GraphicMap pointer!\n");
	return map;
}

// --------------------------------------------------------------------------------------------
static void setRect (JNIEnv * env, jobject rect, const FloatRect& r)
{
	jclass cls = env->GetObjectClass(rect);
	jmethodID setID = env->GetMethodID(cls, "setRect", "(DDDD)V");
	if (!setID) fprintf (stderr, "can't get Rectangle setRect method\n");
	else env->CallVoidMethod (rect, setID, r.left, r.top, r.Width(), r.Height());
}

/*
 * Class:     guidoengine_guidoscoremapbase
 * Method:    disposeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscoremapbase_disposeNative (JNIEnv * env, jobject obj)
{
	Time2GraphicMap* map = (Time2GraphicMap*)env->GetLongField (obj, gMapID);
	delete map;
}

/*
 * Class:     guidoengine_guidoscoremapbase
 * Method:    size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_guidoengine_guidoscoremapbase_size (JNIEnv * env, jobject obj)
{
	Time2GraphicMap* map = (Time2GraphicMap*)env->GetLongField (obj, gMapID);
	return map ? (jint)map->size() : 0;
}


/*
 * Class:     guidoengine_guidoscoremapbase
 * Method:    get
 * Signature: (ILguidoengine/guidosegment;Lguidoengine/guidorectangle;)Z
 */
JNIEXPORT jboolean JNICALL Java_guidoengine_guidoscoremapbase_get (JNIEnv * env, jobject obj, jint index, jobject timeseg, jobject rect)
{
	Time2GraphicMap* map = (Time2GraphicMap*)env->GetLongField (obj, gMapID);
	int size = (int)map->size();
	if (map && (index >= 0) && (index < size)) {
		Time2GraphicMap::const_iterator i = map->begin();
		while (index--) i++;
		TimeSegment t = i->first; 
		setSegment (env, timeseg, t.first, t.second);
		setRect (env, rect, i->second);
		return true;
	}
	return false;
}

/*
 * Class:     guidoengine_guidoscoremapbase
 * Method:    getTime
 * Signature: (Lguidoengine/guidodate;Lguidoengine/guidosegment;Lguidoengine/guidorectangle;)Z
 */
JNIEXPORT jboolean JNICALL Java_guidoengine_guidoscoremapbase_getTime (JNIEnv * env, jobject obj, jobject date, jobject timeseg, jobject rect)
{
	Time2GraphicMap* map = (Time2GraphicMap*)env->GetLongField (obj, gMapID);
	if (map) {
		TimeSegment t; FloatRect r;
		if (GuidoGetTime( getDate (env, date), *map, t, r)) {
			setSegment (env, timeseg, t.first, t.second);
			setRect (env, rect, r);
			return true;
		}
	}
	return false;
}

/*
 * Class:     guidoengine_guidoscoremapbase
 * Method:    getPoint
 * Signature: (FFLguidoengine/guidosegment;Lguidoengine/guidorectangle;)Z
 */
JNIEXPORT jboolean JNICALL Java_guidoengine_guidoscoremapbase_getPoint (JNIEnv * env, jobject obj, jfloat x, jfloat y, jobject timeseg, jobject rect)
{
	Time2GraphicMap* map = (Time2GraphicMap*)env->GetLongField (obj, gMapID);
	if (map) {
		TimeSegment t; FloatRect r;
		if (GuidoGetPoint( x, y, *map, t, r)) {
			setSegment (env, timeseg, t.first, t.second);
			setRect (env, rect, r);
			return true;
		}
	}
	return false;
}

/*
 * Class:     guidoengine_guidoscoremapbase
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidoscoremapbase_Init (JNIEnv * env, jclass cls)
{
	getID (env, cls, gMapID, "fMap", "J");
}
