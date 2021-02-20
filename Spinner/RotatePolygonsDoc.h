// RotatePolygonsDoc.h : interface of the CRotatePolygonsDoc class
//


#pragma once

class CRotatePolygonsDoc : public CDocument
{
protected: // create from serialization only
	CRotatePolygonsDoc();
	DECLARE_DYNCREATE(CRotatePolygonsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CRotatePolygonsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


