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
// gmnview32Doc.h : interface/declaration of class NoteViewerDoc
//
/////////////////////////////////////////////////////////////////////////////

#ifndef NoteViewerDoc_H
#define NoteViewerDoc_H

#include "GUIDOEngine.h"
#include "CGuidoDoc.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class NoteViewerDoc : public CDocument, public CGuidoDoc
{
protected: // obly by serealizing
	NoteViewerDoc();
	DECLARE_DYNCREATE(NoteViewerDoc)

// Attribute
public:
	char *tmpgmnbuf;

// Operationen
public:

// overloads
	// Overrides generiated by the class-wizzard
	//{{AFX_VIRTUAL(NoteViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// implementation
public:

	void updateGMNText();
	virtual void SetModifiedFlag(BOOL value = TRUE);
	virtual BOOL OnSaveDocumentRelease(LPCTSTR lpszPathName,
		bool releasemodify);
	void update(CString & newgmn);
	CString m_GMNText;
	virtual ~NoteViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HRESULT GetDocumentPath(char *path, int bufsize);
	HRESULT GetShell32Version(LPDWORD pdwMajor, LPDWORD pdwMinor);

// generated message map functions
protected:

		//{{AFX_MSG(NoteViewerDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}


#endif
