// DlgLesson.cpp : implementation file
//

#include "stdafx.h"

//#include "resource.h"
#include "mainfrm.h"
#include "AboutDlg.h"
#include "DlgLesson.h"
#include "DlgTitle.h"

//void RemoveAllTextures(void);
//void glOnSize( int cx, int cy);
void TestMessageBox(CString wString);
void ReSizeGLScene(int width, int height);		// Resize And Initialize The GL Window
BOOL StartExeOfNum(CString nStr);
void UpdateDlgTitle();
void SetCurselLesson(CListBox * pBx, CString nStr);
bool CreateGLWindow(CWnd * pWnd, int bits);
void KillGLWindow(CWnd * pWnd);								// Properly Kill The Window
FILE * MyFopen( CString fName, CString sMode);

extern BOOL m_bTm;
extern BOOL m_bGlsm;

extern CString m_strNum;
extern CString m_strLesson;

extern BOOL bTestPrint;

extern CDlgTitle * m_pDlgTitle;
extern BOOL m_bRus;
extern CAboutDlg * m_pAboutDlg;
extern RECT m_viewRect;

extern CStringArray m_strEngArray;
extern CStringArray m_strRusArray;



CString strFormat(CString frmt, ...);
CString GetDataFile(CString fName, BOOL bInform);

void DeinitializeN(void);
int InitGLn(CString nStr);
void ResetListFromFile(CListBox * pBx, CString fName, BOOL bCmd);
CString GetDataFile(CString fName, BOOL bInform);
void RemoveAllTextures(void);
int DrawGLSceneN(void);

static char str[2048];
// CDlgLesson dialog


IMPLEMENT_DYNAMIC(CDlgLesson, CDialog)
CDlgLesson::CDlgLesson(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLesson::IDD, pParent)
	, m_str_lesson(_T(""))
	, m_radio_eng(1)
	, m_radio_rus(0)
	, m_pWnd(NULL)
{
}

CDlgLesson::~CDlgLesson()
{
}

void CDlgLesson::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LESSON, m_list_lesson);
	DDX_LBString(pDX, IDC_LIST_LESSON, m_str_lesson);
}


BEGIN_MESSAGE_MAP(CDlgLesson, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_LESSON, OnLbnDblclkListLesson)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ENG, OnBnClickedRadioEng)
	ON_BN_CLICKED(IDC_RADIO_RUS, OnBnClickedRadioRus)
	ON_LBN_SELCHANGE(IDC_LIST_LESSON, OnLbnSelchangeListLesson)
END_MESSAGE_MAP()

BOOL isFileExist( CString fileName);

// CDlgLesson message handlers

BOOL CDlgLesson::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateLanguage();
 	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
/////////////////////////////////////////////////////////////////////////////
void CDlgLesson::OnLbnDblclkListLesson()
{
OnBnClickedOk();
}
////////////////////////////////////////////////////////////////////////////////

void CDlgLesson::OnBnClickedOk()
{
	CString nStr = GetNumLesson();
	if(!nStr.GetLength() || nStr == m_strNum)
		return;
    CString nString;
	m_list_lesson.GetText(m_list_lesson.GetCurSel(), nString);

	if(m_bGlsm)
	{
	   DeinitializeN();
	   KillGLWindow(m_pWnd);
	   CreateGLWindow(m_pWnd,32);
   //  TestMessageBox("CDlgLesson::OnBnClickedOk\n");
	   bTestPrint = TRUE;
	   if(InitGLn(nStr))
		{
        m_strNum = nStr;
		m_strLesson = nString;
		}//if(InitGLn(nStr))
        else
	    {
		MessageBox("Lesson " + nStr + ":\n" + nString.Mid(nStr.GetLength()+1)
			+ "\n\nnot Available at the Moment",
			"Sorry:", MB_OK | MB_ICONINFORMATION);

		InitGLn(m_strNum);
		return;
	    }//if(!InitGLn(nStr))

      m_bTm = TRUE;
	  UpdateDlgTitle();
 // TestMessageBox("CDlgLesson::OnBnClickedOk\n");
	  ReSizeGLScene(m_viewRect.right,m_viewRect.bottom);
	  DrawGLSceneN();
	  FILE * f = MyFopen("LastNum.txt","wt");
    if(f != NULL)
	{
		fputs(m_strLesson, f);
        fclose(f);
	}//if(f != NULL)
    if(m_pAboutDlg == NULL)
	{
       m_pAboutDlg = new CAboutDlg;
	   m_pAboutDlg->m_pWnd = m_pWnd;
	}
		m_pAboutDlg->UpdateHelp();
	}//
	else
	if(!StartExeOfNum(nStr))
		return;
	else
	{
	AfxGetApp()->CloseAllDocuments(TRUE);
	exit(0);
	}

	OnOK();

	
}

void CDlgLesson::OnBnClickedRadioEng()
{
	m_bRus = !m_bRus;
	UpdateLanguage();
}

void CDlgLesson::OnBnClickedRadioRus()
{
	m_bRus = !m_bRus;
	UpdateLanguage();
}
/////////////////////////////////////////////////////////////////////////////////
void CDlgLesson::UpdateLanguage(void)
{
	CButton * pBtn = (CButton *)GetDlgItem(IDC_RADIO_RUS); 
	pBtn->SetCheck(m_bRus);
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_ENG); 
	pBtn->SetCheck(!m_bRus);


	CListBox * pBx = &m_list_lesson;
	pBx->ResetContent();
	if(!m_strEngArray.GetSize())
		return;
	CStringArray * pSarray = m_bRus ? &m_strRusArray : &m_strEngArray;

	for(int i = 0; i < pSarray->GetSize(); i++)
		pBx->AddString(pSarray->GetAt(i));
 
  SetCurselLesson(pBx, m_strNum);
  UpdateOkButton();


}
////////////////////////////////////////////////////////////////////////////
void CDlgLesson::UpdateOkButton(void)
{

	CButton * pBtn = (CButton *)GetDlgItem(IDOK);
	CString nStr = GetNumLesson();
	pBtn->EnableWindow(nStr.GetLength() && nStr != m_strNum);

}
////////////////////////////////////////////////////////////////////////////
CString CDlgLesson::GetNumLesson(void)
{
	int nn = m_list_lesson.GetCurSel();
	if(nn < 0)
		return "";
	CString tString;


	m_list_lesson.GetText(nn, tString);

	return strtok(strcpy(str,tString), m_strtok);

}
///////////////////////////////////////////////////////////////////////////
void CDlgLesson::OnLbnSelchangeListLesson()
{
	UpdateOkButton();
}
//////////////////////////////////////////////////////////////////////////////
