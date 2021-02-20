#pragma once
#include "MS3D\MS3DFile.h"
#include "MS3D\Shader.h"

class CSpinnerModel
{
protected :
	Shader*			m_pShaider = nullptr;
	CMS3DFile		m_Model;
	GLfloat			m_fNearPlane = 1.0f; 
	GLfloat			m_fFarPlane = 1000.0f;
	bool			m_bDrawScene = true;
	//
	std::vector<GLfloat> m_LightAmbient = { 0.5f, 0.5f, 0.5f, 1.0f };
	std::vector<GLfloat> m_LightDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	std::vector<GLfloat> m_LightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::vector<GLfloat> m_LightPosition = { 300.0f, 300.0f, 500.0f, 1.0f };
public:
	CSpinnerModel();
	~CSpinnerModel();
	//
	int	InitGL( GLvoid );
	int DrawGLScene( GLfloat Angle );
	void MoveLight(GLfloat  dX, GLfloat dY);
	void SetAmbientLight( GLfloat  Val );
	void SetDiffuseLight(GLfloat  Val);
	void SetSpecularLight(GLfloat  Val);
};

