// RotatePolygonsDoc.cpp : implementation of the CRotatePolygonsDoc class
//

#include "stdafx.h"
#include "RotatePolygons.h"

#include "RotatePolygonsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRotatePolygonsDoc

IMPLEMENT_DYNCREATE(CRotatePolygonsDoc, CDocument)

BEGIN_MESSAGE_MAP(CRotatePolygonsDoc, CDocument)
END_MESSAGE_MAP()


// CRotatePolygonsDoc construction/destruction

CRotatePolygonsDoc::CRotatePolygonsDoc()
{
	// TODO: add one-time construction code here

}

CRotatePolygonsDoc::~CRotatePolygonsDoc()
{
}

BOOL CRotatePolygonsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRotatePolygonsDoc serialization

void CRotatePolygonsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CRotatePolygonsDoc diagnostics

#ifdef _DEBUG
void CRotatePolygonsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRotatePolygonsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRotatePolygonsDoc commands
