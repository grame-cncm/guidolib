#include "guidostreamsession.h"
#include <string>

using namespace std;

namespace guidohttpd
{
const string guidostreamsession::sFileBegin = "streamsession";

guidostreamsession::guidostreamsession(const std::string &svgfontfile, std::string id) : guidosession(svgfontfile, id)
{
	fStream = GuidoOpenStream();
}

guidostreamsession::~guidostreamsession()
{
	GuidoCloseStream(fStream);
}

guidoAPIresponse guidostreamsession::addData(string data) {
	if(fArh || fGrh) {
		GuidoFreeGR(fGrh);
		GuidoFreeAR(fArh);
		fGrh = 0;
		fArh = 0;
	}
	delete whyIFailed_;
	whyIFailed_ = 0;
	// Add gmn code to existing stream
	GuidoErrCode err = GuidoWriteStream(fStream, data.c_str());

	if (err != guidoNoErr) {
		return guidoAPIresponse(err);
	}
	// Update gmn code
	this->fGmnCode = GuidoGetStream(fStream);

	// Create Abstract representation
	GuidoParser *parser = GuidoOpenParser();
	fArh = GuidoStream2AR(parser, fStream);
	if (!fArh) {
	  int line;
	  int col;
	  const char *msg;
	  GuidoParserGetErrorCode (parser, line, col, &msg);
	  GuidoCloseParser(parser);
	  return guidoAPIresponse(guidoErrParse, line, col, string(msg));
	}
	GuidoCloseParser(parser);

	// Create Graphic representation
	fGrh = GuidoAR2GRParameterized(fArh, &sDefaultScoreParameters.guidoParameters);
	if (!fGrh) {
		return guidoAPIresponse(guidoErrActionFailed);
	}

	// Resize page to music.
	if (sDefaultScoreParameters.guidoParameters.layoutSettings.resizePage2Music) {
		GuidoErrCode err = GuidoResizePageToMusic (fGrh);
		if (err != guidoNoErr) {
			return guidoAPIresponse(err);
		}
	}
	return guidoAPIresponse::make_happy_response();
}

guidoAPIresponse guidostreamsession::reset() {
	// Add gmn code to existing stream
	GuidoErrCode err = GuidoResetStream(fStream);
	if (err != guidoNoErr) {
		return guidoAPIresponse(err);
	}
	if(fArh || fGrh) {
		GuidoFreeGR(fGrh);
		GuidoFreeAR(fArh);
		fGrh = 0;
		fArh = 0;
	}
	delete whyIFailed_;
	whyIFailed_ = 0;
	return guidoAPIresponse::make_happy_response();
}

}
