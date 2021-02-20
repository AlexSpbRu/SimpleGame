#pragma once
//class COGLDraw
//{	
//protected :
//	std::string		m_AppPath;
//	HDC				m_hDc = 0;
//	bool			m_keys[256];					// Array Used For The Keyboard Routine
//	GLfloat			m_Z = -10.0f;												//
//public:
//	COGLDraw(HDC hDc);
//	virtual ~COGLDraw();
//	virtual bool	Init(int Width, int Height) noexcept;
//	virtual GLvoid	ProcessKeyboard() noexcept;
//	virtual bool	DrawScene() = 0;
//	virtual bool	DrawSceneFast() = 0;
//	//
//	virtual GLvoid	ResizeScene(GLsizei width, GLsizei height) noexcept;
//	GLvoid	OnKeyDown(WPARAM Key) noexcept;
//	GLvoid	OnKeyUp(WPARAM Key) noexcept;
//	virtual GLvoid	OnLMouseDown(const POINTS& Point) = 0;
//	virtual GLvoid	OnLMouseUp(const POINTS& Point) = 0;
//	virtual GLvoid	OnMouseMove(const POINTS& Point) = 0;
//};
//
//
//class COGLDrawWithLight
//{
//	bool		m_bLight = true;
//	bool		m_bLp = false;
//protected :
//	std::vector<GLfloat>	m_LightAmbient = { 0.9f, 0.9f, 0.9f, 1.0f };
//	std::vector<GLfloat>	m_LightDiffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
//	std::vector<GLfloat>	m_LightSpecular = { 1.0f, 1.0f,  1.0f, 1.0f };
//	std::vector<GLfloat>	m_LightPosition = { 500.0f, 500.0f, 2000.0f, 1.0f };
//public :
//	GLvoid	ProcessKeyboard( const bool* Keys, size_t CountKeys ){
//		if ( 'L' >= CountKeys )
//			return;
//		if ( Keys['L'] && !m_bLp ) {
//			m_bLp = TRUE;
//			m_bLight = !m_bLight;
//			if (!m_bLight) {
//				glDisable(GL_LIGHTING);
//			}
//			else {
//				glEnable(GL_LIGHTING);
//			}
//		}
//		if ( !Keys['L'] ) {
//			m_bLp = FALSE;
//		}
//	};
//	bool	Init() {
//			glEnable(GL_LIGHTING);
//			glLightfv(GL_LIGHT1, GL_AMBIENT, m_LightAmbient.data());		// Setup The Ambient Light
//			glLightfv(GL_LIGHT1, GL_DIFFUSE, m_LightDiffuse.data());		// Setup The Diffuse Light
//			glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition.data());		// Position The Light
//			glLightfv(GL_LIGHT1, GL_SPECULAR, m_LightSpecular.data());
//			glEnable(GL_LIGHT1);											// Enable Light One
//			return true;
//		};
//};

//class COGLDrawMove
//{
//protected :
//	GLfloat					m_fxrot = 0.0f;				// X Rotation
//	GLfloat					m_fyrot = 0.0f;				// Y Rotation
//	GLfloat					m_fxspeed = 0.0f;			// X Rotation Speed
//	GLfloat					m_fyspeed = 0.0f;			// Y Rotation Speed// Which Filter To Use
//	GLfloat					m_fz = -10.0f;
//public :
//	GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys) {
//		if (CountKeys <= VK_PRIOR || CountKeys <= VK_NEXT || CountKeys <= VK_UP || CountKeys <= VK_DOWN || CountKeys <= VK_LEFT || CountKeys <= VK_RIGHT)
//			return;
//		if (Keys[VK_PRIOR]) {
//			m_fz -= 0.02f;
//		}
//		if (Keys[VK_NEXT]) {
//			m_fz += 0.02f;
//		}
//		if (Keys[VK_UP]) {
//			m_fxspeed -= 0.01f;
//			IO_TRACE("m_xspeed = %f\n", m_fxspeed);
//		}
//		if (Keys[VK_DOWN]) {
//			m_fxspeed += 0.01f;
//			IO_TRACE("m_xspeed = %f\n", m_fxspeed);
//		}
//		if (Keys[VK_RIGHT]) {
//			m_fyspeed += 0.01f;
//			IO_TRACE("m_yspeed = %f\n", m_fyspeed);
//		}
//		if (Keys[VK_LEFT]) {
//			m_fyspeed -= 0.01f;
//			IO_TRACE("m_yspeed = %f\n", m_fyspeed);
//		}
//	};
//};





