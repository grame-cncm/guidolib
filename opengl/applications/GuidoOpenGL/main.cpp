//
//  main.c
//  GuidoOpenGLViewer
//
//  Created by Dominique Fober on 15/09/05.
//  Copyright Grame 2005. All rights reserved.
//

#include <iostream>
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

FTFont * infoFont;
GLint w_win = 525, h_win = 700;
float tx=0, ty=0, tz=0;
GuidoViewerApp gAppl;
int gMainMenu, gPageMenu;
const char * curFont=0;
float eyex=w_win/2.0f, eyey=h_win/2.0f, eyez=370.0f;

enum { kFirst=1, kNext, kPrevious, kLast };
enum { kReload, kFullScreen, kWindow, kQuit };
enum { kBorder = 10 };

void rect(int x, int y) {
	glBegin (GL_LINE_LOOP);
		glVertex2i (x, y);
		glVertex2i (x, h_win - y);
		glVertex2i (w_win - x, h_win - y);
		glVertex2i (w_win - x, y);
	glEnd();
}

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

void display(void)
{
	glViewport(0, 0, w_win, h_win);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 0.0);
	rect (kBorder, kBorder);
	glViewport(kBorder+1, kBorder+1, w_win-2-2*kBorder, h_win-2-2*kBorder);
	gAppl.DrawScore();

	glColor3f (0.0, 0.0, 0.0);
	info();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	w_win = width;
	h_win = height;
	gAppl.SetSize (width, height);
}

void specialkey (int key, int x, int y)
{
	float step=30.0f;
	switch (key) {
		case GLUT_KEY_F5:		eyez -= step; break;
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
		case kQuit: 
			exit(0);
    }
	glutPostRedisplay();
}

static void page_menu(int i)
{
}

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
    glutAddMenuEntry("Quit", kQuit);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

static void myinit()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor( 1.0, 1.0, 1.0, 1.0);

	glShadeModel(GL_SMOOTH);


	create_menus();
//#ifdef WIN32
	infoFont = new FTGLPixmapFont("../times.ttf");
//#else
//	infoFont = new FTGLPixmapFont("Times");
//#endif
	if (!infoFont || infoFont->Error()) {
		cout << "erreur : infoFont creation failed!" << endl;
		delete infoFont;
		infoFont = 0;
	}
	else {
		infoFont->FaceSize(12);
		infoFont->CharMap(ft_encoding_unicode);
	}
	gAppl.Initialize ();
	curFont = gAppl.FontType();
}

#ifdef WIN32
int WINAPI   WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
#ifdef WIN32
	int argc=1;
	glutInit(&argc, &lpCmdLine);
#else
	glutInit(&argc, argv);
#endif
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize( w_win, h_win);
	glutCreateWindow("OpenGL Guido Viewer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialkey);
//	glutKeyboardFunc(parsekey);
	myinit();
	if (argc > 1)
#ifndef WIN32
        gAppl.OpenFile (argv[1]);
	glutMainLoop();
#endif
	return 0;
}

