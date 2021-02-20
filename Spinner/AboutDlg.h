#include "resource.h"

/**
#include <afxwin.h>

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void  UpdateHelp(void);

	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnDblclkListHelp();
	afx_msg void OnLbnSelchangeListHelp();
};
#pragma once
/**/

// CAboutDlg dialog

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };
    CWnd * m_pWnd;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void  UpdateHelp(void);
	afx_msg void OnLbnDblclkListHelp();
	afx_msg void OnBnClickedOk();
	afx_msg void OnFileOpen();
	afx_msg void OnLbnSelchangeListHelp();
	afx_msg void OnLbnSelchangeListName();
	afx_msg void OnLbnDblclkListName();
};
