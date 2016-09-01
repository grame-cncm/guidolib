/*
 * GSystemQt.h
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
#ifndef GSYSTEMQT_H_
#define GSYSTEMQT_H_

#include "VGSystem.h"

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
