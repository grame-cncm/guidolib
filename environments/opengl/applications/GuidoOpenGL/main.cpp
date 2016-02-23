//
//  main.c
//  GuidoOpenGLViewer
//
//  Created by Dominique Fober on 15/09/05.
//  Copyright Grame 2005. All rights reserved.
//

#include <iostream>
#include <string>
#include <limits.h>		/* PATH_MAX */
#include <unistd.h>		/* execve */
#include <libgen.h>

using namespace std;

#ifdef __APPLE_CC__
	#include <GLUT/glut.h>
	#include <Carbon/Carbon.h>
#else
	#include <GL/glut.h>	//use this include for VC++6
//	#include <glut.h>		//use this include for VC++7
#endif

#include "GuidoViewerApp.h"
#include "FTGL/ftgl.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

// ----------------------------------------------------------------------------
FTFont * infoFont;
GLint w_win = 525, h_win = 700;
float tx=0, ty=0, tz=0;
int gMainMenu, gPageMenu;
GuidoViewerApp gAppl;
const char * curFont=0;
float eyex=w_win/2.0f, eyey=h_win/2.0f, eyez=370.0f;

enum {kFirst, kNext, kPrevious, kLast, kReload, kFullScreen, kWindow, kQuit, kBorder, kZoomIn, kZoomOut};
int win_id;

void rect(int x, int y) {
	glBegin (GL_LINE_LOOP);
		glVertex2i (x, y);
		glVertex2i (x, h_win - y);
		glVertex2i (w_win - x, h_win - y);
		glVertex2i (w_win - x, y);
	glEnd();
}

//---------------------------------------------------------------------------------------------
void info(void)
{
	char buff[512];
	sprintf (buff, "camera info: [%f, %f, %f]", eyex, eyey, eyez);
	if (infoFont) {
		glRasterPos2f( 10.0f , infoFont->LineHeight());
		infoFont->Render(buff);
		glRasterPos2f( 10.0f , 2*infoFont->LineHeight());
		infoFont->Render(curFont);
	}
}

//---------------------------------------------------------------------------------------------
void display(void)
{
	glViewport(0, 0, w_win, h_win);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 0.0);
	//rect (kBorder, kBorder);
	glViewport(kBorder+1, kBorder+1, w_win-2-2*kBorder, h_win-2-2*kBorder);
	gAppl.DrawScore();

	glColor3f (0.0, 0.0, 0.0);
	info();
	glutSwapBuffers();
}

//---------------------------------------------------------------------------------------------
void reshape(int width, int height) {
    w_win = width;
    h_win = height;
    glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, GLdouble (width), 0, GLdouble (height) );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
    //GLKMatrix4MakeOrtho();
    gAppl.SetSize (width, height);    
}

//---------------------------------------------------------------------------------------------
void specialkey (int key, int x, int y)
{
	float step=30.0f;
	switch (key) {
		case GLUT_KEY_F7:		eyez -= step; break;
		case GLUT_KEY_F6:		eyez += step; break;
		case GLUT_KEY_HOME:		eyex=w_win/2.0f; eyey=h_win/2.0f; eyez=370.0f; break;
		case GLUT_KEY_LEFT:		eyex -= step; break;
		case GLUT_KEY_RIGHT:	eyex += step; break;
		case GLUT_KEY_UP:		eyey -= step; break;
		case GLUT_KEY_DOWN:		eyey += step; break;
		default: return;
	}
	glutPostRedisplay();
}

//---------------------------------------------------------------------------------------------
static void main_menu(int i)
{
    switch(i) {
		case kReload: 
			gAppl.Reload();
			break;
		case kFullScreen: 
			gAppl.FullScreen(true);
			glutChangeToMenuEntry(5, "Window", kWindow);
			break;
		case kWindow: 
			gAppl.FullScreen(false);
			glutChangeToMenuEntry(5, "Full Screen", kFullScreen);
			break;
        case kZoomIn:
            gAppl.zoomIn();
            break;
        case kZoomOut:
            gAppl.zoomOut();
            break;
		case kQuit:
            glutDestroyWindow(win_id);
			exit(0);
    }
	glutPostRedisplay();
}

//---------------------------------------------------------------------------------------------
static void page_menu(int i) {
    switch(i) {
        case kFirst:
            if(gAppl.firstPage()) {
                gAppl.DrawScore();
                glutPostRedisplay();
            }
            break;
        case kNext:
            if(gAppl.nextPage()) {
                gAppl.DrawScore();
                glutPostRedisplay();
            }
            break;
        case kPrevious:
            if(gAppl.previousPage()) {
                gAppl.DrawScore();
                glutPostRedisplay();
            }
            break;
        case kLast:
            if(gAppl.lastPage()) {
                gAppl.DrawScore();
                glutPostRedisplay();
            }
            break;
    }
}

//---------------------------------------------------------------------------------------------
static void create_menus(void)
{
    gPageMenu = glutCreateMenu(page_menu);
	glutAddMenuEntry("First", kFirst);
    glutAddMenuEntry("Next", kNext);
    glutAddMenuEntry("Previous ", kPrevious);
    glutAddMenuEntry("Last", kLast);

    gMainMenu = glutCreateMenu(main_menu);
    //glutAddMenuEntry("Load", kLoad);
    glutAddMenuEntry("Reload", kReload);
    glutAddMenuEntry("-----------", -1);
	glutAddSubMenu("Pages", gPageMenu);
	glutAddMenuEntry("Full Screen", kFullScreen);
    glutAddMenuEntry("-----------", 0);
    glutAddMenuEntry("Zoom In", kZoomIn);
    glutAddMenuEntry("Zoom Out", kZoomOut);
    glutAddMenuEntry("-----------", 0);
    glutAddMenuEntry("Quit", kQuit);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


const char* kPerspectiveOpt = "-p";
const char* kFontOpt		= "-font";
const char* kGuidoFontOpt	= "-guidofont";
const char* kHelpOpt		= "-h";
//---------------------------------------------------------------------------------------------
static void usage(const char* appname)
{
	cerr << "usage: " << appname << " gmn-file [options]" << endl;
	cerr << " options: " <<  kFontOpt        << "      fontname  # the text font to be used (full path)" << endl;
	cerr << "          " << kGuidoFontOpt   << " fontname  # the guido font path" << endl;
	cerr << "          " << kPerspectiveOpt << "                   # activate perspective (controlled using the arrows, F6, F7 and home keys)" << endl;
	cerr << "          " << kHelpOpt        << "                   # display this help message" << endl;
}

//---------------------------------------------------------------------------------------------
static void error(const char* appname) { usage(appname); exit(1); }

//---------------------------------------------------------------------------------------------
template<typename T> bool lopt (const char* opt, int argc, char** argv, T& outvalue)
{
	for (int i=2; i < argc; i++) {
		string arg (argv[i]);
		if (arg[0] == '-') {
			if (arg == opt)  {
				if (arg == kHelpOpt)  {
					usage(argv[0]);
					exit(0);
				}
				else if (opt == kPerspectiveOpt) {
					outvalue = true;
					return true;
				}
				else if ((opt == kFontOpt) || (opt == kGuidoFontOpt)) {
					if (argc > (i+1)) {
						outvalue = argv[i+1];
						return true;
					}
					else return false;
				}
				else return false;
			}
		}
	}
	return true;
}


#ifdef APPLE
// ----------------------------------------------------------------------------
static const char* defaultInfoFont() { return "/Library/Fonts/Times New Roman.ttf"; }
#endif

#ifdef WIN32
// ----------------------------------------------------------------------------
static const char* defaultInfoFont() { return "Times New Roman.ttf"; }
#endif

#ifdef LINUX
// ----------------------------------------------------------------------------
static const char* defaultInfoFont() { return "Times New Roman.ttf"; }
#endif


//---------------------------------------------------------------------------------------------
static void myinit( GuidoViewerApp& appl, int argc, char** argv)
{
	string gmnfile = argv[1];
	if (gmnfile[0] == '-') error(argv[0]);

	bool perspective = false;
	lopt (kHelpOpt, argc, argv, perspective);
	if (!lopt (kPerspectiveOpt, argc, argv, perspective)) error(argv[0]);
	
	static string guidofont, textfont;
	if (!lopt (kFontOpt, argc, argv, textfont)) error(argv[0]);
	if (!lopt (kGuidoFontOpt, argc, argv, guidofont)) error(argv[0]);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor( 1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);

	create_menus();
	infoFont = new FTGLPixmapFont(defaultInfoFont());
	if (!infoFont || infoFont->Error()) {
		cout << "erreur : infoFont creation failed!" << endl;
		delete infoFont;
		infoFont = 0;
	}
	else {
		infoFont->FaceSize(12);
		infoFont->CharMap(ft_encoding_unicode);
	}
	appl.Initialize (guidofont.size() ? guidofont.c_str() : 0, textfont.size() ? textfont.c_str() : 0, perspective);
	curFont = appl.FontType();
}

//---------------------------------------------------------------------------------------------
#ifdef WIN32
int WINAPI   WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
#ifdef WIN32
	int argc=1;
	glutInit(&argc, &lpCmdLine);
#else
	if (argc <=1) error(argv[0]);
	glutInit(&argc, argv);
#endif

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA | GLUT_MULTISAMPLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize( w_win, h_win);
    win_id = glutCreateWindow("OpenGL Guido Viewer");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialkey);
    //	glutKeyboardFunc(parsekey);

    myinit(gAppl, argc, argv);
    if (argc > 1)
        gAppl.OpenFile (argv[1]);

	glutMainLoop();
	return 0;
}

