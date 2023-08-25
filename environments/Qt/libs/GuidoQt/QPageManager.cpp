/*
 * QPageManager.cpp
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
#include "QPageManager.h"

#include <assert.h>
#include <QtDebug>

//-------------------------------------------------------------------------
QPageManager::QPageManager()
{
	mFirstPageIndex = 1;
	mGridHeight = 1;
	mGridWidth = 1;
}

//-------------------------------------------------------------------------
QPageManager::~QPageManager()
{
}

#define MAX(a,b) ( ((a)>(b))?(a):(b) )
#define MIN(a,b) ( ((a)<(b))?(a):(b) )
//-------------------------------------------------------------------------
void QPageManager::setPages(const QList<QSizeF>& pages)
{
	mPages.clear();
	mPages << QSizeF(0,0) << pages;
	updateGrid();
}

//-------------------------------------------------------------------------
void QPageManager::setGridHeight( int height )
{
	if ( height <= 0 )
		return;

	mGridHeight = height;
	updateGrid();
}

//-------------------------------------------------------------------------
void QPageManager::setGridWidth( int width )
{
	if ( width <= 0 )
		return;

	mGridWidth = width;
	updateGrid();
}

//-------------------------------------------------------------------------
bool QPageManager::setPage( int index )
{
	if ( index < 1 ) 
		return false;
	if ( index >= mPages.size() )
		return false;

	mFirstPageIndex = index;
	updateGrid();
	return true;
}

//-------------------------------------------------------------------------
QSizeF QPageManager::pageSize(int index) const
{
	return mPages[ index ];
}

//-------------------------------------------------------------------------
QPointF QPageManager::pagePos( int pageIndex ) const
{
	if ( pageIndex < firstVisiblePage() )
		return QPointF(-1,-1);
	if ( pageIndex > lastVisiblePage() )
		return QPointF(-1,-1);

	int iMax = columnIndex(pageIndex);
	int jMax = lineIndex(pageIndex);
	float x = 0 , y = 0;
	for ( int i = 0 ; i < iMax ; i++ )
		x += mColumnWidth[i];
	for ( int j = 0 ; j < jMax ; j++ )
		y += mLineHeight[j];

	return QPointF( x , y );
}

//-------------------------------------------------------------------------
QSizeF QPageManager::totalSize() const
{
	float totalWidth = 0 , totalHeight = 0;
	for ( int i = 0 ; i < mColumnWidth.size() ; i++ )
	{
		totalWidth += mColumnWidth[i];
	}
	for ( int j = 0 ; j < mLineHeight.size() ; j++ )
	{
		totalHeight += mLineHeight[j];
	}
	
	return QSizeF( totalWidth , totalHeight );
}

//-------------------------------------------------------------------------
float QPageManager::lineHeight(int lineIndex) const
{
	return mLineHeight[lineIndex];
}

//-------------------------------------------------------------------------
float QPageManager::columnWidth(int columnIndex) const
{
	return mColumnWidth[columnIndex];
}

//-------------------------------------------------------------------------
int QPageManager::firstVisiblePage() const
{
	return mFirstPageIndex;
}

//-------------------------------------------------------------------------
int QPageManager::lastVisiblePage() const
{
	return MIN( mFirstPageIndex - 1 + ( mGridWidth * mGridHeight ) , (int)mPages.size()-1 );
}

//-------------------------------------------------------------------------
void QPageManager::updateGrid()
{
	mColumnWidth.clear();
	mLineHeight.clear();

	for ( int c = 0 ; c < mGridWidth ; c++ )
		mColumnWidth << 0;
	for ( int l = 0 ; l < mGridHeight ; l++ )
		mLineHeight << 0;

	for ( int i = mFirstPageIndex ; i <= lastVisiblePage() ; i++ )
	{
		int cIndex = columnIndex(i);
		int lIndex = lineIndex(i);
		mColumnWidth[cIndex]	= MAX( mColumnWidth[cIndex] , mPages[i].width() );
		mLineHeight[lIndex]		= MAX( mLineHeight[lIndex] , mPages[i].height() );
	}

}

//-------------------------------------------------------------------------
int QPageManager::columnIndex(int pageIndex) const	{ return (pageIndex - mFirstPageIndex) % mGridWidth; }

//-------------------------------------------------------------------------
int QPageManager::lineIndex(int pageIndex) const	{ return (pageIndex - mFirstPageIndex) / mGridWidth; }
