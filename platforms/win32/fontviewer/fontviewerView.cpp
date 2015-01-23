// fontviewerView.cpp : implémentation de la classe CfontviewerView
//




#include "stdafx.h"
#include "fontviewer.h"
#include "fontviewerDoc.h"
#include "fontviewerView.h"


// CfontviewerView
IMPLEMENT_DYNCREATE(CfontviewerView, CView)

BEGIN_MESSAGE_MAP(CfontviewerView, CView)
END_MESSAGE_MAP()

//#define PRINTEXTEND
#ifdef PRINTEXTEND
#include <fstream>
std::ofstream guido("Guido2.txt");
std::ofstream times("Times.txt");
#endif

// construction ou destruction de CfontviewerView

CfontviewerView::CfontviewerView()
{
	LOGFONT lf;
	LOGFONTW lfw;

	memset( &lf, 0, sizeof(lf));
	strncpy( lf.lfFaceName, "Guido2", 6 );
	lf.lfHeight = - 32; // ! see microsoft gdi doc
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfWeight	= FW_NORMAL;
	mGuidoFont = ::CreateFontIndirect( &lf );

	memset( &lf, 0, sizeof(lf));
	strncpy( lf.lfFaceName, "Times", 5 );
	lf.lfHeight = - 10; // ! see microsoft gdi doc
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfWeight		= FW_NORMAL;
	mFont = ::CreateFontIndirect( &lf );
}

CfontviewerView::~CfontviewerView()
{
}

BOOL CfontviewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

#ifdef PRINTEXTEND
static HGDIOBJ createFont (const char* name, int size)
{
	LOGFONT lf;
	memset( &lf, 0, sizeof(lf));
	strncpy( lf.lfFaceName, name, strlen (name) );
	lf.lfHeight = -size; // ! see microsoft gdi doc
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfWeight	= FW_NORMAL;
	return ::CreateFontIndirect( &lf );
}

static int getExtent(HDC hdc, char glyph)
{
	SIZE winSize;
	::GetTextExtentPoint32( hdc, &glyph, 1, &winSize ); 
	return winSize.cx;
}

#endif

void CfontviewerView::OnDraw(CDC* hdc)
{
#ifdef PRINTEXTEND
	static bool done = false;
	if (!done) {
		const float LSPACE = 50;
		HDC dc = hdc->GetSafeHdc();

		HGDIOBJ guidoFont = createFont ("Guido2", 4*LSPACE);
		::SelectObject( dc, guidoFont );
		guido << "Win32 - GDI - Guido2 font - size: " << 4*LSPACE << std::endl; 
		for ( unsigned char i=32; i<255; i++) {
			guido << int(i) << ": " << getExtent(dc, char(i)) << std::endl; 
		}

		HGDIOBJ timesFont = createFont ("Times", 1.5*LSPACE);
		::SelectObject( dc, timesFont );
		times << "Win32 - GDI - Time font - size: " << 1.5*LSPACE << std::endl; 
		for ( unsigned char i=32; i<255; i++) {
			times << int(i) << ": " << getExtent(dc, char(i)) << std::endl; 
		}
		done = true;
	}
#endif

	if (!mFont || !mGuidoFont) return;
	unsigned int n = 0;
	for (int y = 15 ; y < 735 ; y+=60 ) {
		for (int x = 10 ; x < 1330 ; x+=60 )
		{
			const wchar_t theWChar = (wchar_t)n;
			const char theChar = (char)n;
			char num[20];
			sprintf(num, "%d", n);
			::SelectObject( hdc->GetSafeHdc(), mFont );
			::TextOut( hdc->GetSafeHdc(), x, y, num, strlen(num) );

			::SelectObject( hdc->GetSafeHdc(), mGuidoFont );
			::TextOutW( hdc->GetSafeHdc(), x+16, y, &theWChar, 1 );
			n++;
		}
	}
}


// diagnostics pour CfontviewerView

#ifdef _DEBUG
void CfontviewerView::AssertValid() const
{
	CView::AssertValid();
}

void CfontviewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CfontviewerDoc* CfontviewerView::GetDocument() const // la version non déboguée est en ligne
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CfontviewerDoc)));
	return (CfontviewerDoc*)m_pDocument;
}
#endif //_DEBUG


// gestionnaires de messages pour CfontviewerView
