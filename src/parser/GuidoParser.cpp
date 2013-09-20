/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <locale.h>

#include "GuidoParser.h"
#include "ARFactory.h"

namespace guido
{

//--------------------------------------------------------------------------
GuidoParser::GuidoParser(std::istream* stream) : fStream(stream)
{
	setlocale(LC_NUMERIC, "C");
	fFactory = new ARFactory();
    initScanner();
	fErrorLine = fErrorColumn = 0;
}

//--------------------------------------------------------------------------
GuidoParser::~GuidoParser() 
{
	setlocale(LC_NUMERIC, 0);
	destroyScanner();
	delete fFactory;
}

//--------------------------------------------------------------------------
void GuidoParser::noteInit (const char *id)
{
	faccidentals = 0;
	fzaehlerSet = 0;
	fFactory->createEvent(id);
}

//--------------------------------------------------------------------------
void GuidoParser::noteAcc (int n)		{ faccidentals += n; }
void GuidoParser::noteOct (int n)		{ fFactory->setRegister(n); }

//--------------------------------------------------------------------------
void GuidoParser::noteEnum (long int n)
{
	fzaehlerSet = 1;
	fndots = 0;
	fFactory->setNumerator(n);
	fFactory->setDenominator(1);

	fnt_enum=n;
	fnt_denom=1;
	fnt_enumSet=true;
}

//--------------------------------------------------------------------------
void GuidoParser::noteDenom (long int n)
{
	if (fzaehlerSet == 0) {
		//  reset to standard
		fFactory->setNumerator(1);
	 }
	fFactory->setDenominator(n);
	fndots = 0; // switch off dots

	fnt_denom=n;
	if(!fnt_enumSet)
		fnt_enum=1;
	fzaehlerSet = 0;
}

//--------------------------------------------------------------------------
void GuidoParser::noteDot ()		{ fndots = 1; }
void GuidoParser::noteDdot ()		{ fndots = 2; }
void GuidoParser::noteTdot ()		{ fndots = 3; }

//--------------------------------------------------------------------------
static void mls2dur (int n, int& outNum, int& outDenum)
{
	int base = 1000;
	int num = n / base;
	int r = n % base;
	int denum = 4;
	while (r > 10) {	// approximate musical value (10 mls accuracy)
		denum *= 2;
		base /= 2;
		num = n / base;
		r = n % base;
	}
	outNum = num;
	outDenum = denum;
}

//--------------------------------------------------------------------------
void GuidoParser::noteAbsDur (long int n)
{
	int num, denum;
	mls2dur (n, num, denum);
	noteEnum (num);
	noteDenom (denum);
}

//--------------------------------------------------------------------------
void GuidoParser::appendNote	()
{
	if (faccidentals)
		fFactory->setAccidentals(faccidentals);
	if (fndots)
		fFactory->setPoints(fndots);
	fFactory->addEvent();
}

//--------------------------------------------------------------------------
void GuidoParser::chordInit ()		{ fFactory->createChord(); }
void GuidoParser::chordInitNote	()	{ fFactory->initChordNote(); }
void GuidoParser::seqAppendChord ()	{ fFactory->addChord(); }

//--------------------------------------------------------------------------
void GuidoParser::seqInit ()
{
	fFactory->createVoice();

	fnt_enum=1;
	fnt_denom=4;
	fnt_enumSet=0;
	fzaehlerSet = 0;
}

//--------------------------------------------------------------------------
void GuidoParser::seqExit ()		{ fFactory->addVoice(); }

//--------------------------------------------------------------------------
void GuidoParser::segmInit ()
{
	fFactory->createMusic();
	fzaehlerSet = 0;
	fndots = 0;
}
void GuidoParser::segmExit ()		{ }
void GuidoParser::segmAppendSeq	()	{ }

//--------------------------------------------------------------------------
void GuidoParser::tagStart (const char* id, long int no)
{
	fFactory->createTag (&(id[1]), (no <= 0 ? -1 : no) );
}

//--------------------------------------------------------------------------
void GuidoParser::tagIntArg	(long int n)		{ fFactory->addTagParameter ((int) n); }
void GuidoParser::tagFloatArg (double r)		{ fFactory->addTagParameter(r); }
void GuidoParser::tagArgUnit (const char* unit)	{ fFactory->setUnit(unit); }
void GuidoParser::tagStrArg (const char *s)		{ fFactory->addTagParameter(s); }
void GuidoParser::tagAdd ()						{ fFactory->addTag(); }
void GuidoParser::tagAddArg	(const char *s)		{ fFactory->setParameterName(s); }
void GuidoParser::tagEnd ()						{ fFactory->endTag(); }
void GuidoParser::tagRange ()					{ fFactory->tagRange(); }

//--------------------------------------------------------------------------
ARHandler GuidoParser::parse()
{
	fzaehlerSet = 0;
	faccidentals = 0;
	fndots = 0;
	fnt_enumSet = false;
	fnt_enum =0;
	fnt_denom =1;
	fErrorLine = fErrorColumn = 0;
	_yyparse ();
	return (fErrorLine == 0) ? GuidoFactoryCloseMusic (fFactory) : 0;
}


} // end namespace

