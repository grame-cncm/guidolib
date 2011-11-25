// fontviewerView.h : interface de la classe CfontviewerView
//


#pragma once


class CfontviewerView : public CView
{
protected: // création à partir de la sérialisation uniquement
	CfontviewerView();
	DECLARE_DYNCREATE(CfontviewerView)

// Attributs
public:
	HFONT mFont, mGuidoFont;
	CfontviewerDoc* GetDocument() const;

// Opérations
public:

// Substitutions
public:
	virtual void OnDraw(CDC* pDC);  // substitué pour dessiner cette vue
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implémentation
public:
	virtual ~CfontviewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions générées de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // version de débogage dans fontviewerView.cpp
inline CfontviewerDoc* CfontviewerView::GetDocument() const
   { return reinterpret_cast<CfontviewerDoc*>(m_pDocument); }
#endif

