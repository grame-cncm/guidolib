/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include "ARFactory.h"
#include "GUIDOInternal.h"
#include "GuidoParser.h"
#include "TagParameterFloat.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"

using namespace std;

//--------------------------------------------------------------------------
GuidoParser::GuidoParser()
{
	setlocale(LC_NUMERIC, "C");
	fFactory = 0;
    initScanner();
	fErrorLine = fErrorColumn = 0;
    fStream = NULL;
}

//--------------------------------------------------------------------------
GuidoParser::~GuidoParser() 
{
	setlocale(LC_NUMERIC, 0);
	destroyScanner();
	delete fFactory;
}

//--------------------------------------------------------------------------
static bool getIntValue (const GuidoParser::variable& var, int& value)
{
	try {
		if (var.fType == GuidoParser::kInt) {
			int tmp = std::stoi(var.fValue);
			if (tmp > 0) {
				value = tmp;
				return true;
			}
		}
	}
	catch (exception e) {
		cerr << "Warning - engine settings: incorrect int value " << var.fValue << endl;
	}
	return false;
}

//--------------------------------------------------------------------------
static bool getFloatValue (const GuidoParser::variable& var, float& value)
{
	int tmp;
	if (getIntValue(var, tmp)) {
		value = float(tmp);
		return true;
	}
	try {
		if (var.fType == GuidoParser::kFloat) {
			float tmp = std::stof(var.fValue);
			if (tmp > 0) {
				value = tmp;
				return true;
			}
		}
	}
	catch (exception e) {
		cerr << "Warning - engine settings: incorrect float value " << var.fValue << endl;
	}
	return false;
}

//--------------------------------------------------------------------------
static bool getBoolValue (const GuidoParser::variable& var, int& value)
{
	try {
		if (var.fType == GuidoParser::kInt)
			value = std::stoi(var.fValue);
		else if (var.fValue == "true")
			value = 1;
		else if (var.fValue == "false")
			value = 0;
		else return false;
		return true;
	}
	catch (exception e) {
		cerr << "Warning - engine settings: incorrect bool (int) value " << var.fValue << endl;
	}
	return false;
}

//--------------------------------------------------------------------------
bool GuidoParser::getSettings(GuidoLayoutSettings& settings)
{
	bool retcode = false;
	for (auto e: fEnv) {
		if (e.first == "$SYSTEM_DISTANCE") {
			if (getFloatValue (e.second, settings.systemsDistance )) retcode = true;
		}
		else if (e.first == "$SYSTEM_DISTRIBUTION") {
			int val;
			if (getIntValue (e.second, val )) {
				if ((val >= 1) && (val <=3)) {
					settings.systemsDistribution = val;
					retcode = true;
				}
			}
		}
		else if (e.first == "$SYSTEM_DISTRIBUTION_LIMIT") {
			if (getFloatValue (e.second, settings.systemsDistribLimit )) retcode = true;
		}
		else if (e.first == "$FORCE") {
			if (getFloatValue (e.second, settings.force )) retcode = true;
		}
		else if (e.first == "$SPRING") {
			if (getFloatValue (e.second, settings.spring )) retcode = true;
		}
		else if (e.first == "$NEIGHBORHOOD_SPACING") {
			if (getIntValue (e.second, settings.neighborhoodSpacing )) retcode = true;
		}
		else if (e.first == "$OPTIMAL_PAGE_FILL") {
			if (getIntValue (e.second, settings.optimalPageFill )) retcode = true;
		}
		else if (e.first == "$RESIZE_PAGE_TO_MUSIC") {
			if (getBoolValue (e.second, settings.resizePage2Music )) retcode = true;
		}
		else if (e.first == "$CHECK_LYRICS_COLLISIONS") {
			int val;
			if (getBoolValue (e.second, val )) {
				settings.checkLyricsCollisions = val ? true : false;
				retcode = true;
			}
		}
	}
	return retcode;
}

//--------------------------------------------------------------------------
void GuidoParser::setStream(std::istream *stream)
{
    if (stream)
        fStream = stream;
}

//--------------------------------------------------------------------------
void GuidoParser::noteInit (const char *id)
{
	faccidentals = 0;
	fzaehlerSet = 0;
	fFactory->createEvent(id);
}

//--------------------------------------------------------------------------
void GuidoParser::tabInit (int string, const char *disp)
{
	faccidentals = 0;
	fzaehlerSet = 0;
	fFactory->createTab(string, disp);
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
void GuidoParser::tagParameter (TagParameter* param)					{ if (param) fFactory->addTagParameter(param); }
TagParameter* GuidoParser::floatParam	(float val)						{ return new TagParameterFloat (val); }
TagParameter* GuidoParser::intParam		(int val)						{ return new TagParameterInt (val); }
TagParameter* GuidoParser::strParam		(const char* val)				{ return new TagParameterString (val); }
TagParameter* GuidoParser::floatParam	(float val, const char* unit)
{
	TagParameterFloat* p  = new TagParameterFloat (val);
	p->setUnit (unit);
	return p;
}
TagParameter* GuidoParser::intParam	(int val, const char* unit)
{
	TagParameterInt* p  = new TagParameterInt (val);
	p->setUnit (unit);
	return p;
}
void GuidoParser::setParamName (TagParameter* p, const char* name) { p->setName (name); }

//--------------------------------------------------------------------------
TagParameter* GuidoParser::varParam (const char* name)
{
	variable var;
	if (!getVariable (name, var)) {
		cerr << "unknown variable " << name << " used as tag parameter" << endl;
		return 0;
	}
	
	switch (var.fType) {
		case GuidoParser::kString:	return strParam (var.fValue.c_str());
		case GuidoParser::kInt:		return intParam (std::stoi (var.fValue));
		case GuidoParser::kFloat:	return floatParam (std::stof (var.fValue));
	}
	return 0;
}

//--------------------------------------------------------------------------
// return the next char in stream or in expanded variable
bool GuidoParser::get(char& c)
{
	if (!fVStreams.empty()) {
		vareval& v = fVStreams.top();
		c = *v.fPtr++;
		if (c) return true;

		fVStreams.pop();
		c = ' ';
		return true;
	}

	fStream->get(c);
	return !fStream->eof();
}

//--------------------------------------------------------------------------
bool GuidoParser::variableSymbols (const char* name)
{
	variable var;
	if (!getVariable (name, var)) {
		cerr << "unknown variable " << name << endl;
		return false;
	}

	vareval v;
	v.fName = name;
	v.fValue = var.fValue;
	fVStreams.push (v);
//cerr << "GuidoParser::variableSymbols push '" << v.fName << "' : '" << v.fValue << "'" << endl;
	vareval& tmp = fVStreams.top();
	tmp.fPtr = tmp.fValue.c_str();
	return true;
}

//--------------------------------------------------------------------------
bool GuidoParser::getVariable (const char* name, variable& var)
{
	map<string, variable>::const_iterator i = fEnv.find (name);
	if (i == fEnv.end()) return false;
	var = i->second;
	return true;
}

//--------------------------------------------------------------------------
void GuidoParser::variableDecl (const char* name, const char* value, vartype type)
{
	GuidoParser::variable var;
	var.fValue = value;
	var.fType = type;
	fEnv[name] = var;
}

//--------------------------------------------------------------------------
void GuidoParser::parseError(int line, int column, const char* msg)
{
	extern int	gParseErrorLine;
	gParseErrorLine = line;		// for backward compatibility only
	setError (line, column, msg);
	if (fVStreams.size()) {
		vareval& v = fVStreams.top();
		cerr << "while parsing variable " << v.fName << endl;
		do {
			fVStreams.pop();
		} while (fVStreams.size());
	}
	std::cerr << "error line: " << line << " col: " << column << ": " << msg << std::endl;
}

//--------------------------------------------------------------------------
ARHandler GuidoParser::parse()
{
    /* Parser reinitialization */
	fzaehlerSet = 0;
	faccidentals = 0;
	fndots = 0;
	fnt_enumSet = false;
	fnt_enum = 0;
	fnt_denom = 1;
	fErrorLine = fErrorColumn = 0;
    setlocale(LC_NUMERIC, "C");
    fText = "";

    destroyScanner();
    initScanner();

    delete fFactory;
    fFactory = new ARFactory();
    fEnv.clear();

	_yyparse ();

	if (fErrorLine == 0) {
		ARHandler ar = GuidoFactoryCloseMusic (fFactory);
		GuidoLayoutSettings settings;
		GuidoGetDefaultLayoutSettings (&settings);
		if (getSettings(settings)) {
			 ar->fEngineSettings = new GuidoLayoutSettings;
			 *(ar->fEngineSettings) = settings;
		}
		else ar->fEngineSettings = nullptr;
		return ar;
	}
    
    return 0;
//    return (fErrorLine == 0 ? GuidoFactoryCloseMusic (fFactory) : 0);
}
