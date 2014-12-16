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
// PageButton.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "PageButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageButton

CPageButton::CPageButton()
{
}

CPageButton::~CPageButton()
{
}


BEGIN_MESSAGE_MAP(CPageButton, CButton)
	//{{AFX_MSG_MAP(CPageButton)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CPageButton 

void CPageButton::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen

	// I do not want the focus!
	// GetParent()->SetFocus();
	
}

BOOL CPageButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Insert here code for the handling of messages or/and call std functions

	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return true;
	// return CButton::OnSetCursor(pWnd, nHitTest, message);
}
