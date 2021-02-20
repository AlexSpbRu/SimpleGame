#pragma once
//#include "resource.h"
#define IDC_STATIC_TITLE                1005
#define IDD_DLG_TITLE                   132
// CDlgTitle dialog

class CDlgTitle : public CDialog
{
	DECLARE_DYNAMIC(CDlgTitle)

public:
	CDlgTitle(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTitle();

// Dialog Data
	enum { IDD = IDD_DLG_TITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetTitle(CString tString);

};
