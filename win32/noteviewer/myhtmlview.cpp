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


#include "stdafx.h"
#include "gmnview32.h"
#include "MyHTMLView.h"

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMyHTMLView

IMPLEMENT_DYNCREATE(CMyHTMLView, CHtmlView)

CMyHTMLView::CMyHTMLView()
{
	//{{AFX_DATA_INIT(CMyHTMLView)
		// HINWEIS: Der Klassen-Assistent fügt hier die Elementinitialisierung hinzu
	//}}AFX_DATA_INIT
}

CMyHTMLView::~CMyHTMLView()
{
}

void CMyHTMLView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyHTMLView)
		// HINWEIS: Der Klassen-Assistent fügt hier DDX- und DDV-Aufrufe hinzu
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyHTMLView, CHtmlView)
	//{{AFX_MSG_MAP(CMyHTMLView)
		// HINWEIS - Hier werden Umsetzungsmakros vom Klassen-Assistenten hinzugefügt und entfernt.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHTMLView Diagnose

#ifdef _DEBUG
void CMyHTMLView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CMyHTMLView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyHTMLView message handling

void CMyHTMLView::OnInitialUpdate() 
{
	// TODO: thjis code brings you to any web adress.
	// change this code for the desiered web-address
	LoadFromResource(IDR_HTML1);
}
