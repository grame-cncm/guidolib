/*
 * GSystemQt.h
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef GSYSTEMQT_H_
#define GSYSTEMQT_H_

#include "VGSystem.h"
#include "MusicalSymbols.h"	

#include <QPainter>

/**
*	\brief Qt implementation of the VGSystem interface.
*
*	For now, among the VGDevice factory functions, only the CreateDisplayDevice works, 
*	but you can use the created VGDevice to draw with any QPainter anyway 
*	(QPrinter, QWidget, QImage ...), so you needn't the other factory functions.
*/
class GSystemQt: public VGSystem	
{
	public:

		GSystemQt( QPainter* qPainter = 0 );
		virtual					~GSystemQt();

		// - VGDevice services -------------------------------------------
		virtual VGDevice*		CreateDisplayDevice();	
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );

		// - Font services -----------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;

	protected:
		QPainter*				mQPainter;

};

#endif /*GSYSTEMQT_H_*/
