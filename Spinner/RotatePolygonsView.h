// RotatePolygonsView.h : interface of the CRotatePolygonsView class
//


#pragma once
#include <chrono>

class CRotatePolygonsView : public CView
{
protected: // create from serialization only
	using steady_time = std::chrono::time_point<std::chrono::steady_clock>;
	steady_time			m_LastTime;
	steady_time			m_CurrentTime;
	float				m_fAngularVelocity = 0.0f;
	float				m_fAngularAcceleration = 0.10f;
	float				m_fLastAngle = 0.0f;
	float				m_fCurrentAngle = 0.0f;
	bool				m_bRotateByMouse = false;
	//
	CPoint				m_LastMousePoint = { 0, 0 };
	CPoint				m_CurrentMousePoint = { 0, 0 };
	CPoint				m_Center = { 0, 0 };
	CSpinnerModel		mSpinnerModel;
	//
	char				m_cPressedKey = 0;
	//
	CRotatePolygonsView();
	DECLARE_DYNCREATE(CRotatePolygonsView)

// Attributes
public:
	CRotatePolygonsDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void DrawScene(GLfloat Angle);
// Implementation
public:
	virtual ~CRotatePolygonsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAppAbout();
	afx_msg void OnVkReturnCtrl();
	afx_msg void OnViewFullScreen();
	afx_msg void OnVkEscape();
	afx_msg void OnHelp();
//	afx_msg void OnFilePlay();
//	afx_msg void OnUpdateFilePlay(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnFileOpen();
	afx_msg void OnLanguageRussian();
	afx_msg void OnLanguageEnglish();
	afx_msg void OnUpdateLanguageRussian(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLanguageEnglish(CCmdUI *pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

};

#ifndef _DEBUG  // debug version in RotatePolygonsView.cpp
inline CRotatePolygonsDoc* CRotatePolygonsView::GetDocument() const
   { return reinterpret_cast<CRotatePolygonsDoc*>(m_pDocument); }
#endif

