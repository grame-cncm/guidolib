/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4800)
#endif

#include <sstream>

#include "GUIDOEngineAdapter.h"
#include "GMNCodePrintVisitor.h"
#include "GUIDOInternal.h"
#include "GUIDOParse.h"
#include "guido2.h"
#include "GuidoStream.h"
#include "ARMusic.h"

#ifdef CANVASSYSTEM
#include "canvassystem.h"
#include "canvasdevice.h"
#endif

using namespace std;

//--------------------------------------------------------------
GuidoEngineAdapter::GuidoEngineAdapter()	{}
GuidoEngineAdapter::~GuidoEngineAdapter()	{}


//--------------------------------------------------------------
GuidoErrCode GuidoEngineAdapter::init(GuidoInitDesc * desc)		{	return ::GuidoInit(desc); }
GuidoErrCode GuidoEngineAdapter::init()							{	return ::GuidoInitWithIndependentSVG(); }
void GuidoEngineAdapter::shutdown()								{	::GuidoShutdown(); }

//--------------------------------------------------------------
GuidoErrCode GuidoEngineAdapter::ar2gmn(ARHandler ar, ostream& to)
{
	if ((!ar) || ( ar->armusic == 0 ))
		return guidoErrInvalidHandle;
		
	ARMusic* score = ar->armusic;
	GMNCodePrintVisitor v(to);
	score->accept(v);
//	score->goThrough(&v);
	return guidoNoErr;
}

//--------------------------------------------------------------
GRHandler GuidoEngineAdapter::ar2gr(ARHandler ar)
{
	GRHandler gr;
	GuidoErrCode err = ::GuidoAR2GR (ar, 0, &gr);
	return err ? 0 : gr;
}

//--------------------------------------------------------------
GRHandler GuidoEngineAdapter::ar2gr(ARHandler ar, const GuidoLayoutSettings &settings)
{
	GRHandler gr;
	GuidoErrCode err = ::GuidoAR2GR (ar, &settings, &gr);
	return err ? 0 : gr;
}

//--------------------------------------------------------------
GuidoErrCode GuidoEngineAdapter::updateGR(GRHandler gr)			{	return ::GuidoUpdateGR(gr, 0); }
GuidoErrCode GuidoEngineAdapter::updateGR(GRHandler gr, const GuidoLayoutSettings &settings) {	return ::GuidoUpdateGR(gr, &settings); }
void GuidoEngineAdapter::freeAR(ARHandler ar)					{	::GuidoFreeAR(ar); }
void GuidoEngineAdapter::freeGR(GRHandler gr)					{	::GuidoFreeGR(gr); }
string GuidoEngineAdapter::getErrorString(GuidoErrCode errCode)	{	return ::GuidoGetErrorString(errCode); }

GuidoLayoutSettings GuidoEngineAdapter::getDefaultLayoutSettings()
{
	GuidoLayoutSettings settings;
	::GuidoGetDefaultLayoutSettings(&settings);
	return settings;
}

int GuidoEngineAdapter::countVoices(CARHandler inHandleAR)		{	return ::GuidoCountVoices(inHandleAR); }
int GuidoEngineAdapter::getPageCount(CGRHandler inHandleGR)		{	return ::GuidoGetPageCount(inHandleGR); }
int GuidoEngineAdapter::getSystemCount(CGRHandler inHandleGR, int page)	{	return ::GuidoGetSystemCount(inHandleGR, page); }

GuidoDate GuidoEngineAdapter::duration(CGRHandler inHandleGR)
{
	GuidoDate date;
	::GuidoDuration(inHandleGR, &date);
	return date;
}

int GuidoEngineAdapter::findEventPage(CGRHandler inHandleGR, const GuidoDate& date)		{	return ::GuidoFindEventPage(inHandleGR, date); }
int GuidoEngineAdapter::findPageAt(CGRHandler inHandleGR, const GuidoDate& date)		{	return ::GuidoFindPageAt(inHandleGR, date); }

GuidoDate GuidoEngineAdapter::getPageDate( CGRHandler inHandleGR, int pageNum)
{
	GuidoDate date;
	::GuidoGetPageDate( inHandleGR, pageNum, &date);
	return date;
}

long GuidoEngineAdapter::getParsingTime (const ARHandler ar)		{ return ::GuidoGetParsingTime(ar); }
long GuidoEngineAdapter::getAR2GRTime(const GRHandler gr)			{ return ::GuidoGetAR2GRTime(gr); }
long GuidoEngineAdapter::getOnDrawTime(const GRHandler gr)			{ return ::GuidoGetOnDrawTime(gr); }

GuidoErrCode GuidoEngineAdapter::showElement( GRHandler gr, int elt, bool status)
																	{ return ::GuidoShowElement(gr, GRElement(elt), status); }

GuidoErrCode GuidoEngineAdapter::onDraw(GuidoOnDrawDesc * desc)		{ return ::GuidoOnDraw(desc); }

std::string gr2SVG( const GRHandler handle, int page, std::ostream& out, int w, int h, bool embedFont)
{
	stringstream sstr;
	::GuidoGR2SVG1(handle, page, sstr, embedFont, w, h);
	return sstr.str();
}

GuidoErrCode GuidoEngineAdapter::gr2SVG( const GRHandler handle, int page, std::ostream& out, bool embedFont, const char* font, const int mappingMode)
{
	return ::GuidoGR2SVG(handle, page, out, embedFont, font, mappingMode);
}

std::string GuidoEngineAdapter::gr2SVG( const GRHandler handle, int page, bool embedFont, const int mappingMode)
{
	stringstream sstr;
	::GuidoGR2SVG(handle, page, sstr, embedFont, 0, mappingMode);
	return sstr.str();
}

std::string GuidoEngineAdapter::gr2SVGColored( const GRHandler handle, int page, int r, int g, int b, bool embedfont)
{
	stringstream sstr;
	VGColor color (r, g, b);
	::GuidoGR2SVGColored(handle, page, sstr, color, embedfont);
	return sstr.str();
}

GuidoErrCode GuidoEngineAdapter::abstractExport(const GRHandler handle, int page, std::ostream& out)
{
	return ::GuidoAbstractExport(handle, page, out);
}

string GuidoEngineAdapter::abstractExport(const GRHandler handle, int page)
{
	stringstream sstr;
	::GuidoAbstractExport(handle, page, sstr);
	return sstr.str();
}

GuidoErrCode GuidoEngineAdapter::binaryExport(const GRHandler handle, int page, std::ostream& out)
{
	return ::GuidoBinaryExport(handle, page, out);
}

string GuidoEngineAdapter::binaryExport(const GRHandler handle, int page)
{
	stringstream sstr;
	::GuidoBinaryExport(handle, page, sstr);
	return sstr.str();
}

#ifdef CANVASSYSTEM
GuidoErrCode GuidoEngineAdapter::javascriptExport(const GRHandler handle, int page)
{
	CanvasSystem sys(_src_guido2_svg);
	CanvasDevice dev(&sys);

	GuidoOnDrawDesc desc;              // declare a data structure for drawing
	desc.handle = handle;

	GuidoPageFormat	pf;
	GuidoResizePageToMusic (handle);
	GuidoGetPageFormat (handle, page, &pf);

	desc.hdc = &dev;                    // we'll draw on the device
	desc.page = page;
	desc.updateRegion.erase = true;     // and draw everything
	desc.scrollx = desc.scrolly = 0;    // from the upper left page corner
	desc.sizex = pf.width;
	desc.sizey = pf.height;
	dev.NotifySize(desc.sizex, desc.sizey);
	dev.SelectPenColor(VGColor(0,0,0));
	return GuidoOnDraw (&desc);
}
#endif

void GuidoEngineAdapter::setDrawBoundingBoxes(int bbMap)
{
	::GuidoDrawBoundingBoxes(bbMap);
}

int GuidoEngineAdapter::getDrawBoundingBoxes()
{
	return GuidoGetDrawBoundingBoxes();
}

GuidoPageFormat GuidoEngineAdapter::getPageFormat(CGRHandler inHandleGR, int pageNum)
{
	GuidoPageFormat pf;
	::GuidoGetPageFormat(inHandleGR, pageNum, &pf);
	return pf;
}

void GuidoEngineAdapter::setDefaultPageFormat(const GuidoPageFormat &format)
{
	::GuidoSetDefaultPageFormat(&format);
}

GuidoPageFormat GuidoEngineAdapter::getDefaultPageFormat()
{
	GuidoPageFormat pf;
	::GuidoGetDefaultPageFormat(&pf);
	return pf;
}

float GuidoEngineAdapter::unit2CM(float val)		{	return ::GuidoUnit2CM(val);}
float GuidoEngineAdapter::cm2Unit(float val)		{	return ::GuidoCM2Unit(val);}
float GuidoEngineAdapter::unit2Inches(float val)	{	return ::GuidoUnit2Inches(val);}
float GuidoEngineAdapter::inches2Unit(float val)	{	return ::GuidoInches2Unit(val); }

GuidoErrCode GuidoEngineAdapter::resizePageToMusic(GRHandler inHandleGR)
{
	return ::GuidoResizePageToMusic(inHandleGR);
}

GuidoVersion GuidoEngineAdapter::getVersion()
{
	GuidoVersion version;
	::GuidoGetVersionNums(&version.major, &version.minor, &version.sub);
	version.str = ::GuidoGetVersionStr();
	return version;
}
string 	GuidoEngineAdapter::getVersionStr()
{
	return ::GuidoGetVersionStr();
}

GuidoErrCode GuidoEngineAdapter::checkVersionNums(int major, int minor, int sub)
{
	return ::GuidoCheckVersionNums(major, minor, sub);
}

float GuidoEngineAdapter::getLineSpace()
{
	return ::GuidoGetLineSpace();
}

GuidoErrCode GuidoEngineAdapter::markVoice(ARHandler inHandleAR, int voicenum, const GuidoDate & date,
									  const GuidoDate & duration, unsigned char red, unsigned char green,
									  unsigned char blue)
{
	return ::GuidoMarkVoice(inHandleAR, voicenum, date, duration, red, green, blue);
}

GuidoErrCode GuidoEngineAdapter::setSymbolPath(ARHandler inHandleAR, const std::vector<std::string> &inPaths)
{
	return ::GuidoSetSymbolPath(inHandleAR, inPaths);
}

std::vector<std::string> GuidoEngineAdapter::getSymbolPath(const ARHandler inHandleAR)
{
	std::vector<std::string> inPathVector;
	::GuidoGetSymbolPath(inHandleAR, inPathVector);
	return inPathVector;
}

// Parser section
GuidoParser * GuidoEngineAdapter::openParser()								{	return ::GuidoOpenParser(); }
GuidoErrCode GuidoEngineAdapter::closeParser(GuidoParser * p)				{	return ::GuidoCloseParser(p); }
ARHandler GuidoEngineAdapter::file2AR(GuidoParser *p, const string &file)	{	return ::GuidoFile2AR(p, file.c_str()); }
ARHandler GuidoEngineAdapter::string2AR(GuidoParser * p, const string &gmn)	{	return ::GuidoString2AR (p, gmn.c_str()); }

string GuidoEngineAdapter::getStream(const GuidoStream *gStream) const
{
	// Don't use GuidoGetStream because there is a problem with char * pointer in emscripten
	// return ::GuidoGetStream(gStream);

	return gStream->getStreamStr();
}

ARHandler GuidoEngineAdapter::stream2AR(GuidoParser *p, GuidoStream* stream)
{
	return ::GuidoStream2AR(p, stream);
}

ParserError GuidoEngineAdapter::parserGetErrorCode(GuidoParser *p)
{
	ParserError parseErr;
	const char * msg;
	::GuidoParserGetErrorCode(p, parseErr.line, parseErr.col, &msg);
	if(msg) {
		parseErr.msg = msg;
	}
	return parseErr;
}

GuidoStream * GuidoEngineAdapter::openStream()						{	return ::GuidoOpenStream(); }
GuidoErrCode GuidoEngineAdapter::closeStream(GuidoStream *s)		{	return ::GuidoCloseStream(s); }
GuidoErrCode GuidoEngineAdapter::writeStream(GuidoStream *s, const string &str) {	return ::GuidoWriteStream(s, str.c_str()); }
GuidoErrCode GuidoEngineAdapter::resetStream(GuidoStream *s)		{	return ::GuidoResetStream(s); }

