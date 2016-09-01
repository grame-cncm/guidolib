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

	This is the main application class.

*/

#define NOTEVIEWER_VERSION_NUMBER "0.9.2.2"	// last released version is 0.92

#include "stdafx.h"

// - Standard & system headers
#include <afxadv.h>
#include <io.h>
#include <fstream>

// - Noteviewer Application
#include "gmnview32.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "gmnview32Doc.h"
#include "gmnview32View.h"
#include "VirtualPageDialog.h"
#include "MyHTMLDoc.h"
#include "MyHTMLView.h"
#include "HelpDialog.h"
#include "SpringDialog.h"
#include "SpacingDialog.h"
#include "M2GConvDialog.h"
#include "MDConvLog.h"

// - Toolkit
#include "Midi2Guido.h"
#include "MuseData2Guido.h"
#include "GUIDOEngine.h"
#include "GuidoFeedbackWin32.h"

#include "VGDevice.h"

#ifdef UseGDIPlus
#include "GSystemWin32GDIPlus.h"
#else
#include "GSystemWin32.h"
#endif

//#include "secureio.h"
#include <stdio.h>
#define snprintf _snprintf

#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE

// ----------------------------------------------------------------------------

//GDeviceWin32 * NoteViewerCreateGDevice( HDC hdc, int inWidth, int inHeight )
/*GDeviceWin32 * NoteViewerCreateGDevice( HDC hdc, int inWidth, int inHeight )
{
//	CWinVersion sys;
//	if( sys.WinNTOrGreater())
//		return new GDeviceWin2000( hdc, inWidth, inHeight );
//	else
//		return new GDeviceWin32( hdc, inWidth, inHeight );			
		return new GDeviceWin32( hdc, inWidth, inHeight );			
}*/

// ----------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CGmnview32App, CWinApp)
	//{{AFX_MSG_MAP(CGmnview32App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_VIRTUALPAGE, OnFileVirtualpage)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_FILE_SPRINGPARAMETERS, OnFileSpringparameters)
	ON_COMMAND(ID_VOICECREATESRODS, OnVoicecreatesrods)
	ON_UPDATE_COMMAND_UI(ID_VOICECREATESRODS, OnUpdateVoicecreatesrods)
	ON_COMMAND(ID_SPRINGANDROD, OnSpringandrod)
	ON_UPDATE_COMMAND_UI(ID_SPRINGANDROD, OnUpdateSpringandrod)
	ON_COMMAND(ID_STAFFCREATESRODS, OnStaffcreatesrods)
	ON_UPDATE_COMMAND_UI(ID_STAFFCREATESRODS, OnUpdateStaffcreatesrods)
	ON_COMMAND(ID_SORTELEMENTS, OnSortElements)
	ON_UPDATE_COMMAND_UI(ID_SORTELEMENTS, OnUpdateSortElements)
	ON_COMMAND(ID_FILE_OPENEXAMPLES, OnFileOpenexamples)
	ON_COMMAND(ID_FILE_ADJUSTSPACINGPARAMETERS, OnFileAdjustspacingparameters)
	ON_COMMAND(ID_FILE_IMPORTMIDIFILE, OnFileImportmidifile)
	ON_COMMAND(ID_FILE_IMPORTMUSEDATAFLE, OnFileImportmusedatafle)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_VIEW_DISPLAY_FORCE, OnViewDisplayForce)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DISPLAY_FORCE, OnUpdateViewDisplayForce)
	//}}AFX_MSG_MAP
	// Dateibasierte Standard-Dokumentbefehle
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard-Druckbefehl "Seite einrichten"
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


GuidoFeedbackWin32	CGmnview32App::sFeedback;

// ----------------------------------------------------------------------------
// The only  CGmnview32App-Object

CGmnview32App theApp;

// ----------------------------------------------------------------------------
CGmnview32App::CGmnview32App()
{
//	mDefaultDevice = NoteViewerCreateGDevice( 0, 100, 100 );
//	mDefaultDevice = new GDeviceWin32( 100,100 );
}

// ----------------------------------------------------------------------------
CGmnview32App::~CGmnview32App()
{
	ReleaseRequiredFonts();
	::ReleaseDC( 0, mHDC );
	delete mDefaultDevice;
	delete mDefaultSys;
#ifdef UseGDIPlus
	GSystemWin32GDIPlus::ShutDown();
#endif

/*	if( gFeedback )
	{
		delete gFeedback;
		gFeedback = 0;
	}*/
}

// ----------------------------------------------------------------------------
CString CGmnview32App::GetAppPath() 
{
	char tmp[MAX_PATH];
	::GetModuleFileName(NULL,tmp,MAX_PATH);

	CString pathName(tmp);
	return pathName.Left(pathName.ReverseFind('\\'));
}

// ----------------------------------------------------------------------------
/* the musical font file must be placed in the same folder as 
	the application executable and dll's
*/
bool
CGmnview32App::InstallRequiredFonts()
{
	CString fontPath = GetAppPath();
	fontPath += "\\guido2.ttf";

	int result = ::AddFontResource( fontPath );
	return (result > 0 );
}

// ----------------------------------------------------------------------------
void
CGmnview32App::ReleaseRequiredFonts()
{
	CString fontPath = GetAppPath();
	fontPath += "\\guido2.ttf";
	::RemoveFontResource( fontPath );
}

// ----------------------------------------------------------------------------
BOOL CGmnview32App::InitInstance()
{
	AfxEnableControlContainer();
	// update of registry entry give key a specific string
	// SetRegistryKey(_T("sic!systems"));

	LoadStdProfileSettings();  // load std ini file options (incl. MRU)

	// register document templates
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate( IDR_GMNVWTYPE,
		RUNTIME_CLASS(NoteViewerDoc),
		RUNTIME_CLASS(CChildFrame), // user specific MDI-Child-frame
		RUNTIME_CLASS(CGmnview32View));
	AddDocTemplate(pDocTemplate);

	// create main-MDI-framewindow
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	m_pMainWnd->DragAcceptFiles();	// activate open by drag&drop

	// activate DDE-Execute-Open 
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// parse commandline, and check for environment and DDE commands
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// processing of commandline
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	InstallRequiredFonts();

	// main window is initialist and can be displayed and updated
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	sFeedback.Setup( m_hInstance, IDD_PROGRESS_DIALOG, IDC_STATUS );
	// create Guido Graphical Device objects
	mHDC = ::GetDC(	m_pMainWnd->m_hWnd );

#ifdef UseGDIPlus
	GSystemWin32GDIPlus::Start();
	mDefaultSys = new GSystemWin32GDIPlus(mHDC, NULL);;			//YC
#else
	mDefaultSys = new GSystemWin32(mHDC, NULL);			//YC
#endif

	mDefaultDevice = mDefaultSys->CreateDisplayDevice();
	GuidoInitDesc initDesc;
//	initDesc.feedback = &sFeedback;
	initDesc.graphicDevice = mDefaultDevice;
	initDesc.musicFont = 0;
	initDesc.textFont = 0;

	GuidoErrCode err = GuidoInit( &initDesc );
	if( err != guidoNoErr ) {
		const char * errStr = GuidoGetErrorString( err );
		MessageBox( NULL, errStr, "GuidoLib Error", MB_OK );		
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg-Dialogbox 

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialogboxdata
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_versionstring;
	CString	m_nviewversion;
	//}}AFX_DATA

	// generated by class-wizzard 
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-support
	//}}AFX_VIRTUAL

// implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// no message-Handler
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_versionstring = _T("");
	m_nviewversion = _T("");
	//}}AFX_DATA_INIT

	char buffer[200];
	int major, minor, sub;
	GuidoGetVersionNums( &major, &minor, &sub );
	snprintf( buffer, 200, "Guido Engine v%d.%d.%d", major, minor, sub );

	m_versionstring = buffer;

	this->m_nviewversion = NOTEVIEWER_VERSION_NUMBER " (" __DATE__  ")";
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSIONSTRING, m_versionstring);
	DDX_Text(pDX, IDC_NVIEWVERSION, m_nviewversion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// no message-Handler
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// app command for opening of dialogbox
void CGmnview32App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
/** \brief Open the dialog for the default page format

*/
void CGmnview32App::OnFileVirtualpage() 
{
	// TODO resolve cm / internal unit issues in dialog

	GuidoPageFormat pf;
	GuidoPageFormat dummy = { 0, 0, 0, 0, 0, 0 };
	GuidoGetDefaultPageFormat( &pf );

	// changeable...
	CVirtualPageDialog dialog (NULL, &pf, &dummy );
		
	dialog.m_ignorePF = 0; // was = vpignorepf; unused ?
	dialog.setOnlyDefault(true);

	if (dialog.DoModal() == IDOK)
	{
	/* old Guido api version:
		//	vpignorepf = vpdg.m_ignorePF;
			char * svpsx = new char[20];
			sprintf(svpsx,"%.3fcm",vpsx);
			
			char * svpsy = new char[20];
			sprintf(svpsy,"%.3fcm",vpsy);
			
			char * svpml = new char[20];
			sprintf(svpml,"%.3fcm",vpml);
			
			char * svpmb = new char[20];
			sprintf(svpmb,"%.3fcm",vpmb);
			
			char * svpmt = new char[20];
			sprintf(svpmt,"%.3fcm",vpmt);
			
			char * svpmr = new char[20];
			sprintf(svpmr,"%.3fcm",vpmr);
			GuidoSetDefaultPageSize(svpsx,svpsy,svpml,svpmt,svpmr,svpmb);
			delete [] svpsx;
			delete [] svpsy;
			delete [] svpml;
			delete [] svpmt;
			delete [] svpmr;
			delete [] svpmb;
		*/

		// - Get the dialog results and try to apply to Guido Engine
		
		dialog.getMargins( true, &pf );
		GuidoSetDefaultPageFormat( &pf );
		
		// - Check if the new settings were accepted by the Guido Engine
		
		GuidoPageFormat testFormat;
		GuidoGetDefaultPageFormat( &testFormat );
		
		if( CGuidoDoc::FormatsAreEqual( pf, testFormat ) == false )
		{
			MessageBox(NULL,"The Values that you have entered are not correct."
								"They will be adjusted automatically!",
							"WARNING",MB_OK);
		}
	}
}

void CGmnview32App::OnFilePrintSetup() 
{
	// TODO: insert code
	CPrintDialog pd(TRUE);
	int res = DoPrintDialog(&pd);
	if (res == IDOK)
	{
		// now ask, wether we want to adjust page
		// sizes ...
		// maybe not now ... in the next version ..
	}
}

void CGmnview32App::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(false);	
}

void CGmnview32App::DisplayHTMLWindow()
{

}

void CGmnview32App::OnHelp() 
{
	// TODO: insert Code 
	CHelpDialog helpdlg;
	helpdlg.DoModal();
}

void CGmnview32App::OnFileSpringparameters() 
{
	// TODO: insert Code 

	CSpringDialog dialog( &mGuidoSettings, 0 );
	if (dialog.DoModal() == IDOK)
	{
		// hourglass
		HCURSOR old = ::SetCursor(::LoadCursor(NULL,IDC_WAIT));

	//	GuidoSetSpringParameter( dialog.springpar );
		mGuidoSettings.spring = dialog.springpar;
		
		// - Update GR and redraw	
		ApplyGuidoSettings();
		UpdateAllGuidoDocViews();

		::SetCursor(old);
	}
}

void CGmnview32App::ApplyGuidoSettings()
{
	// TODO: for each document
	
	GRHandler gr = 0;	// TODO

	GuidoUpdateGR( gr, &mGuidoSettings );
}

void CGmnview32App::OnVoicecreatesrods() 
{
/*	TODO rods not yet implemented in Guido ?
	float f;
	GuidoGetSetting( kSettingStaffVoiceRods, &f);
	if (f != (float) 2)
	{
		GuidoSetSetting( kSettingStaffVoiceRods, (float) 2);	
		// recalculate ...
	}*/
}

void CGmnview32App::OnUpdateVoicecreatesrods(CCmdUI* pCmdUI) 
{
/* TODO	rods not yet implemented in Guido ?
	float f;
	GuidoGetSetting( kSettingStaffVoiceRods, &f);
	if ( (int) f == 2)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
		
	// - Code for GUI update	
	GuidoUpdateAllGRMusics();	// (JB) added
	UpdateAllGuidoDocViews();	// (JB) added	
*/
	pCmdUI->SetCheck(0);
}

void CGmnview32App::OnSpringandrod() 
{
/* TODO	
	float f;
	GuidoGetSetting(kSettingDisplaySprings, &f);
	
	if (f == (float) 1)
	{
		// determine if checked ? how to?
		GuidoSetSetting( kSettingDisplaySprings, (float) 0);	
	}
	else
	{
		GuidoSetSetting( kSettingDisplaySprings,(float) 1);
	}

	// - Code for GUI update	
	// GuidoUpdateAllGRMusics();	// (JB) required ?
	UpdateAllGuidoDocViews();	// (JB) added
*/
}

void CGmnview32App::OnUpdateSpringandrod(CCmdUI * pCmdUI) 
{
/* TODO 
	float f;
	GuidoGetSetting( kSettingDisplaySprings, &f);
	if ( (int) f == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
	
	// - Code for GUI update	
	// GuidoUpdateAllGRMusics();	// (JB) required ?
	//	UpdateAllGuidoDocViews();	// (JB) added
	*/
	pCmdUI->SetCheck(0);
}

void CGmnview32App::OnStaffcreatesrods() 
{
/* TODO 
	float f;
	GuidoGetSetting( kSettingStaffVoiceRods, &f);
	if (f != (float)1)
	{
		GuidoSetSetting( kSettingStaffVoiceRods, (float)1 );
		// TODO: a reparse and redisplay must be done 
	}
*/
}

void CGmnview32App::OnUpdateStaffcreatesrods(CCmdUI * pCmdUI) 
{
/*
	float f;
	GuidoGetSetting( kSettingStaffVoiceRods, &f);
	if ((int)f == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);

	// - Code for GUI update	
//	GuidoUpdateAllGRMusics();	// (JB) added
//	UpdateAllGuidoDocViews();	// (JB) added	
*/
	pCmdUI->SetCheck(0);
}

void CGmnview32App::OnSortElements() 
{
/* obsolete: always sorted by the engine
	float f;
	GuidoGetSetting( kSettingSortElements, &f);
	if (f == (float) 1)
	{
		GuidoSetSetting( kSettingSortElements, (float)0);
		// TODO: a reparse and redisplay must be done 
	}
	else
		GuidoSetSetting( kSettingSortElements, (float)1 );
*/
}

void CGmnview32App::OnUpdateSortElements(CCmdUI * pCmdUI) 
{
/* obsolete: always sorted by the engine
	float f;
	GuidoGetSetting(kSettingSortElements, &f );
	if ( (int) f == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
*/
	pCmdUI->SetCheck(0);
}

void CGmnview32App::OnFileOpenexamples() 
{
	// This opens a fileopen dialog box with the examples-directory
	// already open ...

	CString pathName = GetAppPath();

	pathName += "\\examples\\";

	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		"GUIDO Music Notation (*.gmn)|*.gmn||",this->GetMainWnd());

	fileDialog.m_ofn.lpstrTitle = "Open Example File";
	fileDialog.m_ofn.lpstrInitialDir = pathName;

	if (fileDialog.DoModal() == IDOK)
	{
		OpenDocumentFile(fileDialog.GetFileName());
	}
}

void CGmnview32App::OnFileAdjustspacingparameters() 
{
	// float curoptforce = mGuidoSettings.force; //GuidoGetOptForce();
	
	// left side equals a force of 400
	// right side equals a force of 1600
	
	// int oldpagefill = mGuidoSettings.optimalPageFill; // GuidoGetOptPageFill();
	// int oldneighbor = mGuidoSettings.neighborhoodSpacing; // GuidoGetNeighborhoodSpacing();
	// float defsysdis = mGuidoSettings.systemsDistance; //	GuidoGetSetting( kSettingSystemDistance, &defsysdis);

	CSpacingDialog dialog( &mGuidoSettings, 0 );
	if( dialog.DoModal() == IDOK )
	{
		mGuidoSettings.neighborhoodSpacing = dialog.m_specialspacing;
		mGuidoSettings.optimalPageFill = dialog.m_optpagefill;

		if (dialog.m_def_sys_dis_changed)
			mGuidoSettings.systemsDistance = dialog.m_def_sys_dis / 100.0f;

		mGuidoSettings.systemsDistribution = (dialog.m_dis_auto + 1);
		mGuidoSettings.force = (float)(dialog.m_optforceint);
		
		// - Update GR and redraw	
		ApplyGuidoSettings();
		UpdateAllGuidoDocViews();
	}
}

  
  BOOL CALLBACK chldenumproc(HWND hwnd,LPARAM lparm)
  {
	  // inside ...
	  char wndtxt[256];
	  ::GetWindowText(hwnd, wndtxt, 254);
	  return TRUE;
  }

			  
void CGmnview32App::OnFileImportmidifile() 
{
	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,
		"MIDI file(*.mid)|*.mid||",this->GetMainWnd());
	
	fileDialog.m_ofn.lpstrTitle = "Import MIDI File";
	// 	fileDialog.m_ofn.lpstrInitialDir = pathName;
	
	if (fileDialog.DoModal() == IDOK)
	{
		// now we have a MIDI file ....
		
		HCURSOR old = ::SetCursor(::LoadCursor(NULL,IDC_WAIT));		
		
		CString pathName = GetAppPath();
		
		CString midifilename(fileDialog.GetPathName());
		CString midifilepath = midifilename.Left(midifilename.ReverseFind('\\'));
		CString midifiletitle = fileDialog.GetFileTitle();
		
		CString iniName;
//		iniName = pathName + "\\fermata.INI";
		iniName = midifilename.Left(midifilename.ReverseFind('.')) + ".ini";
		
//		FILE * fd = fopen (iniName, "r");
		FILE * fd;
		int err = 0;
#ifdef VC2005
		err = fopen_s (&fd, iniName, "r");
		if (!err) fclose (fd);
#else 
		fd = fopen (iniName, "r");
		if (fd) fclose (fd);
#endif
		else iniName = pathName + "\\fermata.INI";

		// create a temporary name for the GMN-File ....
		char tmpname[MAX_PATH];
		CString tmpgmnname;
		
		// create a temporary name until there is really no file
		// with that name ...
		
		char tmppath[MAX_PATH];
		::GetTempPath(MAX_PATH,tmppath);
		static int i = 3245;
		do
		{
			::GetTempFileName(tmppath,"m2g_",i++,tmpname);
			tmpgmnname = CString(tmpname) +  ".gmn";
		}
		while (_access(tmpgmnname,04)==0);
			
		BOOL resbool;
		
		int logfilemoved = 0;
		CString movedlogfilename(tmpname);
		movedlogfilename += ".log";
		
		CString logfilename(midifilepath);
			
		logfilename += "\\" + midifiletitle + ".LOG";
		
		// if there already is a file with the logfilename
		// we move it to a temporary place  ....
		if (!_access(logfilename,0))
		{
			// there is a file 
			
			resbool = ::MoveFile(logfilename,movedlogfilename);
			if (resbool)
				logfilemoved = 1;
			
		}
		
		int result = midi2gmn(iniName, fileDialog.GetPathName(), tmpgmnname);
		if (!result)
		{
			// the conversion was successfull
			
			// this list saves the currently open MDI window names 
			CStringList mylist;
			
			// before we open the (temporary) gmnfile
			// we iterate through the already open MDI-Files
			// to save the names in a StringList ...
			// Iteration works as follows: the GetWindow function can
			// be used to retrieve the child-windows of the MDI-Client
			// window, which is the first (and only) child of the FrameWindow
			POSITION pos = GetFirstDocTemplatePosition();
			while(pos)
			{
				CDocTemplate * pTemplate = GetNextDocTemplate(pos); 
				POSITION pos2 = pTemplate->GetFirstDocPosition();
				while(pos2)
				{ 
					CDocument * pDocument;
					if(pDocument = pTemplate->GetNextDoc(pos2))
					{
						NoteViewerDoc * mydoc = dynamic_cast<NoteViewerDoc *>(pDocument);
						
						if (mydoc)
						{
							CString mystr = mydoc->GetTitle();
							mylist.AddTail(mystr);
						}
					}
				}
			}
			
			// the temporary gmn-file is opened as a new document
			NoteViewerDoc * mydoc = dynamic_cast
				<NoteViewerDoc *>(OpenDocumentFile(tmpgmnname));
			
			// this removes the temporary file from the MRU-List.
			// which is good, because it is only a temporary file
			m_pRecentFileList->Remove(0);
						
			// now we try to determine an unused filename ...			
			char chosenname[MAX_PATH];
			
			int i = 0;
			while (true)
			{				
				// first, we check if the name is already used in
				// the open window file list ....
				char mytmpchar1[MAX_PATH];
				char mytmpchar2[MAX_PATH];
				if (i>0)
				{
					// one variation with star, the other without star
					snprintf(mytmpchar1,MAX_PATH,"%s%d.gmn *",midifiletitle,i);
					snprintf(mytmpchar2,MAX_PATH,"%s%d.gmn",midifiletitle,i);
				}
				else
				{
					snprintf(mytmpchar1,MAX_PATH,"%s.gmn *",midifiletitle);
					snprintf(mytmpchar2,MAX_PATH,"%s.gmn",midifiletitle);
				}
				
				if (mylist.Find(mytmpchar1) || mylist.Find(mytmpchar2))
				{
					++ i;
					continue;
				}
				
				if (i != 0)
					snprintf(chosenname,MAX_PATH,"%s\\%s%d.gmn",midifilepath,
					midifiletitle,i);
				else 
					snprintf(chosenname,MAX_PATH,"%s\\%s.gmn",midifilepath,
					midifiletitle);
				
				
				if (_access(chosenname,04) == -1)
					break;
				
				++i;
				
				if (i == 100)
				{
					snprintf(chosenname,MAX_PATH,"%s\\fresh_converted.gmn",pathName);
					break;
				}
			}
			
			mydoc->SetTitle(chosenname);
			mydoc->SetPathName(chosenname,FALSE);
			mydoc->SetModifiedFlag(TRUE);
			
			resbool = ::DeleteFile(tmpgmnname);
					
			// we display the logfile ....
			CM2GConvDialog cd;
			
			int linecountmax = 40;
			int dlgend = 1;
			int fulllog = 0;
			do
			{
				cd.m_isfulllog = fulllog ? 1 : 0;
				cd.m_logtext = "";
				dlgend = 1;
				// read the logtext from the logfile ....
				CString line;
				CStdioFile mylfile(logfilename, CFile::modeRead);
				
				// the first 40 lines of the file ....
				int linecount = 0;
				while (mylfile.ReadString(line) && linecount < linecountmax)
				{
					++linecount;
					cd.m_logtext += line;
					cd.m_logtext += "\r\n";
				}
				
				mylfile.Close();
				
				if (cd.DoModal() == IDC_FULLLOG)
				{
					
					fulllog = 1;
					dlgend = 0;
					linecountmax = 20000;
				}
				
			}
			while (!dlgend);			
		}
		
		// we also remove the log-file ...
		resbool = ::DeleteFile(logfilename);
		
		if (logfilemoved)
		{
			resbool = ::MoveFile(movedlogfilename,logfilename);
		}
		
		::SetCursor(old);
		
	} // if (fileDialog.OK);	
}

void CGmnview32App::OnFileImportmusedatafle() 
{
	// TODO: insert code for command handling
	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST,
		"MuseData file(*.md)|*.md||",this->GetMainWnd());
	
	fileDialog.m_ofn.lpstrTitle = "Import MuseData File";
	// 	fileDialog.m_ofn.lpstrInitialDir = pathName;
	
	if (fileDialog.DoModal() == IDOK)
	{
		// now we have a MIDI file ....
		
		// hourglass
		HCURSOR old = ::SetCursor(::LoadCursor(NULL,IDC_WAIT));
		
		CString pathName = GetAppPath();
		
		CString mdfilename(fileDialog.GetPathName());
		CString mdfilepath = mdfilename.Left( mdfilename.ReverseFind('\\'));
		
		CString mdfiletitle;
		mdfiletitle = fileDialog.GetFileTitle();
		
		// create a temporary name for the GMN-File ....
		char tmpname[MAX_PATH];
		CString tmpgmnname;
		// create a temporary name until there is really no file
		// with that name ...
		
		char tmppath[MAX_PATH];
		::GetTempPath(MAX_PATH,tmppath);
		
		int i = 3245;
		do
		{
			::GetTempFileName(tmppath,"m2g_",i++,tmpname);
			tmpgmnname = CString(tmpname) +  ".gmn";
		}
		while (_access(tmpgmnname,04)==0);
		
		BOOL resbool;

		CString logfilename;
		int j = 7864;
		do
		{
			::GetTempFileName(tmppath,"m2g_",j++,tmpname);
			logfilename = CString(tmpname) +  ".log";
		}
		while (_access(logfilename,04)==0);
		
		// we have to create the parameters for 
		// the call to md2gmn(int argc,char **argv)

		int argc = 3;
		char * argv[4];

		argv[0] = (char *) malloc(32);
		strncpy(argv[0],"md2gmn",32);
		argv[1] = (char *) malloc(MAX_PATH+10);
		snprintf(argv[1],MAX_PATH+10,"o%s",tmpgmnname);
		argv[2] = (char *) malloc(MAX_PATH);
		strncpy(argv[2],fileDialog.GetPathName(),MAX_PATH);
		argv[3] = NULL;

		// how can i redirect stdio?

		/* Reassign "stderr" to "freopen.out": */
		
		int result = md2gmn(argc,argv,logfilename);

		free(argv[2]);
		free(argv[1]);
		free(argv[0]);
		
		if (!result)
		{
			// the conversion was successfull

			// this list saves the currently open MDI window names 
			CStringList mylist;
			
			// before we open the (temporary) gmnfile
			// we iterate through the already open MDI-Files
			// to save the names in a StringList ...
			// Iteration works as follows: the GetWindow function can
			// be used to retrieve the child-windows of the MDI-Client
			// window, which is the first (and only) child of the FrameWindow
			POSITION pos = GetFirstDocTemplatePosition();
			while(pos)
			{
				CDocTemplate * pTemplate = GetNextDocTemplate(pos); 
				POSITION pos2 = pTemplate->GetFirstDocPosition();
				while(pos2)
				{
					CDocument * pDocument;
					if(pDocument = pTemplate->GetNextDoc(pos2))
					{
						NoteViewerDoc * mydoc = dynamic_cast<NoteViewerDoc *>(pDocument);
						if (mydoc)
						{
							CString mystr = mydoc->GetTitle();
							mylist.AddTail(mystr);
						}
					}
				}
			}
			
			// the temporary gmn-file is opened as a new document
			NoteViewerDoc * mydoc = dynamic_cast <NoteViewerDoc *>(OpenDocumentFile(tmpgmnname));
			
			// this removes the temporary file from the MRU-List.
			// which is good, because it is only a temporary file
			m_pRecentFileList->Remove(0);
			
			// now we try to determine an unused filename ...			
			char chosenname[MAX_PATH];
			
			int i = 0;
			while (true)
			{
				// first, we check if the name is already used in
				// the open window file list ....
				char mytmpchar1[MAX_PATH];
				char mytmpchar2[MAX_PATH];
				if (i>0)
				{
					// one variation with star, the other without star
					snprintf(mytmpchar1,MAX_PATH,"%s%d.gmn *",mdfiletitle,i);
					snprintf(mytmpchar2,MAX_PATH,"%s%d.gmn",mdfiletitle,i);
				}
				else
				{
					snprintf(mytmpchar1,MAX_PATH,"%s.gmn *",mdfiletitle);
					snprintf(mytmpchar2,MAX_PATH,"%s.gmn",mdfiletitle);
				}
				
				if (mylist.Find(mytmpchar1) || mylist.Find(mytmpchar2))
				{
					++i;
					continue;
				}
				
				if (i != 0)
					snprintf(chosenname,MAX_PATH,"%s\\%s%d.gmn",mdfilepath,
					mdfiletitle,i);
				else 
					snprintf(chosenname,MAX_PATH,"%s\\%s.gmn",mdfilepath,
					mdfiletitle);
				
				
				if (_access(chosenname,04) == -1)
					break;

				++i;

				if (i == 100)
				{
					snprintf(chosenname,MAX_PATH,"%s\\fresh_converted.gmn",pathName);
					break;
				}
			}
			
			mydoc->SetTitle(chosenname);
			mydoc->SetPathName(chosenname,FALSE);
			mydoc->SetModifiedFlag(TRUE);
			
			resbool = ::DeleteFile(tmpgmnname);
		}
		else
		{
			// error duraing conversion, show logfile

			MDConvLog cl;
			
			cl.m_titletext = "Conversion of ";
			cl.m_titletext += fileDialog.GetPathName();
			cl.m_titletext += " failed.";

			CStdioFile myfile(logfilename,CFile::modeRead | 
				CFile::typeBinary);

				long length = long(myfile.GetLength());
				char * buf = (char *) malloc(length+1);
				
//				long numread = myfile.ReadHuge(buf,length);
				long numread = myfile.Read(buf,length);
				buf[numread] = 0;
				myfile.Close();
			cl.m_logtext = buf;
			free(buf);

			cl.DoModal();

		}
		
		// have to do this!
		// we also remove the log-file ...
		resbool = ::DeleteFile(logfilename);
		
		::SetCursor(old);
		
	} // if (fileDialog.OK);
}

void CGmnview32App::OnViewDisplayForce() 
{
/*
	float f;
	GuidoGetSetting(kSettingDisplayForce, &f);
	
	if (f == (float) 1)
	{
		// determine if checked ? how to?
		GuidoSetSetting( kSettingDisplayForce, (float)0);	
	}
	else
	{
		GuidoSetSetting( kSettingDisplayForce, (float)1);
	}

	// - Code for GUI update	
//	GuidoUpdateAllGRMusics();	// (JB) added

	// - Update GR and redraw	
	ApplyGuidoSettings(); ?
	UpdateAllGuidoDocViews();
*/
}

void CGmnview32App::OnUpdateViewDisplayForce(CCmdUI * pCmdUI) 
{
	/*
	float f;
	GuidoGetSetting( kSettingDisplayForce, &f);
	if ( (int) f == 1)
		pCmdUI->SetCheck(1);
	else
		pCmdUI->SetCheck(0);
	*/
	pCmdUI->SetCheck(0);
}

void CGmnview32App::UpdateAllGuidoDocViews() 
{
	// We need to go through ALL document windows.
	POSITION pos = GetFirstDocTemplatePosition();
	while(pos)
	{
		CDocTemplate* pTemplate = GetNextDocTemplate(pos); 
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while(pos2)
		{
			CDocument * pDocument;
			if(pDocument = pTemplate->GetNextDoc(pos2))
			{
				NoteViewerDoc * mydoc = dynamic_cast<NoteViewerDoc *>(pDocument);
				if (mydoc)
				{
					//mydoc->checkMusicHandle();
					POSITION viewpos = mydoc->GetFirstViewPosition();
					while (viewpos)
					{
						CGmnview32View * myview = dynamic_cast<CGmnview32View *>(
											mydoc->GetNextView(viewpos));
						if (myview)
						{
							myview->updatePageButton( mydoc->GetGR());
							myview->InvalidateRect(NULL);
						}
					}
				}
			}
		}
	}
}

