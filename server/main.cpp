i
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
#include "profport.h"

using namespace std;
using namespace guidohttpd;

#define kVersion 0.61f
#define kVersionStr "0.61"

static const char* kPortOpt = "--port";
static const int kDefaultPort = 8000;

static const char* kVersionOpt = "--version";

static const char* kVerboseOpt = "--verbose";
static const int kDefaultVerbose = IP_VERBOSE | HEADER_VERBOSE
  | REQUEST_VERBOSE | URL_VERBOSE | QUERY_VERBOSE
  | CODE_VERBOSE | MIME_VERBOSE | LENGTH_VERBOSE;

static const char* kLogfileOpt = "--logfile";
static const string kDefaultLogfile = "guidohttpdserver.log";

static const char* kInitfileOpt = "--initfile";
static const string kDefaultInitfile = "guidohttpdserver.ini";

static const char* kLogmodeOpt = "--logmode";
static const int kDefaultLogmode = 0;

static const char* kCachedirOpt = "--cachedir";
static const string kDefaultCachedir = "cache";

static const char* kSafeOpt = "--daemon";

static const char* kHelpOpt = "--help";
static const char* kShortHelpOpt = "-h";

static const char* kSvgFontFileOpt = "--svgfontfile";
static const string kDefaultSvgFontFile = "guido2.svg";

//---------------------------------------------------------------------------------
// for the initfile
static const char* logSectionName = "Log";
static const char* logModeName = "mode";
static const char* logFilenameName = "filename";

static const char* fontSectionName = "SVG Font";
static const char* fontFilenameName = "filename";

static const char* cacheSectionName = "Cache";
static const char* cacheDirectoryName = "directory";

static const char* daemonSectionName = "Daemon";
static const char* daemonOnName = "on";

static const char* portSectionName = "Port";
static const char* portNumberName = "number";

//---------------------------------------------------------------------------------
static void usage (char* name)
{
#ifndef WIN32
    name = basename (name);
#endif
    const char *tab = "      ";
    cout << "usage: " << name << " [ options ]" << endl;
    cout << "where options are in:" << endl;
    cout << tab << kPortOpt << " portnum : sets the communication port number (defaults to " << kDefaultPort << ")"<< endl;
    cout << tab << kSafeOpt  << " : used with nohup to make this run as a daemon" << endl;
    cout << tab << kInitfileOpt << " init file name : (defaults to " << kDefaultInitfile << " in the directory of the current executable)" << endl;
    cout << tab << kLogfileOpt << " log file name : (defaults to " << kDefaultLogfile << " in the directory of the current executable - use an empty string to write to STDOUT)" << endl;
    cout << tab << kLogmodeOpt << " log file mode : (defaults to " << kDefaultLogmode << ")" << endl;
    cout << tab << tab << "0 = Apache-like log" << endl;
    cout << tab << tab << "1 = XML logfile" << endl;
    cout << tab << kCachedirOpt << " cache dir name : (defaults to a directory cache in the directory of the current executable)" << endl;
    cout << tab << kSvgFontFileOpt << " name of the svg font file - defaults to a file called " << kDefaultSvgFontFile << " in the executable's directory" << endl;
    cout << tab << kVersionOpt << " version of the server and GUIDO" << endl;
/*
    cout << tab << kVerboseOpt << " verbosity. an integer bitmap that can combine:" << endl;
    cout << tab << tab << "1 (print ip to log)" << endl;
    cout << tab << tab << "2 (print header info pairs to log)" << endl;
    cout << tab << tab << "4 (print method type to log)" << endl;
    cout << tab << tab << "8 (print url to log)" << endl;
    cout << tab << tab << "16 (print url-encoded query pairs to log)" << endl;
    cout << tab << tab << "32 (print return code string to log)" << endl;
    cout << tab << tab << "64 (print mime type string to log)" << endl;
    cout << tab << tab << "128 (print length of return object)" << endl;
    cout << tab << tab << "example: --verbose 25 will print ip, url and query pairs" << endl;
    cout << tab << tab << "default is 255, or everything" << endl;
*/
    cout << tab << kHelpOpt << " : print this help message and exit" << endl;
    cout << "to run as a daemon, do: " << endl;
    cout << tab << "nohup /path/to/guidohttpserver/executable --daemon &" << endl;
}

//---------------------------------------------------------------------------------
static bool launchServer (int port, int verbose, int logmode, string cachedir, string svgfontfile, bool daemon)
{
    bool ret = false;
    guido2img converter;
    startEngine();
    HTTPDServer server(verbose, logmode, cachedir, svgfontfile, &converter);
    server.readFromCache();
    if (server.start(port)) {
        if (daemon) {
            log << "Guido server v." << kVersionStr << " with Guido v." << GuidoGetVersionStr() << " is running on port " << port << logend;
            while (true) { }
        } else {
            cout << "Guido server v." << kVersionStr << " with Guido v." << GuidoGetVersionStr() << " is running on port " << port << endl;
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
    if (bopt (argv, kHelpOpt, false) || bopt (argv, kShortHelpOpt, false)) {
        usage (argv[0]);
        exit (0);
    }
    if (bopt (argv, kVersionOpt, false)) {
        cout << "Guido server v." << kVersionStr << " with Guido v." << GuidoGetVersionStr() << "." << endl;
        exit (0);
    }
    QApplication app(argc , argv); // required by Qt
    char resolved_path[256];
    #ifdef WIN32
      _fullpath(resolved_path, ".");
    #else
      realpath(".", resolved_path);
    #endif
    string applicationPath(resolved_path);

    srand(time(0));
    int port = get_private_profile_int(portSectionName, portNumberName, lopt (argv, kPortOpt, kDefaultPort), kDefaultInitfile.c_str());

    (void) kVerboseOpt;
    int verbose = kDefaultVerbose;//lopt (argv, kVerboseOpt, kDefaultVerbose);

    int logmode = get_private_profile_int(logSectionName, logModeName, lopt (argv, kLogmodeOpt, kDefaultLogmode), kDefaultInitfile.c_str());
    if (logmode > 1)
      logmode = 1;
    if (logmode < 0)
      logmode = 0;

    char buff[512];
    string logfile = sopt (argv, kLogfileOpt, (applicationPath + "/" + kDefaultLogfile).c_str());
    get_private_profile_string(logSectionName, logFilenameName, logfile.c_str(), buff, 512, kDefaultInitfile.c_str());
    logfile = string(buff);

    bool daemon = bopt (argv, kSafeOpt, false);
    daemon = get_private_profile_int(daemonSectionName, daemonOnName, daemon ? 1 : 0, kDefaultInitfile.c_str()) ? true : false;
    gLog = logfile != ""
           ? new logstream (logfile.c_str())
           : new logstream();

    string cachedir = sopt (argv, kCachedirOpt, (applicationPath + "/" + kDefaultCachedir).c_str());
    get_private_profile_string(cacheSectionName, cacheDirectoryName, cachedir.c_str(), buff, 512, kDefaultInitfile.c_str());
    cachedir = string(buff);

    string svgfontfile = sopt (argv, kCachedirOpt, (applicationPath + "/" + kDefaultSvgFontFile).c_str());
    get_private_profile_string(fontSectionName, fontFilenameName, svgfontfile.c_str(), buff, 512, kDefaultInitfile.c_str());
    svgfontfile = string(buff);

    // check to see if svgfontfile exists

    ifstream myfile(svgfontfile.c_str());
    if (!myfile.is_open())
      svgfontfile = "";

    if (daemon) {
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
    return launchServer (port, verbose, logmode, cachedir, svgfontfile, daemon) ? 0 : 1;
}
