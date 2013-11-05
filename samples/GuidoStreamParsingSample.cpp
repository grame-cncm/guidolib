// ---------------------------------------------------------------------------
//	Guido stream parsing example of use
//  Take care that for simplicity, this example doesn't check return codes
// ---------------------------------------------------------------------------

#include <QThread>

#include "GUIDOParse.h"

/* Thread in which parser will do its job
   (we need a thread because stream parsing
   is an asynchronous mechanism */
class QParsingThread : public QThread
{
	Q_OBJECT

public:
	QParsingThread(ParsingClient *inParsingClient) { fParsingClient = inParsingClient)
	
	void      SetParser(GuidoParser *inParser)     { fParser = inParser; }
	void      SetStream(GuidoStream *inStream)     { fStream = inStream; }

protected:
	void run();

	GuidoParser   *fParser;
	GuidoStream   *fStream;
	ARHandler      fARHandler;
	ParsingClient *fParsingClient;
};

void QParsingThread::run()
{
	/* Blocking method in which parser will parse the stream */ 
    fARHandler = GuidoStream2AR(fParser, fStream);
	
	/* Give ARHandler to main thread */
	fParsingClient->setARHandler(fARHandler);
}



class ParsingClient
{
public:
    void parseStream()
	{
		/* Thread creation */
	    QParsingThread *parsingThreadObj = new QParsingThread(this);
	
		/* Parser/stream creation */
      	GuidoStream   *newStream = GuidoOpenStream();
        GuidoParser   *newParser = GuidoOpenParser();

		/* Thread initialization */
	    parsingThreadObj->SetParser(newParser);
       	parsingThreadObj->SetStream(newStream);

	    /* Thread start up */
        parsingThreadObj->start();
	
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
        parsingThreadObj->wait();
	
        /* Final factory recuperation */
        /* /!\ At this point, syntax errors aren't allowed anymore /!\ */
        arh = fARH;

			// Here you have the final valid ARHandler
			
        /* Error display */
        int errorLine, errorCol;
        GuidoParserGetErrorCode(newParser, errorLine, errorCol);
        if (errorLine)
            std::cout << "Error at line " << errorLine << ", row " << errorCol << "." << std::endl;

        /* Parser closure */
        GuidoCloseParser(newParser);
    }
	
	void setARHandler(ARHandler inARH) { fARH = inARH; }
	
protected :
	ARHandler fARH;
}