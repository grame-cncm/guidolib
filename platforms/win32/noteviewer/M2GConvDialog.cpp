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
#include "M2GConvDialog.h"
#include "resource.h"

CM2GConvDialog::CM2GConvDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CM2GConvDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CM2GConvDialog)
	m_logtext = _T("");
	//}}AFX_DATA_INIT
}


void CM2GConvDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CM2GConvDialog)
	DDX_Control(pDX, IDC_FULLLOG, m_fulllog);
	DDX_Text(pDX, IDC_LOGTEXT, m_logtext);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CM2GConvDialog, CDialog)
	//{{AFX_MSG_MAP(CM2GConvDialog)
	ON_BN_CLICKED(IDC_FULLLOG, OnFulllog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CM2GConvDialog 

BOOL CM2GConvDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: add additional initialisations
	
	if (m_isfulllog)
		m_fulllog.EnableWindow(0);
	else
		m_fulllog.EnableWindow(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

void CM2GConvDialog::OnFulllog() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen

	EndDialog(IDC_FULLLOG);
}
