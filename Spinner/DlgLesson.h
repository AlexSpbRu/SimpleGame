#pragma once
#include "afxwin.h"
#define IDD_DLG_LESSON                  131
// CDlgLesson dialog

class CDlgLesson : public CDialog
{
	DECLARE_DYNAMIC(CDlgLesson)

public:
	CDlgLesson(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLesson();

// Dialog Data
	enum { IDD = IDD_DLG_LESSON };
    CWnd * m_pWnd;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnDblclkListLesson();
	CListBox m_list_lesson;
	CString m_str_lesson;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioEng();
	afx_msg void OnBnClickedRadioRus();
	int m_radio_eng;
	int m_radio_rus;
	void UpdateLanguage(void);
    void UpdateOkButton(void);
	CString GetNumLesson(void);
	afx_msg void OnLbnSelchangeListLesson();
};
