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

#include "guidoengine_guidolayout.h"
#include "GUIDOEngine.h"
#include "javaIDs.h"

jfieldID gSystemsDistanceID, gSystemsDistributionID, gOptimalPageFillID;
jfieldID gSystemsDistribLimitID, gForceID, gSpringID, gNeighborhoodSpacingID;

/*
 * Class:     guidoengine_guidolayout
 * Method:    GetDefault
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidolayout_GetDefault (JNIEnv * env, jobject obj)
{
	GuidoLayoutSettings settings;
	GuidoGetDefaultLayoutSettings (&settings);
	env->SetFloatField (obj, gSystemsDistanceID, settings.systemsDistance);
	env->SetIntField (obj, gSystemsDistributionID, settings.systemsDistribution);
	env->SetFloatField (obj, gSystemsDistribLimitID, settings.systemsDistribLimit);
	env->SetFloatField (obj, gForceID, settings.force);
	env->SetFloatField (obj, gSpringID, settings.spring);
	env->SetIntField (obj, gNeighborhoodSpacingID, settings.neighborhoodSpacing);
	env->SetBooleanField (obj, gOptimalPageFillID, settings.optimalPageFill);
}

/*
 * Class:     guidoengine_guidolayout
 * Method:    Init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_guidoengine_guidolayout_Init (JNIEnv * env, jclass cls)
{
	if (!getID (env, cls, gSystemsDistanceID, "fSystemsDistance", "F")) return;
	if (!getID (env, cls, gSystemsDistributionID, "fSystemsDistribution", "I")) return;
	if (!getID (env, cls, gOptimalPageFillID, "fOptimalPageFill", "Z")) return;
	if (!getID (env, cls, gSystemsDistribLimitID, "fSystemsDistribLimit", "F")) return;
	if (!getID (env, cls, gForceID, "fForce", "F")) return;
	if (!getID (env, cls, gSpringID, "fSpring", "F")) return;
	if (!getID (env, cls, gNeighborhoodSpacingID, "fNeighborhoodSpacing", "I")) return;
}

