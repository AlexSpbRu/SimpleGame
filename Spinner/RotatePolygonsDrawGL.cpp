//RotatePolygonsDrawGL
#include "stdafx.h"		    // Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library


int DrawGLScene_04(GLfloat Angle);									// Here's Where We Do All The Drawing
extern BOOL m_bDrawScene;

int DrawGLSceneN( GLvoid )
{
	//if(!m_bDrawScene)
	//	 return TRUE ;
	//m_bDrawScene = FALSE;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// ќчистка экрана и буфера глубины
 //   glLoadIdentity();										// —брос текущей матрицы вида модели
	//glColor3f(1.0f,1.0f,1.0f);	
	//
	//BOOL bRslt = DrawGLScene_04(AngularVelocity);

 //  if(bRslt)
	//SwapBuffers(wglGetCurrentDC());
	//m_bDrawScene = TRUE;

	//return bRslt;
	return false;
}
//////////////////////////////////////////////////////////////////
void UpdateGLScene(void){}
void ProcessKeyboardN(CString nStr){}