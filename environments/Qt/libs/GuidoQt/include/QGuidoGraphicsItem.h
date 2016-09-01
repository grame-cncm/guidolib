/*
 * QGuidoGraphicsItem.h
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
#ifndef QGuidoGraphicsItem_H
#define QGuidoGraphicsItem_H

#include <QGraphicsItem>
#include <QObject>

#include "QGuidoPainter.h"
#include "QPageManager.h"
#include "GUIDOPianoRoll.h"

/**
*	\brief A QGraphicsItem displaying a Guido Score.
*
*	The Guido Score may be loaded via a QString containing the GMN code (setGMNCode()),
*	or via a QString containing the path to a GMN file (setFile()).
*
*	The pages of the Guido Score will be displayed in a "grid of pages":
*		- you can specify the number of columns and lines of this grid
*		with the setGridHeight / setGridWidth functions ;
*		- the pages are placed in the grid in increasing order of indexes ;
*		the first page is at the top-left, the second page is placed at
*		the right of the first page, and so on, until the end of the line,
*		when it goes on on the next line ;
*		- you can specify the first (top left) displayed page with the
*		setPage function.
*		- if the grid is too small to display all the Guido Score pages,
*		it doesn't matter : other pages are simply not visible, and
*		you have to use setPage to display them.
*	See QPageManager for more details.
*
*	\warning Don't forget to use QGuidoPainter's static
*	startGuidoEngine method before building any QGuidoGraphicsItem,
*	or else you'll have an assertion
*	failed in the QGuidoGraphicsItem constructor.
*/
class QGuidoGraphicsItem : public QGraphicsObject
{

	Q_OBJECT

	public:

		/**	\brief Default constructor
		*/
		QGuidoGraphicsItem(QGraphicsItem * parent = 0);
		
		/** \brief Destructor
		*/
		virtual ~QGuidoGraphicsItem();
		
		void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget=0);
		QRectF boundingRect() const { return mBoundingRect; }

		/**	\brief Sets the current Guido Score file to draw.
		*	\param fileName Full path to the Guido Score Notation file.
		*	\return true if the file is a valid Guido Score file.
		*
		*	\note If any GMN code has been previously set, it will be erased.
		*/
		virtual bool setGMNFile(const QString& fileName);

		/**	\brief Returns the current Guido Score file.
		*/
		QString fileName() const;
		
		/**	\brief Sets the current Guido code that will be displayed by the guido item.
		*	\param gmnCode The Guido Music Notation code
		*	\return true if the GMN code is valid.
		*/
		virtual bool setGMNCode(const QString& gmnCode, const QString& path=0);
		
		/**	\brief Sets the current Guido stream that will be displayed by the guido item.
		*	\param gmnStream The Guido Music Notation stream code
		*	\return true if the GMN code is valid.
		*/
        bool setGMNStream(GuidoStream * gmnStream);
    
		/**	\brief Sets the PianoRoll and the boolean fIsPianoRoll to true
		*	\param pRoll the PianoRoll object
		*/
        void setPianoRoll(PianoRoll * pRoll);
    
		/**	\brief Returns the current Guido code.
		*
		*	\note This will work only if the code has been
		*	set with setGMNCode.  If the code has been loaded
		*	via setFile, this will return "".
		*/
		QString gmnCode() const;

		/** \brief Returns the validity of the last GMN code loaded with setGMNCode or setGMNFile
		*/
		bool isGMNValid() const;

		/**
		*	\brief Returns a description of the last encountered error.
		*/
		QString getLastErrorMessage() const;

		/**
		*	\brief Gets the parse error line/col.
		*/
		void getLastParseErrorLine(int &line, int &col) const;

		/**
		*	\brief Sets the Guido layout settings used to draw with this QGuidoPainter
		*
		*	\note You can have more informations on GuidoLayoutSettings in GUIDOlib documentation.
		*/
		virtual void setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings);

		/**
		*	\brief Returns the Guido layout settings of the QGuidoPainter.
		*/
		GuidoLayoutSettings guidoLayoutSettings() const;

		/// \brief sets the minimum systems distance to its default value
		void  resetSystemsDistance();
		/// \brief sets the minimum systems distance
		void  setSystemsDistance(float distance);
		/// \brief returns the minimum systems distance
		float getSystemsDistance() const;
		
		/// \brief Disable/enable automatic ResizePageToMusic
		void setResizePageToMusic(bool isOn);
		/// \brief Returns the state of the automatic ResizePageToMusic mode (enabled or disabled)
		bool isResizePageToMusicOn() const;

		/// \brief Sets the page format used when no page format is specified by the GMN
		void setGuidoPageFormat(const GuidoPageFormat& pageFormat);
		/// \brief Gets the page format used when no page format is specified by the GMN
		GuidoPageFormat guidoPageFormat() const;

		/**
		*	\brief Returns the number of pages of the Guido Score.
		*/
		int pageCount() const;

		/**
		*	\brief Returns the size of a page (specified by its index), in millimeters.
		*
		*	The page format & size are defined in the GMN code.
		*/
		QSizeF pageSizeMM(int pageIndex) const;

		/**
		*	\brief Sets the first displayed page of the Guido Score.
		*
		*	\return True if the pageIndex is valid, false else.
		*/
		bool setPage(int pageIndex);

		/**
		*	\brief Sets the number of the lines of the grid of pages.
		*/
		void setGridHeight( int height );

		/**
		*	\brief Sets the number of the columns of the grid of pages.
		*/
		void setGridWidth( int width );

		/**
		*	\brief Returns the number of lines in the grid of pages.
		*/
		int gridHeight() const;

		/**
		*	\brief Returns the number of columns in the grid of pages.
		*/
		int gridWidth() const;
		
		/**
		*	\brief Returns the first visible page index.
		*/
		int firstVisiblePage() const;

		/**
		*	\brief Returns the last visible page index.
		*/
		int lastVisiblePage() const;
		
		const QPageManager * pageManager() const	{ return mPageManager; }
		const QGuidoPainter * guidoPainter() const	{ return mGuidoPainter; }
		
		/**
		*	\brief Gives access to the GRHandler (graphic representation) of the Score in read-only.
		*/
        CGRHandler   getGRHandler() const { return mGuidoPainter->getGRHandler(); }

		/**
		*	\brief Gives access to the ARHandler (abstract representation) of the Score in read-only.
		*/		
		CARHandler   getARHandler() const { return mGuidoPainter->getARHandler(); }
    

		/// \brief sets the color used to draw the score
		void setScoreColor(const QColor& color)	{ mGuidoPainter->setScoreColor(color); }
		/// \brief returns the color used to draw the score
		const QColor& getScoreColor() const		{ return mGuidoPainter->getScoreColor(); }
		
	Q_SIGNALS:
		void geometryHasChanged();
		
	protected:
	
		void updateGuidoPagesSizes();
		void guidoItemUpdate();
		QRectF computeBoundingRect() const;
		QRect mBoundingRect;
	
		QGuidoPainter * mGuidoPainter;
		QPageManager * mPageManager;
    
        PianoRoll * fPianoRoll;
        bool fIsPianoRoll;
};

#endif //QGuidoGraphicsItem_H
