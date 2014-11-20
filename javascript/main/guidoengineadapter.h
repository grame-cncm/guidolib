#ifndef GUIDOENGINEADAPTER_H
#define GUIDOENGINEADAPTER_H

#include <iostream>
#include "GUIDOParse.h"

using namespace std;

class GuidoEngineAdapter
{
    public:
        GuidoEngineAdapter();
        virtual ~GuidoEngineAdapter();

        void GuidoInit();
        GuidoParser * GuidoOpenParser();
        void CloseParser(GuidoParser * p);

        ARHandler GuidoString2AR (GuidoParser * p, const string guidoCode);
        string getSvg(GuidoParser * p, const string guidoCode);
        vector<string> returnVector();

    private :
        static int nbinstantiation;
        vector<string> testvector;
};

#endif // GUIDOENGINEADAPTER_H
