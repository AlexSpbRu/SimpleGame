#include "stdafx.h"
//#include "gl\glext.h"

#include "utils.h"
#include "Math3D.h"
#include "OGLBuffers.h"

#include "Textures.h"
#include "GLFonts.h"
#include "Particles.h"

#include "OGLdraw14.h"

void GetAppDirectory(std::string& strPath);
GLuint loadBMP(const std::string& imagepath, GLuint& textureID, GLint MAGfilter, GLint MINfilter);


COGLDraw::COGLDraw(HDC hDc) : m_hDc(hDc)
{
	GetAppDirectory(m_AppPath);
	for (auto& b : m_keys) 
		b = false;
}

COGLDraw :: ~COGLDraw()
{

}

bool	COGLDraw::Init(int Width, int Height) noexcept
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return true;
}

GLvoid	COGLDraw::ProcessKeyboard() noexcept
{
	if (m_keys[VK_PRIOR])  // page up
	{
		m_Z -= 0.1f;
	}
	if (m_keys[VK_NEXT])  // page dn
	{
		m_Z += 0.1f;
	}
}

//
GLvoid	COGLDraw::ResizeScene(GLsizei width, GLsizei height ) noexcept
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-5.0f, 5.0f, -5.0f*((GLfloat)height / (GLfloat)width), 5.0f*((GLfloat)height / (GLfloat)width), -10.0f, 100.0f);
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLvoid	COGLDraw::OnKeyDown(WPARAM Key) noexcept
{
	m_keys[Key] = true;
	//IO_TRACE("Key Down : %d\n", Key);
}

GLvoid	COGLDraw::OnKeyUp(WPARAM Key) noexcept
{
	m_keys[Key] = false;
	//IO_TRACE("Key Up : %d\n", Key);
}
