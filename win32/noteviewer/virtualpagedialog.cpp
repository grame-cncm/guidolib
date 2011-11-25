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
// VirtualPageDialog.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "VirtualPageDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogbox CVirtualPageDialog 


CVirtualPageDialog::CVirtualPageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualPageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVirtualPageDialog)
	m_height = 0.0f;
	m_mbottom = 0.0f;
	m_mleft = 0.0f;
	m_mright = 0.0f;
	m_mtop = 0.0f;
	m_width = 0.0f;
	m_ignorePF = FALSE;
	//}}AFX_DATA_INIT
}


void CVirtualPageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirtualPageDialog)
	DDX_Text(pDX, IDC_HEIGHT, m_height);
	DDX_Text(pDX, IDC_MBOTTOM, m_mbottom);
	DDX_Text(pDX, IDC_MLEFT, m_mleft);
	DDX_Text(pDX, IDC_MRIGHT, m_mright);
	DDX_Text(pDX, IDC_MTOP, m_mtop);
	DDX_Text(pDX, IDC_WIDTH, m_width);
	DDX_Check(pDX, IDC_IGNOREPF, m_ignorePF);
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CVirtualPageDialog, CDialog)
	//{{AFX_MSG_MAP(CVirtualPageDialog)
	ON_BN_CLICKED(IDC_DEFPAGESIZE, OnDefpagesize)
	ON_BN_CLICKED(IDC_THISPAGESIZE, OnThispagesize)
	ON_BN_CLICKED(IDC_IGNOREPF, OnIgnorepf)
	ON_BN_CLICKED(IDC_ASPRINT, OnAsprint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CVirtualPageDialog 

CVirtualPageDialog::CVirtualPageDialog(CWnd * pParent,
		GuidoPageFormat * dfFormat,
		GuidoPageFormat * thFormat)
	: CDialog(CVirtualPageDialog::IDD, pParent)
{
	defsx = dfFormat->width;
	defsy = dfFormat->height;
	defml = dfFormat->marginleft;
	defmt = dfFormat->margintop;
	defmr = dfFormat->marginright;
	defmb = dfFormat->marginbottom;

	tpsx  = thFormat->width;
	tpsy  = thFormat->height;
	tpml  = thFormat->marginleft;
	tpmt  = thFormat->margintop;
	tpmr  = thFormat->marginright;
	tpmb  = thFormat->marginbottom;
	
	editable = true;
	onlydefault = false;

	startwithpage = 1; // start with def-page
}

// -----------------------------
//	bool def, 	float *sx, float *sy, float *ml, float *mt, float *mr, float *mb)

void CVirtualPageDialog::getMargins( bool def, GuidoPageFormat * outFormat )
{
	if (def)
	{
		outFormat->width = defsx;
		outFormat->height = defsy;
		outFormat->marginleft = defml;
		outFormat->margintop = defmt;
		outFormat->marginright = defmr;
		outFormat->marginbottom = defmb;
	}
	else
	{
		outFormat->width = tpsx;
		outFormat->height = tpsy;
		outFormat->marginleft = tpml;
		outFormat->margintop = tpmt;
		outFormat->marginright = tpmr;
		outFormat->marginbottom = tpmb;
	}
}

BOOL CVirtualPageDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!onlydefault)
	{
		CWnd *mywnd = GetDlgItem(IDC_THISPAGESIZE);
		mywnd->EnableWindow();
	}

	// The default-size is standard ....
	if (startwithpage == 1 || onlydefault)
	{
		SendDlgItemMessage(IDC_DEFPAGESIZE,BM_SETCHECK,1);

		m_height = defsy;
		m_width = defsx;
		m_mleft = defml;
		m_mtop = defmt;
		m_mright = defmr;
		m_mbottom = defmb;
		UpdateData(false);
		editable = 1;
	}
	else if (startwithpage == 2) // ?
	{
		SendDlgItemMessage(IDC_THISPAGESIZE,BM_SETCHECK,1);

		m_height = tpsy;
		m_width = tpsx;
		m_mleft = tpml;
		m_mtop = tpmt;
		m_mright = tpmr;
		m_mbottom = tpmb;
		UpdateData(false);

		if (m_ignorePF)
			editable = 1;
		else
			editable = 0;
	}

	setEditable(editable);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX- property pages should return FALSE
}

void CVirtualPageDialog::setOnlyDefault(bool value)
{
	onlydefault = value;
}

bool CVirtualPageDialog::getOnlyDefault()
{
	return onlydefault;
}

void CVirtualPageDialog::setEditable(bool value)
{
	editable = value;
	if (editable)
	{
		SendDlgItemMessage(IDC_HEIGHT,WM_ENABLE,(WPARAM) true);
		SendDlgItemMessage(IDC_WIDTH,WM_ENABLE,(WPARAM) true);
		SendDlgItemMessage(IDC_MRIGHT,WM_ENABLE,(WPARAM) true);
		SendDlgItemMessage(IDC_MLEFT,WM_ENABLE,(WPARAM) true);
		SendDlgItemMessage(IDC_MTOP,WM_ENABLE,(WPARAM) true);
		SendDlgItemMessage(IDC_MBOTTOM,WM_ENABLE,(WPARAM) true);

		GetDlgItem(IDC_ASPRINT)->EnableWindow(true);
	}
	else
	{
		SendDlgItemMessage(IDC_HEIGHT,WM_ENABLE,(WPARAM) false);
		SendDlgItemMessage(IDC_WIDTH,WM_ENABLE,(WPARAM) false);
		SendDlgItemMessage(IDC_MRIGHT,WM_ENABLE,(WPARAM) false);
		SendDlgItemMessage(IDC_MLEFT,WM_ENABLE,(WPARAM) false);
		SendDlgItemMessage(IDC_MTOP,WM_ENABLE,(WPARAM) false);
		SendDlgItemMessage(IDC_MBOTTOM,WM_ENABLE,(WPARAM) false);
		GetDlgItem(IDC_ASPRINT)->EnableWindow(false);
	}
}

bool CVirtualPageDialog::getEditable()
{
	return editable;
}

void CVirtualPageDialog::OnDefpagesize() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	// this is called, when DefPageSize is clicked ...
	if (!editable)
		setEditable(true);
	UpdateData();
	tpsy = m_height;
	tpsx = m_width;
	tpml = m_mleft;
	tpmt = m_mtop;
	tpmr = m_mright;
	tpmb = m_mbottom;
	m_height = defsy;
	m_width = defsx;
	m_mleft = defml;
	m_mtop = defmt;
	m_mright = defmr;
	m_mbottom = defmb;
	UpdateData(false);
}

void CVirtualPageDialog::OnThispagesize() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	int checked = (int) SendDlgItemMessage(IDC_IGNOREPF,BM_GETCHECK);
	if (checked)
	{
		setEditable(true);
	}
	else
	{
		setEditable(false);
	}

	UpdateData();
	defsy = m_height;
	defsx = m_width;
	defml = m_mleft;
	defmt = m_mtop;
	defmr = m_mright;
	defmb = m_mbottom;

	m_height = tpsy;
	m_width = tpsx;
	m_mleft = tpml;
	m_mtop = tpmt;
	m_mright = tpmr;
	m_mbottom = tpmb;
	UpdateData(false);
	
}

void CVirtualPageDialog::OnOK() 
{
	// TODO: Zusätzliche Prüfung hier einfügen
	UpdateData();
	if (IsDlgButtonChecked(IDC_DEFPAGESIZE))
	{
		defsy = m_height;
		defsx = m_width;
		defml = m_mleft;
		defmt = m_mtop;
		defmr = m_mright;
		defmb = m_mbottom;
	}
	else
	{
		tpsy = m_height;
		tpsx = m_width;
		tpml = m_mleft;
		tpmt = m_mtop;
		tpmr = m_mright;
		tpmb = m_mbottom;
	}

	
	CDialog::OnOK();
}

void CVirtualPageDialog::OnIgnorepf() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	// now, if the button is checked, and we
	// are on the thispagesize (not default-page-size)
	// then we can edit the values ....


	if (IsDlgButtonChecked(IDC_IGNOREPF))
		m_ignorePF = true;
	else
		m_ignorePF = false;
	if (IsDlgButtonChecked(IDC_THISPAGESIZE))
	{
		if (IsDlgButtonChecked(IDC_IGNOREPF))
		{
			if (!editable)
				setEditable(true);
		}	
		else
		{ // not checked!
			if (editable)
				setEditable(false);
		}
	}
}

void CVirtualPageDialog::OnAsprint() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen

	// determine the acutal paper-size of the default
	// printer and set the sizes so that they match ...

	PRINTDLG *pPrintDlg = new PRINTDLG;

/*	memset(&pdlg,0,sizeof(PRINTDLG));
	pdlg.lStructSice = sizeof(PRINTDLG);
	pdlg.Flags = PD_RETURNDEFAULT; */

	if (!AfxGetApp()->GetPrinterDeviceDefaults(pPrintDlg))
	{
		// not sucessful ...
		// no printer-devicedefaults ...
		
	}
	else
	{

		// Get pointers to the two setting structures.
		DEVNAMES FAR *lpDevNames =
			(DEVNAMES FAR *)::GlobalLock(pPrintDlg->hDevNames);
		DEVMODE FAR *lpDevMode = 
			(DEVMODE FAR *)::GlobalLock(pPrintDlg->hDevMode);

		// Get the specific driver information.
		CString szDriver((LPTSTR)lpDevNames +
			lpDevNames->wDriverOffset);
		CString szDevice((LPTSTR)lpDevNames +
			lpDevNames->wDeviceOffset);
		CString szOutput((LPTSTR)lpDevNames +
			lpDevNames->wOutputOffset);
      
		// Create a CDC object according to the current settings.
		CDC pDC;
		pDC.CreateDC(szDriver, szDevice, szOutput, lpDevMode);
    
		// Query this CDC object for the width and height of the current
		// page.

		m_width = (float) pDC.GetDeviceCaps(HORZSIZE) / 10.0f;
		m_height = (float) pDC.GetDeviceCaps(VERTSIZE) / 10.0f;
    
		// Get rid of the CDC object.            
		pDC.DeleteDC();
		// Unlock the pointers to the setting structures.
		::GlobalUnlock(pPrintDlg->hDevNames);
		::GlobalUnlock(pPrintDlg->hDevMode);          
	}

    delete pPrintDlg; 

	// margins are not affected ...

	UpdateData(false);

}
