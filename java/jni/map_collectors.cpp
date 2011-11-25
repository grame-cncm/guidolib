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


#include "map_collectors.h"

// --------------------------------------------------------------------------------------------
// JavaCollector
// --------------------------------------------------------------------------------------------
JavaCollector::JavaCollector (JNIEnv * env, jobject jcollector) : fEnv(env), fCollector(jcollector)
{
	fErrNotified = false;
	fClass = fEnv->GetObjectClass(fCollector);
	fGuidoDateClass = fEnv->FindClass("guidoengine/guidodate");
    if (fGuidoDateClass == NULL) 
 		fprintf(stderr, "JavaCollector::JavaCollector got NULL guidodate class\n");
	fGuidoSegmentClass = fEnv->FindClass("guidoengine/guidosegment");
    if (fGuidoSegmentClass == NULL) 
 		fprintf(stderr, "JavaCollector::JavaCollector got NULL guidosegment class\n");
}

// --------------------------------------------------------------------------------------------
JavaCollector::~JavaCollector ()
{
	if (fGuidoDateClass) fEnv->DeleteLocalRef(fGuidoDateClass);
	if (fGuidoSegmentClass) fEnv->DeleteLocalRef(fGuidoSegmentClass);
}


// --------------------------------------------------------------------------------------------
jobject JavaCollector::newDate (int num, int denum) const
{
	jmethodID constructor = fEnv->GetMethodID (fGuidoDateClass, "<init>", "(II)V");
	if (constructor == NULL) {
		fprintf(stderr, "JavaCollector::newDate got NULL constructor\n");
		return 0;
	}
	return fEnv->NewObject(fGuidoDateClass, constructor, num, denum); 
}

jobject JavaCollector::newSegment (const TimeSegment& segm) const
{
	jobject start = newDate(segm.first.num, segm.first.denom);
	if (!start) return 0;
	jobject end   = newDate(segm.second.num, segm.second.denom);
	if (!end) return 0;

	jmethodID constructor = fEnv->GetMethodID (fGuidoSegmentClass, "<init>", "(Lguidoengine/guidodate;Lguidoengine/guidodate;)V");
	if (constructor == NULL) {
		fprintf(stderr, "JavaCollector::newSegment got NULL constructor\n");
		return 0;
	}
	return fEnv->NewObject(fGuidoSegmentClass, constructor, start, end); 
}

// --------------------------------------------------------------------------------------------
// JavaTimeMapCollector
// --------------------------------------------------------------------------------------------
JavaTimeMapCollector::~JavaTimeMapCollector ()  {}

// --------------------------------------------------------------------------------------------
JavaTimeMapCollector::JavaTimeMapCollector (JNIEnv * env, jobject jcollector) : JavaCollector(env, jcollector) {}

void JavaTimeMapCollector::Time2TimeMap( const TimeSegment& from, const TimeSegment& to ) 
{
	jmethodID javaCallback = fEnv->GetMethodID (fClass, "Time2TimeMap", "(Lguidoengine/guidosegment;Lguidoengine/guidosegment;)V");
	if (javaCallback == NULL) {
		if (!fErrNotified) {
			fprintf(stderr, "JavaTimeMapCollector::Time2TimeMap got NULL jmethodID\n");
			fErrNotified = true;
		}
		return;
	}

	jobject start = newSegment(from);
	jobject end   = newSegment(to);
	if (start && end) 
		fEnv->CallVoidMethod (fCollector, javaCallback, start, end);
}


// --------------------------------------------------------------------------------------------
// JavaMapCollector
// --------------------------------------------------------------------------------------------
JavaMapCollector::~JavaMapCollector()
{
	if (fGuidoRectClass) fEnv->DeleteLocalRef(fGuidoRectClass);
	if (fGuidoEltInfoClass) fEnv->DeleteLocalRef(fGuidoEltInfoClass);
}

// --------------------------------------------------------------------------------------------
JavaMapCollector::JavaMapCollector(JNIEnv * env, jobject jcollector)  : JavaCollector(env, jcollector)
{
	fGuidoRectClass = fEnv->FindClass("guidoengine/guidorect");
    if (fGuidoRectClass == NULL) 
 		fprintf(stderr, "JavaTimeMapCollector::JavaTimeMapCollector got NULL guidorect class\n");
	fGuidoEltInfoClass = fEnv->FindClass("guidoengine/guidoelementinfo");
    if (fGuidoEltInfoClass == NULL) 
 		fprintf(stderr, "JavaTimeMapCollector::JavaTimeMapCollector got NULL guidoelementinfo class\n");
}

// --------------------------------------------------------------------------------------------
jobject JavaMapCollector::newRect (const FloatRect& r) const
{
	jmethodID constructor = fEnv->GetMethodID (fGuidoRectClass, "<init>", "(IIII)V");
	if (constructor == NULL) {
		fprintf(stderr, "JavaMapCollector::newRect got NULL constructor\n");
		return 0;
	}
	return fEnv->NewObject(fGuidoRectClass, constructor, int(r.left), int(r.top), int(r.right), int(r.bottom)); 
}

// --------------------------------------------------------------------------------------------
jobject	JavaMapCollector::newEltInfo (const GuidoElementInfos& inf) const
{
	jmethodID constructor = fEnv->GetMethodID (fGuidoEltInfoClass, "<init>", "(III)V");
	if (constructor == NULL) {
		fprintf(stderr, "JavaMapCollector::newMapInfo got NULL constructor\n");
		return 0;
	}
	return fEnv->NewObject(fGuidoEltInfoClass, constructor, inf.type, inf.staffNum, inf.voiceNum); 
}

// --------------------------------------------------------------------------------------------
void JavaMapCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos ) 
{
	jmethodID javaCallback = fEnv->GetMethodID (fClass, "Graph2TimeMap", "(Lguidoengine/guidorect;Lguidoengine/guidosegment;Lguidoengine/guidoelementinfo;)V");
	if (javaCallback == NULL) {
		if (!fErrNotified) {
			fprintf(stderr, "JavaTimeMapCollector::Graph2TimeMap got NULL jmethodID\n");
			fErrNotified = true;
		}
		return;
	}

	jobject jrect = newRect(box);
	jobject jsegm = newSegment(dates);
	jobject jinfo = newEltInfo(infos);
	if (jrect && jsegm && jinfo)
		fEnv->CallVoidMethod (fCollector, javaCallback, jrect, jsegm, jinfo);
}
