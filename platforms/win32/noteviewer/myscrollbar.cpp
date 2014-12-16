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
// MyScrollBar.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "MyScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyScrollBar

CMyScrollBar::CMyScrollBar()
{
}

CMyScrollBar::~CMyScrollBar()
{
}


BEGIN_MESSAGE_MAP(CMyScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CMyScrollBar)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CMyScrollBar 

BOOL CMyScrollBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Insert here code for the handling of messages or/and call std functions
	
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return true;
	// return CScrollBar::OnSetCursor(pWnd, nHitTest, message);
}
