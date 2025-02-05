/*
 * QGuidoAR.h
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
#ifndef GUIDO_AR_H
#define GUIDO_AR_H


#include <QString>

class QGuidoAR
{

	public:
	
		/*!	\brief	Builds a Guido Score putting in sequence the Guido Score 
		*			built from GMN code str1 and the Guido Score built from 
		*			GMN code str2, and returns the corresponding GMN code.
		*/
		static QString sequence( const QString& str1 , const QString& str2 );

		typedef enum ParallelAlign
		{
			Right , Left
		} ParallelAlign;

		/*!	\brief	Builds a Guido Score putting in parallel the Guido Score 
		*			built from GMN code str1 and the Guido Score built from 
		*			GMN code str2, and returns the corresponding GMN code.
		*/
		static QString parallel( const QString& str1 , const QString& str2 , ParallelAlign parallelAlign = Left );
		
		static QString interleave( const QString& str1 , const QString& str2 );
		
		static QString duration( const QString& str1 , const QString& str2 );
		
		static QString transpose( const QString& str1 , const QString& str2 );
		
		static QString head( const QString& str1 , const QString& str2 );
		
		static QString tail( const QString& str1 , const QString& str2 );

		static QString top( const QString& str1 , const QString& str2 );
		
		static QString bottom( const QString& str1 , const QString& str2 );

		static QString rythm( const QString& str1 , const QString& str2 );
		
		static QString pitch( const QString& str1 , const QString& str2 );
		
	#ifdef USEMidiShare
		static int	getMidiRef( const QString& gmnCode );
		static void midiPlay( int midiRef );
		static void midiPause( int midiRef );
		static void midiResume( int midiRef );
		static void midiStop( int midiRef );
		static void midiClose( int midiRef );
		static void midiExport( const QString& gmnCode , const QString& outMidiFile );
	#else
		static int	getMidiRef( const QString& )	{ return -1; }
		static void midiPlay( int )	{}
		static void midiPause( int ) {}
		static void midiResume( int ) {}
		static void midiStop( int ) {}
		static void midiClose( int ) {}
		static void midiExport( const QString& , const QString& ) {}
	#endif
};

#endif //GUIDO_AR_H
