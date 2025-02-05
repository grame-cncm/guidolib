/*
 * QGuidoAR.cpp
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
#include "QGuidoAR.h"


#include <sstream>
#include <assert.h>

#ifdef WIN32
# pragma warning (disable : 4786)
# define basename(name)	(name)
# define _CRT_SECURE_NO_DEPRECATE
#else 
# include <libgen.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <QtDebug>

#include "libguidoar.h"

typedef guido::garErr (*GuidoAROperation)( const char * , const char * , std::ostream& );

/*
//--------------------------------------------------------------------------------
QString operate( const QString& str1 , const QString& str2 ,guido::operation * guidoOperation )
{
	guido::guidoparser r;
	guido::SARMusic g1 = r.parseString( str1.toUtf8().data() );
	if (!g1) 
//		readErr(*argsPtr);
		return "";

	guido::SARMusic g2 = r.parseString( str2.toUtf8().data() );
	if (!g2) 
//		readErr(*argsPtr);
		return "";

	g1 = (*guidoOperation)(g1, g2);

	if (g1) 
	{
		guido::Sguidoelement result = g1;
		std::ostringstream oss;
		oss << result;
		QString str_result( oss.str().c_str() );
		return str_result;
	}
	return "";
}
*/

//--------------------------------------------------------------------------------
QString operate( const QString& str1 , const QString& str2 , GuidoAROperation operation )
{
	std::ostringstream oss;
	guido::garErr err = operation( str1.toUtf8().constData() , str2.toUtf8().constData() , oss );
	if ( err == guido::kNoErr )
		return QString( oss.str().c_str() );
	else
		return "Error";
}


//--------------------------------------------------------------------------------
QString QGuidoAR::sequence( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGSeq );
}

//--------------------------------------------------------------------------------
GuidoAROperation parallelAlignToOperation( QGuidoAR::ParallelAlign parallelAlign )
{
	switch (parallelAlign)
	{
		case QGuidoAR::Left:	return guido::guidoGPar;
		case QGuidoAR::Right:	return guido::guidoGRPar;
		default :				assert(0); return guido::guidoGPar;
	}
}

//--------------------------------------------------------------------------------
QString QGuidoAR::parallel( const QString& str1 , const QString& str2 , ParallelAlign parallelAlign )
{
	return operate( str1 , str2 , parallelAlignToOperation(parallelAlign) );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::duration( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGSetDuration );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::interleave( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGSeq );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::transpose( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGTranpose );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::head( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGHead );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::tail( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGTail );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::top( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGTop );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::bottom( const QString& str1 , const QString& str2 )
{
	return operate( str1 , str2 , guido::guidoGBottom );
}

//--------------------------------------------------------------------------------
QString QGuidoAR::rythm( const QString& str1 , const QString& str2 )
{
	std::ostringstream oss;
	guido::garErr err = guido::guidoApplyRythm( str1.toUtf8().constData() , str2.toUtf8().constData() , guido::kApplyForwardLoop , oss );
	if ( err == guido::kNoErr )
		return QString( oss.str().c_str() );
	else
		return "Error";
}

//_______________________________________________________________________________		
QString QGuidoAR::pitch( const QString& str1 , const QString& str2 )
{
	std::ostringstream oss;
	guido::garErr err = guido::guidoApplyPitch( str1.toUtf8().constData() , str2.toUtf8().constData() , guido::kApplyForwardLoop , guido::kUseLowest , oss );
	if ( err == guido::kNoErr )
		return QString( oss.str().c_str() );
	else
		return "Error";
}

#ifdef USEMidiShare

#include "guidoparser.h"
#include "midicontextvisitor.h"
#include "midiconverter.h"
#include <Player.h>

//_______________________________________________________________________________
void QGuidoAR::midiExport( const QString& gmnCode , const QString& outMidiFile )
{
	assert( outMidiFile.length() );

	guido::guidoparser r;
	guido::Sguidoelement score = r.parseString( gmnCode.toUtf8().data() );
	if (score) 
	{
		guido::midiconverter mc;
		
		int err = mc.score2midifile(score, outMidiFile.toUtf8().data() );
		if (err != noErr)
			std::cerr << "error " << err << " while converting GMN to midifile " << outMidiFile.toUtf8().data() << std::endl;
	}
	else
	{
		std::cerr << "error : can't convert GMN to a valid score" << std::endl;
	}
}

//_______________________________________________________________________________
int	QGuidoAR::getMidiRef( const QString& gmnCode )
{
	guido::guidoparser r;
	
	guido::Sguidoelement score = r.parseString( gmnCode.toUtf8().data() );
	if (score) 
	{
		guido::midiconverter mc;
		short ref = mc.score2player(score, gmnCode.toUtf8().data());
		if (ref > 0) {
			MidiConnect (ref, 0, true);
			return ref;
		}
	}
	return -1;
}

//_______________________________________________________________________________
void QGuidoAR::midiPlay( int midiRef )		{ StartPlayer(midiRef); }
void QGuidoAR::midiPause( int midiRef )		{ PausePlayer(midiRef); }
void QGuidoAR::midiResume( int midiRef )	{ ContPlayer(midiRef); }
void QGuidoAR::midiStop( int midiRef )		{ StopPlayer (midiRef); }
void QGuidoAR::midiClose( int midiRef )
{
	MidiConnect	(midiRef, 0, false);
	ClosePlayer	(midiRef);
}
#endif
