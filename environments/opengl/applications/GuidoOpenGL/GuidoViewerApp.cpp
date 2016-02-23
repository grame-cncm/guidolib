
#include <iostream> 
#include <string.h>
#include <sstream>
#include <stdlib.h>

using namespace std;

#include <GLUT/glut.h>

// - App
#include "GuidoViewerApp.h"
#include "GuidoDoc.h"

#include "GUIDOEngine.h"

//#include "DebugDevice.h"
#include "GDeviceGL.h"
#include "GSystemGL.h"

#ifdef GUIDO_USE_MUSICXML
	#include "guidoconverter.h"
	#include "LibMusicXML.h"	
	using namespace MusicXML;
#endif

#define DEVDEBUG	0
#if DEVDEBUG
# define DevDebug(dev)	new DebugDevice(new dev)
#else
# define DevDebug(dev)	new dev
#endif

#define kFontType		GSystemGL::kPolygonFont
//	GSystemGL::kPixmapFont
//	GSystemGL::kBitmapFont
//	GSystemGL::kOutlineFont
//	GSystemGL::kPolygonFont
//	GSystemGL::kExtrudeFont

// ----------------------------------------------------------------------------
GuidoViewerApp::GuidoViewerApp() : zoomFactor(1)
{
	fDocument = 0;
	fDevice = 0;
	fPerspective = false;
    pageNum = 1;
}

// ----------------------------------------------------------------------------
GuidoViewerApp::~GuidoViewerApp()
{	
	delete fDocument;
	delete fDevice;
	delete fSystem;
}

// ----------------------------------------------------------------------------
const char * GuidoViewerApp::FontType() const
{
	switch (kFontType) {
		case GSystemGL::kPixmapFont:	return "PixmapFont";
		case GSystemGL::kBitmapFont:	return "BitmapFont";
		case GSystemGL::kOutlineFont:	return "OutlineFont";
		case GSystemGL::kPolygonFont:	return "PolygonFont";
		case GSystemGL::kExtrudeFont:	return "ExtrudeFont";
        case GSystemGL::kTextureFont:   return "TextureFont";
        case GSystemGL::kBFont:         return "BufferFont";
		default: return "unknown";
	}
}

// ----------------------------------------------------------------------------
void GuidoViewerApp::SetDevice(VGDevice * inDevice)
{
	delete fDevice;
	fDevice = inDevice;
}

#ifdef APPLE
// ----------------------------------------------------------------------------
static const char* defaultTextFont()  { return "/Library/Fonts/Times New Roman.ttf"; }
static const char* defaultGuidoFont() {
	static char buffer[512];
	const char* path =  getenv("HOME");
	snprintf(buffer, 512, "%s/Library/Fonts/guido2.ttf", path ? path : "");
	return buffer;
}
#endif

#ifdef WIN32
// ----------------------------------------------------------------------------
static const char* defaultTextFont()	{ return "C:\\windows\fonts\times.ttf"; }
static const char* defaultGuidoFont()	{ return "guido2.ttf";} // the guido font is expected in the current folder
#endif

#ifdef LINUX
// ----------------------------------------------------------------------------
static const char* defaultTextFont()	{ return "/usr/X11R6/lib/X11/fonts/TTF/Times.ttf"; }
static const char* defaultGuidoFont()	{ return "guido2.ttf"; } // the guido font is expected in the current folder
#endif

// ----------------------------------------------------------------------------
bool GuidoViewerApp::Initialize(const char* guidofont, const char* textfont, bool perspective)
{	
	fPerspective = perspective;
	GuidoInitDesc desc;
	fSystem = new GSystemGL(kFontType);
	fDevice = fSystem->CreateDisplayDevice();
	
	const char* gf = guidofont ? guidofont : defaultGuidoFont();
	const char* tf = textfont  ? textfont  : defaultTextFont();

	desc.graphicDevice = fDevice;
	desc.textFont  = tf;
	desc.musicFont = gf;
	GuidoErrCode result = GuidoInit( &desc );
	if( result != guidoNoErr ) {
		ReportGuidoError( result );
		return false;
	}
	return true;
}

// ----------------------------------------------------------------------------
void GuidoViewerApp::FullScreen(bool on) 
{
	if (on) {
		fWinW = fWidth;
		fWinH = fHeight;
		fWinX = glutGet(GLUT_WINDOW_X);
		fWinY = glutGet(GLUT_WINDOW_Y);
		glutFullScreen();
	}
	else {
		glutSetWindowTitle(fDocument ? fDocument->File().c_str() : "OpenGL Guido Viewer");
		glutReshapeWindow(fWinW, fWinH);
		glutPositionWindow(fWinX, fWinY);
	}
}

// ----------------------------------------------------------------------------
void GuidoViewerApp::SetSize(int w, int h) 
{ 
	fWidth  = w; 
	fHeight = h;
	if (fDevice) fDevice->NotifySize(w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1);
 	glMatrixMode(GL_MODELVIEW);
    glEnable (GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glLoadIdentity();
}

//----------------------------------------------------------------------------
void GuidoViewerApp::SetCamera(int w, int h, bool perspective)
{
	extern float eyex, eyey, eyez;
	if (perspective) {
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		gluPerspective( 90, 1, 1, 5000);
		glMatrixMode(GL_MODELVIEW);
        glEnable (GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		glLoadIdentity();
		gluLookAt( eyex, eyey, eyez, w/2, h/2, 0.0, 0.0, 1.0, 0.0);
	}
	else {
        double h_temp = (h*zoomFactor) - h;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w*zoomFactor, 0, h*zoomFactor, -1, 1);
        glTranslatef(0, h_temp, 0);
		glMatrixMode(GL_MODELVIEW);
        glEnable (GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		glLoadIdentity();
	}
}

//----------------------------------------------------------------------------
void GuidoViewerApp::DrawScore()
{
	if (!fDocument || !fDevice) return;

	GuidoOnDrawDesc desc;
	//int pageNum = 1;

	GuidoPageFormat pf;
	GuidoGetPageFormat(	fDocument->GetGR(), pageNum, &pf );

	desc.handle = fDocument->GetGR();
	desc.hdc = fDevice;
	desc.page = pageNum;

    desc.updateRegion.erase = true;     // and draw everything
	desc.scrollx = desc.scrolly = 0;    // from the upper left page corner
	desc.isprint = false;
#ifndef useglOrtho
	desc.sizex = fWidth;
	desc.sizey = fHeight;      // size of the drawing region
	SetCamera(fWidth, fHeight, fPerspective);
#else
    desc.sizex = pf.width;
	desc.sizey = pf.height;      // size of the drawing region

	GLdouble right, top;
	float ratio = pf.width / fWidth;
	if ( (pf.height / ratio) < fHeight) {
		right = pf.width;
		top = fHeight * ratio; 
	}
	else {
		top = pf.height; 
		ratio = pf.height / fHeight;
		right = fWidth * ratio ;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, right, 0, top, -1, 1);
	glTranslatef(0, top, 0 );
	glEnable(GL_MULTISAMPLE);
	SetCamera(right, top, fPerspective);
#endif
	GuidoErrCode err = GuidoOnDraw(&desc);
}

// ------------------------------ ---------------------------------------------
void GuidoViewerApp::Reload()
{
	if (fDocument) {
		string file = fDocument->Path();
		OpenFile (file.c_str());
        fDocument->SetPageNum(pageNum);
	}
}

// ---------------------------------------------------------------------------
void GuidoViewerApp::OpenFile( const char * file )
{
	GuidoDoc * doc;
	const char * ext = &file[strlen(file)-4];
	if (!strcmp(ext,".gmn")) 
		doc = OpenGuidoFile( file );
	else
		doc = OpenXMLFile( file );
	if (doc) {
		if (fDocument) delete fDocument;
		fDocument = doc;
		glutSetWindowTitle(fDocument->File().c_str());
	}
}

// ---------------------------------------------------------------------------
GuidoDoc * GuidoViewerApp::OpenGuidoFile( const char * file ) 
{
	GuidoDoc *doc = new GuidoDoc ();
	if (!doc) return 0;

	GuidoErrCode err = doc->Read (file);
	if ( err != guidoNoErr ) {
		ReportGuidoError( err );
		delete doc;
		return 0;
	}
	GuidoResizePageToMusic( doc->GetGR() );
	return doc;
}

// ---------------------------------------------------------------------------
GuidoDoc * GuidoViewerApp::OpenXMLFile( const char * file )
{
#ifdef GUIDO_USE_MUSICXML 
    TMusicXMLFile file;
	SScore score = file.read(file);
	if (score) {
	    guidoconverter c;
		int gr = c.makeGR(score);
		if( GuidoValidHandle( gr )) {
			gXMLFile = true;
		}
		else ReportGuidoError( gr );
	}
	else ReportError( "Error: cannot read file:", pathString );
#else
	return 0;
#endif
}

// ----------------------------------------------------------------------------
void GuidoViewerApp::ReportGuidoError( GuidoErrCode inGuidoErr )
{
	string errorStr ( GuidoGetErrorString( inGuidoErr ));
		
	// - In the case of a parse error, add the line where the error occured.
	if( inGuidoErr == guidoErrParse ) {
		ostringstream errLine;
		errLine << " \nline: " << GuidoGetParseErrorLine();
		errorStr += errLine.str();
	}
	else	// Guido engine already displays parse errors.
		cerr << "Guido error: " << errorStr.c_str() << endl;	
}

// ----------------------------------------------------------------------------
void GuidoViewerApp::ReportError( const char * inMessage, const char * inDetail )
{
	cerr << "GuidoViewer error: " << inMessage;
	if( inDetail )
		cerr << ", " << inDetail;
	cerr << endl;
}

bool GuidoViewerApp::firstPage() {
    if (!fDocument || !fDevice) {
        return false;
    }
    else {
        pageNum = 1;
        fDocument->SetPageNum(pageNum);
    }
    return true;
}

bool GuidoViewerApp::nextPage() {
    if (!fDocument || !fDevice) {
        return false;
    }
    if(fDocument->PageCount()==1 || pageNum>=fDocument->PageCount()) {
        //pageNum--;
        return false;
    }
    else {
        pageNum++;
        fDocument->SetPageNum(pageNum);
        return true;
    }
}

bool GuidoViewerApp::previousPage() {
    if (!fDocument || !fDevice)
        return false;
    if(pageNum==1)
        return false;
    else {
        pageNum--;
        fDocument->SetPageNum(pageNum);
        return true;
    }
    
}

bool GuidoViewerApp::lastPage() {
    if (!fDocument || !fDevice) {
        return false;
    }
    else if (fDocument->PageCount()==1) {
        return false;
    }
    else {
        pageNum = fDocument->PageCount();
        fDocument->SetPageNum(pageNum);
        return true;
    }
    
}

