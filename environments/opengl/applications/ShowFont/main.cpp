//
//  main.c
//  OpenGLFontViewer
//
//  Created by Dominique Fober on 15/09/05.
//  Copyright Grame 2005. All rights reserved.
//

#include <iostream>
using namespace std;

#ifdef __APPLE_CC__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "FTGL/ftgl.h"

FTFont * infoFont, * guidoFont;
enum { kBaseW=840, kBaseH=600 };
GLint w_win = kBaseW, h_win = kBaseH;
#define kLines	20
#define kLimit	250
int lineH	= kBaseH/(kLines+1);
int colW	= kBaseW*kLines/(kLimit+kLines) + 1;
int top		= kBaseH - lineH - 5;

void display(void)
{
	float x1, y1, z1, x2, y2, z2;
	char buff[5];

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 0.0);
	int line=1;
	int x = 5, y = top;
	for (unsigned int i=1; i<kLimit; i++) {
		sprintf (buff, "%d", i);
		glRasterPos2i(x,y);
		infoFont->Render(buff);
		infoFont->BBox( buff, x1, y1, z1, x2, y2, z2);

		if (i >32 && i < 224) {
			glRasterPos2i(x+(int)(x2-x1)+10,y);
			buff[0]=i;
			buff[1]=0;
			guidoFont->Render(buff, 1);
		}
		if (++line > kLines) {
			x += colW; y = top; line=1;
		}
		else y -= lineH;
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	w_win = w;
	h_win = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, kBaseW, 0, kBaseH, -1, 1);
//	glOrtho(0, w, 0, h, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void myinit()
{
	glClearColor( 1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
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

	guidoFont = new FTGLBitmapFont("../../../../src/guido2.ttf");
//	guidoFont = new FTGLPixmapFont("../../../../src/guido2.ttf");
//	guidoFont = new FTGLPolygonFont("../../../../src/guido2.ttf");
	if (!guidoFont || guidoFont->Error()) {
		cout << "erreur : guidoFont creation failed!" << endl;
		exit(1);
	}
	guidoFont->FaceSize(24);

	int cmc = guidoFont->CharMapCount();
	FT_Encoding* encoding = guidoFont->CharMapList();
	while (cmc--) {
		switch (*encoding++) {
			case ft_encoding_none:		cout << "ft_encoding_none" << endl; break;
			case ft_encoding_unicode:	cout << "ft_encoding_unicode" << endl; break;
			case ft_encoding_symbol:	cout << "ft_encoding_symbol" << endl; break;
			case ft_encoding_latin_1:	cout << "ft_encoding_latin_1" << endl; break;
			case ft_encoding_latin_2:	cout << "ft_encoding_latin_2" << endl; break;

			case ft_encoding_sjis:		cout << "ft_encoding_sjis" << endl; break;
			case ft_encoding_gb2312:	cout << "ft_encoding_gb2312" << endl; break;
			case ft_encoding_big5:		cout << "ft_encoding_big5" << endl; break;
			case ft_encoding_wansung:	cout << "ft_encoding_wansung" << endl; break;
			case ft_encoding_johab:		cout << "ft_encoding_johab" << endl; break;

			case ft_encoding_adobe_standard:cout << "ft_encoding_adobe_standard" << endl; break;
			case ft_encoding_adobe_expert:	cout << "ft_encoding_adobe_expert" << endl; break;
			case ft_encoding_adobe_custom:	cout << "ft_encoding_adobe_custom" << endl; break;
			case ft_encoding_apple_roman:	cout << "ft_encoding_apple_roman" << endl; break;
			default:	cout << "ft_encoding unknown" << endl; break;
		}
	}

//	guidoFont->CharMap(ft_encoding_apple_roman);
	if (!guidoFont->CharMap(ft_encoding_unicode))
		cerr << "cannot set guido font encoding to ft_encoding_unicode" << endl;
}

#ifdef WIN32
int WINAPI   WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc=1;
	glutInit(&argc, &lpCmdLine);
#else
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
#endif
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize( w_win, h_win);
	glutCreateWindow("OpenGL Guido Font Viewer");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//	glutSpecialFunc(specialkey);
//	glutKeyboardFunc(parsekey);
	myinit();
	glutMainLoop();
	return 0;
}

