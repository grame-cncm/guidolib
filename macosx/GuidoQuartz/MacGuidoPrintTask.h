/*
 * file : MacGuidoPrintTask.h
 * 
 *	Copyright (C) 2004 Grame
 *
 *		01/2004		JB	creation
 *
 */

#ifndef MacGuidoPrintTask_H
#define MacGuidoPrintTask_H

#include "MacPrintTask.h"
#include "GUIDOEngine.h"

// ---------------------------------------------------------------------------
class MacGuidoPrintTask : public MacPrintTask
{
	public:
							MacGuidoPrintTask();
		virtual				~MacGuidoPrintTask();

				void		SetGuidoGR( GRHandler inGRHandle ) { mGuidoGR = inGRHandle; }
				GRHandler	GetGuidoGR() const { return mGuidoGR; }

		
	protected:

		virtual bool		DoDrawPageContent( CGContextRef inContext, 
													int inPageNum ); // >= 1

		GRHandler mGuidoGR;
};

#endif
