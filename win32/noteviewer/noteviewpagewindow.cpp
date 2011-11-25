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
// NoteViewPageWindow.cpp : implementation of class CNoteViewPageWindow
// What is this class for ?

#include "stdafx.h"
#include "gmnview32View.h"
#include "gmnview32.h"

#include "NoteViewPageWindow.h"
#include "GDeviceWin32.h"
#include "GUIDOEngine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoteViewPageWindow 

IMPLEMENT_DYNCREATE( CNoteViewPageWindow, CWnd)

BEGIN_MESSAGE_MAP(CNoteViewPageWindow, CWnd)
	//{{AFX_MSG_MAP(CNoteViewPageWindow)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoteViewPageWindow construction/deconstruction

CNoteViewPageWindow::~CNoteViewPageWindow()
{
}


/////////////////////////////////////////////////////////////////////////////
void CNoteViewPageWindow::OnDraw(CDC* pDC)
{
/*	const int deviceSizeX = pDC->GetDeviceCaps( HORZRES );
	const int deviceSizeY = pDC->GetDeviceCaps( VERTRES );

	CGmnview32View * parent = (CGmnview32View *) GetParent();
	if (parent->GetGuidoHandleGR())
	{
		GuidoOnDrawDesc c;
		c.handle = parent->GetGuidoHandleGR();
		GDeviceWin32 * device = NoteViewerCreateGDevice( pDC->m_hDC, deviceSizeX, deviceSizeY );
		c.hdc = device;
		c.page = parent->GetPage();
		c.updateRegion.erase = 1;
		c.scrollx = parent->GetScrollX();
		c.scrolly = parent->GetScrollY();
		c.zoom = (float)parent->GetZoom();	// obsolete, use sizex and sizey instead
		c.sizex = 0;
		c.sizey = 0;
		c.isprint = 0;

		// finale draw something
		::GuidoOnDraw( &c );
		delete device;
	}
*/
}

/////////////////////////////////////////////////////////////////////////////
// CNoteViewPageWindow Diagnose

#ifdef _DEBUG
void CNoteViewPageWindow::AssertValid() const
{
	CWnd::AssertValid();
}

void CNoteViewPageWindow::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}

#endif //_DEBUG


BOOL CNoteViewPageWindow::Create(CWnd * pParentWnd)
{
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 0;
	r.bottom = 0;
	return CWnd::Create(NULL,"CNoteViewPageWindow",WS_CHILD |
		WS_VISIBLE,r,pParentWnd,1);

}
