
#include <iostream> 
#include <string>
#include <sstream>

using namespace std;

#ifdef __APPLE_CC__
	#include <GLUT/glut.h>
#else
//	#include <glut.h>		//use this include for VC++7
	#include <GL/glut.h>	//use this include for VC++6
#endif


// - App
#include "GuidoViewerApp.h"
#include "GuidoDoc.h"

#include "GUIDOEngine.h"

//#include "DebugDevice.h"
//#include "OGLDevice.h"
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

//#define kFontType		OGLDevice::kPolygonFont
#define kFontType		GSystemGL::kPolygonFont
//	OGLDevice::kPixmapFont
//	OGLDevice::kBitmapFont
//	OGLDevice::kOutlineFont
//	OGLDevice::kPolygonFont
//	OGLDevice::kExtrudeFont

// ----------------------------------------------------------------------------
GuidoViewerApp::GuidoViewerApp()
{
	fDocument = 0;
	fDevice = 0;
	fPerspective = false;
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
		default: return "unknown";
	}
}

// ----------------------------------------------------------------------------
void GuidoViewerApp::SetDevice(VGDevice * inDevice)
{
	delete fDevice;
	fDevice = inDevice;
}

// ----------------------------------------------------------------------------
bool GuidoViewerApp::Initialize()
{	
	GuidoInitDesc desc;
//	fDevice = DevDebug(OGLDevice(kFontType));
	fSystem = new GSystemGL(kFontType);
	fDevice = fSystem->CreateDisplayDevice();


	desc.graphicDevice = fDevice;
	//desc.textFont = "Times";
//#if defined(WIN32) || defined(linux)
//	desc.musicFont = "guido2.ttf";
//	desc.textFont = "times.ttf";
//#else
	//desc.musicFont = "../Resources/guido2.ttf";
//	desc.textFont = "Times";
//#endif
desc.musicFont = "../../../src/guido2.ttf";
desc.textFont = "../times.ttf";
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
		glLoadIdentity();
		gluLookAt( eyex, eyey, eyez, w/2, h/2, 0.0, 0.0, 1.0, 0.0);
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, 0, h, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
}

//----------------------------------------------------------------------------
void GuidoViewerApp::DrawScore()
{
	if (!fDocument || !fDevice) return;

	GuidoOnDrawDesc desc;
	int pageNum = 1;

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
	SetCamera(right, top, fPerspective);
#endif
	GuidoErrCode err = GuidoOnDraw(&desc);
}

// ---------------------------------------------------------------------------
string GuidoViewerApp::SelectOneFile()
{
	return "../../../regression-tests/accidental-oct.gmn";
}

// ---------------------------------------------------------------------------
void GuidoViewerApp::OpenFile()
{
	string file = SelectOneFile();
	if( file.length() > 0) {
		cout << "opening file " << file << endl;
		OpenFile(file.c_str());
	} 
}
// ------------------------------ ---------------------------------------------
void GuidoViewerApp::Reload()
{
	if (fDocument) {
		string file = fDocument->Path();
		cout << "reloading file " << file << endl;
		OpenFile (file.c_str());
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
