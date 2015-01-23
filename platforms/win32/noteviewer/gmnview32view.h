/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef CGmnview32View_H
#define CGmnview32View_H


// #include "NoteViewPageWindow.h"
#include "gmnview32Doc.h"
#include "PageButton.h"	// addaed by ClassView
#include "MyScrollBar.h"
#include "MyStatic.h"

class VGSystem;


class CGmnview32View : public CView
{
protected: // create from by seralising
	CGmnview32View();
	DECLARE_DYNCREATE(CGmnview32View)

public:

	NoteViewerDoc * GetDocument();

	// Overrides generiated by the class-wizzard
	//{{AFX_VIRTUAL(CGmnview32View)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnMCINotify(WPARAM wParam,LPARAM lParam);
	//}}AFX_VIRTUAL

public:

	virtual			~CGmnview32View();

			void	updatePageButton(GRHandler mhandle);
			void	SaveAsGMN(LPCTSTR lpszPathName);
	virtual void	SaveAsEPS(LPCTSTR lpszPathName);
	virtual void	SaveAsMID(LPCTSTR lpszPathName);

    virtual int		GetScrollX() const		{ return mScrollX; }
    virtual int		GetScrollY() const		{ return mScrollY; }
	virtual int		GetPage() const			{ return mPage; }
	virtual float	GetZoom() const			{ return mZoom; }
	virtual GRHandler GetGuidoHandleGR()	{ return GetDocument()->GetGR(); }

			void	ApplyGmnText( CString * inText );

protected:
	virtual void	updateScrollBars(CSize * inSize = NULL);
	virtual	void	CalcScorePixelSize( float * outWidth, float * outHeight );	
	
			void	CalcZoomIn();
			void	CalcZoomOut();

			VGSystem*	mSys;	//YC

	char tmpmidifile[MAX_PATH];

	float mZoom;

	int mPage;
	int mScrollX;
	int mScrollY;

	int mWindowWidthVirtual;
	int mWindowHeightVirtual;

	bool midiactive;

//	int psizey;
//	int psizex;
//	float pzoom;
	int mPageToPrint;
	BOOL mIsPrintPreview;

	CFont myfont;
	CMyStatic pageNumStatic;
	CPageButton pagebutton;
	CPageButton pageplusbutton;
	CPageButton pageminusbutton;
	CMyScrollBar hscroll;
	CMyScrollBar vscroll;

	// CNoteViewPageWindow myNVPWindow;
	// CWindow myNoteView;

	BOOL isactivated;

	//{{AFX_MSG(CGmnview32View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewZoom();
	afx_msg void OnUpdateViewZoom(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnViewGotopage();
	afx_msg void OnUpdateViewGotopage(CCmdUI* pCmdUI);
	afx_msg void OnFileVirtualpage();
	afx_msg void OnEditGmntext();
	afx_msg void OnPlayAsMIDI();
	afx_msg void OnClose();
	afx_msg void OnTestggs();
	afx_msg void OnFileStopMidiPlayback();
	afx_msg void OnUpdateFileStopmidiplayback(CCmdUI* pCmdUI);
	afx_msg void OnFileReload();
	afx_msg void OnUpdateFileReload(CCmdUI* pCmdUI);
	afx_msg void OnFileExportgif();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


inline NoteViewerDoc* CGmnview32View::GetDocument()
   { return (NoteViewerDoc*)m_pDocument; }


//{{AFX_INSERT_LOCATION}}


#endif

