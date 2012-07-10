
#ifndef WIN32
#include <libgen.h>
#endif
#include <stdio.h>
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
#include <sys/stat.h>
//--------------------------------------------------------------------------
int main(int argc, char **argv)
{
    QApplication app( argc , argv );	// required by Qt
    // Our process ID and Session ID
    /*
    pid_t pid, sid;
    // Fork off the parent process
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    // If we got a good PID, then we can exit the parent process.
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
   
    // Change the file mode mask
    umask(0);
    
    // Open any logs here
    
    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0) {
        // Log the failure
        exit(EXIT_FAILURE);
    }
    
    
    
    // Change the current working directory
    if ((chdir("/")) < 0) {
        // Log the failure
        exit(EXIT_FAILURE);
    }
    
    // Close out the standard file descriptors
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);
*/
	int port = getPortOption (argc, argv, kPortOpt, kDefaultPort);
	guido2img converter;

	QGuidoPainter::startGuidoEngine();						// starts the guido engine
	HTTPDServer server(port, &converter);
	if (server.start(port)) {
		//cout << "Guido server v." << kVersionStr << " is running on port " << port << endl;
		//cout << "Type 'q' to quit" << endl;
        while (true) { }
        /*
		do {
			char c = getchar();
			if (c == 'q') break;
		} while (true);
         */
		server.stop();
		return 0;
	}
	//cerr << "can't start Guido httpd server" << endl;
	QGuidoPainter::stopGuidoEngine();						// stop the guido engine
	return 1;
}

