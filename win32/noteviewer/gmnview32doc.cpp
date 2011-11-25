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

#include <shlwapi.h> // this is probably needed for shell-versioning stuff ...

#include "gmnview32View.h"
#include "gmnview32Doc.h"
#include "GUIDOEngine.h"

#include <io.h>
//#include <fstream.h>
#include <fstream>
using namespace std;

//#include "secureio.h"
#include <stdio.h>
#define snprintf _snprintf

#pragma warning (disable : 4996)	// for _CRT_SECURE_NO_DEPRECATE

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(NoteViewerDoc, CDocument)
BEGIN_MESSAGE_MAP(NoteViewerDoc, CDocument)
	//{{AFX_MSG_MAP(NoteViewerDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


NoteViewerDoc::NoteViewerDoc()
{
	tmpgmnbuf = 0;
}

NoteViewerDoc::~NoteViewerDoc()
{
	if (tmpgmnbuf)
		::free(tmpgmnbuf);

	// we should show a Sand-Clock here, it might take a while ....
	// hourglass
	HCURSOR old = ::SetCursor(::LoadCursor(NULL,IDC_WAIT));
	CGuidoDoc::FreeARGR();	
	::SetCursor( old );
}


HRESULT NoteViewerDoc::GetShell32Version(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	HINSTANCE   hShell32;
	
	if( IsBadWritePtr(pdwMajor, sizeof(DWORD)) || 
		IsBadWritePtr(pdwMinor, sizeof(DWORD)))
		return E_INVALIDARG;
	
	*pdwMajor = 0;
	*pdwMinor = 0;
	
	//Load the DLL.
	hShell32 = LoadLibrary(TEXT("shell32.dll"));
	
	if(hShell32)
	{
		HRESULT           hr = S_OK;
		DLLGETVERSIONPROC pDllGetVersion;
		
		/*
		You must get this function explicitly because earlier versions of the DLL 
		don't implement this function. That makes the lack of implementation of the 
		function a version marker in itself.
		*/
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hShell32, TEXT("DllGetVersion"));
		
		if(pDllGetVersion)
		{
			DLLVERSIONINFO    dvi;
			
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			
			hr = (*pDllGetVersion)(&dvi);
			
			if(SUCCEEDED(hr))
			{
				*pdwMajor = dvi.dwMajorVersion;
				*pdwMinor = dvi.dwMinorVersion;
			}
		}
		else
		{
		/*
		If GetProcAddress failed, the DLL is a version previous to the one 
		shipped with IE 3.x.
			*/
			*pdwMajor = 4;
			*pdwMinor = 0;
		}
		FreeLibrary(hShell32);	
		return hr;
	}
	return E_FAIL;
}

// this returns a path to a document directory. Depending on 
// the Shell32 Version it decides ...
HRESULT NoteViewerDoc::GetDocumentPath(char *path, int bufsize)
{
	HINSTANCE   hShell32;
	
	// Load the DLL.
	hShell32 = LoadLibrary(TEXT("shell32.dll"));
	
	if(hShell32)
	{
		HRESULT           hr = S_OK;
		typedef HRESULT (CALLBACK *SHPROC)(HWND,unsigned short *,int,int);
		/*
		You must get this function explicitly because earlier versions of the DLL 
		don't implement this function. That makes the lack of implementation of the 
		function a version marker in itself.
		*/
		SHPROC pSHGetSpecialFolderPath = (SHPROC)GetProcAddress(hShell32, TEXT("SHGetSpecialFolderPathA"));
		
		if(pSHGetSpecialFolderPath)
		{
			hr = (*pSHGetSpecialFolderPath)( (HWND) NULL, (unsigned short *) path,CSIDL_PERSONAL,FALSE);
			if (SUCCEEDED(hr))
			{
			}
			else strncpy(path, AfxGetApp()->m_pszHelpFilePath, bufsize);
		}
		else
		{
			// we need to set the path of another directory ...
			strncpy(path, AfxGetApp()->m_pszHelpFilePath, bufsize);
		}
		FreeLibrary(hShell32);		
		return hr;
	}
	return E_FAIL;
}

BOOL NoteViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// ZU ERLEDIGEN: Hier Code zur Reinitialisierung einfügen
	// (SDI-Dokumente verwenden dieses Dokument)
	m_GMNText = "% Insert your music here\r\n[  ]\r\n";

	// create a file that is untitled ...
	char filename[MAX_PATH];
	char tmppath[MAX_PATH];

	GetDocumentPath(tmppath, MAX_PATH);
	int i = 0;
	while (true)
	{
		if (i != 0)
			snprintf(filename,MAX_PATH,"%s\\untitled%d.gmn",tmppath,i);
		else 
			snprintf(filename,MAX_PATH,"%s\\untitled.gmn",tmppath);
		if (_access(filename,0) == -1)
			break;
		i++;
		if (i == 100)
			return FALSE;
	}
	this->SetTitle(filename);
	this->SetPathName(filename,FALSE);
	update(m_GMNText);
	SetModifiedFlag(FALSE);	// (JB) consider new, empty documents as not modified.
	return TRUE;
}

void NoteViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: insert code for save
	}
	else
	{
		// how do I get my filename?
		// TODO: insert code for load
	}
}

#ifdef _DEBUG
void NoteViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void NoteViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


BOOL NoteViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
		
	GuidoErrCode err = ParseFileARGR( lpszPathName );
	CFile myf(lpszPathName,CFile::typeBinary | CFile::modeRead);

	// this is the length of the file ...
	long length = (long)myf.GetLength();
	if (length>0)
	{
		// the size is more than 5 K it takes a while to read a file of this size ...
		// we get a temporary buffer to hold the information
		// and put it into the m_GMNText when needed 
		// (or in a background process ... yet to be determined)
		if (tmpgmnbuf) ::free(tmpgmnbuf);
		tmpgmnbuf = (char *) malloc(length+100);
//		int readlength = myf.ReadHuge(tmpgmnbuf,length);	
		int readlength = myf.Read(tmpgmnbuf,length);	
		tmpgmnbuf[readlength] = 0;
		m_GMNText = "";
		myf.Close();
	}
	else
	{
		myf.Close();
		CStdioFile myf(lpszPathName,CFile::typeText | CFile::modeRead);
		CString line;

		while (myf.ReadString(line))
		{
			m_GMNText += line;
			m_GMNText += "\r\n";
		}
		if (line.GetLength()>0)
		{
			m_GMNText += line;
			m_GMNText += "\r\n";
		}
		myf.Close();
	}
	SetModifiedFlag(FALSE);
	return TRUE;
}



void NoteViewerDoc::OnFileSaveAs()
{
	// Now do the file-save-dialog
	CString title = GetTitle();
	int i = title.Find(".gmn",0);
	title.Left(i);
	CFileDialog mfd(FALSE,"",title.Left(i),
		OFN_NOREADONLYRETURN | OFN_PATHMUSTEXIST
		| OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"MIDI File (*.mid)|*.mid|Encapsulated Postscript Files (*.eps)|*.eps|GUIDO MusicNotation Files (*.gmn)|*.gmn||");

	if (mfd.DoModal() == IDOK)
	{
		POSITION pos = GetFirstViewPosition();
		CGmnview32View *myview = (CGmnview32View *) 
		GetNextView(pos);

		if (mfd.GetFileExt() == "gmn")
		{
			myview->SaveAsGMN(mfd.GetFileName());
			// Do nothing
			SetTitle(mfd.GetFileName());
			SetPathName(mfd.GetFileName());
			SetModifiedFlag(FALSE);

		}
		else if (mfd.GetFileExt() == "eps")
		{
			myview->SaveAsEPS(mfd.GetFileName());
		}
		else if (mfd.GetFileExt() == "mid")
		{
			myview->SaveAsMID(mfd.GetFileName());
		}
	}
}

void NoteViewerDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsModified());
	
}

void NoteViewerDoc::update(CString & newgmn)
{
	// a new guido string has arrived ...
	m_GMNText = newgmn;
	GuidoErrCode err = ParseStringARGR( newgmn );
	SetModifiedFlag();
}

BOOL NoteViewerDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// we do not use the serialization function ...
	// the user has explicitly asked to save the file ...

	// this is for BORLAND / SALIERI - Debug purposes ...
	// GuidoExportMusic(mGuidoHandle,lpszPathName,0);
	if (tmpgmnbuf)
	{
		ofstream myof(lpszPathName,ios::binary);
		myof << tmpgmnbuf;
		myof.close();
	}
	else
	{
		ofstream myof(lpszPathName,ios::binary);
		const char *gmn = m_GMNText;
		myof << gmn;
		myof.close();
	}
	SetModifiedFlag(FALSE);
	return TRUE;
}

void NoteViewerDoc::SetModifiedFlag(BOOL value)
{
	CDocument::SetModifiedFlag(value);

	// now set the document names, so they reflect the new setting ...
	CString txt = GetTitle();
	if (value)
	{
		CString rtxt = txt.Right(2);
		if (txt.Right(2).Compare(" *"))
		{
			txt += " *";
			SetTitle(txt);
		}
	}
	else if (!txt.Right(2).Compare(" *"))
		SetTitle(txt.Left(txt.GetLength()-2));
}


BOOL NoteViewerDoc::OnSaveDocumentRelease(LPCTSTR lpszPathName, bool releasemodify)
{
	BOOL modified = IsModified();
	BOOL ret = OnSaveDocument(lpszPathName);
	if (releasemodify)
		SetModifiedFlag(FALSE);
	else
		SetModifiedFlag(modified);
	return ret;
}


/** this routine converts the tmpgmnbuf into the real text 
*/
void NoteViewerDoc::updateGMNText()
{
	m_GMNText = tmpgmnbuf;

	if (m_GMNText.Find("\r\n") == -1)
		m_GMNText.Replace("\n","\r\n");
	::free(tmpgmnbuf);
	tmpgmnbuf = NULL;
}
