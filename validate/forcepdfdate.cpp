
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#ifndef WIN32
# include <libgen.h>
#else
# define basename(a) a
#endif
#include <string.h>

#include <iostream>

using namespace std;

static void usage (char * name)
{
	cout << "usage: " << basename(name) << " date <files>" << endl;
	cout << "       where date should have the form 'yyyymmddhhmmss'" << endl;
	exit (1);
}

static bool checkdate (const char* date)
{
	if (strlen(date) != 14) return false;	
	while (*date) {
		if ((*date < '0') || (*date > '9'))
			return false;
		date++;
	}
	return true;
}

const char* cdatestart = "CreationDate (D:";
static bool creationdate (FILE* fd)
{
	char buffer[20];
	int size = strlen (cdatestart);
	int n = fread (buffer, 1, size, fd);
	if (n == size) {
		buffer[n] = 0;
		if (strcmp(buffer, cdatestart) == 0) return true;
		fseek (fd, 1-n, SEEK_CUR);
	}
	return false;
}

static void force (const char* file, const char* date) 
{
	FILE * fd = fopen (file, "r+b");
	if (fd) {
		while (!feof(fd)) {
			int c = getc(fd);
			if (c == 'C') {
				ungetc (c, fd);
				if (creationdate (fd)) {
					fseek(fd, ftell(fd), SEEK_SET);
					fwrite (date, 1, strlen(date), fd);
					fflush(fd);
					break;
				}
			}
		}
		fclose (fd);
	}
	else cerr << "can't open file " << file << endl;
}


int main (int argc, char * argv[])
{
	if (argc < 3) usage(argv[0]);
	
	const char* date = argv[1];
	if (!checkdate(date)) usage(argv[0]);

	for (int i=2; i<argc; i++)
		force ( argv[i], date);
	return 0;
}
