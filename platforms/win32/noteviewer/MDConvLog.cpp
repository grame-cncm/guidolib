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
// MDConvLog.cpp: implementation file
//

#include "stdafx.h"
#include "gmnview32.h"
#include "MDConvLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogbox MDConvLog 


MDConvLog::MDConvLog(CWnd* pParent /*=NULL*/)
	: CDialog(MDConvLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MDConvLog)
	m_logtext = _T("");
	m_titletext = _T("");
	//}}AFX_DATA_INIT
}


void MDConvLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MDConvLog)
	DDX_Text(pDX, IDC_LOGTEXT, m_logtext);
	DDX_Text(pDX, IDC_TITLETEXT, m_titletext);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MDConvLog, CDialog)
	//{{AFX_MSG_MAP(MDConvLog)
		// REMARK: the class wizzard inserts here macros for messages
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// message handler routines MDConvLog 
