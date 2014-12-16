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
// ExportGifDialog.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "ExportGifDialog.h"
//#include "secureio.h"
#include <stdio.h>
#define snprintf _snprintf

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE

/////////////////////////////////////////////////////////////////////////////
// Dialogbox CExportGifDialog 
CExportGifDialog::CExportGifDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExportGifDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportGifDialog)
	m_adjustpagesize = TRUE;
	m_page = 1;
	m_sizex = 600;
	m_sizey = 800;
	m_zoomfactor = 1.0f;
	m_filename = _T("");
	m_pagerange = _T("");
	//}}AFX_DATA_INIT
	
}


void CExportGifDialog::DoDataExchange(CDataExchange* pDX)
{
	
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportGifDialog)
	DDX_Control(pDX, IDC_FILENAMEGIF, m_ctrl_filename);
	DDX_Control(pDX, IDC_SELECTZOOM, m_ctrl_selectzoom);
	DDX_Control(pDX, IDC_ZOOMFACTOR, m_ctrl_zoom);
	DDX_Control(pDX, IDC_SIZEY, m_ctrl_sizey);
	DDX_Control(pDX, IDC_SIZEX, m_ctrl_sizex);
	DDX_Check(pDX, IDC_ADJUSTPAGESIZE, m_adjustpagesize);
	DDX_Text(pDX, IDC_PAGENUM, m_page);
	DDV_MinMaxUInt(pDX, m_page, 1, m_numpages);
	DDX_Text(pDX, IDC_SIZEX, m_sizex);
	DDV_MinMaxUInt(pDX, m_sizex, 10, 3200);
	DDX_Text(pDX, IDC_SIZEY, m_sizey);
	DDV_MinMaxUInt(pDX, m_sizey, 10, 3200);
	DDX_Text(pDX, IDC_ZOOMFACTOR, m_zoomfactor);
	DDV_MinMaxFloat(pDX, m_zoomfactor, 0.1f, 10.0f);
	DDX_Text(pDX, IDC_FILENAMEGIF, m_filename);
	DDV_MaxChars(pDX, m_filename, 512);
	DDX_Text(pDX, IDC_PAGERANGE, m_pagerange);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CExportGifDialog, CDialog)
	//{{AFX_MSG_MAP(CExportGifDialog)
	ON_BN_CLICKED(IDC_SELECTZOOM, OnSelectzoom)
	ON_BN_CLICKED(IDC_SELECTSIZE, OnSelectsize)
	ON_BN_CLICKED(IDC_SELECTFILENAME, OnSelectfilename)
	ON_BN_CLICKED(IDC_ADJUSTPAGESIZE, OnAdjustpagesize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CExportGifDialog 

void CExportGifDialog::OnSelectzoom() 
{
	m_ctrl_sizex.EnableWindow(FALSE);
	m_ctrl_sizey.EnableWindow(FALSE);
	m_ctrl_zoom.EnableWindow(TRUE);
	m_iszoom = TRUE;
}

void CExportGifDialog::OnSelectsize() 
{
	m_ctrl_sizex.EnableWindow(TRUE);
	m_ctrl_sizey.EnableWindow(TRUE);
	m_ctrl_zoom.EnableWindow(FALSE);

	m_iszoom = FALSE;
}

BOOL CExportGifDialog::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: insert here your own code and/or call base-class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CExportGifDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: add additional initialisations
	OnSelectzoom();
	m_ctrl_selectzoom.SetCheck(1);
	m_iszoom = TRUE;
	
	char buf[50];
	snprintf(buf,50,"(%d-%d)",1,m_numpages);
	m_pagerange = buf;
	UpdateData(FALSE);

	int ln = m_filename.GetLength();
	m_ctrl_filename.SetSel(ln,ln);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

void CExportGifDialog::OnSelectfilename() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
		// TODO: Code für Befehlsbehandlungsroutine hier einfügen
	CFileDialog fileDialog(FALSE,".gif",this->m_filename,OFN_HIDEREADONLY 
		| OFN_PATHMUSTEXIST,
		"GIF file(*.gif)|*.gif||",this);

	fileDialog.m_ofn.lpstrTitle = "Export GIF File";

	if (fileDialog.DoModal() == IDOK)
	{
		// then we have a new filename ...
		m_filename = fileDialog.GetPathName();
		this->UpdateData(FALSE);
		
	}
}

void CExportGifDialog::OnAdjustpagesize() 
{
	if (m_adjustpagesize)
		m_adjustpagesize = FALSE;
	else
		m_adjustpagesize = TRUE;

}
