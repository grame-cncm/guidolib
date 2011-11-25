// fontviewerDoc.h : interface de la classe CfontviewerDoc
//


#pragma once


class CfontviewerDoc : public CDocument
{
protected: // création à partir de la sérialisation uniquement
	CfontviewerDoc();
	DECLARE_DYNCREATE(CfontviewerDoc)

// Attributs
public:

// Opérations
public:

// Substitutions
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implémentation
public:
	virtual ~CfontviewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions générées de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
};


