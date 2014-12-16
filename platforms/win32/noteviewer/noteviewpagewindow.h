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
// gmnview32View.h : interface/declaration of class CGmnview32View
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NoteViewPageWindow_H__F789540D_718F_11D2_9307_444553540000__INCLUDED_)
#define AFX_NoteViewPageWindow_H__F789540D_718F_11D2_9307_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CNoteViewPageWindow : public CWnd
{
public:
	CNoteViewPageWindow() {}
protected: // Nur aus Serialisierung erzeugen
//	CNoteViewPageWindow() {}

	DECLARE_DYNCREATE(CNoteViewPageWindow)

// Attribute
public:

	
// Operationen
public:

// overloads
	// Overrides generiated by the class-wizzard
	//{{AFX_VIRTUAL(CGmnview32View)
	public:
	virtual void OnDraw(CDC* pDC);  // overload for drawing of view
	protected:
//	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
//	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// implementation
public:
	virtual BOOL Create(CWnd *pParentWnd);
	virtual ~CNoteViewPageWindow();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// generated message map functions
protected:

	//{{AFX_MSG(CGmnview32View)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // !defined(AFX_NoteViewPageWindow_H__F789540D_718F_11D2_9307_444553540000__INCLUDED_)
