#pragma once

struct	SParticle				//  Structure For Particle
{
	bool		active = true;					// Active (Yes/No)
	float		life;					// Particle Life
	float		fade;					// Fade Speed
	//
	float		r;						// Red Value
	float		g;						// Green Value
	float		b;						// Blue Value
	//
	float		m_fx = 0.0f;						// X Position
	float		m_fy = 0.0f;						// Y Position
	float		m_fz = 0.0f;						// Z Position
	//
	float		xg;						// X Gravity
	float		yg;						// Y Gravity
	float		zg;						// Z Gravity
	//
	float		m_fMass = 0.0f;
	float		m_fVx = 0.0f;
	float		m_fVy = 0.0f;
	float		m_fVz = 0.0f;
	//
	float		m_fDrawSize = 0.5f;
};

struct CMass
{
	float		m_fX = 0.0f;
	float		m_fY = 0.0f;
	float		m_fZ = 0.0f;
	float		m_fMass = 1.0f;
	CMass() = default;
	CMass(float X, float Y, float Z, float Mass) : m_fX(X),  m_fY(Y),  m_fZ(Z),  m_fMass(Mass)  {};
};

class CModel
{
protected :
	float		m_fXspeed = 0.0f;					// Base X Speed (To Allow Keyboard Direction Of Tail)
	float		m_fYspeed = 0.0f;					// Base Y Speed (To Allow Keyboard Direction Of Tail)
	float		m_fZspeed = 0.0f;					// Base Y Speed (To Allow Keyboard Direction Of Tail)
public:
	CModel() {};
	virtual void AddMass(float X, float Y, float Z, float Mass) = 0;
	virtual void InitParticle(SParticle& Particle, float Vx, float Vy, float Vz ) = 0;    // задается начальное направление движения
	virtual void CaclParticle(SParticle& Particle, double Dt) = 0;
	virtual GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys);   // управление потоком частиц
};

class CModelGravitation : public CModel
{
	float						m_GravityConst = 1.0f;
	size_t						m_CountOfMass = 0;
	std::vector< CMass >		m_vMasses;
	bool						m_bADD = false;		// Increase m_GravityConst
	bool						m_bSUBTRACT = false;
public :
	CModelGravitation() {};
	virtual void AddMass( float X, float Y, float Z, float Mass  ){
		m_vMasses.emplace_back(CMass(X, Y, Z, Mass));
	}
	virtual void InitParticle(SParticle& Particle, float Vx, float Vy, float Vz);
	virtual void CaclParticle(SParticle& Particle, double Dt );
	virtual GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys);
};

class CModelStream : public CModel
{
	const GLfloat m_Colors[12][3] =		// Rainbow Of Colors
	{
		{ 1.0f,0.5f,0.5f },{ 1.0f,0.75f,0.5f },{ 1.0f,1.0f,0.5f },{ 0.75f,1.0f,0.5f },
		{ 0.5f,1.0f,0.5f },{ 0.5f,1.0f,0.75f },{ 0.5f,1.0f,1.0f },{ 0.5f,0.75f,1.0f },
		{ 0.5f,0.5f,1.0f },{ 0.75f,0.5f,1.0f },{ 1.0f,0.5f,1.0f },{ 1.0f,0.5f,0.75f }
	};
	bool		m_bRainbow = true;					// Rainbow Mode?
	bool		m_bSp = false;						// Spacebar Pressed?
	bool		m_bRp = false;						// Enter Key Pressed?

	float		m_fSlowdown = 2.0f;					// Slow Down Particles
	//float		m_fZoom = -40.0f;					// Used To Zoom Out
	GLuint		m_iCol = 0;							// Current Color Selection
	GLuint		m_IDelay = 0;						// Rainbow Effect Delay
	//
	bool		m_bNUMPAD8 = false;		// If Number Pad 8 And Y Gravity Is Less Than 1.5 Increase Pull Upwards
	bool		m_bNUMPAD2 = false;		// If Number Pad 2 And Y Gravity Is Greater Than -1.5 Increase Pull Downwards
	bool		m_bNUMPAD6 = false;		// If Number Pad 6 And X Gravity Is Less Than 1.5 Increase Pull Right
	bool		m_bNUMPAD4 = false;		// If Number Pad 4 And X Gravity Is Greater Than -1.5 Increase Pull Left
	bool		m_bTAB = false;			// Tab Key Causes A Burst
public:
	CModelStream() {};
	virtual void AddMass(float X, float Y, float Z, float Mass) {};
	virtual void InitParticle(SParticle& Particle, float Vx, float Vy, float Vz);
	virtual void CaclParticle(SParticle& Particle, double Dt);
	virtual GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys);
};

class CParticlesManager
{
	const GLfloat m_Colors[12][3] =		// Rainbow Of Colors
	{
		{ 1.0f,0.5f,0.5f },{ 1.0f,0.75f,0.5f },{ 1.0f,1.0f,0.5f },{ 0.75f,1.0f,0.5f },
		{ 0.5f,1.0f,0.5f },{ 0.5f,1.0f,0.75f },{ 0.5f,1.0f,1.0f },{ 0.5f,0.75f,1.0f },
		{ 0.5f,0.5f,1.0f },{ 0.75f,0.5f,1.0f },{ 1.0f,0.5f,1.0f },{ 1.0f,0.5f,0.75f }
	};
	//    временные
	bool		rainbow = true;					// Rainbow Mode?
	bool		sp = false;						// Spacebar Pressed?
	bool		rp = false;						// Enter Key Pressed?

	float		slowdown = 2.0f;				// Slow Down Particles
	float		xspeed = 0.0f;					// Base X Speed (To Allow Keyboard Direction Of Tail)
	float		yspeed = 0.0f;					// Base Y Speed (To Allow Keyboard Direction Of Tail)
	float		zspeed = 0.0f;					// Base Y Speed (To Allow Keyboard Direction Of Tail)
	float		m_fZoom = -20.0f;				// Used To Zoom Out
	GLuint		col = 0;						// Current Color Selection
	GLuint		delay = 0;						// Rainbow Effect Delay
	//
	CGLTexture		m_Texture;
	SYSTEMTIME		m_LastDwayTime;
protected:
	CModel*			m_Model = nullptr;
	std::vector< SParticle >		m_vParticles;
public:
	CParticlesManager(size_t Count) : m_vParticles(Count) { };
	void SetModel(CModel*	Model) {
		m_Model = Model;
	}
	bool CreateTexture(const std::string& ImageName);
	GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys);
	GLvoid	InitPaticles( float Vx, float Vy, float Vz );
	GLvoid	AddPaticles( const SParticle& Pat );
	int DrawGLScene();
};
