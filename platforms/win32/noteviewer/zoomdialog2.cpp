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
// ZoomDialog2.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "ZoomDialog2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogbox CZoomDialog2 


CZoomDialog2::CZoomDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDialog2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CZoomDialog2)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CZoomDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CZoomDialog2)
	DDX_Control(pDX, IDC_EDITZOOM, m_ZoomEdit);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CZoomDialog2, CDialog)
	//{{AFX_MSG_MAP(CZoomDialog2)
	ON_WM_VSCROLL()
	ON_EN_UPDATE(IDC_EDITZOOM, OnUpdateEditzoom)
	ON_EN_CHANGE(IDC_EDITZOOM, OnChangeEditzoom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CZoomDialog2 

CZoomDialog2::CZoomDialog2(CWnd *pParent,float p_zoom)
	: CDialog(CZoomDialog2::IDD, pParent)

{
	zoom = p_zoom;
}

BOOL CZoomDialog2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: add additional initialisations
	m_Slider.SetRange(1,500);
	m_Slider.SetPos(501 - (int) (zoom*100));
	char txt[50];

	wsprintf(txt,"%d %%",501-m_Slider.GetPos());

	m_ZoomEdit.SetWindowText(txt);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

void CZoomDialog2::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Insert here code for the handling of messages or/and call std functions

	// 
	zoom = (501 - m_Slider.GetPos()) / (float) 100;

	char txt[50];
	wsprintf(txt,"%d %%",501-m_Slider.GetPos());
	m_ZoomEdit.SetWindowText(txt);
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

float CZoomDialog2::getZoom()
{

	return zoom;

}

void CZoomDialog2::OnUpdateEditzoom() 
{
	
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	
}

void CZoomDialog2::OnChangeEditzoom() 
{

	
}
