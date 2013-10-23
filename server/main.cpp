
#ifndef WIN32
#include <libgen.h>
#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#include <sys/stat.h>

#include <ctime>

/*
 * For Linux, otherwise:
 * /usr/bin/ld: errno: TLS definition in /lib/x86_64-linux-gnu/libc.so.6 section .tbss mismatches non-TLS reference in main.o
 * /lib/x86_64-linux-gnu/libc.so.6: could not read symbols: Bad value
 */
#include <errno.h>

#include <QApplication>

#include "QGuidoPainter.h"
#include "guido2img.h"
#include "server.h"
#include "utilities.h"
#include "engine.h"

using namespace std;
using namespace guidohttpd;

#define kVersion 0.50f
#define kVersionStr "0.50"

static const char* kPortOpt = "--port";
static const int kDefaultPort = 8000;

static const char* kLogfileOpt = "--logfile";
static const string kDefaultLogfile = "guidohttpdserver.log";

static const char* kSafeOpt = "--safe";
static const char* kHelpOpt = "--help";
static const char* kShortHelpOpt = "-h";

//---------------------------------------------------------------------------------
static void usage (char* name)
{
#ifndef WIN32
    name = basename (name);
#endif
    const char * tab = "             ";
    cout << "usage: " << name << " [ options ]" << endl;
    cout << "where options are in:" << endl;
    cout << tab << kPortOpt << " portnum : sets the communication port number (defaults to " << kDefaultPort << ")"<< endl;
    cout << tab << kSafeOpt  << " : closes all file descriptors and moves the program to a safe place " << endl;
    cout << tab << kLogfileOpt << " name : (defaults to " << kDefaultLogfile << "  - ignored when not in safe mode)" << endl;
    cout << tab << kHelpOpt << " : print this help message and exit" << endl;
}

//---------------------------------------------------------------------------------
static bool launchServer (int port, const char * logfile, bool safe)
{
    bool ret = false;
    guido2img converter;
    startEngine();
    HTTPDServer server(port, &converter);
    if (server.start(port)) {
        log << "Guido server v." << kVersionStr << " is running on port " << port << logend;
        if (safe) {
            while (true) { }
        } else {
            cout << "Type 'q' to quit" << endl;
            do {
                char c = getchar();
                if (c == 'q') {
                    break;
                }
            } while (true);
        }
        server.stop();
        ret = true;
    } else {
        log << "Can't start Guido httpd server on the specified port. Try a different port." << logend;
    }
    stopEngine();
    return ret;
}

//---------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    QCoreApplication app(argc , argv); // required by Qt
    srand(time(0));
    int port = lopt (argv, kPortOpt, kDefaultPort);
    string logfile = sopt (argv, kLogfileOpt, kDefaultLogfile);
    bool safe = bopt (argv, kSafeOpt, false);
    if (bopt (argv, kHelpOpt, false) || bopt (argv, kShortHelpOpt, false)) {
        usage (argv[0]);
        exit (0);
    }
    gLog = safe ? new logstream ( logfile.c_str() ) : new logstream();

    if (safe) {
        // below is commented out because of Mac OS X problems with daemons
        /*
        // Our process ID and Session ID
        // Fork off the parent process
        pid_t pid = fork();
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
        pid_t sid = setsid();
        if (sid < 0) {
            log << "SID creation failed (errno " << errno << ")" << logend;
            exit(EXIT_FAILURE);
        }
        */
        // Change the current working directory
        if ((chdir("/")) < 0) {
            log << "Could not change the directory to the root directory." << logend;
            exit(EXIT_FAILURE);
        }

        // Close out the standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }
    return launchServer (port, logfile.c_str(), safe) ? 0 : 1;
}

