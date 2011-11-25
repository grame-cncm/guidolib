//
//  main.cpp
//  GuidoFontViewer
//
//  Created by Dominique Fober on 09/04/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#include <Carbon/Carbon.h>
#include "TApplication.h"
#include "TWindow.h"
#include "FontDrawer.h"

// Our custom application class
class CarbonApp : public TApplication
{
    public:
							
                            CarbonApp() {}
        virtual             ~CarbonApp() {}
        
    protected:
		void				printPDF ();
		CGContextRef		MyCreatePDFContext (const CGRect *inMediaBox,  CFStringRef path);
        virtual Boolean     HandleCommand( const HICommandExtended& inCommand );
};

// Our main window class
class MainWindow : public TWindow
{
    public:
                           MainWindow();
        virtual             ~MainWindow() {}
        
        static void         Create();
		HIViewRef			getView()	{ return mView; }
        
    protected:
        virtual Boolean     HandleCommand( const HICommandExtended& inCommand );
		HIViewRef			mView;
};

//--------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    CarbonApp app;
    
    // Create a new window. A full-fledged application would do this from an AppleEvent handler
    // for kAEOpenApplication.
    MainWindow::Create();
    
    app.Run();
    return 0;
}

//--------------------------------------------------------------------------------------------
void CarbonApp::printPDF ()
{
    CGRect mediaBox;
	int pageWidth=604;
	int pageHeight=780;
    mediaBox = CGRectMake (0, 0, pageWidth, pageHeight);
    CGContextRef myPDFContext = MyCreatePDFContext (&mediaBox, CFSTR("/Users/fober/Desktop/QuartzGuidoFont.pdf"));
    CGContextBeginPage(myPDFContext, &mediaBox);
#if 0
        CGContextSetRGBFillColor (myPDFContext, 1, 0, 0, 1);
        CGContextFillRect (myPDFContext, CGRectMake (0, 0, 200, 100 ));
        CGContextSetRGBFillColor (myPDFContext, 0, 0, 1, .5);
        CGContextFillRect (myPDFContext, CGRectMake (0, 0, 100, 200 ));
#else
	FontDrawer fd;
//	fd.fDevice->Frame (10, 10, 200, 200);
	fd.Draw(myPDFContext);
#endif
    CGContextEndPage(myPDFContext);
    CGContextRelease(myPDFContext);	
}

//--------------------------------------------------------------------------------------------
CGContextRef  CarbonApp::MyCreatePDFContext (const CGRect *inMediaBox,  CFStringRef path)
{
    CGContextRef myOutContext = NULL;
    CFURLRef url; 
    url = CFURLCreateWithFileSystemPath (NULL,  path, kCFURLPOSIXPathStyle, false);
    if (url != NULL) {
        myOutContext = CGPDFContextCreateWithURL (url, inMediaBox, NULL);
        CFRelease(url);
    }
    return myOutContext;
}

//--------------------------------------------------------------------------------------------
Boolean CarbonApp::HandleCommand( const HICommandExtended& inCommand )
{
    switch ( inCommand.commandID )
    {
        case kHICommandNew:
            MainWindow::Create();
            return true;
            
 		case kHICommandPrint:
//            printPDF();
            return true;
         
        default:
            return false;
    }
}

//--------------------------------------------------------------------------------------------
void MainWindow::Create()
{
    MainWindow* wind = new MainWindow();

    // Position new windows in a staggered arrangement on the main screen
    RepositionWindow( *wind, NULL, kWindowCascadeOnMainScreen );
    
    // The window was created hidden, so show it
    wind->Show();
}

// ----------------------------------------------------------------------------
// 		* MyDrawEventHandler
// ----------------------------------------------------------------------------
OSStatus MyDrawEventHandler (EventHandlerCallRef myHandler, EventRef event, void *userData)
{
	MainWindow * w = (MainWindow *)userData;
	CGContextRef myContext;
	GetEventParameter (event, kEventParamCGContextRef, typeCGContextRef, NULL, sizeof (CGContextRef), NULL, &myContext);
	HIRect r;
	::HIViewGetBounds (w->getView(), &r);

	FontDrawer fd;
	fd.Draw(myContext, r);
	return noErr;
}

//--------------------------------------------------------------------------------------------
MainWindow::MainWindow( ) : TWindow( CFSTR("MainWindow") ), mView(0)
{
	HIViewID kViewID = { 'view', 1000 };
	HIViewFindByID (HIViewGetRoot(GetWindowRef()), kViewID, &mView);

	const EventTypeSpec myHIViewSpec[] = {	kEventClassControl, kEventControlDraw };
	::InstallEventHandler (::GetControlEventTarget (mView), NewEventHandlerUPP (MyDrawEventHandler),
		GetEventTypeCount (myHIViewSpec), myHIViewSpec, this, NULL);
}

//--------------------------------------------------------------------------------------------
Boolean MainWindow::HandleCommand( const HICommandExtended& inCommand )
{
    switch ( inCommand.commandID )
    {
        // Add your own command-handling cases here
        
        default:
            return false;
    }
}
