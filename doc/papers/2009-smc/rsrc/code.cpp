
#include <iostream>

#include "AROthers.h"
#include "ARNote.h"
#include "ARTypes.h"
#include "tree_browser.h"
#include "visitor.h"
#include "guidoparser.h"

using namespace guido;
using namespace std;

struct countnotes : public visitor<SARNote>
{
    int fCount;
         countnotes() : fCount(0)	{}
    void visitStart( SARNote& elt ) { fCount++; }
};

struct countnotespredicat {
    bool operator () (const Sguidoelement elt) const 
		{ return (dynamic_cast<ARNote*>((guidoelement*)elt) ? true : false); }
};


static void count(Sguidoelement score)
{
	countnotespredicat p;
	// count notes using STL
	int count = count_if(score->begin(), score->end(), p);
	cout << "count notes with STL: " << count << endl; 
	// count notes using a visitor
	countnotes cnv;
	tree_browser<guidoelement> tb(&cnv);
	tb.browse (*score);
	count =  cnv.fCount;
	cout << "count notes with visitor: " << cnv.fCount << endl; 
}

int main(int argc, char** argv)
{
	guidoparser r;
	const char * file = argv[1];
	Sguidoelement score = r.parseFile( file );
	if (score) {
		count (score);
	}
}

