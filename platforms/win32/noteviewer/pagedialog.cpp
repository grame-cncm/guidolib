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
// PageDialog.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "PageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogbox CPageDialog 


CPageDialog::CPageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageDialog)
	m_MaxPage = _T("");
	m_MinPage = _T("");
	//}}AFX_DATA_INIT
}


void CPageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDialog)
	DDX_Control(pDX, IDC_PAGESLIDER, m_PageSlider);
	DDX_Control(pDX, IDC_PAGEEDIT, m_PageControl);
	DDX_Text(pDX, IDC_MAXPAGE, m_MaxPage);
	DDX_Text(pDX, IDC_MINPAGE, m_MinPage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDialog, CDialog)
	//{{AFX_MSG_MAP(CPageDialog)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CPageDialog 

BOOL CPageDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: add additional initialisations
	m_PageSlider.SetRange(pmin,pmax);
	m_PageSlider.SetPos(page);
	char txt[20];
	wsprintf(txt,"%d",page);
	m_PageControl.SetWindowText(txt);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

CPageDialog::CPageDialog(CWnd *pParent, int min, int max, int cur)
	: CDialog(CPageDialog::IDD, pParent)
{

	m_MinPage.Format("%d",min);
	m_MaxPage.Format("%d",max);

	pmin = min;
	pmax = max;
	page = cur;
}

void CPageDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Insert here code for the handling of messages or/and call std functions
	
	page = m_PageSlider.GetPos();
	char txt[20];
	wsprintf(txt,"%d",page);
	m_PageControl.SetWindowText(txt);

	// CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CPageDialog::getPage()
{
	return page;
}
