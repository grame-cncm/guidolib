/*
 * QSwitcher.h
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
#ifndef SWITCHER_H
#define SWITCHER_H

#include <QObject>
//#include <QGraphicsRectItem>
//#include <QPen>
//#include <QBrush>
#include <QMap>
#include <QList>
#include <QtDebug>

/*!
*	\brief A class to determine, among a set of T objects, which one is "active", using flags and switches.
*
*	The QSwitcher has 2 types of variable :
*		- flags :	
*					- You can set a flag "on" or "off" with QSwitcher::setFlag(int flagId, bool isOn).
*					- One or various flags may be active at one time, or even none.
*					- When you create a flag, you associate it with a T object. You also 
*					give it a priority : QSwitcher::addFlag(int flagId , int priority , const T& object).
*		- switchs:	there is always one (and only one) active switch (QSwitcher::setActiveSwitch(int switchId) ).
*
*	The "active" T object is the one associated with the highest priority "on" flag.
*
*	The switch are used to associate more than one T object with a flag : you can add
*	others T object to a flag, associating each new T object with 
*	a switch ( QSwitcher::setSwitchedFlag(int flagId, int switchId , const T& object) ) ; 
*	the "active" T object will be the one associated with the active switch.
*	
*	To sum-up :
*		- first, the QSwitcher determine the highest priority "on" flag
*		- then, the QSwitcher checks, for this flag, if there is a specific T object associated with the active switch. 
*		- if there is one, use it. If not, use the standard flag's T object.
*/
template <class T>
class QSwitcher
{
	typedef QMap<int,T>		QMapIntBrush;
	typedef QMap<int,T>		QMapIntPen;

	public:

		QSwitcher()
		{
			mSwitch = 0;
		}

		int		switchValue()  const	{ return mSwitch; }
		bool	isFlagOn(int flagId)	{ return mOnFlags.contains( flagId ); }

		void setFlag(int flagId, bool isOn)
		{
			//Checks if we know the specified flagId
			if ( !mActiveObjects.contains(flagId) )
			{
				qWarning( "QSelectionItem::setFlag : unknown flagId : %d" , flagId );
				return;
			}
			
			//Adds or remove flagId from the "activated ids" list.
			if ( isOn )
			{
				if ( !mOnFlags.contains(flagId) )
					mOnFlags << flagId;
			}
			else
				mOnFlags.removeAll( flagId );

			updateActiveObject();
		}

		void setActiveSwitch(int switchId)
		{
			if ( switchId != mSwitch )
			{
				mSwitch = switchId;
				updateActiveObject();
			}
		}


		/*!
		*	\brief Creates a new flag.
		*	\param flagId : id, will be used to set the flag on/off. Must be >= 0.
		*	\param priority: the flag's priority. Must be >= 0
		*	\param object: the T object associated with this flag.
		*/
		void addFlag(int flagId , int priority , const T& object)
		{
			mActiveObjects[flagId][0] = object;
			mPriorities[flagId] = priority;
		}

		void setSwitchedFlag( int flagId , int switchId , const T& object )
		{
			if ( switchId < 0 )
				qCritical("QSelectionItem::setSwitchedFlag : invalid switchId : %d" , switchId);
			if ( !mActiveObjects.contains(flagId) )
				qCritical("QSelectionItem::setSwitchedFlag : unknown flagId : %d" , flagId);

			mActiveObjects[flagId][switchId] = object;
		}

		const T&		activeObject() const		{ return mActiveObject; }

	protected:

		void updateActiveObject()
		{
			//Find the highest priority flag.
			int maxPriority = -1;
			int id = -1;
			for ( int i = 0 ; i < mOnFlags.size() ; i++ )
			{
				int priority = mPriorities[mOnFlags[i]];
				if ( priority > maxPriority )
				{
					id = mOnFlags[i];
					maxPriority = priority;
				}
			}

			if ( id >= 0 )
			{
				//Use pen and brush for this couple (flag,switch)
				//if pen/brush have been defined for this switch. 
				//Else, use default pen/brush for this flag (switch=0)
				if ( mActiveObjects[id].contains(mSwitch) )
					mActiveObject = mActiveObjects[id][mSwitch];
				else
					mActiveObject = mActiveObjects[id][0];
			}
		}

		T	mActiveObject;

		QMap<int,QMapIntPen>	mActiveObjects;
		QMap<int,int>			mPriorities;
		QList<int>				mOnFlags;

		int mSwitch;
};

#endif //SWITCHER_H
