/*
 * QPageManager.h
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
#ifndef PAGE_MANAGER_H 
#define PAGE_MANAGER_H

#include <QList>
#include <QSizeF>
#include <QPointF>

/**
*	\brief Arranges a set of pages in a grid.
*	
*	Basically: 
*		1. give a set of pages to the QPageManager (setPages) (a page is defined by its size),
*		2. specify the number of lines/columns of the grid of pages (setGridHeight / setGridWidth) ;
*		3. define the first visible page (setPage) ;
*		4. then the QPageManager can tell you the position of each page in the grid.
*
*	The pages are placed in the grid in increasing order of indexes ;
*	the first page is at the top-left, the second page is placed at
*	the right of the first page, and so on, until the end of the line,
*	when it goes on on the next line.
*
*	The total number of pages may be greater than gridWidth() * gridHeight() ; you can
*	get the firstVisiblePage() and the lastVisiblePage(). Other pages are just considered
*	as non-visible at that moment.
*
*	Each line has its own height, which is the one of the highest item of the line.
*	Each column has its own width, which is the width of the item with the biggest width of the column.
*
*	Notes:
*		- lineIndex & columnIndex start at 0.
*		- pageIndex starts at 1 (like in a book).
*/
class QPageManager
{

	public:

		/**	\brief Default constructor
		*/
		QPageManager();
		
		/** \brief Destructor
		*/
		virtual ~QPageManager();
		
		/**
		*	\brief Sets the set of pages.
		*/
		void setPages(const QList<QSizeF>& pages);
		
		/**
		*	\brief Sets the height of the grid ( <=> number of lines )
		*
		*	If invalid argument (<=0), does nothing.
		*/
		void setGridHeight( int height );

		/**
		*	\brief Sets the width of the grid ( <=> number of columns )
		*
		*	If invalid argument (<=0), does nothing.
		*/
		void setGridWidth( int width );
		
		/**
		*	\brief Sets the index of the first visible page.
		*
		*	\return False if invalid index.
		*/
		bool setPage( int index );
		
		/**
		*	\brief Returns the size of the page (as defined by setPages)
		*
		*	\warning The index parameter starts with 1 and no more with 0 
		*	(in opposition with the setPages function)
		*/
		QSizeF pageSize(int index) const;
		
		/**
		*	\brief Returns the position of the page.
		*
		*	If the page is not visible, returns (-1,-1).
		*/
		QPointF pagePos( int pageIndex ) const;

		/**
		*	\brief Returns the current total size of the grid of pages.
		*/
		QSizeF totalSize() const;
		
		/**
		*	\brief Returns the height of a line defined by its index.
		*/
		float lineHeight(int lineIndex) const;

		/**
		*	\brief Returns the width of a column defined by its index.
		*/
		float columnWidth(int columnIndex) const;
		
		/**
		*	\brief Returns the first visible page (top-left of the grid) index.
		*/
		int firstVisiblePage() const;
		
		/**
		*	\brief Returns the last visible page (bottom-right of the grid) index.
		*/
		int lastVisiblePage() const;
		
		/**
		*	\brief Returns the grid's width (<=> number of columns)
		*/
		int gridWidth() const { return mGridWidth; }

		/**
		*	\brief Returns the grid's height (<=> number of lines)
		*/
		int gridHeight() const { return mGridHeight; }

	protected:
	
		void updateGrid();
		int columnIndex(int pageIndex) const;
		int lineIndex(int pageIndex) const;

		int mFirstPageIndex;
		int mGridHeight , mGridWidth;
		QList<float> mLineHeight , mColumnWidth;
		QList<QSizeF> mPages;
};

#endif //PAGE_MANAGER_H
