// MainFrm.h : interface of the CMainFrame class
//
#define GetMainFrameGlob     ((CMainFrame *) AfxGetApp()->m_pMainWnd)
#define TIMER_PLAY 1

#define PI  3.1415926f
#define KFR  180.0/PI
#define RFK  PI/180.0

#define    MAP_SIZE  1024    // Размер карты вершин //LESSON 34
#define    MAX_MAP  16    // 51
#define    MAX_STEP  16    // 51

//#define    MAP_SIZE  45    // Размер карты вершин //LESSON 34
//#define    STEP_SIZE  4      // Ширина и высота каждого квадрата //LESSON 34
#define MAX_CADR 24
#define  m_strtok    ". :_-"

#pragma once
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);

   void OnVkReturnCtrl();
   void HideMenu();
   void ShowMenu();
   //void OnFilePlay();
   void SetTitlePos(void);
 //  void OnFileOpen();
   void OnVkEscape();


   afx_msg void OnTimer(UINT nIDEvent);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnVkUpCtrl();
   afx_msg void OnMove(int x, int y);
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


