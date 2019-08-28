

#include <fstream>
#include <iostream>

using namespace std;

static void usage( const char* name) {
	cerr << "usage: " << name << " <text file>" << endl;
	exit (1);
}


int main (int argc, char* argv[])
{
	if (argc != 2) usage (argv[0]);
	
	const char* file = argv[1];
	const char* sep = "";
	fstream f (argv[1], ios_base::in);
	cout << "{ ";
	bool end=false;
	do {
		int c = f.get();
		if (f.eof()) break;
		if (c == '\n') 	cout << endl;
		else {
			cout << sep << c ;
			sep = ", ";
		}
	} while (true);
	cout << endl << "};" << endl;
	return 0;
}
