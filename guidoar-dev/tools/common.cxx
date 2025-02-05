
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#ifdef WIN32
# pragma warning (disable : 4786 4996)
static const char* basename (const char* name) {
	const char * sep = strrchr (name, '\\');
	return sep ? ++sep : name;
}
# define _CRT_SECURE_NO_DEPRECATE
#else 
# include <libgen.h>
#endif


#include "libguidoar.h"

using namespace std;
using namespace guido;

static const char* scoredesc = 	"score may refer to a file, standard input when '-' or litteral gmn code";

//_______________________________________________________________________________
bool floatVal (const char* str, float& val) 
{
	int n = sscanf(str, "%f", &val);
	return n == 1;
}

//_______________________________________________________________________________
bool intVal (const char* str, int& val)  
{
	int n = sscanf(str, "%d", &val);
	return n == 1;
}

//_______________________________________________________________________________
bool rationalVal (const char* str, rational& val)  
{
	int num, denom;
	int n = sscanf(str, "%d/%d", &num, &denom);
	if (n==2) {
		val.set (num, denom);
		return true;
	}
	return false;
}

//_______________________________________________________________________________
static bool checkfile (const char* file) 
{
	FILE* fd = fopen(file, "r");
	if (fd) {
		fclose (fd);
		return true;
	}
	return false;
}

//_______________________________________________________________________________
static bool readfile (FILE * fd, string& content) 
{
	if (!fd) return false;
	do {
		int c = getc(fd);
		if (feof(fd) || ferror(fd)) break;
		content += c;
	} while (true);
	return ferror(fd) == 0;
}

//_______________________________________________________________________________
void error(garErr err)
{
	switch (err) {
		case kNoErr:
			cerr << "no error" << endl;
			break;
		case kInvalidFile:
			cerr << "invalid file" << endl;
			break;
		case kInvalidArgument:
			cerr << "invalid argument" << endl;
			break;
		case kOperationFailed:
			cerr << "operation failed" << endl;
			break;
	}
}

//_______________________________________________________________________________
static bool gmnVal (const char* str, string& val, string& _stdin)  
{
	string pipe ("-");
	if (pipe == str) {			// value to be read from stdin
		if (_stdin.empty() && !readfile (stdin, _stdin)) goto err;
		val = _stdin;
	}
	else if (checkfile (str)) {	// value to be read from a file
		FILE* fd = fopen(str, "r");
		bool ret = readfile (fd, val);
		fclose (fd);
		if (!ret) goto err;
	}	
	else val = str;				// not a file, assume it is gmn code
	return true;

err:
	cerr << "error while reading score \"" << str << "\"" << endl;
	return false;	
}
