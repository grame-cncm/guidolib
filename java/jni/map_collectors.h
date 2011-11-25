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

#ifndef __map_collector__
#define __map_collector__

#include <jni.h>
#include "GUIDOScoreMap.h"


// --------------------------------------------------------------------------------------------
class JavaCollector
{
	protected :
		JNIEnv *	fEnv;
		jobject		fCollector;
		jclass		fClass;
		jclass		fGuidoDateClass;
		jclass		fGuidoSegmentClass;
		bool		fErrNotified;
	
	jobject		newDate (int, int) const;
	jobject		newSegment (const TimeSegment& segm) const;
	
	public:
				 JavaCollector (JNIEnv * env, jobject jcollector);
		virtual ~JavaCollector ();
};

// --------------------------------------------------------------------------------------------
class JavaTimeMapCollector : public JavaCollector, public TimeMapCollector
{
	public:
				 JavaTimeMapCollector (JNIEnv * env, jobject jcollector);
		virtual ~JavaTimeMapCollector ();
		void	Time2TimeMap( const TimeSegment& from, const TimeSegment& to );
};


// --------------------------------------------------------------------------------------------
class JavaMapCollector : public JavaCollector, public MapCollector
{
	jclass		fGuidoRectClass;
	jclass		fGuidoEltInfoClass;

	jobject		newRect (const FloatRect& r) const;
	jobject		newEltInfo (const GuidoElementInfos& segm) const;

	public:
				 JavaMapCollector(JNIEnv * env, jobject jcollector);
		virtual ~JavaMapCollector ();
		void	Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos );
};

#endif
