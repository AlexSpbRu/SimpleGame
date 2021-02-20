#include "stdafx.h"

#include "Textures.h"
#include "Particles.h" 


bool CParticlesManager::CreateTexture(const std::string& ImageName)
{
	if (!m_Texture.CreateTexture(ImageName, GL_LINEAR, GL_LINEAR, 1))
		return false;
	m_Texture.Bind();
	return true;
}

GLvoid	CParticlesManager::ProcessKeyboard(const bool* Keys, size_t CountKeys)
{
	if (m_Model != nullptr)
		m_Model->ProcessKeyboard( Keys,  CountKeys );
	//
	if (Keys[VK_PRIOR])	m_fZoom += 0.1f;		// Zoom In
	if (Keys[VK_NEXT])	m_fZoom -= 0.1f;		// Zoom Out

}

GLvoid	CParticlesManager::InitPaticles(float Vx, float Vy, float Vz)
{
	if (m_Model == nullptr) return;
	auto num = 0;
	size_t count = m_vParticles.size();
	for (auto& pat : m_vParticles)				
	{
		m_Model->InitParticle( pat, Vx, Vy, Vz );
	}
	GetSystemTime(&m_LastDwayTime);
}

GLvoid	CParticlesManager::AddPaticles(const SParticle& Pat)
{
	m_vParticles.push_back(Pat);
}

int CParticlesManager::DrawGLScene()										// Here's Where We Do All The Drawing
{
	SYSTEMTIME	sys_time;
	GetSystemTime(&sys_time);
	double Dt = (sys_time.wHour - m_LastDwayTime.wHour)*3600.0 + (sys_time.wMinute - m_LastDwayTime.wMinute)*60.0 +
		sys_time.wSecond - m_LastDwayTime.wSecond + (sys_time.wMilliseconds - m_LastDwayTime.wMilliseconds) / 1000.0;
	m_LastDwayTime = sys_time;
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
	glLoadIdentity();										// Reset The ModelView Matrix

	for (auto& pat : m_vParticles)
	{
		//if (pat.active)								// If The Particle Is Active
		{
			float x = pat.m_fx;						// Grab Our Particle X Position
			float y = pat.m_fy;						// Grab Our Particle Y Position
			float z = pat.m_fz + m_fZoom;						// Used To Zoom Out;					// Particle Z Pos + Zoom
																// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glColor4f(pat.r, pat.g, pat.b, pat.life);

			glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
			glTexCoord2d(1, 1); glVertex3f(x + pat.m_fDrawSize, y + pat.m_fDrawSize, z); // Top Right
			glTexCoord2d(0, 1); glVertex3f(x - pat.m_fDrawSize, y + pat.m_fDrawSize, z); // Top Left
			glTexCoord2d(1, 0); glVertex3f(x + pat.m_fDrawSize, y - pat.m_fDrawSize, z); // Bottom Right
			glTexCoord2d(0, 0); glVertex3f(x - pat.m_fDrawSize, y - pat.m_fDrawSize, z); // Bottom Left
			glEnd();												// Done Building Triangle Strip

			if( m_Model != nullptr )
				m_Model->CaclParticle( pat, Dt);
		}
	}
	return TRUE;											// Everything Went OK
}


GLvoid	CModel::ProcessKeyboard(const bool* Keys, size_t CountKeys)   // управление потоком частиц
{

	// If Up Arrow And Y Speed Is Less Than 200 Increase Upward Speed
	if (Keys[VK_UP] && (m_fYspeed<200)) m_fYspeed += 1.0f;
	// If Down Arrow And Y Speed Is Greater Than -200 Increase Downward Speed
	if (Keys[VK_DOWN] && (m_fYspeed>-200)) m_fYspeed -= 1.0f;
	// If Right Arrow And X Speed Is Less Than 200 Increase Speed To The Right
	if (Keys[VK_RIGHT] && (m_fXspeed<200)) m_fXspeed += 1.0f;
	// If Left Arrow And X Speed Is Greater Than -200 Increase Speed To The Left
	if (Keys[VK_LEFT] && (m_fXspeed>-200)) m_fXspeed -= 1.0f;
}

void CModelGravitation::InitParticle(SParticle& Particle, float Vx, float Vy, float Vz)
{
	Particle.active = true;									// Make All The Particles Active
	Particle.life = 1.0f;									// Give All The Particles Full Life
	Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
	Particle.r = float(rand() % 50) / 50.0f;				// Select Red Rainbow Color
	Particle.g = float(rand() % 50) / 50.0f;				// Select Green Rainbow Color
	Particle.b = float(rand() % 50) / 50.0f;				// Select Blue Rainbow Color
	Particle.m_fVx = Vx + float((rand() % 10) - 6.0f)/5.0f;		// Random Speed On X Axis
	Particle.m_fVy = Vy + float((rand() % 10) - 5.0f)/5.0f;		// Random Speed On Y Axis
	Particle.m_fVz = Vz;// +float((rand() % 10) - 5.0f)*5.0f;		// Random Speed On Z Axis
	Particle.xg = 0.0f;										// Set Horizontal Pull To Zero
	Particle.yg = -0.8f;										// Set Vertical Pull Downward
	Particle.zg = 0.0f;
	Particle.m_fMass = 1.0f;
	Particle.m_fx = 0.0f;
	Particle.m_fy = 0.0f;
	Particle.m_fz = 0.0f;
}

void CModelGravitation::CaclParticle(SParticle& Particle, double Dt)
{
	if (!Particle.active) 
		return;
	float ax = 0.0f;
	float ay = 0.0f;
	float az = 0.0f;
	float minDist = 1000000.0f;
	for (auto& mass : m_vMasses) {
		float dx = Particle.m_fx - mass.m_fX;
		float dy = Particle.m_fy - mass.m_fY;
		float dz = Particle.m_fz - mass.m_fZ;
		float dist = sqrtf(dx*dx + dy*dy + dz*dz );
		float Xang = -dx / (dist+0.00001);
		float Yang = -dy / (dist + 0.00001);
		float Zang = -dz / (dist + 0.00001);
		float F = mass.m_fMass*m_GravityConst / (Particle.m_fMass*dist*dist);
		ax += Xang*F;
		ay += Yang*F;
		az += Zang*F;
		minDist = min( minDist, dist);
	}
	Particle.life = minDist;
	Particle.m_fVx += ax*Dt;
	Particle.m_fVy += ay*Dt;
	Particle.m_fVz += az*Dt;
	Particle.m_fx += Particle.m_fVx*Dt;
	Particle.m_fy += Particle.m_fVy*Dt;
	Particle.m_fz += Particle.m_fVz*Dt;
	
	//
	if (Particle.life > 100.0f || Particle.life < 0.1f) { //  частица улетела или сгорела
		InitParticle( Particle, m_fXspeed, m_fYspeed, 0.0f );
	}
}

GLvoid	CModelGravitation::ProcessKeyboard(const bool* Keys, size_t CountKeys)   // управление потоком частиц
{

	// If Up Arrow And Y Speed Is Less Than 200 Increase Upward Speed
	if (Keys[VK_UP] && (m_fYspeed<200)) m_fYspeed += 0.1f;
	// If Down Arrow And Y Speed Is Greater Than -200 Increase Downward Speed
	if (Keys[VK_DOWN] && (m_fYspeed>-200)) m_fYspeed -= 0.1f;
	// If Right Arrow And X Speed Is Less Than 200 Increase Speed To The Right
	if (Keys[VK_RIGHT] && (m_fXspeed<200)) m_fXspeed += 0.1f;
	// If Left Arrow And X Speed Is Greater Than -200 Increase Speed To The Left
	if (Keys[VK_LEFT] && (m_fXspeed>-200)) m_fXspeed -= 0.1f;
	m_bADD = Keys[VK_ADD];
	m_bSUBTRACT = Keys[VK_SUBTRACT];
	//
	if (m_bADD) {
		m_GravityConst += 0.01f;
		m_GravityConst = min(10.0f, m_GravityConst);
	}
	if (m_bSUBTRACT) {
		m_GravityConst -= 0.01f;
		m_GravityConst = max(0.01f, m_GravityConst);
	}
}


GLvoid	CModelStream::ProcessKeyboard(const bool* Keys, size_t CountKeys)
{
	if (Keys[VK_ADD] && (m_fSlowdown>1.0f)) m_fSlowdown -= 0.01f;		// Speed Up Particles
	if (Keys[VK_SUBTRACT] && (m_fSlowdown<4.0f)) m_fSlowdown += 0.01f;	// Slow Down Particles

	if (Keys[VK_RETURN] && !m_bRp)			// Return Key Pressed
	{
		m_bRp = true;						// Set Flag Telling Us It's Pressed
		m_bRainbow = !m_bRainbow;				// Toggle Rainbow Mode On / Off
	}
	if (!Keys[VK_RETURN]) m_bRp = false;		// If Return Is Released Clear Flag

	if ((Keys[' '] && !m_bSp) || (m_bRainbow && (m_IDelay  >25)))	// Space Or Rainbow Mode
	{
		if (Keys[' '])	m_bRainbow = false;	// If Spacebar Is Pressed Disable Rainbow Mode
		m_bSp = true;						// Set Flag Telling Us Space Is Pressed
		m_IDelay = 0;						// Reset The Rainbow Color Cycling Delay
		m_iCol++;							// Change The Particle Color
		if (m_iCol>11)	m_iCol = 0;				// If Color Is To High Reset It
	}
	if (!Keys[' '])	m_bSp = false;			// If Spacebar Is Released Clear Flag

	m_bNUMPAD8 = Keys[VK_NUMPAD8];
	m_bNUMPAD2 = Keys[VK_NUMPAD2];
	m_bNUMPAD6 = Keys[VK_NUMPAD6];
	m_bNUMPAD4 = Keys[VK_NUMPAD4];
	m_bTAB = Keys[VK_TAB];

	m_IDelay++;							// Increase Rainbow Mode Color Cycling Delay Counter
	CModel::ProcessKeyboard(Keys, CountKeys);
}


void CModelStream::InitParticle(SParticle& Particle, float Vx, float Vy, float Vz)
{
	static auto num = 0;
	Particle.active = true;									// Make All The Particles Active
	Particle.life = 1.0f;									// Give All The Particles Full Life
	Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
	Particle.r = m_Colors[num % 12][0];				// Select Red Rainbow Color
	Particle.g = m_Colors[num % 12][1];				// Select Green Rainbow Color
	Particle.b = m_Colors[num % 12][2];				// Select Blue Rainbow Color
	Particle.m_fVx = float((rand() % 50) - 26.0f)*10.0f;		// Random Speed On X Axis
	Particle.m_fVy = float((rand() % 50) - 25.0f)*10.0f;		// Random Speed On Y Axis
	Particle.m_fVz = float((rand() % 50) - 25.0f)*10.0f;		// Random Speed On Z Axis
	Particle.xg = 0.0f;										// Set Horizontal Pull To Zero
	Particle.yg = -0.8f;										// Set Vertical Pull Downward
	Particle.zg = 0.0f;										// Set Pull On Z Axis To Zero
	num++;

}


void CModelStream::CaclParticle(SParticle& Particle, double Dt)
{
	if (Particle.active)							// If The Particle Is Active
	{
		Particle.m_fx += Particle.m_fVx / (m_fSlowdown * 1000);// Move On The X Axis By X Speed
		Particle.m_fy += Particle.m_fVy / (m_fSlowdown * 1000);// Move On The Y Axis By Y Speed
		Particle.m_fz += Particle.m_fVz / (m_fSlowdown * 1000);// Move On The Z Axis By Z Speed

		Particle.m_fVx += Particle.xg;			// Take Pull On X Axis Into Account
		Particle.m_fVy += Particle.yg;			// Take Pull On Y Axis Into Account
		Particle.m_fVz += Particle.zg;			// Take Pull On Z Axis Into Account
		Particle.life -= Particle.fade;		// Reduce Particles Life By 'Fade'

		if (Particle.life < 0.0f)					// If Particle Is Burned Out
		{
			Particle.life = 1.0f;					// Give It New Life
			Particle.fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value
			Particle.m_fx = 0.0f;						// Center On X Axis
			Particle.m_fy = 0.0f;						// Center On Y Axis
			Particle.m_fz = 0.0f;						// Center On Z Axis
			Particle.m_fVx = m_fXspeed + float((rand() % 60) - 32.0f);	// X Axis Speed And Direction
			Particle.m_fVy = m_fYspeed + float((rand() % 60) - 30.0f);	// Y Axis Speed And Direction
			Particle.m_fVz = float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			Particle.r = m_Colors[m_iCol][0];			// Select Red From Color Table
			Particle.g = m_Colors[m_iCol][1];			// Select Green From Color Table
			Particle.b = m_Colors[m_iCol][2];			// Select Blue From Color Table
		}
		// If Number Pad 8 And Y Gravity Is Less Than 1.5 Increase Pull Upwards
		if (m_bNUMPAD8 && (Particle.yg < 1.5f )) Particle.yg += 0.01f;
		// If Number Pad 2 And Y Gravity Is Greater Than -1.5 Increase Pull Downwards
		if (m_bNUMPAD2 && (Particle.yg > -1.5f)) Particle.yg -= 0.01f;
		// If Number Pad 6 And X Gravity Is Less Than 1.5 Increase Pull Right
		if (m_bNUMPAD6 && (Particle.xg < 1.5f)) Particle.xg += 0.01f;
		// If Number Pad 4 And X Gravity Is Greater Than -1.5 Increase Pull Left
		if (m_bNUMPAD4 && (Particle.xg > -1.5f)) Particle.xg -= 0.01f;

		if (m_bTAB)								// Tab Key Causes A Burst
		{
			Particle.m_fx = 0.0f;								// Center On X Axis
			Particle.m_fy = 0.0f;								// Center On Y Axis
			Particle.m_fz = 0.0f;								// Center On Z Axis
			Particle.m_fVx = float((rand() % 50) - 26.0f)*10.0f;	// Random Speed On X Axis
			Particle.m_fVy = float((rand() % 50) - 25.0f)*10.0f;	// Random Speed On Y Axis
			Particle.m_fVz = float((rand() % 50) - 25.0f)*10.0f;	// Random Speed On Z Axis
		}
	}
}


