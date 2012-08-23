
#ifndef WIN32
#include <libgen.h>
#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <QtGui/QApplication>

#include <sys/stat.h>

#include "QGuidoPainter.h"
#include "guido2img.h"
#include "HTTPDServer.h"
#include "utilities.h"


using namespace std;
using namespace guidohttpd;

#define kVersion	 0.50f
#define kVersionStr	"0.50"

static const char* kPortOpt	= "-port";
static const int kDefaultPort = 8000;

static const char* kLogfileOpt	= "-logfile";
static const string kDefaultLogfile = "guidohttpdserver.log";

static const char* kDaemonOpt	= "-daemon";

int main(int argc, char **argv)
{
    QApplication app( argc , argv );	// required by Qt
    int port = lopt (argv, kPortOpt, kDefaultPort);
    string logfile = sopt (argv, kLogfileOpt, kDefaultLogfile);
    bool daemon = bopt (argv, kDaemonOpt);

    if (daemon) {
        // Our process ID and Session ID
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
            write_to_log ("SID created for the child process less than 0.\n", logfile, daemon);
            exit(EXIT_FAILURE);
        }
            
        // Change the current working directory
        if ((chdir("/")) < 0) {
            write_to_log ("Could not change the directory to the root directory.\n", logfile, daemon);
            exit(EXIT_FAILURE);
        }
        
        // Close out the standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    guido2img converter;

    QGuidoPainter::startGuidoEngine();						// starts the guido engine
    HTTPDServer server(port, logfile, daemon, &converter);
    if (server.start(port)) {
        if (daemon) {
          while (true) { }
        }
        else {
            cout << "Guido server v." << kVersionStr << " is running on port " << port << endl;
            cout << "Type 'q' to quit" << endl;
            do {
                    char c = getchar();
                    if (c == 'q') break;
            } while (true);
        }
        server.stop();
        return 0;
    }
    else
      write_to_log ("Can't start Guido httpd server.\n", logfile, daemon);

    QGuidoPainter::stopGuidoEngine();						// stop the guido engine
    return 1;
}

