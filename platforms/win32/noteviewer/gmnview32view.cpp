/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2004  Grame

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

#include "stdafx.h"

#include <fstream>
using namespace std;

#include <io.h>

// - GuidoLib 
#include "GDevicePostScript.h"
#include "GUIDOEngine.h"
#include "GuidoFeedbackWin32.h"

#include "Guido2Midi.h"
#include "Guido2GIF.h"

#include "GUIDOScoreMap.h"

// - NoteViewer
#include "gmnview32.h"
#include "gmnview32View.h"
#include "gmnview32Doc.h"

#include "NoteViewPageWindow.h"
#include "ZoomDialog2.h"
#include "PageDialog.h"
#include "VirtualPageDialog.h"
#include "GMNTextDialog.h"
#include "ExportGifDialog.h"

#ifdef UseGDIPlus
#include "GSystemWin32GDIPlus.h"
#include "GDeviceWin32GDIPlus.h"
#else
#include "GSystemWin32.h"
#include "GDeviceWin32.h"
#endif

#include <mmsystem.h>		// for MCI
#include <vfw.h>			// for MCIWnd

//#include "secureio.h"
#include <stdio.h>
#define snprintf _snprintf
#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE


IMPLEMENT_DYNCREATE(CGmnview32View, CView)

BEGIN_MESSAGE_MAP(CGmnview32View, CView)
	//{{AFX_MSG_MAP(CGmnview32View)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_ZOOM, OnViewZoom)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM, OnUpdateViewZoom)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_VIEW_GOTOPAGE, OnViewGotopage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GOTOPAGE, OnUpdateViewGotopage)
	ON_COMMAND(ID_FILE_VIRTUALPAGE, OnFileVirtualpage)
	ON_COMMAND(ID_EDIT_GMNTEXT, OnEditGmntext)
	ON_COMMAND(ID_PLAYASMIDI, OnPlayAsMIDI)
	ON_WM_CLOSE()
	ON_COMMAND(ID_TESTGGS, OnTestggs)
	ON_COMMAND(ID_FILE_STOPMIDIPLAYBACK, OnFileStopMidiPlayback)
	ON_UPDATE_COMMAND_UI(ID_FILE_STOPMIDIPLAYBACK, OnUpdateFileStopmidiplayback)
	ON_COMMAND(ID_FILE_RELOAD, OnFileReload)
	ON_UPDATE_COMMAND_UI(ID_FILE_RELOAD, OnUpdateFileReload)
	ON_WM_SETFOCUS()
//	ON_MESSAGE(MM_MCINOTIFY, OnMCINotify)
	ON_COMMAND(ID_FILE_EXPORTGIF, OnFileExportgif)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// ----------------------------------------------------------------------------
CGmnview32View::CGmnview32View()
{
	mZoom = 0.75f;	// Initial zoom
	mPage = 1;
	mScrollX = 0;
	mScrollY = 0;

	mWindowWidthVirtual = 0;
	mWindowHeightVirtual = 0;
	
	midiactive = false;

	mPageToPrint = 0;
	mIsPrintPreview = 0;
}

// ----------------------------------------------------------------------------
CGmnview32View::~CGmnview32View()
{
	if (midiactive)
	{
		midiactive = false;
		mciSendString("stop temp wait",NULL,0,0);
		mciSendString("close temp",NULL,0,0);

		remove(tmpmidifile);
	}
}

// ----------------------------------------------------------------------------
int CGmnview32View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// - Creates the scroll bar

	CRect r(lpCreateStruct->x,
			  lpCreateStruct->y,
			  lpCreateStruct->cx,
			  lpCreateStruct->cy);
	hscroll.Create(SBS_HORZ | SBS_BOTTOMALIGN, r, this, 1);
	hscroll.ShowScrollBar();

	vscroll.Create(SBS_VERT | SBS_RIGHTALIGN, r, this, 7);
	vscroll.ShowScrollBar();

	// - Creates tool buttons
	pagebutton.Create("",BS_PUSHBUTTON,	r,this,2);
	pagebutton.ShowWindow(SW_SHOW);

	pageplusbutton.Create("+",BS_PUSHBUTTON, r,this,5);
	pageplusbutton.ShowWindow(SW_SHOW);
	
	pageminusbutton.Create("-",BS_PUSHBUTTON, r,this,6);

	pageminusbutton.ShowWindow(SW_SHOW);

	pageNumStatic.Create("",SS_CENTER, r,this,3);

	myfont.CreateFont( (int)(GetSystemMetrics(SM_CYHSCROLL)*0.9),
		0,0,0,0,0,0,0,ANSI_CHARSET,0,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,VARIABLE_PITCH | FF_SWISS,NULL);

	// pageNumStatic.SetFont(&myfont);
	pagebutton.SetFont(&myfont);
	pageplusbutton.SetFont(&myfont);
	pageminusbutton.SetFont(&myfont);

	// now create the NoteViewWindow
	//myNVPWindow.Create(this);
	//myNVPWindow.ShowWindow(SW_SHOW);	
		
	return 0;
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	updatePageButton( GetGuidoHandleGR());
	updateScrollBars();

#ifdef UseGDIPlus
	mSys = new GSystemWin32GDIPlus((GetDC())->m_hDC, NULL);
#else
	mSys = new GSystemWin32((GetDC())->m_hDC, NULL);
#endif
}

// ----------------------------------------------------------------------------
BOOL CGmnview32View::PreCreateWindow(CREATESTRUCT& cs)
{
	// If this is not there, the scrollbar gets
	// Repainted all the time ... we do not want this!
	cs.style |= WS_CLIPCHILDREN;
	
	return CView::PreCreateWindow(cs);
}

// ----------------------------------------------------------------------------
/** \brief Draw the current music score page on a graphic device (printer or screen)

	The size of musical elements does not not depend on the current window size.		
*/
void CGmnview32View::OnDraw(CDC * pDC)
{
	NoteViewerDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	const GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR )
	{
		// - Get the device (screen or printer) size
		const int deviceWidth = pDC->GetDeviceCaps( HORZRES );
		const int deviceHeight = pDC->GetDeviceCaps( VERTRES );

		VGDevice * device = 0;

		// - Setup the drawing infos structure
		GuidoOnDrawDesc desc;
		if (mPageToPrint)				// If greater than zero we are currently printing.
		{
			const int printWidth = deviceWidth;
			const int printHeight = deviceHeight;
#ifdef UseGDIPlus
			((GSystemWin32GDIPlus*)mSys)->SetPrinterDC(pDC->m_hDC);
#else
			((GSystemWin32*)mSys)->SetPrinterDC(pDC->m_hDC);
#endif
			device = mSys->CreatePrinterDevice();
			device->NotifySize(printWidth, printHeight);

			desc.page = mPageToPrint;
			desc.scrollx = desc.scrolly = 0;
			desc.sizex = printWidth;
			desc.sizey = printHeight;
			// desc.isprint = 1;			// (JB) isprint is now ignored.
		}
		else
		{
			CRect rect;
			GetClientRect(&rect);
			pDC->ExcludeClipRect(0,
				rect.bottom - GetSystemMetrics( SM_CYHSCROLL ), 
				rect.right - GetSystemMetrics( SM_CXVSCROLL ),
				rect.bottom);
		
			// Setup a graphic context with the current screen resolution, because size
			// of the drawing only depends on the zoom, not the window size.
			float gdWidth, gdHeight;
			CalcScorePixelSize( &gdWidth, &gdHeight );

			device = mSys->CreateDisplayDevice( );

			desc.page = mPage;
			desc.scrollx = mScrollX;		// Guido scroll is in virtual parameters (not pixels)
			desc.scrolly = mScrollY;
			desc.sizex = int(gdWidth * mZoom); //deviceWidth * mZoom; // deviceSizeX * zoom;
			desc.sizey = int(gdHeight * mZoom); //deviceHeight * mZoom; // deviceSizeY * zoom;
		}
		desc.handle = handleGR;
		desc.hdc = device;
		desc.updateRegion.erase = 1;	// Redraw everything .updateRect is not directly given ...

//		int n = GetTickCount();
		GuidoOnDraw( &desc );
//		int elapsed = GetTickCount() - n;
//#ifdef UseGDIPlus
//		long meantime = GDeviceWin32GDIPlus::Time2DrawMusic();
//#else
//		long meantime = GDeviceWin32::Time2DrawMusic();
//#endif
		delete device;
	}
	else // No score to draw, display the error.
	{
		NoteViewerDoc * doc = GetDocument();
		GuidoErrCode err = doc->GetLastError();

		pDC->TextOut( 30, 30, GuidoGetErrorString( err ));
		switch( err )
		{
			case guidoErrParse:
			{
				const int errorLine = doc->GetParseErrorLine();
				char txt[64];
				snprintf(txt,64,"Line: %d", errorLine);
				pDC->TextOut(30, 50, txt );
				pDC->TextOut(30, 70, "Try to correct the error by pressing CTRL-E");
				break;
			}

			case guidoErrNoMusicFont:
				pDC->TextOut(30, 50, "\"Guido2.ttf\"" );
				break;
		}
	}
}


// ----------------------------------------------------------------------------
/** \brief Return the size of our score page, in pixels.

	Currently, we decide that the "pixel" resolution of the score depends
	on the screen resolution. i.e: for a zoom = 1, the score width equal
	the screen width (heights are different).

	The "pixel" size and the Guido internal size have the same with/height ratio.

	\param outWidth on output, the width of the score, in pixel units.
	\param outHeight on output, the height of the score, in pixel units.
*/
void 
CGmnview32View::CalcScorePixelSize( float * outWidth, float * outHeight )	
{
	HDC screenDC = ::GetDC( 0 );

	const GRHandler handleGR = GetGuidoHandleGR();

	const int screenWidth = ::GetDeviceCaps( screenDC, HORZRES );
	// const int screenHeight = ::GetDeviceCaps( screenDC, VERTRES );
	
	GuidoPageFormat format;
	GuidoGetPageFormat( handleGR, GetPage(), &format );

	const float virtualSizeX = format.width; // (float)GuidoGetSizeX( handleGR );
	const float virtualSizeY = format.height; // (float)GuidoGetSizeY( handleGR );
	const float ratio = virtualSizeY / virtualSizeX;

	*outWidth = float(screenWidth);
	*outHeight = screenWidth * ratio;

	::ReleaseDC( 0, screenDC );
}

// ============================================================================
//			Printing
// ============================================================================

BOOL CGmnview32View::OnPreparePrinting(CPrintInfo* pInfo)
{
	GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR )
	{
		const int oldpage = mPageToPrint; //;GuidoGetPage( handleGR );
		const int npages = GuidoGetPageCount( handleGR );
		pInfo->m_nCurPage = oldpage;
		pInfo->SetMaxPage(npages);
	}
	return DoPreparePrinting(pInfo);
}

// ----------------------------------------------------------------------------
/** \brief Called once, just after OnPreparePrinting()
*/
void CGmnview32View::OnBeginPrinting(CDC * pDC, CPrintInfo * /* pInfo*/ )
{
}

// ----------------------------------------------------------------------------
/** \brief Called after OnBeginPrinting() and for each page.
*/void 
CGmnview32View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnPrepareDC(pDC, pInfo);

	if (pInfo)
	{
		// handle the zoomfactor for the pDC ...
		const GRHandler handleGR = GetGuidoHandleGR();
		if( handleGR )
		{
			mPageToPrint = pInfo->m_nCurPage;	
			//GuidoSetPage( handleGR, mPageToPrint );
			// now set the Window and Viewport origins ...
		
			
		//	pDC->SetMapMode( MM_ANISOTROPIC );
		//	pDC->SetGraphicsMode( GM_COMPATIBLE );
			
			
			/* 	pDC->SetWindowExt(GuidoGetSizeX(handleGR), GuidoGetSizeY(handleGR));
				int resx1 = pDC->GetDeviceCaps(HORZRES);
				int resy1 = pDC->GetDeviceCaps(VERTRES);
				pDC->SetViewportExt(resx1,resy1);
			*/
		}
	}
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	mIsPrintPreview = pInfo->m_bPreview;
	CView::OnPrint(pDC, pInfo);
}

// ----------------------------------------------------------------------------
/** \brief Called once after the last printed page.
*/
void CGmnview32View::OnEndPrinting(CDC * pDC, CPrintInfo * /*pInfo*/)
{
	// GuidoSetPage( GetGuidoHandleGR(), mPage);
	mPageToPrint = 0;
}

// ============================================================================
//		Mouse, events and scrollbars
// ============================================================================
void 
CGmnview32View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect r;
	GetClientRect(&r);
	if (point.y >= r.bottom - GetSystemMetrics(SM_CYHSCROLL))
		return;
	if (point.x >= r.right - GetSystemMetrics(SM_CXVSCROLL))
		return;

	CalcZoomIn();

	updateScrollBars();

	Invalidate(true);
}

// ----------------------------------------------------------------------------
void 
CGmnview32View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown( nFlags, point );
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Insert here code for the handling of messages or/and call std functions

	CRect r;
	GetClientRect(&r);
	if (point.y >= r.bottom - GetSystemMetrics(SM_CYHSCROLL))
		return;

	CalcZoomOut();

	updateScrollBars();

	Invalidate(true);
	// CView::OnRButtonDown(nFlags, point);
}

// ----------------------------------------------------------------------------
void 
CGmnview32View::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	OnRButtonDown( nFlags, point );
}

// ----------------------------------------------------------------------------
void CGmnview32View::CalcZoomIn()
{
	mZoom *= 5.0f / 4.0f;

	if (mZoom > 20.0f)	mZoom = 20.0f;	//(JB) changed from 5 to 20
} 

void CGmnview32View::CalcZoomOut()
{
	mZoom *= 4.0f / 5.0f;

	if (mZoom < 0.01f)	mZoom = 0.01f;
} 

// ----------------------------------------------------------------------------
void CGmnview32View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: inser code for message handling
	 if (nType == SIZE_MINIMIZED  ||
		  // sizeType == SIZE_MAXHIDE ||
		  nType == SIZE_MAXSHOW)
			return;

	 const int oldscrollx = mScrollX;
	 const int oldscrolly = mScrollY;

	 CSize size(cx,cy);

	 int hy = GetSystemMetrics(SM_CYHSCROLL);
	 int hx = GetSystemMetrics(SM_CXVSCROLL);

	 vscroll.MoveWindow(cx-hx,0,hx,cy-hy); 
	 hscroll.MoveWindow(80,cy-hy,cx-80,hy);


	 pageminusbutton.MoveWindow(0,cy-hy,20,hy);
	 
	 pagebutton.MoveWindow(20,cy-hy,40,hy);

	 pageplusbutton.MoveWindow(60,cy-hy,20,hy);

	 // pageNumStatic.MoveWindow(0,cy-hy,100,hy);


	 updateScrollBars(
		 //&size
		 );

	 int erase = 0;
	 if (oldscrollx != mScrollX || oldscrolly != mScrollY)
		erase = 1;
	 Invalidate(erase);
	 // UpdateWindow();
}

// ----------------------------------------------------------------------------
void CGmnview32View::updateScrollBars(CSize * inSize)
{
	const GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR == 0 ) return;

	// - Get the size of the inner part of our window (without scrollbars)
	RECT rect;
	if (inSize)
	{
		rect.left = 0;
		rect.top = 0;
		rect.right = inSize->cx;
		rect.bottom = inSize->cy;
	}
	else
	{
		GetClientRect(&rect);
		rect.bottom -= GetSystemMetrics(SM_CYHSCROLL);
		rect.right -= GetSystemMetrics(SM_CXVSCROLL);
	}

	// - Get the guido score size in virtual units
	float gdWidth, gdHeight;
	CalcScorePixelSize( &gdWidth, &gdHeight );

	GuidoPageFormat format;
	GuidoGetPageFormat( handleGR, GetPage(), &format );

	const float virtualSizeX = (float)format.width; // (float)GuidoGetSizeX( handleGR );
	const float virtualSizeY = (float)format.height; // (float)GuidoGetSizeY( handleGR );

	const float pixelToVirtual = virtualSizeX / gdWidth;

	mWindowWidthVirtual = int((rect.right - rect.left) * pixelToVirtual / mZoom);	
	mWindowHeightVirtual = int((rect.bottom - rect.top) * pixelToVirtual / mZoom);



//	const int screenWidth = ::GetDeviceCaps( hdc, HORZRES );
//	const int screenHeight = ::GetDeviceCaps( hdc, VERTRES );

	// - Get the screen pixel size, just as in OnDraw()
	HWND winH = *this;
	HDC hdc = ::GetDC( winH );
//	::ReleaseDC( winH, screendevice->DetachHDC());

/*
	GDeviceWin32 * screendevice = NoteViewerCreateGDevice( hdc, screenWidth, screenHeight ); 

	// - Convert from pixel to guido units: we need to setup a 
	// device context with the same paramaters as in OnDraw().
	// TODO: Beter !
	float tempX = winsizexLP;
	float tempY = winsizeyLP;
	GuidoConvertToVirtualUnits( *screendevice, handleGR, mZoom, &tempX, &tempY);
	winsizexLP = tempX;
	winsizeyLP = tempY;
	delete screendevice;

*/

	// -- Update scrollbars settings
	SCROLLINFO desc;
	desc.cbSize = sizeof( SCROLLINFO );
    desc.fMask = SIF_ALL; // SIF_PAGE; 
	desc.nMin = 0; 
	desc.nTrackPos = 0; 

	// - Setup the horizontal scrollbar according to new settings

	if( mWindowWidthVirtual > virtualSizeX ) // disable bar if window is larger than score
	{
		mScrollX = 0;
		hscroll.SetScrollPos( 0 );
		hscroll.EnableScrollBar(ESB_DISABLE_BOTH);
	}
	else
	{
		hscroll.EnableScrollBar(ESB_ENABLE_BOTH);
	
		desc.nMax = int(virtualSizeX); 
	    desc.nPage = mWindowWidthVirtual; 
	    desc.nPos = mScrollX; 
		hscroll.SetScrollInfo( &desc, true );
	}

	// - Setup the vertical scrollbar according to new settings

	if( mWindowHeightVirtual > virtualSizeY )
	{
		 mScrollY = 0;
		 vscroll.SetScrollPos( 0 );
		 vscroll.EnableScrollBar(ESB_DISABLE_BOTH);
	}
	else
	{
		vscroll.EnableScrollBar(ESB_ENABLE_BOTH);
	
		desc.nMax = int(virtualSizeY); 
	    desc.nPage = mWindowHeightVirtual; 
	    desc.nPos = mScrollY; 
		vscroll.SetScrollInfo( &desc, true );
	 }
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar) 
{
	const int prevScroll = mScrollX;
	const GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR == 0 ) return;

	if (nSBCode == SB_ENDSCROLL)
	  return;

	GuidoPageFormat format;
	GuidoGetPageFormat( handleGR, GetPage(), &format );

	const float virtualSizeX = format.width; // (float)GuidoGetSizeX( handleGR );
	//const float virtualSizeY = format.height; // (float)GuidoGetSizeY( handleGR );

	const int maxx = int(virtualSizeX - mWindowWidthVirtual);

	switch (nSBCode)
	{
		case SB_BOTTOM:		mScrollX = maxx;			break;
		case SB_TOP:		mScrollX = 0;				break;
	
		case SB_LINEDOWN:	mScrollX += mWindowWidthVirtual/10;	break;
		
		case SB_LINEUP:  	mScrollX -= mWindowWidthVirtual/10; 	break;
		case SB_PAGEDOWN:	mScrollX += mWindowWidthVirtual;		break;
		case SB_PAGEUP:		mScrollX -= mWindowWidthVirtual;		break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:		mScrollX = nPos;		break;
	}

	if (mScrollX > maxx) mScrollX = maxx;
	else if (mScrollX < 0) mScrollX = 0;

	if( mScrollX != prevScroll )
	{
		hscroll.SetScrollPos(mScrollX);
		Invalidate(true);
	}
	  
  // UpdateWindow();

  // I don't know, if we need this?
	// CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar) 
{
	const int prevScroll = mScrollY;
	const GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR == 0 ) return;

	if (nSBCode == SB_ENDSCROLL)
		return;

	GuidoPageFormat format;
	GuidoGetPageFormat( handleGR, GetPage(), &format );
	const float virtualSizeY = format.height; // (float)GuidoGetSizeY( handleGR );
	const int maxy = int(virtualSizeY - mWindowHeightVirtual);

	 // EINFUEGEN>> Ihren Code fuegen Sie hier ein.
	switch (nSBCode)
	{
		case SB_BOTTOM:		mScrollY = maxy;		break;
		case SB_TOP:		mScrollY = 0;			break;
		case SB_ENDSCROLL:	break;
		case SB_LINEDOWN:	mScrollY += mWindowHeightVirtual / 10;	break;
		case SB_LINEUP:		mScrollY -= mWindowHeightVirtual / 10;	break;
		case SB_PAGEDOWN:	mScrollY += mWindowHeightVirtual;			break;		
		case SB_PAGEUP:		mScrollY -= mWindowHeightVirtual;			break;
		
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:		mScrollY = nPos;			break;
	}

	if (mScrollY > maxy) mScrollY = maxy;
	else if (mScrollY < 0) mScrollY = 0;
 
	if( mScrollY != prevScroll )
	{
		vscroll.SetScrollPos(mScrollY);
		Invalidate(true);
	}

  // UpdateWindow();
	// CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	SHORT shift = GetKeyState(VK_SHIFT) & 0x80;
	SHORT pd,pu;
	if (!shift)
	{
		pd = SB_PAGEDOWN;
		pu = SB_PAGEUP;

	} else
	{
		pd = SB_LINEDOWN;
		pu = SB_LINEUP;
	}
			
	const GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR )
	{
		int oldpage = GetPage();
		int npages = GuidoGetPageCount( handleGR );

		if (nChar == VK_PRIOR)
		{
			mPage -= nRepCnt;
		}
		else if (nChar == VK_NEXT)
		{
			mPage += nRepCnt;
		}
	
	/* Handled by OnChar

		else if (nChar == VK_ADD)
		{
			for (UINT i=0;i<nRepCnt;i++)
				CalcZoomIn();
			
			updateScrollBars();
			Invalidate(true);
		}
		else if (nChar == VK_SUBTRACT)
		{
			for (UINT i=0;i<nRepCnt;i++)
				CalcZoomOut();
			
			updateScrollBars();
			Invalidate(true);
		}
	*/
		else if (nChar == VK_DOWN && vscroll.IsWindowEnabled())
		{
			PostMessage(WM_VSCROLL,pd,(LPARAM) vscroll.m_hWnd);
		}
		else if (nChar == VK_UP && vscroll.IsWindowEnabled())
		{
			PostMessage(WM_VSCROLL,pu,(LPARAM) vscroll.m_hWnd);
		}
		else if (nChar == VK_LEFT && hscroll.IsWindowEnabled())
		{
			PostMessage(WM_HSCROLL,pu,(LPARAM) hscroll.m_hWnd);
		}
		else if (nChar == VK_RIGHT && hscroll.IsWindowEnabled())
		{
			PostMessage(WM_HSCROLL,pd,(LPARAM) hscroll.m_hWnd);
		}
		else
		{
			CView::OnKeyDown(nChar, nRepCnt, nFlags);
		}

		if (mPage < 1 )		mPage = 1;
		if (mPage > npages)	mPage = npages;

		if (oldpage != mPage)
		{
			updatePageButton( handleGR );
			updateScrollBars();
			Invalidate(true);
		}
	}
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnActivateView(BOOL bActivate, CView * pActivateView, CView * pDeactiveView) 
{
	// TODO: insert here your own code and/or call base-class

/*	if (bActivate &&
		pActivateView == pDeactiveView)
		isactivated = true;
	else
		isactivated = false; */
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == '+')
	{
		for (UINT i=0;i<nRepCnt;i++)
			CalcZoomIn();

		updateScrollBars();
		Invalidate(true);
	}
	else if (nChar == '-')
	{
		for (UINT i=0;i<nRepCnt;i++)
			CalcZoomOut();
		
		updateScrollBars();
		Invalidate(true);
	}
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}

// ----------------------------------------------------------------------------
int CGmnview32View::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	int myret = CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	/* if (myret == MA_ACTIVATE && GetMDIFrame()->MDIGetActive() 
		== this)
		return MA_ACTIVATEANDEAT; */
	return myret;
}

// ----------------------------------------------------------------------------
BOOL CGmnview32View::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// this is the page+button
	if (wParam == 5)
	{
		SendMessage(WM_KEYDOWN,(WPARAM) VK_NEXT,(LPARAM)
			MAKELPARAM(1,0));
		SetFocus();
		return true;
	}
	else if (wParam == 6) // page-button
	{
		SendMessage(WM_KEYDOWN,(WPARAM) VK_PRIOR, (LPARAM)
			MAKELPARAM(1,0));
		SetFocus();
		return true;
	}
	else if (wParam == 2) // just the number button
	{
		OnViewGotopage();
	}

	SetFocus();
	
	return CView::OnCommand(wParam, lParam);
}

// ----------------------------------------------------------------------------
BOOL CGmnview32View::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CView::OnNotify(wParam, lParam, pResult);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnViewZoom() 
{
	// TODO: Code fur Befehlsbehandlungsroutine hier einfügen
	// this is called, when the user selects the zoom-Menu

	CZoomDialog2 myzd( this, mZoom );

	if (myzd.DoModal() == IDOK)
	{
		mZoom = myzd.getZoom();
				
		updateScrollBars();

		Invalidate(true);
	} 
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnUpdateViewZoom(CCmdUI* pCmdUI) 
{
}

// ----------------------------------------------------------------------------
BOOL CGmnview32View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//	if ( m_ChangeCursor )
//    {
		::SetCursor(AfxGetApp()->LoadCursor(IDC_LUPE));
        return TRUE;
//    }

//	return CView::OnSetCursor(pWnd, nHitTest, message);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnViewGotopage() 
{
	GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR == 0 ) return;

	const int pageCount = GuidoGetPageCount( handleGR );
	if (pageCount > 1)
	{
		CPageDialog mypd(this, 1, pageCount, mPage);
		if (mypd.DoModal() == IDOK)
		{
			mPage = mypd.getPage();

			updatePageButton( handleGR );
			updateScrollBars();
			Invalidate(true);
		}
	}
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnUpdateViewGotopage(CCmdUI * pCmdUI) 
{
	int pageCount = GuidoGetPageCount( GetGuidoHandleGR());

	pCmdUI->Enable( pageCount > 1 );
}

// ----------------------------------------------------------------------------
void CGmnview32View::updatePageButton( GRHandler inHandleGR )
{
	if( inHandleGR == 0 ) return;

	const int pageCount = GuidoGetPageCount( inHandleGR );
	if (mPage > pageCount)	mPage = pageCount;

//	GuidoSetPage( inHandleGR, mPage );

	char buf[10];
	wsprintf(buf,"%d/%d",mPage, pageCount );
	//pageNumStatic.SetWindowText(buf);
	pagebutton.SetWindowText(buf);
	
	pageplusbutton.EnableWindow( mPage < pageCount );
	pageminusbutton.EnableWindow( mPage > 1);
	pagebutton.EnableWindow( pageCount > 1 );
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnFileVirtualpage() 
{
/*	TODO: adapt to new guido api
	
const GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR )
	{
		float dsx,dsy,dml,dmt,dmr,dmb; // Default-Page-Size
		float sx,sy,ml,mt,mr,mb;

		dsx = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpsx;
		dsy = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpsy;
		dml = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpml;
		dmt = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpmt;
		dmr = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpmr;
		dmb = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpmb;
//		int ignorepf = dynamic_cast<CGmnview32App *>(AfxGetApp())->vpignorepf;

		GuidoGetPageSizeCm( handleGR, &sx, &sy, &ml, &mt, &mr, &mb );
		CVirtualPageDialog vpd(this,dsx,dsy,dml,dmt,dmr,dmb,
			sx,sy,ml,mt,mr,mb);
		vpd.m_ignorePF = ignorepf;

		vpd.startwithpage = 2;

		if (vpd.DoModal() == IDOK)
		{
			// retrieve the pagesize 
			// (see, whether IgnorePF is set?)

			// retrieve default-page-size ...
			vpd.getMargins(true,&dsx,&dsy,&dml,&dmt,&dmr,&dmb);

			char * svpsx = new char[20];
			sprintf(svpsx,"%.3fcm",dsx);
			
			char * svpsy = new char[20];
			sprintf(svpsy,"%.3fcm",dsy);
			
			char * svpml = new char[20];
			sprintf(svpml,"%.3fcm",dml);
			
			char * svpmb = new char[20];
			sprintf(svpmb,"%.3fcm",dmb);
			
			char * svpmt = new char[20];
			sprintf(svpmt,"%.3fcm",dmt);
			
			char * svpmr = new char[20];
			sprintf(svpmr,"%.3fcm",dmr);
			
			GuidoSetDefaultPageSize(svpsx,svpsy,svpml,svpmt,svpmr,svpmb);
			
			delete [] svpsx;
			delete [] svpsy;
			delete [] svpml;
			delete [] svpmt;
			delete [] svpmr;
			delete [] svpmb;
			
			// check wether it changed ...
			
			float tstx,tsty,tstml,tstmt,tstmr,tstmb;
			GuidoGetDefaultPageSizeCm(&tstx,&tsty, &tstml,&tstmt,&tstmr,&tstmb);
			if ( (int) ( dsx *100 ) != (int) (tstx * 100) ||
				dsy != tsty ||
				dml != tstml ||
				dmt != tstmt ||
				dmr != tstmr ||
				dmb != tstmb)
			{
				::MessageBox(NULL,"The Values for the default page-size\n"
					"you have entered are not correct.\n"
					"They will be adjustet automatically!","WARNING",MB_OK);
				dsx = tstx;
				dsy = tsty;
				dml = tstml;
				dmt = tstmt;
				dmr = tstmr;
				dmb = tstmb;
			}
			
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpsx = dsx;
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpsy = dsy;
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpml = dml;
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpmt = dmt;
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpmr = dmr;
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpmb = dmb;
			dynamic_cast<CGmnview32App *>(AfxGetApp())->vpignorepf 
				= vpd.m_ignorePF;


			updatePageButton( handleGR );
			updateScrollBars();
			Invalidate();
		}
	}
	*/
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnEditGmntext() 
{
	CGMNTextDialog txtdlg;
	txtdlg.SetGuidoView( this );

	if (GetDocument()->tmpgmnbuf)
	{
		HCURSOR old = ::SetCursor(::LoadCursor(NULL,IDC_WAIT));
		GetDocument()->updateGMNText();
		::SetCursor(old);
	}
	
	// - Check if the text must be changed.
	CString * docText = &GetDocument()->m_GMNText;
	CString originalText (*docText);

	CString * newText = 0;
	txtdlg.m_GMNText = originalText;

	const int result = txtdlg.DoModal();
	if(result == IDOK)
	{
		newText = &txtdlg.m_GMNText;	// Will apply the new text.
	}
	else if( result == IDCLOSE )		// closing the dialog actually generate an IDCANCEL result :-/	
	{
		newText = 0;					// Will just leave the score as is.
	}
	else
		newText = &originalText;		// Cancel: will restore the original text.


	if( newText && (*docText != *newText ))
		ApplyGmnText( newText );
}

// ----------------------------------------------------------------------------
void
CGmnview32View::ApplyGmnText( CString * inText )
{
	// submit the new text ....
	GetDocument()->update( *inText );

	const GRHandler handleGR = GetGuidoHandleGR();
	updatePageButton( handleGR );
	updateScrollBars();
	Invalidate();
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnPlayAsMIDI() 
{
 	if (midiactive)
	{
		midiactive = false;
		mciSendString("stop temp wait",NULL,0,0);
		mciSendString("close temp",NULL,0,0);

		remove(tmpmidifile);
		// this closes midi-stuff ....
		// OnMCINotify(0,0);
	}

	// first, we need to create a midi-file from
	// the current data ....
	char tmppath[MAX_PATH];
	GetTempPath(MAX_PATH,tmppath);
	GetTempFileName(tmppath,"gmn",0,tmpmidifile);

	char tmpgmn[MAX_PATH];
	GetTempFileName(tmppath,"g2m",0,tmpgmn);

	// save the document as a temporary file ...
	GetDocument()->OnSaveDocumentRelease(tmpgmn,false);
	char **argv = new char*[10];
	argv[0] = new char[100];
	argv[1] = new char[MAX_PATH];
	argv[2] = new char[MAX_PATH];
	argv[3] = new char[255];
	argv[4] = new char[255];
	// this is new Martin Friedmanns MIDI-Player
	strncpy(argv[0], "g2m_neu", 100);
	strncpy(argv[1], tmpgmn, MAX_PATH);
	strncpy(argv[2], tmpmidifile, MAX_PATH);
	strncpy(argv[3], "ce(9)", 255);
	strncpy(argv[4], "quiet", 255);

	try
	{
		gmn2midi(5,(char **) argv);
	}
	catch (...)
	{
		// here we catch everything ....
		ASSERT(FALSE);
	}

	delete [] argv[4];
	delete [] argv[3];
	delete [] argv[2];
	delete [] argv[1];
	delete [] argv[0];
	delete [] argv;

	// now we have the midi-file ...

	char playstring[MAX_PATH+200];
	wsprintf(playstring,"open %s type sequencer alias temp wait",
		tmpmidifile);
	// now play it ....
	unsigned long ret = mciSendString(playstring,NULL,0,0);

	ret = mciSendString("play temp notify" //  wait"
			, NULL,0,(HWND) *this);

	midiactive = true;

	remove(tmpgmn);
	
	// ret= mciSendString("close temp",NULL,0,0); 
}

// ----------------------------------------------------------------------------
BOOL CGmnview32View::OnMCINotify(WPARAM wParam, LPARAM lParam) 
{
	// mciSendString("stop temp wait",NULL,0,0);

	if (midiactive && wParam != MCI_NOTIFY_SUPERSEDED
		&& wParam != MCI_NOTIFY_ABORTED)
	{
		mciSendString("close temp wait",NULL,0,0);
		remove(tmpmidifile);
		midiactive = false;
	}
	return TRUE;
//	return CView::OnNotify(wParam, lParam, pResult);
}

// ----------------------------------------------------------------------------
BOOL CGmnview32View::DestroyWindow() 
{
	if (midiactive)
	{
		midiactive = false;
		mciSendString("stop temp wait",NULL,0,0);
		mciSendString("close temp",NULL,0,0);

		remove(tmpmidifile);

	}
	return CView::DestroyWindow();
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnClose() 
{
	// TODO: Insert here code for the handling of messages or/and call std functions
	
	if (midiactive)
	{
		midiactive = false;
		mciSendString("stop temp wait",NULL,0,0);
		mciSendString("close temp",NULL,0,0);

		remove(tmpmidifile);
	}

	CView::OnClose();
}

// ----------------------------------------------------------------------------
/** \brief this tests the GGS (Guido Graphic Stream)
*/
void CGmnview32View::OnTestggs() 
{
/*	TODO: adapt to new guido api ?
	
char * s = "testlin.gmn";
	GRHandler handleGR = GuidoParse(s);
	if( handleGR )
	{
		int p = GuidoGetPageCount( handleGR );
    	if (p > 0)
		{
			// get the ggsoutput of first page ...
			GuidoGraphicStreamOutput( handleGR,1);

			// now we call the input ...
			GuidoGraphicStreamInput( handleGR,1,"\\add<\"qnote\",650,320>\n");
		}

		GuidoFreeGR( handleGR );
    }
	*/
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnFileStopMidiPlayback() 
{
	// this stops midiplayback ...
	if (midiactive)
	{
		midiactive = false;
		mciSendString("stop temp wait",NULL,0,0);
		mciSendString("close temp",NULL,0,0);

		remove(tmpmidifile);
	}	
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnUpdateFileStopmidiplayback(CCmdUI* pCmdUI) 
{
	if (midiactive)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnFileReload() 
{
	NoteViewerDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->IsModified())
	{
		// Then we have to first save the old document,
		// if this is desired ... ?
		pDoc->SaveModified();

	}

	// now we have to see, whether the file exists on the disk

	if (_access(pDoc->GetPathName(),04) == -1)
	{
		// no file there for reading ....
		MessageBox("The file you are trying to reload does not exist.\n"
			"Reload will be ignored","Reload Error",MB_OK);
		return;
	}

	// now we close the docuemnt and reload the new one ....

	pDoc->FreeARGR();


	pDoc->OnOpenDocument( pDoc->GetPathName());

	GRHandler handleGR = GetGuidoHandleGR();
			
	updatePageButton( handleGR );
	updateScrollBars();
	Invalidate();
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnUpdateFileReload(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

// ----------------------------------------------------------------------------
// - New version: uses GDevicePostscript
void CGmnview32View::SaveAsEPS(LPCTSTR lpszPathName)
{
	GRHandler handleGR = GetGuidoHandleGR();
	if( handleGR == 0 ) return;

	//float widthCm, heightCm, ml, mt, mr, mb;	// size and margins
	GuidoPageFormat format;
	GuidoGetPageFormat( handleGR, mPage, &format );

	const float psWidth = GuidoUnit2Inches( format.width );
	const float psHeight = GuidoUnit2Inches( format.height );

	const char * versionString = "Guido Note Viewer";

	CString fontFile = CGmnview32App::GetAppPath();
	fontFile += "\\guido2_font.eps";

	GDevicePostScript psDevice( int(psWidth), int(psHeight), lpszPathName, versionString, (LPCTSTR)fontFile );

	GuidoOnDrawDesc desc;
	desc.handle = handleGR;
	desc.hdc = &psDevice;
	desc.updateRegion.erase = 1;
	desc.isprint = 0;
	desc.page = mPage;
	desc.sizex = int(psWidth);		// override zoom	
	desc.sizey = int(psHeight);		// override zoom
	desc.scrollx = 0;
	desc.scrolly = 0;
	GuidoOnDraw( &desc );
}

// ----------------------------------------------------------------------------
void CGmnview32View::SaveAsGMN(LPCTSTR lpszPathName)
{
	GetDocument()->OnSaveDocument(lpszPathName);
}

// ----------------------------------------------------------------------------
void CGmnview32View::OnFileExportgif() 
{
	NoteViewerDoc * mydoc = GetDocument();
	if (mydoc == 0) return;

	GRHandler handleGR = mydoc->GetGR();
	if( handleGR == 0 ) return;

	// we first get the valid page-range ....
	// we create a GIF-name that has not been used so far ...
	
	int numpages = GuidoGetPageCount( handleGR );
	if (numpages <= 0) return;

	CString mypath = mydoc->GetPathName();
	// now we cut the ending ....
	if (mypath.Find(".gmn") == mypath.GetLength() - 4
		|| mypath.Find(".GMN") == mypath.GetLength() - 4)
	{
		mypath = mypath.Left(mypath.GetLength()-4);
	}

	CString gifname;
	int cnt = 0;
	do
	{
		gifname = mypath;
		if (cnt == 0)
		{
			gifname += ".gif";
		}
		else	
		{
			char tmpbuf[10];
			snprintf(tmpbuf,10,"%d",cnt);
			gifname += tmpbuf;
			gifname +=  ".gif";
		}
		if (_access(gifname,0))
			break;
		++cnt;
	}
	while (cnt < 1000);

	if (cnt >= 1000)
	{
		gifname = mypath + ".gif";
	}

	CExportGifDialog egd;


	egd.m_filename = gifname;
	egd.m_numpages = numpages;
_start:


	if (egd.DoModal() == IDOK)
	{
		// then we can export the gif-file ....
		// we need to check the file name ....
		if (!_access(egd.m_filename,0) && 
			_access(egd.m_filename,02))
		{
			// the file exists, but it has only read permission ....
			// The file exists, ask for overwirte?
			char buf[MAX_PATH+100];
			snprintf(buf, MAX_PATH+100, "The file %s \nhas READ-ONLY permission.\n"
				"Please choose another file name",egd.m_filename);
			MessageBox(buf,"Export failed",MB_OK);

			goto _start;
			
		}
		else if (!_access(egd.m_filename,00))
		{
			// The file exists, ask for overwirte?
			char buf[MAX_PATH+100];
			snprintf(buf, MAX_PATH+100, "The file %s exists.\nOverwrite file?",egd.m_filename);
			if (MessageBox(buf,"Overwrite file?",MB_YESNO) == IDNO)
			{
				goto _start;
			}
		}

		ofstream myofs(egd.m_filename);
		if (myofs.fail())
		{
			// The file exists, ask for overwirte?
			char buf[MAX_PATH+100];
			snprintf(buf, MAX_PATH+100, "The file \"%s\" \ncan not be created.",egd.m_filename);
			MessageBox(buf,"Choose another filename",MB_OK);
			
			goto _start;
			
		}
		/*

		The guido2gif work like:

		if zoom == 1
		The content of a .gif should always equal to a single score-page 
		(size given by \pageFormat or default). The physical size of the .gif (if displayed on 
		the current screen, crop/pagesizeadjust=0) should be also equal (or similar as possible)
		 to that score-page-size pagewidth = 10cm -> gif-width == 10cm on 75dpi!
		If crop or pageSizeadjust = 1 the scaling should stay the same, only unused 
		parts should be removed.


		if zoom != 1
		The content of the .gif should also be equal to the score-page content. The physical size 
		if the .gif should be shrinked or expanded accoring to the zoom.

		*/

		// - Then we have a filename and settings ...
		
		// - Me make a copy of the GR structure to avoid side effects involved
		// by things such as GuidoAdjustPageSize().

		//GRHandler handleGR = GetGuidoHandleGR();
		ARHandler handleAR = mydoc->GetAR();
		GuidoLayoutSettings * settings = 0;
		GRHandler copyHandleGR = 0;

		GuidoErrCode err = GuidoAR2GR( handleAR, settings, &copyHandleGR );

		const int pageNum = egd.m_page;
		if( copyHandleGR )
		{
			if( egd.m_adjustpagesize )
				GuidoResizePageToMusic( copyHandleGR );
			
			int gifWidth = 100;	// in pixels
			int gifHeight = 100;
			
			if (egd.m_iszoom == 1)
			{
				const float theDpi = float(72.27);
				G2GIFCalcSizeFromZoom( copyHandleGR, pageNum, egd.m_zoomfactor, 
													theDpi, &gifWidth, &gifHeight );	
			}
			else
			{
				gifWidth = 	egd.m_sizex;
				gifHeight = egd.m_sizey;
			}
			
			const char * commentText = 0;

			G2GIFCreatePicture( copyHandleGR,
								pageNum,
								gifWidth,
								gifHeight,
								egd.m_filename,
								commentText );
		
			GuidoFreeGR( copyHandleGR );	
		}
	}
}

// ----------------------------------------------------------------------------
void CGmnview32View::SaveAsMID(LPCTSTR lpszPathName)
{
	// first, we need to create a midi-file from
	// the current data ....
	char tmppath[MAX_PATH];
	GetTempPath(MAX_PATH,tmppath);
	char tmpgmn[MAX_PATH];
	GetTempFileName(tmppath,"gmn",0,tmpgmn);

	// save the document as a temporary file ...
	GetDocument()->OnSaveDocumentRelease(tmpgmn,false);
	char **argv = new char*[10];
	argv[0] = new char[100];
	argv[1] = new char[MAX_PATH];
	argv[2] = new char[MAX_PATH];
	argv[3] = new char[255];
	// this is new Martin Friedmanns MIDI-Player
	strncpy(argv[0],"g2m_neu",100);
	strncpy(argv[1],tmpgmn, MAX_PATH);
	strncpy(argv[2],lpszPathName, MAX_PATH);
	strncpy(argv[3],"quiet", 255);

	try
	{
		gmn2midi(4,(char **) argv);
	}
	catch (...)
	{
		// here we catch everything ....
		ASSERT(FALSE);
	}

	delete [] argv[3];
	delete [] argv[2];
	delete [] argv[1];
	delete [] argv[0];
	delete [] argv;

	remove(tmpgmn);
}

