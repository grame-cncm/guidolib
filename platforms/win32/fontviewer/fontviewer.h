// fontviewer.h : fichier d'en-tête principal pour l'application fontviewer
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux


// CfontviewerApp:
// Consultez fontviewer.cpp pour l'implémentation de cette classe
//

class CfontviewerApp : public CWinApp
{
public:
	CfontviewerApp();


// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CfontviewerApp theApp;