// MainFrm.cpp : implementation of the CMainFrame class
//
// *      Adopted to MFC by Vladimir Petrov

#include "stdafx.h"
#include "GLobResource.h"

#include "MainFrm.h"
#include "AboutDlg.h"
#include "DlgTitle.h"
//include "DlgLesson.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void TestMessageBox(CString wString);
CMenu * MakeDubleMenu(CMenu * pMenu);
//int InitGL_30(void);
int DrawGLSceneN(void);		// Here's Where We Do All The Drawing
void UpdateGLScene(void);
void DeinitializeN(void);

CAboutDlg * m_pAboutDlg = NULL;
extern bool m_bFullScreen;
extern CString m_strNum;

CString m_strNum = "01";
CString m_strLesson; 

CDlgTitle * m_pDlgTitle = NULL;
static CMenu* m_pRemMenu = NULL;

BOOL m_bGlsm = FALSE;

extern int	mouse_x, mouse_y;                          //Lesson 32
void Selection(void);								// This Is Where Selection Is Done

char * CommonCommand[]=
{
"Ctrl + Enter  :Full Screen/Normal Screen",
"Alt + F4       :Exit",
"Ctrl + P         :Play/Stop",
"F1               :This Help",
"Ctrl + L         :Select Lesson",
"",
};


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_OPEN,
	ID_FILE_PLAY,
	ID_APP_ABOUT,
};

/////////////////////////////////////////////////////////////////////////////



// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/**
 if (!m_wndToolBar.Create(this) ||
      !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
   {
      TRACE0("Failed to create toolbar\n");
      return -1;      // fail to create
   }
/**/

/**	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
/**
	if (!m_wndToolBar.Create(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
/**/
/**/
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadBitmap(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
/**/
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);




	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::OnHelp()
{
	if(m_pAboutDlg == NULL)
		m_pAboutDlg = new CAboutDlg();
    m_pAboutDlg->ShowWindow(SW_SHOWNORMAL);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
//	TestMessageBox("CMainFrame::OnSize\n");
	CFrameWnd::OnSize(nType, cx, cy);

}
/////////////////////////////////////////////////////////////////////
void CMainFrame::OnVkReturnCtrl()
{
	m_bFullScreen = ! m_bFullScreen;
if (m_bFullScreen) // Попробуем перейти в полноэкранный режим?
{
    ModifyStyle(WS_CAPTION, 0);
	m_wndToolBar.ShowWindow(SW_HIDE);
	m_wndStatusBar.ShowWindow(SW_HIDE);
 	ShowWindow(SW_SHOWMAXIMIZED);
	HideMenu();
}
else
 {
   ModifyStyle(WS_CAPTION, WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE); 
	ShowMenu();
	m_wndToolBar.ShowWindow(SW_SHOWNORMAL);
	m_wndStatusBar.ShowWindow(SW_SHOWNORMAL);
	ShowWindow(SW_SHOWNORMAL);
}//if (m_bFullScreen)
}
/////////////////////////////////////////////////////////
void CMainFrame::HideMenu()
{
   CMenu* pMenu = GetMenu();
   if(m_pRemMenu == NULL)
      m_pRemMenu = MakeDubleMenu(pMenu);
   SetMenu(NULL);
}
////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ShowMenu()
{
   SetMenu(m_pRemMenu);
   m_hMenuDefault = m_pRemMenu->m_hMenu;
}
////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void CMainFrame::SetTitlePos(void)
{
	if(m_pDlgTitle == NULL)
		return;

	CRect rect;
	GetWindowRect(&rect);
	CRect tRect;
	m_pDlgTitle->GetClientRect(&tRect);
	m_pDlgTitle->MoveWindow(rect.right-tRect.Width()-(m_bFullScreen ? 0:5), 
		rect.top+ (m_bFullScreen ? 5:42) , 
		tRect.Width(), tRect.Height());
	m_pDlgTitle->ShowWindow(SW_SHOWNORMAL);
}
//////////////////////////////////////////////////////////////////////////

void CMainFrame::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
		 case TIMER_PLAY:
		//	UpdateGLScene();
		//	DrawGLSceneN();
		  break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnVkEscape()
{

}
//////////////////////////////////////////////////////////////////////////////

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
//	mouse_x = point.x; mouse_y = point.y;

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
//	mouse_x = point.x; mouse_y = point.y;
//	Selection();
//    DrawGLSceneN();
	CFrameWnd::OnLButtonDown(nFlags, point);
}
///////////////////////////////////////////////////////////////
void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);
	SetTitlePos();
}
////////////////////////////////////////////////

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
