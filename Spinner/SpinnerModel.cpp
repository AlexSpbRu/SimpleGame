#include "stdafx.h"
#include "Textures.h"
#include "SpinnerModel.h"


CSpinnerModel::CSpinnerModel()
{
}


CSpinnerModel::~CSpinnerModel()
{
}

CGLTexture*			m_Image = nullptr;
void GetAppDirectory(std::string& strPath);




int CSpinnerModel::InitGL(GLvoid)									// All Setup For OpenGL Goes Here
{
	std::string  m_AppPath;
	GetAppDirectory(m_AppPath);
	std::string picName = m_AppPath + "\Data\\Game\\spinner2.png";
	m_Image = new CGLTexture();
	bool ret = m_Image->CreateTexture(picName, GL_LINEAR, GL_LINEAR, false);
	void InitAPI();
	InitAPI();
	//CheckMyDir();
	//fNearPlane = 1.0f; fFarPlane = 1000.0f;
	//pModel = new CMS3DFile();										// Memory To Hold The Model
	if (m_Model.LoadFromFileFast("d:\\Projects\\GameGL\\Bin\\Data\\Game\\spinner.ms3d" ) == false )		// Loads The Model And Checks For Errors
	{
		MessageBox(NULL, "Couldn't load the model data\\model.ms3d", "Error", MB_OK | MB_ICONERROR);
		return 0;													// If Model Didn't Load Quit
	}
	//	pModel->prepareModelForAnimation();
	//pModel->reloadTextures();										// Loads Model Textures
	m_pShaider = new Shader("d:\\Projects\\OpenGL_tests\\lesson31\\ModelLoad\\Bin\\shaders\\vert.shader", "d:\\Projects\\OpenGL_tests\\lesson31\\ModelLoad\\Bin\\shaders\\frag.shader");
	m_Model.prepareModel(m_pShaider->getShader());
	//	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping ( NEW )
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	//
	glEnable(GL_BLEND);										// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Type Of Blending To Use

	/*std::vector<GLfloat> m_LightAmbient = { 0.5f, 0.5f, 0.5f, 1.0f };
	std::vector<GLfloat> m_LightDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	std::vector<GLfloat> m_LightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::vector<GLfloat> m_LightPosition = { 300.0f, 300.0f, 500.0f, 1.0f };*/
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, m_LightAmbient.data());		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, m_LightDiffuse.data());		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition.data());		// Position The Light
	glLightfv(GL_LIGHT1, GL_SPECULAR, m_LightSpecular.data());
	glEnable(GL_LIGHT1);
	//
	glShadeModel(GL_SMOOTH);										// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);							// Black Background
	glClearDepth(1.0f);												// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);										// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);											// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				// Really Nice Perspective Calculations

	return TRUE;													// Initialization Went OK
}



int  CSpinnerModel::DrawGLScene( GLfloat Angle )											// Here's Where We Do All The Drawing
{
	if (!m_bDrawScene)
		return FALSE;
	m_bDrawScene = false;
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear The Screen And The Depth Buffer
	glLoadIdentity();												// Reset The Modelview Matrix
	gluLookAt(0, 0, 600/*375, 375, 375*/, 0, 0, 0, 0, 1, 0);	// (3) Eye Postion (3) Center Point (3) Y-Axis Up Vector
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(Angle, 0.0f, 1.0f, 0.0f);									// Rotate On The Y-Axis By yrot
																		
	m_Model.draw31();
	

	SwapBuffers(wglGetCurrentDC());
	m_bDrawScene = true;
	return TRUE;													// Keep Going
}

void CSpinnerModel::MoveLight(GLfloat  dX, GLfloat dY)
{
	m_LightPosition[0] += dX;
	m_LightPosition[1] += dY;
	glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition.data());
}

void CSpinnerModel::SetAmbientLight(GLfloat  Val)
{
	std::vector<GLfloat>  amb;
	for (auto& c : m_LightAmbient) {
		c = min(1.0f, max(0.0f, c*Val));
		amb.push_back(c);
	}
	IO_TRACE("Ambient : ( %f , %f, %f )\n", amb[0],  amb[1],  amb[2]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb.data());
}

void CSpinnerModel::SetDiffuseLight(GLfloat  Val)
{
	std::vector<GLfloat>  amb;
	for (auto& c : m_LightDiffuse) {
		c = min(1.0f, max(0.0f, c*Val));
		amb.push_back(c);
	}
	IO_TRACE("Diffuse : ( %f , %f, %f )\n", amb[0], amb[1], amb[2]);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, amb.data());
}

void CSpinnerModel::SetSpecularLight(GLfloat  Val)
{
	std::vector<GLfloat>  amb;
	for (auto& c : m_LightSpecular) {
		c = min(1.0f, max(0.0f, c*Val));
		amb.push_back(c);
	}
	IO_TRACE("Specular : ( %f , %f, %f )\n", amb[0], amb[1], amb[2]);
	glLightfv(GL_LIGHT1, GL_SPECULAR, amb.data());
}