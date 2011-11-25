#ifndef CGmnview32App_H
#define CGmnview32App_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2004	Grame

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

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// #include "resource.h"       // Hauptsymbole
#include "GuidoFeedbackWin32.h"
#include "CGuidoDoc.h"

class VGDevice;
class VGSystem;

#define UseGDIPlus

// --------------------------------------------------------------
class CGmnview32App : public CWinApp
{
public:
						 CGmnview32App();
	virtual				~CGmnview32App();

	static	CString		GetAppPath();
	static	GuidoFeedbackWin32	sFeedback;

// overloads
	// Overrides generiated by the class-wizzard
	//{{AFX_VIRTUAL(CGmnview32App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// implementation

	//{{AFX_MSG(CGmnview32App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileVirtualpage();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void DisplayHTMLWindow();
	afx_msg void OnHelp();
	afx_msg void OnFileSpringparameters();
	afx_msg void OnVoicecreatesrods();
	afx_msg void OnUpdateVoicecreatesrods(CCmdUI* pCmdUI);
	afx_msg void OnSpringandrod();
	afx_msg void OnUpdateSpringandrod(CCmdUI* pCmdUI);
	afx_msg void OnStaffcreatesrods();
	afx_msg void OnUpdateStaffcreatesrods(CCmdUI* pCmdUI);
	afx_msg void OnSortElements();
	afx_msg void OnUpdateSortElements(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenexamples();
	afx_msg void OnFileAdjustspacingparameters();
	afx_msg void OnFileImportmidifile();
	afx_msg void OnFileImportmusedatafle();
	afx_msg void OnViewDisplayForce();
	afx_msg void OnUpdateViewDisplayForce(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// The parameters for the virtual sizes ...

	void	ApplyGuidoSettings();
	void	UpdateAllGuidoDocViews();

protected:
	bool 	InstallRequiredFonts();
	void	ReleaseRequiredFonts();

	GuidoLayoutSettings mGuidoSettings;

	HDC 				mHDC;
	VGDevice *			mDefaultDevice;
	VGSystem *			mDefaultSys;
};


#endif 
