//GLRoutine.cpp
#include "stdafx.h"		    // Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <direct.h>		
#include <math.h>		
#include "MainFrm.h"		// Header File For The Glaux Library



void TestMessageBox(CString wString);
CString strFormat(CString frmt, ...);

static char str[512];

GLfloat  fNearPlane = 1.0f, fFarPlane = 1000.0f, fViewAngle = 45.f, fAspect;
RECT m_viewRect;
int	mouse_x, mouse_y;                          //Lesson 32
bool	m_bFullScreen=FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

extern BOOL m_bTm;
extern CString m_strNum;
BOOL isFileExist( CString fileName);

/**/
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)									// Prevent A Divide By Zero By
		height=1;										// Making Height Equal One

	m_viewRect.left = m_viewRect.top =0;
    m_viewRect.right = width;
    m_viewRect.bottom = height;
//if(m_strNum == "27")
//TestMessageBox("ReSizeGLScene\n" +  strFormat("w = %d h = %d\n", m_viewRect.right, m_viewRect.bottom));
	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
 
	//if(m_strNum == "21"||m_strNum == "24")
	//	 glOrtho(0.0f,width,height,0.0f,-1.0f,1.0f);				// Create Ortho 640x480 View (0,0 At Top Left)
	//else	// Calculate The Aspect Ratio Of The Window
	gluPerspective( fViewAngle, (GLfloat)width/(GLfloat)height, fNearPlane, fFarPlane );

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

}
///////////////////////////////////////////////////////////////////////////////////////////
GLvoid KillGLWindow(CWnd * pWnd)								// Properly Kill The Window
{
HWND hWnd = pWnd->m_hWnd;
HDC	hDC= GetDC(hWnd);		// Private GDI Device Context
HGLRC hRC = wglCreateContext(hDC);	// Permanent Rendering Context

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	
}
///////////////////////////////////////////////////////////////////////////////
void DrawTextureCube(GLfloat a)
{
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-a, -a,  a);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( a, -a,  a);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( a,  a,  a);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-a,  a,  a);
		// Back Face			  
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-a, -a, -a);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-a,  a, -a);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( a,  a, -a);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( a, -a, -a);
		// Top Face				  
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-a,  a, -a);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-a,  a,  a);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( a,  a,  a);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( a,  a, -a);
		// Bottom Face			  
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-a, -a, -a);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( a, -a, -a);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( a, -a,  a);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-a, -a,  a);
		// Right face			  
		glTexCoord2f(1.0f, 0.0f); glVertex3f( a, -a, -a);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( a,  a, -a);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( a,  a,  a);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( a, -a,  a);
		// Left Face			  
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-a, -a, -a);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-a, -a,  a);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-a,  a,  a);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-a,  a, -a);
	glEnd();
}
/////////////////////////////////////////////////////////////////////////////
bool CreateGLWindow(CWnd * pWnd, int bits)
{
static	PIXELFORMATDESCRIPTOR pfd=					// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// Size Of This Pixel Format Descriptor
		1,												// Version Number
		PFD_DRAW_TO_WINDOW |							// Format Must Support Window
		PFD_SUPPORT_OPENGL |							// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,								// Must Support Double Buffering
		PFD_TYPE_RGBA,									// Request An RGBA Format
		bits,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,								// Color Bits Ignored
		0,												// No Alpha Buffer
		0,												// Shift Bit Ignored
		0,												// No Accumulation Buffer
		0, 0, 0, 0,										// Accumulation Bits Ignored
		16,												// 16Bit Z-Buffer (Depth Buffer)  
		1,												// Use Stencil Buffer ( * Important * )
		0,												// No Auxiliary Buffer
		PFD_MAIN_PLANE,									// Main Drawing Layer
		0,												// Reserved
		0, 0, 0											// Layer Masks Ignored
	};



     HDC hDC;

	 if (!(hDC=GetDC(pWnd->m_hWnd)))								// Did We Get A Device Context?
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}
     int PixelFormat;
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))		// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))			// Are We Able To Set The Pixel Format?
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	HGLRC hRC;

	if (!(hRC=wglCreateContext(hDC)))					// Are We Able To Get A Rendering Context?
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))						// Try To Activate The Rendering Context
	{
		KillGLWindow(pWnd);									// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

    CRect rect;
	pWnd->GetClientRect(&rect);
	SetForegroundWindow(pWnd->m_hWnd);						// Slightly Higher Priority
	ReSizeGLScene(rect.Width(), rect.Height());					// Set Up Our Perspective GL Screen

	return TRUE;
}
///////////////////////////////////////////////////////////////////
HDC GetMainDC(void)
{
	return GetDC(GetMainFrameGlob->m_hWnd);
}
///////////////////////////////////////////////////////////////////////////
//CBitmap * LoadMyBitmap(CString fName)                // Loads A Bitmap Image
/**
{

_getcwd(str, MAX_PATH);

//TestMessageBox("Start LoadBMP\n" + (CString) Filename + "\n" + (CString)str);

        FILE *File=NULL;                                // File Handle
        if (!isFileExist(Filename))        // Make Sure A Filename Was Given
        {   
TestMessageBox("no file exist LoadBMP\n" + (CString) Filename + "\n" + (CString)str);
         return NULL;  
		}
                return auxDIBImageLoad(Filename);       // Load The Bitmap And Return A Pointer
        return NULL;                                    // If Load Failed Return NULL
}
/**/
///////////////////////////////////////////////////////////////////////
void SetGLDisable(void)
{
        fViewAngle = 45; fNearPlane = 0.1f; fFarPlane = 100;  
	    glClearColor(0,0,0,0);
		glDisable(GL_LIGHTING);
	    glDisable(GL_FOG);
	    glDisable(GL_LIGHT1);
	    glDisable(GL_LIGHT0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_GEN_S);    // Выключим генерацию координат текстуры для S ( Lesson 23)
        glDisable(GL_TEXTURE_GEN_T);    // Выключим генерацию координат текстуры для T ( Lesson 23 )
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_CULL_FACE);
	//	ReSizeGLScene(m_viewRect.right, m_viewRect.top);
}
//////////////////////////////////////////////////////////////////////
 void CreatePatchMap(int nSize, int nPatch, BYTE *pMountMap)  //Lesson 44
{
struct vct					//Lesson 44
{
	double v[3];
};
#define    HEIGHT  250	//LESSON 44


	for (int i = 0; i < nSize; i++)
    	for (int j = 0; j < nSize; j++)
			pMountMap[i + j * nSize] = 150;
//return;
//TestMessageBox("CreatePatchMap\n0");
	AfxGetApp()->m_pMainWnd->BeginWaitCursor();
	CPtrArray  pArray;
	int mrj = 100;
	for(int n = 0; n < nPatch; n++)
	{
       struct vct * pV = new struct vct;
	   pArray.Add(pV);
	   pV->v[0] = mrj + rand() % (nSize - mrj - mrj); 
	   pV->v[1] = mrj + rand() % (nSize - mrj - mrj);
//	   pV->v[2] = mrj + rand() % (HEIGHT - mrj);
	   pV->v[2] = mrj - rand() % (HEIGHT - mrj);

	}//for(int i = 0; i < nPatch; i++)

//TestMessageBox("0.1");
	for (int i = 0; i < nSize; i++)
    	for (int j = 0; j < nSize; j++)
		{
			int ht = 0;
	      for(int n = 0; n < nPatch; n++)
	      {
       struct vct * pV = (struct vct *) pArray.GetAt(n);
	   int id = (int) pV->v[0];
	   int jd = (int) pV->v[1];
	   int hd = (int) pV->v[2];
	   ht = max(ht, (int)((hd * exp(-0.0001 *((i-id)*(i-id) + (j-jd)*(j-jd))))));
		  }//for(int i = 0; i < nPatch; i++)
			pMountMap[i + j * nSize] += ht;
//      if(ht)
//	  {
//	   CString wString;
//	   wString.Format("x = %d   y = %d   h = %d\n", i,j,ht);
//	   TestMessageBox("CreatePatchMap\n" + wString);
//	  }
  	   }//for( j = 0; j < nSize; j ++)

//	for (int i = 0; i < nSize; i++)
//    	for (int j = 0; j < nSize; j++)
//			pMountMap[i + j * nSize] -= 149;
	
		
		
		AfxGetApp()->m_pMainWnd->EndWaitCursor();
 //TestMessageBox("CreatePatchMap\nFin");

}
///////////////////////////////////////////////////////////////////////
	