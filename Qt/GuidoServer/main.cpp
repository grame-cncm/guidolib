
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <string>
#include <stdlib.h>
#include <QtGui/QApplication>

#include "QGuidoPainter.h"
#include "guido2img.h"
#include "HTTPDServer.h"

using namespace std;
using namespace guidohttpd;

#define kVersion	 0.50f
#define kVersionStr	"0.50"

static const char* kPortOpt	= "-port";
static const int kDefaultPort = 8000;

//--------------------------------------------------------------------------
// utility for command line arguments 
//--------------------------------------------------------------------------
static int getPortOption (int argc, char *argv[], const std::string& option, int defaultValue)
{
	for (int i=0; i < argc-1; i++) {
		if (option == argv[i]) {
			int val = strtol( argv[i+1], 0, 10);
			if (val) return val;
		}
	}
	return defaultValue;
}

//--------------------------------------------------------------------------
int main(int argc, char **argv)
{
	int port = getPortOption (argc, argv, kPortOpt, kDefaultPort);
	QApplication app( argc , argv );	// required by Qt
	guido2img converter;

	QGuidoPainter::startGuidoEngine();						// starts the guido engine
	HTTPDServer server(port, &converter);
	if (server.start(port)) {
		cout << "Guido server v." << kVersionStr << " is running on port " << port << endl;
		cout << "Type 'q' to quit" << endl;
		do {
			char c = getchar();
			if (c == 'q') break;
		} while (true);
		server.stop();
		return 0;
	}
	cerr << "can't start Guido httpd server" << endl;
	QGuidoPainter::stopGuidoEngine();						// stop the guido engine
	return 1;
}

