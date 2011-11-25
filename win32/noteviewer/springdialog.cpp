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
// SpringDialog.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "SpringDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GUIDOEngine.h"
//#include "secureio.h"
#include <stdio.h>
#define snprintf _snprintf
#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE

/////////////////////////////////////////////////////////////////////////////
// Dialogbox CSpringDialog 

CSpringDialog::CSpringDialog( GuidoLayoutSettings * settings, CWnd * pParent )
	: CDialog(CSpringDialog::IDD, pParent)
{
	mSettings = settings;
	//{{AFX_DATA_INIT(CSpringDialog)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}


void CSpringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpringDialog)
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_EDITSPRING, m_EditSpring);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpringDialog, CDialog)
	//{{AFX_MSG_MAP(CSpringDialog)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CSpringDialog 

BOOL CSpringDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_slider.SetRange(100,500);
	float npar = mSettings->spring; // was GuidoGetSpringParameter(&npar);

	npar *= 100;
	if (npar<100)
		npar = 100;
	if (npar>500)
		npar = 500;

	char txt[50];
	springpar = npar/100;
	snprintf(txt,50,"%.2f",springpar);
	m_EditSpring.SetWindowText(txt);

	int npos = 600 - (int) npar;
	m_slider.SetPos(npos);
	// TODO: add additional initialisations
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

void CSpringDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Insert here code for the handling of messages or/and call std functions

	int npos = m_slider.GetPos();
	springpar = (600 - npos ) / 100.0f;
	
	char txt[40];
	snprintf(txt,40,"%.2f",springpar);
	m_EditSpring.SetWindowText(txt);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
