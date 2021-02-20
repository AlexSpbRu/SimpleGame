// RotatePolygonsView.cpp : implementation of the CRotatePolygonsView class

#include "stdafx.h"
#include "stdafx.h"		    // Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#define _USE_MATH_DEFINES
#include <math.h>

#include "SpinnerModel.h"

#include "RotatePolygons.h"
#include "MainFrm.h"

#include "RotatePolygonsDoc.h"
#include "RotatePolygonsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_PLAY 137
#define ID_TIMER_WAIT 139


bool CreateGLWindow(CWnd * pWnd, int bits);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);		// Resize And Initialize The GL Window
//int InitGL_04(GLvoid);									// All Setup For OpenGL Goes Here
//int DrawGLScene_04( GLfloat Angle);						// Here's Where We Do All The Drawing
GLvoid KillGLWindow(CWnd * pWnd);							// Properly Kill The Window
bool PlayEnableN(void);
CString strFormat(CString frmt, ...);
CString GetLessonName(CString nStr);

extern CString m_strNum;

BOOL m_bDrawScene = TRUE;

BOOL m_bTm = FALSE;
BOOL m_bRus = FALSE;

CStringArray m_strEngArray;
CStringArray m_strRusArray;

void CRotatePolygonsView :: DrawScene( GLfloat Angle )
{
	if(!m_bDrawScene)
		return ;
	m_bDrawScene = FALSE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Очистка экрана и буфера глубины
    glLoadIdentity();										// Сброс текущей матрицы вида модели
	glColor3f(1.0f,1.0f,1.0f);	
	
	BOOL bRslt = mSpinnerModel.DrawGLScene(Angle);// DrawGLScene_04(Angle);
	if(bRslt)
		SwapBuffers(wglGetCurrentDC());
	m_bDrawScene = TRUE;

}


// CRotatePolygonsView

IMPLEMENT_DYNCREATE(CRotatePolygonsView, CView)

BEGIN_MESSAGE_MAP(CRotatePolygonsView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VK_RETURN_CTRL, OnVkReturnCtrl)
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_COMMAND(ID_VK_ESCAPE, OnVkEscape)
	ON_COMMAND(ID_HELP, OnHelp)
	//ON_COMMAND(ID_FILE_PLAY, OnFilePlay)
	//ON_UPDATE_COMMAND_UI(ID_FILE_PLAY, OnUpdateFilePlay)
	//ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_TIMER()
	//ON_WM_TIMER()
	//ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_LANGUAGE_RUSSIAN, OnLanguageRussian)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, OnLanguageEnglish)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_RUSSIAN, OnUpdateLanguageRussian)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_ENGLISH, OnUpdateLanguageEnglish)

	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_KEYUP()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CRotatePolygonsView construction/destruction

CRotatePolygonsView::CRotatePolygonsView()
{
	// TODO: add construction code here

}

CRotatePolygonsView::~CRotatePolygonsView()
{
	KillGLWindow(this);
}

BOOL CRotatePolygonsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRotatePolygonsView drawing

void CRotatePolygonsView::OnDraw(CDC* /*pDC*/)
{
	if(!m_bDrawScene)
		return;
	m_bDrawScene = FALSE;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Очистка экрана и буфера глубины
    glLoadIdentity();										// Сброс текущей матрицы вида модели
    glColor3f(1.0f,1.0f,1.0f);

	BOOL bRslt = mSpinnerModel.DrawGLScene(m_fCurrentAngle);//DrawGLScene_04(m_fCurrentAngle);

	if(bRslt)
		SwapBuffers(wglGetCurrentDC());
	m_bDrawScene = TRUE;


}
//////////////////////////////////////////////////////////////////////


// CRotatePolygonsView printing

BOOL CRotatePolygonsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRotatePolygonsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRotatePolygonsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRotatePolygonsView diagnostics

#ifdef _DEBUG
void CRotatePolygonsView::AssertValid() const
{
	CView::AssertValid();
}

void CRotatePolygonsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRotatePolygonsDoc* CRotatePolygonsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRotatePolygonsDoc)));
	return (CRotatePolygonsDoc*)m_pDocument;
}
#endif //_DEBUG


// CRotatePolygonsView message handlers

int CRotatePolygonsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_Center = { lpCreateStruct->cx/2, lpCreateStruct->cy/2};
	CreateGLWindow(this,32);

	return 0;
}

void CRotatePolygonsView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
//    CreateListLessons();
	mSpinnerModel.InitGL(); //	InitGL_04();
	m_strNum = "04";
	GetDocument()->SetTitle(GetLessonName(m_strNum));
	SetTimer(ID_TIMER_PLAY, 1, NULL);
	m_bTm = TRUE;

}
///////////////////////////////////////////////////////////////////////

void CRotatePolygonsView::OnSize(UINT nType, int cx, int cy)
{
	m_Center = { cx / 2, cy / 2 };
	CView::OnSize(nType, cx, cy);

	ReSizeGLScene(cx,cy);
}
//////////////////////////////////////////////////////////////////////////

void CRotatePolygonsView::OnAppAbout(){GetMainFrameGlob->OnHelp();}
void CRotatePolygonsView::OnVkReturnCtrl(){GetMainFrameGlob->OnVkReturnCtrl();}
void CRotatePolygonsView::OnViewFullScreen(){OnVkReturnCtrl();}
void CRotatePolygonsView::OnVkEscape(){GetMainFrameGlob->OnVkEscape();}
void CRotatePolygonsView::OnHelp(){OnAppAbout();}


void CRotatePolygonsView::OnTimer(UINT nIDEvent)
{
	double t = 0.0f;
	if (!m_bRotateByMouse) {
		m_CurrentTime = std::chrono::steady_clock::now();
		double dT = (m_CurrentTime - m_LastTime).count() / 1000000000.0f;
		if( m_fAngularVelocity >= 0.0f )
			m_fAngularVelocity -= m_fAngularVelocity*m_fAngularAcceleration*dT;
		else
			m_fAngularVelocity += m_fAngularVelocity*m_fAngularAcceleration*dT;
		m_fCurrentAngle += m_fAngularVelocity*dT;
		if (m_fCurrentAngle > 360.0f) m_fCurrentAngle -= 360.0f;
		else if (m_fCurrentAngle < -360.0f) m_fCurrentAngle += 360.0f;
		m_fLastAngle = m_fCurrentAngle;
		m_LastTime = m_CurrentTime;
		t = m_CurrentTime.time_since_epoch().count() / 1000000000.0f;
		//IO_TRACE(" Angle = %f  AngularVelocity = %f time = %f\n", m_fCurrentAngle, m_fAngularVelocity, t);
	}
	switch(nIDEvent)
	{
	case ID_TIMER_PLAY: {
		static double tlast = 0.0f;
		if (tlast == t)
			int f = 333;
			//m_CurrentTime = std::chrono::steady_clock::now();
			//auto dT = m_CurrentTime.time_since_epoch().count() / 1000000000.0f; //(m_CurrentTime - m_LastTime).count() / 1000000000.0f;
			mSpinnerModel.DrawGLScene(-m_fCurrentAngle);
			IO_TRACE(" Кадров = %f\n", 1/(t- tlast + 0.0000001f));
			tlast = t;
			}
			break;
		case ID_TIMER_WAIT:
			break;
	}

	CView::OnTimer(nIDEvent);
}
////////////////////////////////////////////////////////////////////

void CRotatePolygonsView::OnLanguageRussian()
{
	BOOL bRem = m_bRus;
	m_bRus = TRUE;
	if(m_bRus == bRem)
		return;
GetDocument()->SetTitle(GetLessonName(m_strNum));
}
void CRotatePolygonsView::OnLanguageEnglish()
{
	BOOL bRem = m_bRus;
	m_bRus = FALSE;
	if(m_bRus == bRem)
		return;
GetDocument()->SetTitle(GetLessonName(m_strNum));
}
void CRotatePolygonsView::OnUpdateLanguageRussian(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRus);
	pCmdUI->Enable(!m_bRus);
}
void CRotatePolygonsView::OnUpdateLanguageEnglish(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bRus);
	pCmdUI->Enable(m_bRus);
}
/////////////////////////////////////////////////////////////////////////////
GLfloat	d_rtri = 0.0f;
GLfloat	d_rquad = 0.0f;

void CRotatePolygonsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_cPressedKey = nChar;
	// TODO: Add your message handler code here and/or call default
	if (nChar == 187) {
		d_rtri += 0.5;
		d_rquad += 0.5;
	} else if(nChar == 189 ) {
		d_rtri -= 0.1;
		d_rquad -= 0.1;
	} 
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CRotatePolygonsView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_CurrentTime = std::chrono::steady_clock::now();
	m_LastTime = m_CurrentTime;
	
	m_LastMousePoint = point;

	if (m_cPressedKey == 0) {
		m_fLastAngle = atan2(m_LastMousePoint.y - m_Center.y, m_LastMousePoint.x - m_Center.x) / M_PI*180.0f;
		m_fCurrentAngle = m_fLastAngle;
		m_CurrentMousePoint = point;
		m_bRotateByMouse = true;
	}
	else if (m_cPressedKey == 'l' || m_cPressedKey == 'L') {

	}
	CView::OnLButtonDown(nFlags, point);
}


void CRotatePolygonsView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_cPressedKey == 0) {
		m_bRotateByMouse = false;
	}
	
	CView::OnLButtonUp(nFlags, point);
}


void CRotatePolygonsView::OnMouseMove(UINT nFlags, CPoint point )
{
	if (m_cPressedKey == 0 && m_bRotateByMouse) {
		m_CurrentTime = std::chrono::steady_clock::now();
		m_fCurrentAngle = atan2(point.y - m_Center.y, point.x - m_Center.x) / M_PI*180.0f;
		m_fAngularVelocity = (m_fCurrentAngle - m_fLastAngle) / ((m_CurrentTime - m_LastTime).count() / 1000000000.0f);

		m_fLastAngle = m_fCurrentAngle;
		m_LastTime = m_CurrentTime;
		float t = m_CurrentTime.time_since_epoch().count() / 1000000000.0f;
		//IO_TRACE(" Angle = %f  AngularVelocity = %f time = %f\n", m_fCurrentAngle, m_fAngularVelocity, t );
		//
		if( (point.x - m_Center.x)*(point.x - m_Center.x)  + (point.y - m_Center.y)*(point.y - m_Center.y) >
			min(m_Center.x*m_Center.x, m_Center.y*m_Center.y )*0.7 )
			m_bRotateByMouse = false;
	}
	else if (m_cPressedKey == 'l' || m_cPressedKey == 'L') {
		mSpinnerModel.MoveLight(point.x - m_LastMousePoint.x, point.y - m_LastMousePoint.y);
	}
	else if (m_cPressedKey == 'a' || m_cPressedKey == 'A') {
		GLfloat		k = point.x - m_LastMousePoint.x;
		if (k < 0.0f)  k = 0.9f;
		else k = 1.1f;
		mSpinnerModel.SetAmbientLight( k );
	}
	else if (m_cPressedKey == 's' || m_cPressedKey == 'S') {
		GLfloat		k = point.x - m_LastMousePoint.x;
		if (k < 0.0f)  k = 0.9f;
		else k = 1.1f;
		mSpinnerModel.SetSpecularLight(k);
	}
	else if (m_cPressedKey == 'd' || m_cPressedKey == 'D') {
		GLfloat		k = point.x - m_LastMousePoint.x;
		if (k < 0.0f)  k = 0.9f;
		else k = 1.1f;
		mSpinnerModel.SetDiffuseLight(k);
	}

	m_LastMousePoint = point;
	CView::OnMouseMove(nFlags, point);
}


void CRotatePolygonsView::OnMouseLeave()
{
	m_bRotateByMouse = false;

	CView::OnMouseLeave();
}


void CRotatePolygonsView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_cPressedKey = 0;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CRotatePolygonsView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnChar(nChar, nRepCnt, nFlags);
}
