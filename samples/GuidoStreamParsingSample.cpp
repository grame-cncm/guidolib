#include "ParsingClient.h"

#include <iostream>
#include "GUIDOParse.h"
#include <windows.h>

typedef struct MyData {
    GuidoStream *s;
    GuidoParser *p;
    ARHandler arh;
} MYDATA, *PMYDATA;

DWORD WINAPI threadParser(LPVOID lpParam)
{
    PMYDATA pDataArray;
    pDataArray = (PMYDATA)lpParam;

    /* Blocking method in which parser will parse the stream */
    pDataArray->arh = GuidoStream2AR(pDataArray->p, pDataArray->s);

    ExitThread(0);
}

void parseStream()
{
    /* Parser/stream creation */
    GuidoStream   *newStream = GuidoOpenStream();
    GuidoParser   *newParser = GuidoOpenParser();

    /* thread server creation */
    PMYDATA pDataArray[1];
    HANDLE  hThreadArray[1];
    DWORD   dwThreadIdArray[1];

    pDataArray[0] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                sizeof(MYDATA));

    pDataArray[0]->s = newStream;
    pDataArray[0]->p = newParser;

    hThreadArray[0] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size
            threadParser,           // thread function name
            pDataArray[0],          // argument to thread function
            0,                      // use default creation flags
            &dwThreadIdArray[0]);   // returns the thread identifier


    ARHandler arh;

    /* Stream writing */
    GuidoWriteStream(newStream, "[ \\meter<\"4/4\"> a");    //Care about char escape :
                                                            // write "\\meter" and not "\meter"

    /* Parser's current state recuperation */
    arh = GuidoParser2AR(newParser);

    // Here you have a valid ARHandler (voice have been closed)
    // You can convert it to GR or make whatever you want with

    /* Stream writing */
    GuidoWriteStream(newStream, "\\cluster({c,");

    /* Parser's current state recuperation */
    arh = GuidoParser2AR(newParser);

    // Here you have again a valid ARHandler (chord, cluster-tag
    // and voice have been closed)

    /* Stream writing */
    GuidoWriteStream(newStream, "e}{a/2, f2})]");

    /* Stream closure : parser will automatically finish his job */
    GuidoCloseStream(newStream);

    /* Wait for parsing thread to end */
    WaitForSingleObject(hThreadArray[0], INFINITE);

    // Here, arh is the final valid ARHandler

    /* Error display */
    int errorLine, errorCol;
    GuidoParserGetErrorCode(newParser, errorLine, errorCol);
    if (errorLine)
        std::cout << "Error at line " << errorLine << ", row " << errorCol << "." << std::endl;

    /* Parser closure */
    GuidoCloseParser(newParser);
}


int main()
{
    parseStream();

	return 0;
}