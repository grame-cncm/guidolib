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
#include "gmnview32View.h"

#include "GMNTextDialog.h"


CGMNTextDialog::CGMNTextDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGMNTextDialog::IDD, pParent)
{
	mGuidoView = 0;

	//{{AFX_DATA_INIT(CGMNTextDialog)
	m_GMNText = _T("");
	//}}AFX_DATA_INIT
}


void CGMNTextDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGMNTextDialog)
	DDX_Text(pDX, IDC_GMNTEXT, m_GMNText);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CGMNTextDialog, CDialog)
	//{{AFX_MSG_MAP(CGMNTextDialog)
	ON_COMMAND(ID_GMN_APPLY, OnApplyGmnText)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines CGMNTextDialog 

void  
CGMNTextDialog::OnApplyGmnText()
{
	if( mGuidoView == 0 ) return;

	CString text ( "internal error" );

	const CEdit * editor = static_cast<CEdit *>(GetDlgItem( IDC_GMNTEXT ));
	if( editor )
	{
		editor->GetWindowText( text );
	}

	mGuidoView->ApplyGmnText( &text );
}
