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
// SpacingDialog.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "SpacingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GUIDOEngine.h"
//#include "secureio.h"
#include <stdlib.h>
#include <stdio.h>
#define snprintf _snprintf
#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE


/////////////////////////////////////////////////////////////////////////////
// Dialogbox CSpacingDialog 


CSpacingDialog::CSpacingDialog( GuidoLayoutSettings * settings, CWnd * pParent )
	: CDialog(CSpacingDialog::IDD, pParent)
{
	mSettings = settings;
	m_def_sys_dis_changed = 0;
	float sys_dis = settings->systemsDistance;


	//{{AFX_DATA_INIT(CSpacingDialog)
	m_optforceint = 0;
	m_optpagefill = settings->optimalPageFill;
	m_specialspacing = settings->neighborhoodSpacing;
	m_def_sys_dis = 0;
	m_dis_auto = (int) sys_dis - 1;
	//}}AFX_DATA_INIT
}


void CSpacingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpacingDialog)
	DDX_Control(pDX, IDC_DEFAULT_SYS_DIS, m_def_sys_dis_disp);
	DDX_Control(pDX, IDC_SLIDER_DEF_SYS_DIS, m_ctrl_def_sys_dis);
	DDX_Control(pDX, IDC_FORCEDISPLAY, m_ctrl_forcedisplay);
	DDX_Control(pDX, IDC_OPTFORCE, m_slider);
	DDX_Slider(pDX, IDC_OPTFORCE, m_optforceint);
	DDX_Check(pDX, IDC_CHECK1, m_optpagefill);
	DDX_Check(pDX, IDC_SPECRHYTHM, m_specialspacing);
	DDX_Slider(pDX, IDC_SLIDER_DEF_SYS_DIS, m_def_sys_dis);
	DDX_Radio(pDX, IDC_DIS_AUTO, m_dis_auto);
	//}}AFX_DATA_MAP
}

#ifdef VC6
#define _itoa_s(v,str,l,r) itoa(v,str,r)
#endif

BOOL CSpacingDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_optpagefill = mSettings->optimalPageFill;
	m_specialspacing = mSettings->neighborhoodSpacing;
	m_slider.SetRange(400,1500);

	float optforce = mSettings->force;
	
	int npos = (int) optforce;
	m_slider.SetPos(npos);

	char mychar[20];
	_itoa_s(npos,mychar,20,10);
	this->m_ctrl_forcedisplay.SetWindowText(mychar);

	float defsysdis = mSettings->systemsDistance;;

	// this is a value in cm ....
	// map it into an int - range ...
	defsysdis *= 100; 
	if (defsysdis < 10)
		defsysdis = 10;
	if (defsysdis > 400)
		defsysdis = 400;
	m_ctrl_def_sys_dis.SetRange(10,400);
	m_ctrl_def_sys_dis.SetPos((int) defsysdis);
	snprintf(mychar,20,"%.2fcm",defsysdis / 100.0);
	m_def_sys_dis_disp.SetWindowText(mychar);


	// TODO: add additional initialisations
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

BEGIN_MESSAGE_MAP(CSpacingDialog, CDialog)
	//{{AFX_MSG_MAP(CSpacingDialog)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CSpacingDialog 

void CSpacingDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if ((CSliderCtrl *) pScrollBar == &m_slider)
	{
		int npos = m_slider.GetPos();

		char mychar[20];
		_itoa_s(npos,mychar,20,10);
		this->m_ctrl_forcedisplay.SetWindowText(mychar);
	}
	else if ((CSliderCtrl *) pScrollBar == &m_ctrl_def_sys_dis)
	{
		float defsysdis = (float)m_ctrl_def_sys_dis.GetPos();
		if (defsysdis != m_def_sys_dis)
		{
			m_def_sys_dis_changed = 1;
			if (defsysdis < 10)
				defsysdis = 10;
			if (defsysdis > 400)
				defsysdis = 400;
			char mychar[20];
			snprintf(mychar,20,"%.2fcm",defsysdis / 100.0);
			m_def_sys_dis_disp.SetWindowText(mychar);
		}
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
