//RotatePolygonsDrawInit
/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 *		Conversion to Visual Studio.NET done by Grant James(ZEUS)
 *      Adopted to MFC by Vladimir Petrov
 */
#include "stdafx.h"		    // Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include "Textures.h"

#include "MainFrm.h"		

extern BOOL m_bTm  ;
CGLTexture*			m_Image = nullptr;
void GetAppDirectory(std::string& strPath);

int InitGL_04(GLvoid)										// All Setup For OpenGL Goes Here
{
	std::string  m_AppPath;
	GetAppDirectory(m_AppPath);
	std::string picName = m_AppPath + "\Data\\Game\\spinner2.png";
	m_Image = new CGLTexture();
	bool ret = m_Image->CreateTexture( picName, GL_LINEAR, GL_LINEAR, false);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	if(! m_bPlay )
		m_bTm = TRUE;
	return TRUE;										// Initialization Went OK
}

static GLfloat	rtri = 0;				// Angle For The Triangle ( NEW )
static GLfloat	rquad = 0;				// Angle For The Quad ( NEW )
GLfloat	d_rtri = 0.0f;	
GLfloat	d_rquad = 0.0f;


int DrawGLScene_04( GLfloat Angle)									// Here's Where We Do All The Drawing
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	//glLoadIdentity();									// Reset The Current Modelview Matrix
	//glTranslatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
	//glRotatef(rtri,0.0f,0.0f,1.0f);						// Rotate The Triangle On The Y axis ( NEW )
	//glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
	//	glColor3f(1.0f,0.0f,0.0f);						// Set Top Point Of Triangle To Red
	//	glVertex3f( 0.0f, 1.0f, 0.0f);					// First Point Of The Triangle
	//	glColor3f(0.0f,1.0f,0.0f);						// Set Left Point Of Triangle To Green
	//	glVertex3f(-1.0f,-1.0f, 0.0f);					// Second Point Of The Triangle
	//	glColor3f(0.0f,0.0f,1.0f);						// Set Right Point Of Triangle To Blue
	//	glVertex3f( 1.0f,-1.0f, 0.0f);					// Third Point Of The Triangle
	//glEnd();											// Done Drawing The Triangle
	//glLoadIdentity();									// Reset The Current Modelview Matrix
	//glTranslatef(1.5f,0.0f,-6.0f);						// Move Right 1.5 Units And Into The Screen 6.0
	//glRotatef(rquad,0.0f,0.0f, 1.0f);					// Rotate The Quad On The X axis ( NEW )
	//glColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
	//glBegin(GL_QUADS);									// Draw A Quad
	//	glVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
	//	glVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
	//	glVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
	//	glVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
	//glEnd();											// Done Drawing The Quad
	//rtri+= d_rtri;											// Increase The Rotation Variable For The Triangle ( NEW )
	//rquad -= d_rquad;										// Decrease The Rotation Variable For The Quad ( NEW )
	//return TRUE;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Keep Going
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(Angle, 0.0f, 0.0f, 1.0f);
	m_Image->Bind();
	glBegin(GL_QUADS);						// Draw A Textured Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.0f, -2.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, -2.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	return true;
}
////////////////////////////////////////////////////
