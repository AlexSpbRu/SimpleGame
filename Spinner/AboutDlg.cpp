#include "stdafx.h"
#include "MainFrm.h"
#include "AboutDlg.h"

#define IDC_LIST_NAME 1012

///////////////////////////////////////////////////////////
void ResetListFromFile(CListBox * pBx, CString fName, BOOL bCmd);
CString strFormat(CString frmt, ...);
extern CString m_strNum;
extern CString m_strLesson;
void SetCurselOfCommand(CString aString, CListBox * pBx);
CString GetDataFile(CString fName, BOOL bInform);
void ProcessKeyboardN(CString strCmnd);
void ResetListFromFile(CListBox * pAx, CListBox * pBx, CString fName);

void TestMessageBox(CString wString);

extern BOOL m_bTm;
extern BOOL m_bGlsm;
// CAboutDlg dialog

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
	,m_pWnd(NULL)
{
	Create(IDD);
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_HELP, OnLbnDblclkListHelp)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_LBN_SELCHANGE(IDC_LIST_HELP, OnLbnSelchangeListHelp)
	ON_LBN_SELCHANGE(IDC_LIST_NAME, OnLbnSelchangeListName)
	ON_LBN_DBLCLK(IDC_LIST_NAME, OnLbnDblclkListName)
END_MESSAGE_MAP()


// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateHelp();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
	
}
/////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::UpdateHelp(void)
{
	SetWindowText(m_strLesson);

	CStatic * pStat = (CStatic *)GetDlgItem(IDC_LESSON_TITLE);
	if(pStat != NULL)
		pStat->SetWindowText(m_strLesson);
	//TestMessageBox("CAboutDlg::UpdateHelp start");
CListBox * pListA = (CListBox *)GetDlgItem(IDC_LIST_HELP);
	CListBox * pListB = (CListBox *)GetDlgItem(IDC_LIST_NAME);


    if(pListA == NULL)
		return ;
	pListA->ResetContent();

   if(pListB == NULL)
		return ;
	pListB->ResetContent();
	
	
	//   ResetListFromFile(pList, "../../HelpText//"+strFormat("%s_", m_strNum) + "GLhelp.txt", TRUE);
    CString strH = m_strNum + "_GLhelp.txt";
	if(m_bGlsm)
       strH = "HelpText/" + strH;
	
	ResetListFromFile(pListA,pListB, strH);
    SetCurselOfCommand("Ctrl + L", pListA);
    OnLbnSelchangeListHelp();
	CButton * pBtn = (CButton *)GetDlgItem(IDOK);
	int nn = pListA->GetCurSel();
	pBtn->EnableWindow(nn >= 0);

//TestMessageBox("CAboutDlg::UpdateHelp fin");


}
/////////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnLbnDblclkListHelp()
{
	OnBnClickedOk();
}
///////////////////////////////////////////////////////////

void CAboutDlg::OnBnClickedOk()
{
	CListBox * pList = (CListBox *)GetDlgItem(IDC_LIST_HELP);
    if(pList == NULL)
		return ;
	int nn = pList->GetCurSel();
	if(nn < 0)
		return;
	CString tString ;
	pList->GetText(nn, tString);
/**
	nn = tString.Find(':');
	if(nn < 0)
		return;
	tString = tString.Left(nn);
	while(tString.Right(1)== " ")
		tString = tString.Left(tString.GetLength()-1);
/**/
//TestMessageBox("CAboutDlg::OnBnClickedOk\n'" + tString + "'");

	if(tString == "Ctrl + Enter") GetMainFrameGlob->OnVkReturnCtrl();
	else if(tString == "Alt + F4")    { AfxGetApp()->CloseAllDocuments(TRUE);exit(0);}
	else if(tString == "Ctrl + P")  m_bTm = TRUE;
	else if(tString == "F1")          GetMainFrameGlob->OnHelp();
	
	else if(tString == "Space")       ProcessKeyboardN("VK_SPACE");
	else if(tString == "A")			  ProcessKeyboardN("A");	
	else if(tString == "B")           ProcessKeyboardN("B");
    else if(tString == "C")			  ProcessKeyboardN("C");	
    else if(tString == "D")			  ProcessKeyboardN("D");
	else if(tString == "E")			  ProcessKeyboardN("E");
	else if(tString == "F")			  ProcessKeyboardN("F");
	else if(tString == "G")			  ProcessKeyboardN("G");
	else if(tString == "L")           ProcessKeyboardN("L");
	else if(tString == "R")           ProcessKeyboardN("R");
	else if(tString == "Z")			  ProcessKeyboardN("Z");
	else if(tString == "M")			  ProcessKeyboardN("M");
	else if(tString == "U")     	  ProcessKeyboardN("U");
	else if(tString == "1")			  ProcessKeyboardN("1");
	else if(tString == "2")			  ProcessKeyboardN("2");
	else if(tString == "3")			  ProcessKeyboardN("3");
	else if(tString == "4")			  ProcessKeyboardN("4");
	else if(tString == "S")			  ProcessKeyboardN("S");
	else if(tString == "Up Arrow")	  ProcessKeyboardN("VK_UP");
	else if(tString == "Down Arrow")  ProcessKeyboardN("VK_DOWN");
	else if(tString == "Left Arrow")  ProcessKeyboardN("VK_LEFT");
	else if(tString == "Right Arrow") ProcessKeyboardN("VK_RIGHT");
	else if(tString == "Page Up")	  ProcessKeyboardN("VK_PRIOR");
	else if(tString == "Page Down")	  ProcessKeyboardN("VK_NEXT");
	else if(tString == "Return")	  ProcessKeyboardN("VK_RETURN");
	else if(tString == "Enter")	      ProcessKeyboardN("VK_RETURN");
	else if(tString == "Tab")		  ProcessKeyboardN("VK_TAB");
	else if(tString == "+")			  ProcessKeyboardN("VK_ADD");
	else if(tString == "-")			  ProcessKeyboardN("VK_SUBTRACT");
/**/
//	OnOK();
}
///////////////////////////////////////////////////////////////////////


void CAboutDlg::OnLbnSelchangeListHelp()
{
		CListBox * pListA = (CListBox *)GetDlgItem(IDC_LIST_HELP);
        CListBox * pListB = (CListBox *)GetDlgItem(IDC_LIST_NAME);
		int nn = pListA->GetCurSel();
		pListB->SetCurSel(nn);
}
//////////////////////////////////////////////////////
void CAboutDlg::OnLbnSelchangeListName()
{
		CListBox * pListA = (CListBox *)GetDlgItem(IDC_LIST_HELP);
        CListBox * pListB = (CListBox *)GetDlgItem(IDC_LIST_NAME);
		int nn = pListB->GetCurSel();
		pListA->SetCurSel(nn);

		
}


void CAboutDlg::OnLbnDblclkListName()
{
	OnBnClickedOk();
}
////////////////////////////////////////////////////
