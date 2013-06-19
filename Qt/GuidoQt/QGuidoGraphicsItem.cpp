/*
 * QGuidoGraphicsItem.cpp
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
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <assert.h>

#include "QPageManager.h"
#include "QGuidoGraphicsItem.h"
#include "QGuidoPainter.h"

//-------------------------------------------------------------------------
QGuidoGraphicsItem::QGuidoGraphicsItem(QGraphicsItem * parent)
	: QGraphicsObject(parent)
{
	mGuidoPainter = QGuidoPainter::createGuidoPainter();
	
	mPageManager = new QPageManager();
	
	assert(mGuidoPainter);
	// QGuidoPainter::createGuidoPainter() returned NULL. 
	// You forgot to call QGuidoPainter::startGuidoEngine.
	
}

//-------------------------------------------------------------------------
QGuidoGraphicsItem::~QGuidoGraphicsItem()
{
	delete mPageManager;

	QGuidoPainter::destroyGuidoPainter( mGuidoPainter );
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * )
{	
	for ( int i = mPageManager->firstVisiblePage() ; i <= mPageManager->lastVisiblePage() ; i++ )
	{
		QRectF pageRect;
		pageRect.moveTo( mPageManager->pagePos(i) );
		pageRect.setSize( mPageManager->pageSize(i) );
		mGuidoPainter->draw( painter , i , pageRect.toRect() , option->exposedRect.toRect() );
	}
}

//-------------------------------------------------------------------------
bool QGuidoGraphicsItem::setGMNFile(const QString& fileName)
{
	bool loadOk = mGuidoPainter->setGMNFile( fileName );
	updateGuidoPagesSizes();
	guidoItemUpdate();
	return loadOk;
}
	
//-------------------------------------------------------------------------
bool QGuidoGraphicsItem::setGMNCode(const QString& gmnCode)
{
	bool loadOk = mGuidoPainter->setGMNCode( gmnCode );
	updateGuidoPagesSizes();
	guidoItemUpdate();
	return loadOk;
}

//-------------------------------------------------------------------------
QString QGuidoGraphicsItem::gmnCode() const
{
	return mGuidoPainter->gmnCode();
}

//-------------------------------------------------------------------------
bool QGuidoGraphicsItem::isGMNValid() const
{
	return mGuidoPainter->isGMNValid();
}

//-------------------------------------------------------------------------
QString QGuidoGraphicsItem::getLastErrorMessage() const
{
	return mGuidoPainter->getLastErrorMessage();
}

//-------------------------------------------------------------------------
int	QGuidoGraphicsItem::getLastParseErrorLine() const
{
	return mGuidoPainter->getLastParseErrorLine();
}

//-------------------------------------------------------------------------
QString QGuidoGraphicsItem::fileName() const
{
	return mGuidoPainter->fileName();
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings)
{
	mGuidoPainter->setGuidoLayoutSettings( layoutSettings );
	updateGuidoPagesSizes();
	guidoItemUpdate();
}

//-------------------------------------------------------------------------
GuidoLayoutSettings QGuidoGraphicsItem::guidoLayoutSettings() const
{
	return mGuidoPainter->guidoLayoutSettings();
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::resetSystemsDistance()
{
	mGuidoPainter->resetSystemsDistance();
	updateGuidoPagesSizes();
	guidoItemUpdate();
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::setSystemsDistance(float distance)
{
	mGuidoPainter->setSystemsDistance(distance);
	updateGuidoPagesSizes();
	guidoItemUpdate();
}

//-------------------------------------------------------------------------
float QGuidoGraphicsItem::getSystemsDistance() const
{
	return mGuidoPainter->getSystemsDistance();
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::setResizePageToMusic(bool isOn)
{
	if ( isOn != isResizePageToMusic() )
	{
		mGuidoPainter->setResizePageToMusic(isOn);
		updateGuidoPagesSizes();
		guidoItemUpdate();
	}
}

//-------------------------------------------------------------------------
bool QGuidoGraphicsItem::isResizePageToMusic() const
{ 
	return mGuidoPainter->isResizePageToMusic(); 
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::setGuidoPageFormat(const GuidoPageFormat& pageFormat)
{
	mGuidoPainter->setGuidoPageFormat(pageFormat);
	updateGuidoPagesSizes();
	guidoItemUpdate();
}

//-------------------------------------------------------------------------
GuidoPageFormat QGuidoGraphicsItem::guidoPageFormat() const
{
	return mGuidoPainter->guidoPageFormat();
}

//-------------------------------------------------------------------------
bool QGuidoGraphicsItem::setPage(int pageIndex)
{
	bool result = mPageManager->setPage(pageIndex);
	guidoItemUpdate();
	return result;
}

//-------------------------------------------------------------------------
int QGuidoGraphicsItem::pageCount() const
{
	return mGuidoPainter->pageCount();
}

//-------------------------------------------------------------------------
int QGuidoGraphicsItem::firstVisiblePage() const
{
	return mPageManager->firstVisiblePage();
}

//-------------------------------------------------------------------------
QSizeF QGuidoGraphicsItem::pageSizeMM(int pageIndex) const
{
	return mGuidoPainter->pageSizeMM(pageIndex);
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::setGridHeight( int height )
{
	mPageManager->setGridHeight( height );
	guidoItemUpdate();
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::setGridWidth( int width )
{
	mPageManager->setGridWidth( width );
	guidoItemUpdate();
}

//-------------------------------------------------------------------------
int QGuidoGraphicsItem::lastVisiblePage() const
{
	return mPageManager->lastVisiblePage();
}

//-------------------------------------------------------------------------
int QGuidoGraphicsItem::gridHeight() const
{
	return mPageManager->gridHeight();
}

//-------------------------------------------------------------------------
int QGuidoGraphicsItem::gridWidth() const
{
	return mPageManager->gridWidth();
}

#include <QtDebug>

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::updateGuidoPagesSizes()
{
	QList<QSizeF> pages;
	for ( int i = 1 ; i <= pageCount() ; i++ )
		pages << pageSizeMM(i);

	mPageManager->setPages(pages);
}

//-------------------------------------------------------------------------
void QGuidoGraphicsItem::guidoItemUpdate()
{
	QRect newGeometry = computeBoundingRect().toRect();
	if ( newGeometry != mBoundingRect )
	{
		prepareGeometryChange();
		mBoundingRect = newGeometry;
		Q_EMIT geometryHasChanged();
	}
	else
	{
		update();
	}
}

//-------------------------------------------------------------------------
QRectF QGuidoGraphicsItem::computeBoundingRect() const
{
	QSizeF s = mPageManager->totalSize();
	return QRectF( 0,0,s.width(),s.height() );
}
