#include "guidoengineadapter.h"
#include "GUIDOEngine.h"
#include "GUIDOParse.h"
#include <cstring>
#include <sstream>
#include "guido2.h"
#include "GuidoStream.h"

#ifdef CANVASSYSTEM
#include "canvassystem.h"
#include "canvasdevice.h"
#endif

GuidoEngineAdapter::GuidoEngineAdapter()
{
}

GuidoEngineAdapter::~GuidoEngineAdapter()
{
}


GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoInit(GuidoInitDesc * desc)
{
	return ::GuidoInit(desc);
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoInitWithIndependentSVG()
{
	return ::GuidoInitWithIndependentSVG();
}

#ifdef CANVASSYSTEM
GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoInitWithJavascript() {
	GuidoInitDesc desc;

	VGSystem * gSystem= new CanvasSystem(reinterpret_cast<char *>(______src_guido2_svg));
	desc.graphicDevice = gSystem->CreateMemoryDevice(20,20);
	desc.musicFont = "Guido2";
	desc.textFont  = "Times";
	return GuidoInit (&desc);
}
#endif

GUIDOAPI(void) GuidoEngineAdapter::guidoShutdown()
{
	::GuidoShutdown();
}

GUIDOAPI(GRHandler) GuidoEngineAdapter::guidoAR2GR(ARHandler ar)
{
	GRHandler gr;
	GuidoErrCode err = ::GuidoAR2GR (ar, 0, &gr);
	return err ? 0 : gr;
}

GUIDOAPI(GRHandler) GuidoEngineAdapter::guidoAR2GR(ARHandler ar, const GuidoLayoutSettings &settings)
{
	GRHandler gr;
	GuidoErrCode err = ::GuidoAR2GR (ar, &settings, &gr);
	return err ? 0 : gr;
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoUpdateGR(GRHandler gr)
{
	return ::GuidoUpdateGR(gr, 0);
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoUpdateGR(GRHandler gr, const GuidoLayoutSettings &settings)
{
	return ::GuidoUpdateGR(gr, &settings);
}

GUIDOAPI(void) GuidoEngineAdapter::guidoFreeAR(ARHandler ar)
{
	::GuidoFreeAR(ar);
}

GUIDOAPI(void) GuidoEngineAdapter::guidoFreeGR(GRHandler gr)
{
	::GuidoFreeGR(gr);
}

GUIDOAPI(string) GuidoEngineAdapter::guidoGetErrorString(GuidoErrCode errCode)
{
	return ::GuidoGetErrorString(errCode);
}

GUIDOAPI(GuidoLayoutSettings) GuidoEngineAdapter::guidoGetDefaultLayoutSettings()
{
	GuidoLayoutSettings settings;
	::GuidoGetDefaultLayoutSettings(&settings);
	return settings;
}

GUIDOAPI(int) GuidoEngineAdapter::guidoCountVoices(CARHandler inHandleAR)
{
	return ::GuidoCountVoices(inHandleAR);
}

GUIDOAPI(int) GuidoEngineAdapter::guidoGetPageCount(CGRHandler inHandleGR)
{
	return ::GuidoGetPageCount(inHandleGR);
}

GUIDOAPI(int) GuidoEngineAdapter::guidoGetSystemCount(CGRHandler inHandleGR, int page)
{
	return ::GuidoGetSystemCount(inHandleGR, page);
}

GUIDOAPI(GuidoDate) GuidoEngineAdapter::guidoDuration(CGRHandler inHandleGR)
{
	GuidoDate date;
	::GuidoDuration(inHandleGR, &date);
	return date;
}

GUIDOAPI(int) GuidoEngineAdapter::guidoFindEventPage(CGRHandler inHandleGR, const GuidoDate& date)
{
	return ::GuidoFindEventPage(inHandleGR, date);
}

GUIDOAPI(int) GuidoEngineAdapter::guidoFindPageAt(CGRHandler inHandleGR, const GuidoDate& date)
{
	return ::GuidoFindPageAt(inHandleGR, date);
}

GUIDOAPI(GuidoDate) GuidoEngineAdapter::guidoGetPageDate( CGRHandler inHandleGR, int pageNum)
{
	GuidoDate date;
	::GuidoGetPageDate( inHandleGR, pageNum, &date);
	return date;
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoOnDraw(GuidoOnDrawDesc * desc)
{
	return ::GuidoOnDraw(desc);
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoSVGExport(const GRHandler handle, int page, std::ostream& out, const char* fontfile, const char* fontspec)
{
	return ::GuidoSVGExportWithFontSpec(handle, page, out, fontfile, fontspec);
}

GUIDOAPI(string) GuidoEngineAdapter::guidoSVGExport(const GRHandler handle, int page)
{
	stringstream sstr;
	::GuidoSVGExportWithFontSpec(handle, page, sstr, 0, reinterpret_cast<char *>(______src_guido2_svg));
	return sstr.str();
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoAbstractExport(const GRHandler handle, int page, std::ostream& out)
{
	return ::GuidoAbstractExport(handle, page, out);
}

GUIDOAPI(string) GuidoEngineAdapter::guidoAbstractExport(const GRHandler handle, int page)
{
	stringstream sstr;
	::GuidoAbstractExport(handle, page, sstr);
	return sstr.str();
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoBinaryExport(const GRHandler handle, int page, std::ostream& out)
{
	return ::GuidoBinaryExport(handle, page, out);
}

GUIDOAPI(string) GuidoEngineAdapter::guidoBinaryExport(const GRHandler handle, int page)
{
	stringstream sstr;
	::GuidoBinaryExport(handle, page, sstr);
	return sstr.str();
}

#ifdef CANVASSYSTEM
GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoJavascriptExport(const GRHandler handle, int page)
{
	CanvasSystem sys(reinterpret_cast<char *>(______src_guido2_svg));
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

GUIDOAPI(void) GuidoEngineAdapter::guidoSetDrawBoundingBoxes(int bbMap)
{
	::GuidoDrawBoundingBoxes(bbMap);
}

GUIDOAPI(int) GuidoEngineAdapter::guidoGetDrawBoundingBoxes()
{
	return GuidoGetDrawBoundingBoxes();
}

GUIDOAPI(GuidoPageFormat) GuidoEngineAdapter::guidoGetPageFormat(CGRHandler inHandleGR, int pageNum)
{
	GuidoPageFormat pf;
	::GuidoGetPageFormat(inHandleGR, pageNum, &pf);
	return pf;
}

GUIDOAPI(void) GuidoEngineAdapter::guidoSetDefaultPageFormat(const GuidoPageFormat &format)
{
	::GuidoSetDefaultPageFormat(&format);
}

GUIDOAPI(GuidoPageFormat) GuidoEngineAdapter::guidoGetDefaultPageFormat()
{
	GuidoPageFormat pf;
	::GuidoGetDefaultPageFormat(&pf);
	return pf;
}

GUIDOAPI(float) GuidoEngineAdapter::guidoUnit2CM(float val)
{
	return ::GuidoUnit2CM(val);
}

GUIDOAPI(float) GuidoEngineAdapter::guidoCM2Unit(float val)
{
	return ::GuidoCM2Unit(val);
}

GUIDOAPI(float) GuidoEngineAdapter::guidoUnit2Inches(float val)
{
	return ::GuidoUnit2Inches(val);
}

GUIDOAPI(float) GuidoEngineAdapter::guidoInches2Unit(float val)
{
	return ::GuidoInches2Unit(val);
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoResizePageToMusic(GRHandler inHandleGR)
{
	return ::GuidoResizePageToMusic(inHandleGR);
}

GUIDOAPI(GuidoVersion) GuidoEngineAdapter::guidoGetVersion()
{
	GuidoVersion version;
	::GuidoGetVersionNums(&version.major, &version.minor, &version.sub);
	version.str = ::GuidoGetVersionStr();
	return version;
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoCheckVersionNums(int major, int minor, int sub)
{
	return ::GuidoCheckVersionNums(major, minor, sub);
}

GUIDOAPI(float) GuidoEngineAdapter::guidoGetLineSpace()
{
	return ::GuidoGetLineSpace();
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoMarkVoice(ARHandler inHandleAR, int voicenum, const GuidoDate & date,
									  const GuidoDate & duration, unsigned char red, unsigned char green,
									  unsigned char blue)
{
	return ::GuidoMarkVoice(inHandleAR, voicenum, date, duration, red, green, blue);
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoSetSymbolPath(ARHandler inHandleAR, const std::vector<std::string> &inPaths)
{
	return ::GuidoSetSymbolPath(inHandleAR, inPaths);
}
GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoGetSymbolPath(const ARHandler inHandleAR, std::vector<std::string> &inPathVector)
{
	return ::GuidoGetSymbolPath(inHandleAR, inPathVector);
}

// Parser section
GUIDOAPI(GuidoParser *) GuidoEngineAdapter::guidoOpenParser()
{
	return ::GuidoOpenParser();
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoCloseParser(GuidoParser * parser)
{
	return ::GuidoCloseParser(parser);
}

GUIDOAPI(ARHandler)	GuidoEngineAdapter::guidoFile2AR(GuidoParser *p, const string &file)
{
	return ::GuidoFile2AR(p, file.c_str());
}


GUIDOAPI(ARHandler) GuidoEngineAdapter::guidoString2AR(GuidoParser * p, const string &gmnCode)
{
	return ::GuidoString2AR (p, gmnCode.c_str());
}

GUIDOAPI(string) GuidoEngineAdapter::guidoGetStream(GuidoStream *gStream)
{
	// Don't use GuidoGetStream because there is a problem with char * pointer in emscripten
	// return ::GuidoGetStream(gStream);

	return gStream->getGlobalStringStream()->str();
}

GUIDOAPI(ARHandler) GuidoEngineAdapter::guidoStream2AR(GuidoParser *p, GuidoStream* stream)
{
	return ::GuidoStream2AR(p, stream);
}

GUIDOAPI(ParserError) GuidoEngineAdapter::guidoParserGetErrorCode(GuidoParser *p)
{
	ParserError parseErr;
	const char * msg;
	::GuidoParserGetErrorCode(p, parseErr.line, parseErr.col, &msg);
	if(msg) {
		parseErr.msg = msg;
	}
	return parseErr;
}

GUIDOAPI(GuidoStream *) GuidoEngineAdapter::guidoOpenStream()
{
	return ::GuidoOpenStream();
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoCloseStream(GuidoStream *s)
{
	return ::GuidoCloseStream(s);
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoWriteStream(GuidoStream *s, const string &str)
{
	return ::GuidoWriteStream(s, str.c_str());
}

GUIDOAPI(GuidoErrCode) GuidoEngineAdapter::guidoResetStream(GuidoStream *s)
{
	return ::GuidoResetStream(s);
}
