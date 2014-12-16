// fontviewerDoc.cpp : implémentation de la classe CfontviewerDoc
//

#include "stdafx.h"
#include "fontviewer.h"

#include "fontviewerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfontviewerDoc

IMPLEMENT_DYNCREATE(CfontviewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CfontviewerDoc, CDocument)
END_MESSAGE_MAP()


// construction ou destruction de CfontviewerDoc

CfontviewerDoc::CfontviewerDoc()
{
	// TODO : ajoutez ici le code d'une construction unique

}

CfontviewerDoc::~CfontviewerDoc()
{
}

BOOL CfontviewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO : ajoutez ici le code de réinitialisation
	// (les documents SDI réutiliseront ce document)

	return TRUE;
}




// sérialisation de CfontviewerDoc

void CfontviewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO : ajoutez ici le code de stockage
	}
	else
	{
		// TODO : ajoutez ici le code de chargement
	}
}


// diagnostics pour CfontviewerDoc

#ifdef _DEBUG
void CfontviewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CfontviewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// commandes pour CfontviewerDoc
