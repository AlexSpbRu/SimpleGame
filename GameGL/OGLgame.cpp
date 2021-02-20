#include "stdafx.h"

#include <ctime>
#include <chrono>
//#include <windows.h>
#include <Mmsystem.h>
//#include <timeAPI.h>
#ifdef OPENGLES20
#pragma comment ( lib, "libGLESv2.lib" )
#endif
//#include "Math3D.h"
#include "Matrix.h"
#include "Shader.h"
#include "OGLBuffers.h"

#include "utils.h"

#include "Textures.h"
#include "FontImage.h"
#include "Particles.h"



#include "OGLControl.h"
#include "OGLgame.h"





struct	 STimer		 											
{
	__int64				frequency;									// Timer Frequency
	double				resolution;									// Timer Resolution
//	unsigned long		mm_timer_start;								// Multimedia Timer Start Value
//	unsigned long		mm_timer_elapsed;							// Multimedia Timer Elapsed Time
//	bool				performance_timer;							// Using The Performance Timer?
	__int64				performance_timer_start;					// Performance Timer Start Value
	__int64				performance_timer_elapsed;					// Performance Timer Elapsed Time
};

STimer	gameTimer;

void TimerInit(void)										// Initialize Our Timer (Get It Ready)
{
	memset(&gameTimer, 0, sizeof(gameTimer));						// Clear Our Timer Structure

															// Check To See If A Performance Counter Is Available
															// If One Is Available The Timer Frequency Will Be Updated
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&gameTimer.frequency))
	{		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *)&gameTimer.performance_timer_start);
		// Calculate The Timer Resolution Using The Timer Frequency
		gameTimer.resolution = (float)(((double)1.0f) / ((double)gameTimer.frequency));
		// Set The Elapsed Time To The Current Time
		gameTimer.performance_timer_elapsed = gameTimer.performance_timer_start;
	}
}

double TimerGetTime()										// Get Time In Milliseconds
{
	__int64 time;											// time Will Hold A 64 Bit Integer

	QueryPerformanceCounter((LARGE_INTEGER *)&time);	// Grab The Current Performance Time
														// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
	return ((time - gameTimer.performance_timer_start) * gameTimer.resolution)*1000.0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

SFontSerial fonts[] = {
#include "alphabet_free_png.h"
#include "digits_font.h"
};

constexpr auto FontsCount = sizeof(fonts) / sizeof(SFontSerial);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CPlaySound :: play_sound(const std::string& FileName, EPlayType  Type) {
	unsigned int soundFlag = SND_SYNC;
	if (Type == EPlayType::Loop)
		soundFlag |= SND_LOOP;
	PlaySound( FileName.c_str(), NULL, soundFlag );
}


int CPlaySound :: playSound(const std::string& FileName, EPlayType  Type) {
	//auto fut = std::async(std::launch::async, &CPlaySound::play_sound, this, FileName, Type);
	std::thread th(&CPlaySound::play_sound, this, FileName, Type);
	th.detach();
	return 0;
}

void CPlaySound::stopPlay(int Id) {
	PlaySound(NULL, NULL, 0);
}

///----------------------------------------------------------------------------------

//bool	CGamePlayer :: m_bKeyRIGHT = false;
//bool	CGamePlayer :: m_bKeyLEFT = false;
//bool	CGamePlayer :: m_bKeyDOWN = false;
//bool	CGamePlayer :: m_bKeyUP = false;
//bool	CGamePlayer :: m_bMouseRIGHT = false;
//bool	CGamePlayer :: m_bMouseLEFT = false;
//bool	CGamePlayer :: m_bMouseDOWN = false;
//bool	CGamePlayer :: m_bMouseUP = false;


GLvoid	CGamePlayer :: ProcessKeyboard(const bool* Keys, size_t CountKeys)
{
	m_bKeyRIGHT = Keys[VK_RIGHT];
	m_bKeyLEFT = Keys[VK_LEFT];
	m_bKeyDOWN = Keys[VK_DOWN];
	m_bKeyUP = Keys[VK_UP];

}


GLvoid	CGamePlayer::Move() noexcept
{
	if (m_pGame == nullptr) __debugbreak();

	if ( ( m_bKeyRIGHT || m_bMouseRIGHT ) && m_iX < m_pGame->m_iHorSize - 1  && m_fFy == m_iY * m_pGame->m_SellHeight &&
		  (m_fFx == m_iX * m_pGame->m_SellWidth || m_eDirection == EDirection::Left)) {
		m_pGame->ActivatePrize(m_iX, m_iY, true);
		m_iX++;										// Move The Player Right
		m_eDirection = EDirection::Right;
	}
	if ( ( m_bKeyLEFT || m_bMouseLEFT )  && m_iX > 0 && ( m_fFx == m_iX * m_pGame->m_SellWidth  || m_eDirection == EDirection::Right )&&
							m_fFy == m_iY * m_pGame->m_SellHeight) {
		m_iX--;									// Move The Player Left
		m_pGame->ActivatePrize(m_iX, m_iY, true);
		m_eDirection = EDirection::Left;
	}
	if ( ( m_bKeyDOWN || m_bMouseDOWN )  && m_iY < m_pGame->m_iVertSize - 1 && m_fFx == m_iX * m_pGame->m_SellWidth &&
			( m_fFy == m_iY * m_pGame->m_SellHeight || m_eDirection == EDirection::Top) ) {
		m_pGame->ActivatePrize(m_iX, m_iY, false);
		m_iY++;											// Move The Player Down
		m_eDirection = EDirection::Bottom;
	}
	if ( ( m_bKeyUP || m_bMouseUP )  && m_iY > 0 && m_fFx == m_iX * m_pGame->m_SellWidth && 
				( m_fFy == m_iY * m_pGame->m_SellHeight || m_eDirection == EDirection::Bottom ) ) {
		m_iY--;										// Move The Player Up
		m_pGame->ActivatePrize(m_iX, m_iY, false);
		m_eDirection = EDirection::Top;
	}


	if (m_fFx < m_iX * m_pGame->m_SellWidth) {						// Is Fine Position On X Axis Lower Than Intended Position?
		m_fFx += m_pGame->m_iSteps[m_pGame->m_iSpeed];				// If So, Increase The Fine X Position
		m_fFx = min(m_fFx, int(m_iX * m_pGame->m_SellWidth));
	}

	if (m_fFx > m_iX * m_pGame->m_SellWidth) {						// Is Fine Position On X Axis Greater Than Intended Position?
		m_fFx -= m_pGame->m_iSteps[m_pGame->m_iSpeed];				// If So, Decrease The Fine X Position
		m_fFx = max(m_fFx, int(m_iX * m_pGame->m_SellWidth));
	}

	if (m_fFy < m_iY * m_pGame->m_SellHeight) {				// Is Fine Position On Y Axis Lower Than Intended Position?
		m_fFy += m_pGame->m_iSteps[m_pGame->m_iSpeed];		// If So, Increase The Fine Y Position
		m_fFy = min(m_fFy, int(m_iY * m_pGame->m_SellHeight));
	}

	if (m_fFy > m_iY * m_pGame->m_SellHeight) {				// Is Fine Position On Y Axis Lower Than Intended Position?
		m_fFy -= m_pGame->m_iSteps[m_pGame->m_iSpeed];		// If So, Decrease The Fine Y Position
		m_fFy = max(m_fFy, int(m_iY * m_pGame->m_SellHeight));
	}
}

void CGamePlayer::ResetObject(bool NewLvel) noexcept
{
	m_iX = 0;											// Reset Player X Position To Far Left Of The Screen
	m_iY = 0;											// Reset Player Y Position To The Top Of The Screen
	m_fFx = 0;											// Set Fine X Position To Match
	m_fFy = 0;											// Set Fine Y Position To Match
	m_fSpin = 0.0f;
}



CGamePlayerImage::~CGamePlayerImage()
{
	
}


#ifndef OPENGL_SHADER
//  новое рисование через буферы

void	CGamePlayerImage::Draw(CTextVertexColorBuffer& Buffer) const noexcept
{
	CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Player)->Bind();
	GLfloat verOut[m_iVertAmount * 3];
	CMatrix::setIdentityM(m_pGame->m_ModelMatrix);
	CMatrix::translateM(m_pGame->m_ModelMatrix, m_fFx + m_pGame->m_fGameFieldX, m_fFy + m_pGame->m_fGameFieldY, -3.0f);
	auto spin = 0.0f;
	switch (m_eDirection) {
	case  EDirection::None:
	case  EDirection::Right:
	case  EDirection::Left: spin = 0.0f;
		break;
	case  EDirection::Top: spin = -90.0f;
		break;
	case  EDirection::Bottom: spin = 90.0f;
		break;
	}
	CMatrix::setRotateM(m_pGame->m_ModelMatrix, spin, 0.0f, 0.0f, 1.0f);
	CMatrix::scaleM(m_pGame->m_ModelMatrix, 2.0f, 2.0f, 1.0f);
	CMatrix::multiplyMM(m_pGame->m_ResultMatrix, m_pGame->m_ModelMatrix, m_pGame->m_ProjMatrix);

	for (auto i = 0; i < m_iVertAmount; ++i)
		glhVec3TransformCoord(verOut + i * Buffer.VertexDimension, m_fVertex + i * Buffer.VertexDimension, &m_pGame->m_ResultMatrix);
	Buffer.AddVertex(verOut, verOut + m_iVertAmount * Buffer.VertexDimension);

	Buffer.AddTexture(m_fTexCoord, m_fTexCoord + m_iVertAmount * Buffer.TextureDimension);
	Buffer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

#else

void	CGamePlayerImage::DrawES(CTextVertexColorBuffer& Buffer) const noexcept
{

	auto spin = 0.0f;
	switch (m_eDirection) {
	case  EDirection::None:
	case  EDirection::Right:
	case  EDirection::Left: spin = 180.0f;
		break;
	case  EDirection::Top: spin = -90.0f;
		break;
	case  EDirection::Bottom: spin = 90.0f;
		break;
	}

	Buffer.AffineTransformation2D( Vec3{ GLfloat(m_fFx + m_pGame->m_fGameFieldX), m_pGame->m_fMaxHeight - GLfloat(m_fFy + m_pGame->m_fGameFieldY), -0.4f },
		spin, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ m_pGame->GameObjectSize(),m_pGame->GameObjectSize(), 1.0f} );
	Buffer.AddVertices(m_fVertex, m_iVertAmount*3, m_fTexCoord, m_iVertAmount*2, { 1.0f, 1.0f, 1.0f, 1.0f } );
}

#endif


////////////////////////////////////////////////////////////////////////////////////////////

COGLGame*					CEnemyModel::m_pGame = nullptr;
CEnemyModel::MapModels		CEnemyModel::m_MapModels = { 
	{EModelType::Simple, nullptr }, 
	{EModelType::Physical, nullptr} 
};


void		CEnemyModel::Init(COGLGame* Game) noexcept
{
	m_pGame = Game;
}

CEnemyModel*	CEnemyModel::GetModel( EModelType  Type) noexcept
{
	auto model = m_MapModels.find(Type);
	if (model == m_MapModels.end())
		return nullptr;
	if ((*model).second == nullptr) {
		switch (Type) {
		case EModelType::Simple:
			(*model).second = new CEnemyModel();
			break;
		case EModelType::Physical: 
			(*model).second = new CEnemyModelPhysical();
			break;
		}
	}
	return (*model).second;
}


EDirection	CEnemyModel::CalcDirection(int& X, int& Y, GLfloat Fx, GLfloat Fy) noexcept
{
	EDirection  ret = EDirection::None;
	bool onX = Fy == Y*m_pGame->m_SellHeight;
	
	if (X < m_pGame->m_player.m_iX && onX) {// Move The Enemy Right
		ret = EDirection::Right;
		X++;
		X = min(X, m_pGame->m_iHorSize);
	}
	if (X > m_pGame->m_player.m_iX && onX) {// Move The Enemy Left
		ret = EDirection::Left;
		X--;
		X = max(X, 0);
	}
	bool onY = Fx == X*m_pGame->m_SellWidth;
	if (Y < m_pGame->m_player.m_iY && onY) {// Move The Enemy Down
		ret = EDirection::Bottom;
		Y++;
		Y = min(Y, m_pGame->m_iVertSize);
	}
	if (Y > m_pGame->m_player.m_iY && onY) {// Move The Enemy Up
		ret = EDirection::Top;
		Y--;
		Y = max(Y, 0);
	}
	return ret;
}


EDirection	CEnemyModelPhysical::CalcDirection(int& X, int& Y, GLfloat Fx, GLfloat Fy) noexcept
{
	EDirection  ret = EDirection::None;
	//  вычисляем вектор направления движения
	auto xDir = m_pGame->m_player.m_fFx - Fx;
	auto yDir = m_pGame->m_player.m_fFy - Fy;
	for (auto& enemy : m_pGame->m_vEnemy) { 
		if (enemy.m_fFx != Fx || enemy.m_fFy != Fy) {   //  сам пропускается
			auto dFx = Fx - enemy.m_fFx;
			auto dFy = Fy - enemy.m_fFy;
			auto R = dFx * dFx + dFy * dFy;
			xDir += dFx * 15000 / R;
			
			yDir += dFy * 15000 / R;
		}
	}
	//
	bool onX = Fy == Y*m_pGame->m_SellHeight;
	bool onY = Fx == X*m_pGame->m_SellWidth;
	if ((onX && !onY) || ((onX && onY) && (abs(xDir) > abs(yDir)))) {
		if (xDir > 0 &&  X < m_pGame->m_player.m_iX ) {// Move The Enemy Right
			ret = EDirection::Right;
			X++;
			X = min(X, m_pGame->m_iHorSize);
		}
		else 	if (xDir < 0 && X > m_pGame->m_player.m_iX) {// Move The Enemy Left
			ret = EDirection::Left;
			X--;
			X = max(X, 0);
		}
	}
	//
	if ((!onX && onY) || ((onX && onY) && (abs(yDir) > abs(xDir)))) {
		if (yDir > 0 && Y < m_pGame->m_player.m_iY) {// Move The Enemy Down
			ret = EDirection::Bottom;
			Y++;
			Y = min(Y, m_pGame->m_iVertSize);
		}
		else	if (yDir < 0 && Y > m_pGame->m_player.m_iY) {// Move The Enemy Up
			ret = EDirection::Top;
			Y--;
			Y = max(Y, 0);
		}
	}
	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef OPENGL_SHADER
//  новое рисование через буферы

void	CGameEnemy::Draw(CTextVertexColorBuffer& Buffer) const noexcept
{
	Check();
	if (m_State == State::Dead)
		return;
	CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Enemy)->Bind();

	GLfloat verOut[m_iVertAmount * 3];
	CMatrix::setIdentityM(m_pGame->m_ModelMatrix);
	CMatrix::translateM(m_pGame->m_ModelMatrix, m_fFx + m_pGame->m_fGameFieldX, m_fFy + m_pGame->m_fGameFieldY, -3.0f);
	auto spin = 0.0f;
	switch (m_eDirection) {
		case  EDirection::None:
		case  EDirection::Right:
		case  EDirection::Left: spin = 0.0f;
			break;
		case  EDirection::Top: spin = -90.0f;
			break;
		case  EDirection::Bottom: spin = 90.0f;
			break;
	}
	CMatrix::setRotateM(m_pGame->m_ModelMatrix, spin, 0.0f, 0.0f, 1.0f);
	CMatrix::scaleM(m_pGame->m_ModelMatrix, 2.0f, 2.0f, 1.0f);

	for (auto i = 0; i < m_iVertAmount; ++i)
		glhVec3TransformCoord(verOut + i * Buffer.VertexDimension, m_fVertex + i * Buffer.VertexDimension, &m_pGame->m_ModelMatrix);
	Buffer.AddVertex(verOut, verOut + m_iVertAmount * Buffer.VertexDimension);
	
	Buffer.AddTexture(m_fTexCoord, m_fTexCoord + m_iVertAmount * Buffer.TextureDimension);
	Buffer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	
}

#else
//   Рисование шейдерами
void	CGameEnemy::DrawES( CTextVertexColorBuffer& Buffer ) const noexcept
{
	Check();
	if (m_State == State::Dead)
		return;
	auto spin = 0.0f;
	switch (m_eDirection) {
	case  EDirection::None:
	case  EDirection::Right:
	case  EDirection::Left: spin = 180.0f;
		break;
	case  EDirection::Top: spin = -90.0f;
		break;
	case  EDirection::Bottom: spin = 90.0f;
		break;
	}

	Buffer.AffineTransformation2D(Vec3{ GLfloat(m_fFx + m_pGame->m_fGameFieldX), m_pGame->m_fMaxHeight - GLfloat(m_fFy + m_pGame->m_fGameFieldY), -0.4f },
		spin, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ m_pGame->GameObjectSize(), m_pGame->GameObjectSize(), 1.0f });
	Buffer.AddVerticesAndTransform(m_fVertex, m_iVertAmount * 3, m_fTexCoord, m_iVertAmount * 2, { 1.0f, 1.0f, 1.0f, 1.0f });
	Buffer.LoadIdentity();
}

#endif

GLvoid	CGameEnemy::Move() noexcept
{
	Check();
	if ( m_State == State::Dead )  
		return;
	int Xlast = m_fFx / m_pGame->m_SellWidth, Ylast = m_fFy / m_pGame->m_SellHeight;
//	EMoveDirection  lastDirection = m_eDirection;
	m_eDirection = m_pModel->CalcDirection(m_iX, m_iY, m_fFx, m_fFy);
	//
	GLfloat maxfDeceleration = 1.0f;
	for (auto& hg : m_pGame->m_Prizes)
		if (hg->m_eType == CGamePrize::EType::Hourglass && hg->m_eActive == CGamePrize::EState::Start) {
			maxfDeceleration += dynamic_cast<CGameHourglass*>(hg)->m_fDeceleration;
		}
	//
	if (m_fFx < m_iX * m_pGame->m_SellWidth)	{// Is Fine Position On X Axis Lower Than Intended Position?
		m_fFx += m_pGame->m_iSteps[m_iSpeed]/ maxfDeceleration;	// If So, Increase Fine Position On X Axis
		m_fFx = min(m_fFx, int(m_iX * m_pGame->m_SellWidth));
		//m_fSpin += m_pGame->m_iSteps[m_iSpeed];	// Spin Enemy Clockwise
	}
	if (m_fFx > m_iX * m_pGame->m_SellWidth)	{// Is Fine Position On X Axis Higher Than Intended Position?
		m_fFx -= m_pGame->m_iSteps[m_iSpeed] / maxfDeceleration;	// If So, Decrease Fine Position On X Axis
		m_fFx = max(m_fFx, int(m_iX * m_pGame->m_SellWidth));
		//m_fSpin -= m_pGame->m_iSteps[m_iSpeed];	// Spin Enemy Counter Clockwise
	}
	if (m_fFy < m_iY * m_pGame->m_SellHeight)	{// Is Fine Position On Y Axis Lower Than Intended Position?
		m_fFy += m_pGame->m_iSteps[m_iSpeed] / maxfDeceleration;	// If So, Increase Fine Position On Y Axis
		m_fFy = min(m_fFy, int(m_iY * m_pGame->m_SellHeight));
		//m_fSpin += m_pGame->m_iSteps[m_iSpeed];	// Spin Enemy Clockwise
	}
	if (m_fFy > m_iY * m_pGame->m_SellHeight)	{// Is Fine Position On Y Axis Higher Than Intended Position?
		m_fFy -= m_pGame->m_iSteps[m_iSpeed] / maxfDeceleration;	// If So, Decrease Fine Position On Y Axis
		m_fFy = max(m_fFy, int(m_iY * m_pGame->m_SellHeight));
		//m_fSpin -= m_pGame->m_iSteps[m_iSpeed];	// Spin Enemy Counter Clockwise
	}
	//IO_TRACE("m_iX = %d  m_iY = %d  m_fFx = %f  m_fFy = %f \n", m_iX, m_iY, m_fFx, m_fFy);
	Check();
	//
	//int X = m_fFx / m_pGame->m_SellWidth, Y = m_fFy / m_pGame->m_SellHeight;
	//if (Xlast != X || Ylast != Y) {    //  Перешли с одной линии на другую
	//	if ( (lastDirection == EMoveDirection::Left || lastDirection == EMoveDirection::Right ) && m_pGame->m_vHorLine[Xlast][Ylast] > 1)  // двигался горизонтально
	//		m_pGame->m_vHorLine[Xlast][Ylast] -= 10, IO_TRACE("m_vHorLine[%d][%d] = %d\n", Xlast, Ylast, m_pGame->m_vHorLine[Xlast][Ylast]);
	//	if ( ( lastDirection == EMoveDirection::Top || lastDirection == EMoveDirection::Bottom) && m_pGame->m_vVertLine[Xlast][Ylast] > 1 )  // двигался вертикально
	//		m_pGame->m_vVertLine[Xlast][Ylast] -= 10, IO_TRACE("m_vVertLine[%d][%d] = %d\n", Xlast, Ylast, m_pGame->m_vVertLine[Xlast][Ylast]);
	//	if (m_eDirection == EMoveDirection::Left || m_eDirection == EMoveDirection::Right)  // двигался горизонтально
	//		m_pGame->m_vHorLine[X][Y] += 10, IO_TRACE("m_vHorLine[%d][%d] = %d\n", X, Y, m_pGame->m_vHorLine[X][Y]);
	//	if (m_eDirection == EMoveDirection::Top || m_eDirection == EMoveDirection::Bottom)  // двигался вертикально
	//		m_pGame->m_vVertLine[X][Y] += 10, IO_TRACE("m_vVertLine[%d][%d] = %d\n", X, Y, m_pGame->m_vVertLine[X][Y]);
	//}
	//

}

bool	CGameEnemy::Deactivate() noexcept
{
	m_fFx = -100;	
	m_fFy = -100;
	m_iX = -1;
	m_iY = -1;
	m_State = CGameEnemy::State::Dead;
	return true;
}

void	CGameEnemy::ResetObject(bool NewLvel) noexcept
{
	Check();
	if (m_State != State::Dead || NewLvel ) {
		m_iX = 5 + rand() % (m_pGame->m_iHorSize - 5);		// Select A Random X Position
		m_iY = 3 + rand() % (m_pGame->m_iVertSize - 3);		// Select A Random Y Position
		m_fFx = m_iX * m_pGame->m_SellWidth;					// Set Fine X To Match
		m_fFy = m_iY * m_pGame->m_SellHeight;					// Set Fine Y To Match
	}
	if ( NewLvel ) {
		m_fSpin = 0.0f;
		m_iSpeed = rand() % (m_pGame->m_iSpeed - 4 + m_pGame->m_iLevel);
		m_iSpeed = min(m_iSpeed, m_pGame->m_iSpeed);
		m_iSpeed = max(m_iSpeed, 1);
		m_State = State::Active;
	}
	Check();
}


//CGamePrize::EType		CGamePrize::m_ePrevType = CGamePrize::EType::None;


#ifndef OPENGL_SHADER
//  новое рисование через буферы

void	CGamePrize::Draw(CTextVertexColorBuffer& Buffer) const noexcept
{
	if (m_eActive != EState::Activated)
		return;
	if (m_ePrevType != m_eType) {
		if (m_ePrevType != EType::None)
			Buffer.DrawArray(GL_TRIANGLES);

		if (m_eType == EType::Hourglass)
			CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Hourglass)->Bind();
		else if (m_eType == EType::Bomb)
			CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Bomb)->Bind();
		
		m_ePrevType = m_eType;
	}

	GLfloat verOut[m_iVertAmount * 3];
	CMatrix::setIdentityM(m_pGame->m_ModelMatrix);
	CMatrix::translateM(m_pGame->m_ModelMatrix, m_fFx + m_pGame->m_fGameFieldX, m_fFy + m_pGame->m_fGameFieldY, -3.0f);
	CMatrix::setRotateM(m_pGame->m_ModelMatrix, m_fSpin, 0.0f, 0.0f, 1.0f);
	CMatrix::scaleM(m_pGame->m_ModelMatrix, 2.0f, 2.0f, 1.0f);

	for (auto i = 0; i < m_iVertAmount; ++i)
		glhVec3TransformCoord(verOut + i * Buffer.VertexDimension, m_fVertex + i * Buffer.VertexDimension, &m_pGame->m_ModelMatrix);
	Buffer.AddVertex(verOut, verOut + m_iVertAmount * Buffer.VertexDimension);

	Buffer.AddTexture(m_fTexCoord, m_fTexCoord + m_iVertAmount * Buffer.TextureDimension);
	Buffer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
}

#else

//   Рисование шейдерами
void	CGamePrize::DrawES(CTextVertexColorBuffer& Buffer) const noexcept
{
	Buffer.AffineTransformation2D(Vec3{ GLfloat(m_fFx + m_pGame->m_fGameFieldX), m_pGame->m_fMaxHeight - GLfloat(m_fFy + m_pGame->m_fGameFieldY), -0.4f },
		m_fSpin, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ m_pGame->GameObjectSize(), m_pGame->GameObjectSize(), 1.0f });
	Buffer.AddVerticesAndTransform(m_fVertex, m_iVertAmount * 3, m_fTexCoord, m_iVertAmount * 2, { 1.0f, 1.0f, 1.0f, 0.75f });
	Buffer.LoadIdentity();
}

#endif

GLvoid	CGameHourglass::Move() noexcept
{
	if (m_eActive == EState::Stop || m_eActive == EState::NotActive)
		return;

	if (m_eActive == EState::Active) {	// Is The hourglass fx Variable Equal To 0 And The fy
		bool stop = false;
		while (!stop) {
			stop = true;
			m_fFx = (rand() % (m_pGame->m_iHorSize - 1) + 1)*m_pGame->m_SellWidth;	// Give The Hourglass A Random X Value
			m_fFy = (rand() % m_pGame->m_iVertSize)*m_pGame->m_SellHeight;			// Give The Hourglass A Random Y Value
			for (const auto& hg : m_pGame->m_Prizes) {
				if (hg->m_eActive == EState::Activated && m_fFx == hg->m_fFx &&  m_fFy == hg->m_fFy )
					stop = false;
			}
		}
		m_eActive = EState::Activated;
		//CPlaySound::GetInstance().playSound("Data/hourglass.wav", CPlaySound::EPlayType::Once);
		PlaySound("Data/hourglass.wav", NULL, SND_ASYNC);	// If So, Play The Hourglass Appears Sound
	}

	// If The Player Hits The Hourglass While It's Being Displayed On The Screen
	if ( abs( m_pGame->m_player.m_fFx - m_fFx ) <= m_pGame->m_iSteps[m_pGame->m_iSpeed]  && 
		abs( m_pGame->m_player.m_fFy - m_fFy ) <= m_pGame->m_iSteps[m_pGame->m_iSpeed] && m_eActive == EState::Activated)
	{
		// Play Freeze Enemy Sound
		//CPlaySound::GetInstance().playSound("Data/freeze.wav", CPlaySound::EPlayType::Loop);
		PlaySound("Data/freeze.wav", NULL, SND_ASYNC | SND_LOOP);
		m_eActive = EState::Start;
		m_StartTime = m_pGame->m_CurrentTime;
		m_HourglassId = m_pGame->StartProgressHourglass(0.0f, m_fDuration);
		IO_TRACE("STRAT Hourglass : id = %d x= %f y = %f\n", m_HourglassId, m_fFx, m_fFy);
	}

	m_fSpin -= 0.25f*m_pGame->m_iSteps[m_pGame->m_iSpeed];				// Spin The Hourglass Counter Clockwise
	if (m_fSpin < 0.0f) {								// Is The spin Value Less Than 0?
		m_fSpin += 360.0f;								// If So, Add 360
	}

	
	//if (m_eActive == EState::Start )
	//	m_iLifetime += 1;
	auto cnt = (m_pGame->m_CurrentTime - m_StartTime).count()/1000000;
	if( m_HourglassId != 0xffff0000 )
		m_pGame->SetValueProgressHourglass(m_HourglassId, cnt);
	if ( m_eActive == EState::Start && cnt > m_fDuration )	
	{													
		//CPlaySound::GetInstance().stopPlay(0);
		PlaySound(NULL, NULL, 0);						
		m_fFx = 0;										
		m_fFy = 0;
		//m_iLifetime = 0;
		m_eActive = EState::Stop;
		m_pGame->StopProgressHourglass(m_HourglassId);
		m_HourglassId = 0xffff0000;
	}
}

void	CGameHourglass::ResetObject(bool NewLvel) noexcept
{
	if (NewLvel) {
		auto val = (rand() % (2 + 2 * m_pGame->m_iLevel))*1000.0f;
		m_fDuration = max( 3000.0f, val);
		if (m_fDuration == 0.0f)  __debugbreak();
		m_fDeceleration = max( 2.0f, float( rand() % 10 ) );
		m_HourglassId = 0xffff0000;
		IO_TRACE("Hourglass : Duration = %f  Deceleration = %f \n", m_fDuration, m_fDeceleration);
	}
}

GLvoid	CGameBomb::Move() noexcept
{
	if (m_eActive == EState::Stop || m_eActive == EState::NotActive)
		return;

	if (m_eActive == EState::Active) {	
		bool stop = false;
		while (!stop) {
			stop = true;
			m_fFx = (rand() % (m_pGame->m_iHorSize - 1) + 1)*m_pGame->m_SellWidth;	// Give The Bomb  Random X Value
			m_fFy = (rand() % m_pGame->m_iVertSize)*m_pGame->m_SellHeight;			// Give The Bomb  Random Y Value
			for (const auto& hg : m_pGame->m_Prizes) {
				if (hg->m_eActive == EState::Activated && m_fFx == hg->m_fFx &&  m_fFy == hg->m_fFy)
					stop = false;
			}
		}
		m_eActive = EState::Activated;
		//PlaySound("Data/hourglass.wav", NULL, SND_ASYNC);	// If So, Play The Hourglass Appears Sound
	}

	//
	if ( m_eActive == EState::Activated )
		for (auto& enemy : m_pGame->m_vEnemy) {
			enemy.Check();
			if( enemy.m_State != CGameEnemy::State::Dead &&  abs(enemy.m_fFx - m_fFx) <= m_pGame->m_iSteps[m_pGame->m_iSpeed] &&
				abs(enemy.m_fFy - m_fFy) <= m_pGame->m_iSteps[m_pGame->m_iSpeed] && m_eActive == EState::Activated ) {
				// Play Freeze Enemy Sound
				enemy.Deactivate();
				//CPlaySound::GetInstance().playSound("Data/Die.wav", CPlaySound::EPlayType::Once);
				PlaySound("Data/Die.wav", NULL, SND_ASYNC );
				m_eActive = EState::Stop;
			}
			enemy.Check();
		}
}

//CGLTexture	CGameLive::m_Image;

CGameLive::CGameLive(COGLGame* Game) : CGameObject(Game)/*, m_Image(CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Live))*/ {
	m_eTextureType = CTextureManager::EType::Live;
};



GLvoid	CGameLive::Move() noexcept
{
	if (m_State == EGameLiveState::Active) {
		m_fSpin += 2;
		if (m_fSpin > 360.0f)
			m_fSpin = 0.0f;
		m_fScale = (0.75f + 0.25f*sin(m_fSpin / 180 * M_PIF))*m_pGame->GameObjectSize();
	}
	else if (m_State == EGameLiveState::Fall) {
		m_fSpin += 2;
		if (m_fSpin > 360.0f)
			m_fSpin = 0.0f;
		m_fScale +=  0.03f;
		m_fFy += 0.001f;
		m_iAlpha -= 0.01f;
		if (m_iAlpha < 0.0f) {
			m_iAlpha = 1.0f;
			m_State = EGameLiveState::NotActive;
		}
	}
}

void	CGameLive::ResetObject(bool NewLvel) noexcept
{
	m_fSpin = rand() % 180;
	m_State = EGameLiveState::Active;
	m_iAlpha = 1.0f;
	m_fScale = m_pGame->GameObjectSize()*0.5f;
}
#ifndef OPENGL_SHADER


void CGameLive::Draw(CTextVertexColorBuffer& Buffer) const noexcept
{
	if (m_State == EGameLiveState::NotActive)
		return;
	CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Live)->Bind();

	GLfloat verOut[m_iVertAmount*3];
	CMatrix::setIdentityM(m_pGame->m_ModelMatrix);
	CMatrix::translateM(m_pGame->m_ModelMatrix, m_iX, m_iY, -4.0f);
	CMatrix::setRotateM(m_pGame->m_ModelMatrix, m_fSpin, 0.0f, 0.0f, 1.0f);
	CMatrix::scaleM(m_pGame->m_ModelMatrix, m_fScale, m_fScale, 1.0f);

	for (auto i = 0; i < m_iVertAmount; ++i)
		glhVec3TransformCoord(verOut + i * Buffer.VertexDimension, m_fVertex + i * Buffer.VertexDimension, &m_pGame->m_ModelMatrix);
	Buffer.AddVertex( verOut, verOut + m_iVertAmount * Buffer.VertexDimension);
	if (m_State == EGameLiveState::Fall)
		auto hhh = 7777;
	for( auto i = 0 ; i < m_iVertAmount ; ++i)
		Buffer.AddColor(1.0f, 1.0f, 1.0f, m_iAlpha);
	Buffer.AddTexture(m_fTexCoord, m_fTexCoord + m_iVertAmount * Buffer.TextureDimension);
}


#else

void	CGameLive::DrawES(CTextVertexColorBuffer& Buffer) const noexcept
{
	if (m_State == EGameLiveState::NotActive)
		return;
	
	Buffer.AffineTransformation2D(Vec3{ m_pGame->m_fMaxWidth*m_fFx, m_pGame->m_fMaxHeight*( 1.0f - m_fFy), -0.4f },
		m_fSpin, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ m_fScale, m_fScale, 1.0f });
	Buffer.AddVerticesAndTransform(m_fVertex, m_iVertAmount * 3, m_fTexCoord, m_iVertAmount * 2, { 1.0f, 1.0f, 1.0f, 0.75f });
	Buffer.LoadIdentity();
}

#endif

//-------------------------------------------------------------------------------------------------------
//  ось барабана по осо Х, барабан делится на 11 частей - 10 цифр и пустое поле.
//void CSymbolDrum::GetVertexTexture(std::vector<GLfloat>&  Vertex, std::vector<GLfloat>&  Tex, std::vector<unsigned int>&  Index ) const
//{
//	Vertex.resize(( m_iNumSign+1)*2*3);
//	Tex.resize(( m_iNumSign+1)*2*2);
//	Index.resize(m_iNumSign*3*2);
//	//
//	float dAng = M_PIF*2.0f / m_iNumSign, Ang = 0.0f, offAng = m_fAngle/*-( 0.5f + m_iSymbol) *dAng*/;
//
//	for (auto i = 0, iVert = 0, iTex = 0, iInd = 0; i <= m_iNumSign; ++i, Ang += dAng ) {
//		float zCur = m_fZ + m_fRad*cos(Ang + offAng);
//		float yCur = m_fY + m_fRad*sin(Ang + offAng);
//		Vertex[iVert] = m_fX; Vertex[iVert + 1] = Vertex[iVert + 4] = yCur; Vertex[iVert + 2] = Vertex[iVert + 5] = zCur;
//		Vertex[iVert + 3] = m_fX + m_fWidth;
//		//  цифры на текстуре располагаются по вертикали, начиная с 0
//		Tex[iTex] = 0.0f; Tex[iTex + 1] = Tex[iTex + 3] = 1.0f / m_iNumSign*i;
//		Tex[iTex + 2] = 1.0f;
//		//
//		auto countVertex = iVert / 3;
//		if (iVert != 0) {
//			countVertex += 2;
//			Index[iInd++] = countVertex - 4; Index[iInd++] = countVertex - 3; Index[iInd++] = countVertex - 2;
//			Index[iInd++] = countVertex - 1; Index[iInd++] = countVertex - 2; Index[iInd++] = countVertex - 3;
//		}
//		iVert += 6, iTex += 4;
//	}
//}
//
//
//void SlidingTexture::GetVertexTexture(std::vector<GLfloat>&  Vertex, std::vector<GLfloat>&  Tex, unsigned int IndexStart, std::vector<unsigned int>&  Index ) const
//{
//	Vertex = {	m_fX, m_fY,  m_fZ, 
//				m_fX + m_fWidth, m_fY, m_fZ, 
//				m_fX, m_fY + m_fHeight, m_fZ, 
//				m_fX + m_fWidth, m_fY + m_fHeight, m_fZ };
//	Tex = { 0.0f, m_fSlider, 1.0f, m_fSlider, 0.0f, m_fSlider - 1.0f / m_iNumSign, 1.0f, m_fSlider - 1.0f / m_iNumSign };
//	Index = { IndexStart, IndexStart + 1, IndexStart + 2, IndexStart + 2, IndexStart + 1, IndexStart + 3};
//}
//
//void SlidingTexture::GetVertexTextureES( float Width, float Height, std::vector<GLfloat>&  Vertex, std::vector<GLfloat>&  Tex, unsigned int IndexStart, std::vector<unsigned int>&  Index ) const
//{
//	auto sz = min(Height, Width);
//	Vertex = {	m_fX*Width, m_fY*Height,  m_fZ,
//				m_fX*Width + m_fWidth*sz, m_fY*Height, m_fZ,
//				m_fX*Width, m_fY*Height + m_fHeight*sz, m_fZ,
//				m_fX*Width + m_fWidth*sz, m_fY*Height + m_fHeight*sz, m_fZ };
//	Tex = { 0.0f, m_fSlider - 1.0f / m_iNumSign, 1.0f, m_fSlider - 1.0f / m_iNumSign, 0.0f, m_fSlider, 1.0f, m_fSlider };
//	Index = { IndexStart, IndexStart + 2, IndexStart + 1, IndexStart + 2, IndexStart + 3, IndexStart + 1};
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

COGLGame::COGLGame(int Width, int Height) : m_vVertLine( m_iHorSize, std::vector<SWayPart>(m_iVertSize-1, SWayPart(this)) ),
							m_vHorLine(m_iHorSize -1, std::vector<SWayPart>(m_iVertSize, SWayPart(this)) ), m_player(this),
							m_vLives(m_iLives, CGameLive(this) ),
							m_btnPause(CTextureManager::EType::Pause, CTextureManager::EType::Start),
							m_ControlSpeed( CTextureManager::EType::Speed,  CTextureManager::EType::Speed), 
							m_PanelButton(CTextureManager::EType::PanelBtn, CTextureManager::EType::PanelBtn),
							m_ControlPanel(CTextureManager::EType::Panel),
							m_ScoreControl(CTextureManager::EType::Digits), m_StageControl(CTextureManager::EType::Digits), m_LevelControl(CTextureManager::EType::Digits),
							m_StartPanel(CTextureManager::EType::StartPanel), m_StartButton(CTextureManager::EType::PanelBtn, CTextureManager::EType::PanelBtn),
							m_GamePanel(CTextureManager::EType::None )
{
	m_fMaxHeight = Height;
	m_fMaxWidth = Width;
	//			конкретные размеры элементов
	m_fSmallFont = m_fSmallFontConst*Height;
	m_fMediumFont = m_fMediumFontConst*Height;
	m_fLargeFont = m_fLargeFontConst*Height;
	m_fControlHeight = m_fControlHeightConst*Height;
	m_fGameObjectSize = m_fGameObjectSizeConst*Height;
	//
	m_iVertCount = m_iHorSize*(m_iVertSize - 1);
	m_iHorCount = ( m_iHorSize - 1 )*m_iVertSize;
	//m_vLines.resize(m_iVertCount + m_iHorCount, SWayPart(this));
	//
	ResetLives();
	//
	for (auto& b : m_keys)
		b = false;
	//std::time_t result = std::time(NULL);
	m_CurrentTime = std::chrono::steady_clock::now();
	srand((unsigned)time(NULL));

	TimerInit();
	//
	GetAppDirectory(m_AppPath);
	//
	WIN32_FIND_DATA	FindFileData;
	//		поиск и чтение всех каталогов
	HANDLE	hFind;
	std::string		PATH = m_AppPath + "data\\game\\Girls\\";//"data\\game\\Animals\\"
	std::string		file = PATH + "*.*";

	//CGLTexture		Background;
	//	поиск файлов в текущей директории
	hFind = ::FindFirstFile(file.c_str(), &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (hFind != INVALID_HANDLE_VALUE)
		{
			if (!(FindFileData.dwFileAttributes &  (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_HIDDEN)))
			{
				std::string path = FindFileData.cFileName;
//				IO_TRACE("\n Find file - %s\\%s \n", file.c_str(), path.c_str());
				m_BackNames.emplace_back(PATH +path);
				//Background.CreateTexture(PATH + path, GL_LINEAR, GL_LINEAR, 1);
				//Background.DeleteTexture();
			}
			if (!::FindNextFile(hFind, &FindFileData))
				break;
		}
		::FindClose(hFind);
	}
	m_iCountBack = m_BackNames.size();
	//
	m_vUsedBack.resize(m_iCountBack);
	for (auto& c : m_vUsedBack)  
		c = 0;
	//
	CFontImageManager::GetInstance().InitFonts(fonts, FontsCount);
	//
	/*m_ControlArrowLeft.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_ControlArrowRight.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_ControlArrowUp.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_ControlArrowDown.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
	m_ControlArrowLeft.SetVisible(false);
	m_ControlArrowRight.SetVisible(false);
	m_ControlArrowUp.SetVisible(false);
	m_ControlArrowDown.SetVisible(false);*/
	
	m_GamePanel.SetAlign(EHorAlign::Full, EVertAlign::Full);
	m_GamePanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f },  1.0f, 1.0f, ERotation::R0);
	
	m_ControlSpeed.SetPosition(Vec3{ 0.01f+m_fControlHeightConst, 0.01f, -2.0f}, m_fControlHeightConst,  ERotation::R0);
	m_ControlSpeed.SetAlign(EHorAlign::RightBind, EVertAlign::No);
	m_ControlSpeed.SetColor(m_SpeedColor[m_iSpeed], m_SpeedColor[m_iSpeed]);
	m_GamePanel.AddControl(&m_ControlSpeed);
	
	m_btnPause.SetAlign(EHorAlign::RightBind, EVertAlign::No);
	m_btnPause.SetPosition(Vec3{ 0.02f + 2.0f*m_fControlHeightConst, 0.01f, -2.0f}, m_fControlHeightConst,  ERotation::R0);
	m_btnPause.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	m_btnPause.SetFixed(true);
	m_GamePanel.AddControl(&m_btnPause);

	m_GameNameContr.SetFont(0);
	m_GameNameContr.SetText(L"GRID CRAZY");
	m_GameNameContr.SetPosition(Vec3{ 0.2f, 0.01f + m_fMediumFontConst, 0.0f }, 0.7f*m_fMediumFontConst, m_fMediumFontConst);
	m_GameNameContr.SetColor({ 1.0f, 0.5f, 1.0f, 1.0f });
	m_GamePanel.AddControl(&m_GameNameContr);

	m_ScoreContr.SetFont(0);
	m_ScoreContr.SetText(L"Score");
	m_ScoreContr.SetPosition(Vec3{ 0.5f, 0.01f + m_fSmallFontConst, 0.0f }, 0.7f*m_fSmallFontConst, m_fSmallFontConst);
	m_ScoreContr.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	m_GamePanel.AddControl(&m_ScoreContr);

	m_StageContr.SetFont(0);
	m_StageContr.SetText(L"Stage");
	m_StageContr.SetPosition(Vec3{ 0.01f, 0.02f + 2.0f*m_fSmallFontConst, 0.0f }, 0.7f*m_fSmallFontConst, m_fSmallFontConst);
	m_StageContr.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	m_GamePanel.AddControl(&m_StageContr);

	m_LeveContrl.SetFont(0);
	m_LeveContrl.SetText(L"Level");
	m_LeveContrl.SetPosition(Vec3{ 0.01f, 0.01f + m_fSmallFontConst, 0.0f }, 0.7f*m_fSmallFontConst, m_fSmallFontConst);
	m_LeveContrl.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	m_GamePanel.AddControl(&m_LeveContrl);

	m_GamePanel.SetVisible(false);
	//
	m_ControlHourglass.SetPosition(Vec3{0.0f, 0.0f, -2.0f}, 2.0f* m_fControlHeightConst, 2.0f*m_fControlHeightConst, ERotation::R0);
	m_ControlHourglass.SetColor({ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	m_ControlHourglass.SetMaxMin(0.0f, 1000.0f);
	m_ControlHourglass.SetValue(0.0f);
	m_ControlHourglass.SetVisible(false);
	m_ControlHourglass.SetAlign(EHorAlign::Center, EVertAlign::Center);
	m_GamePanel.AddControl(&m_ControlHourglass);
	//
	//m_Contro lPanel.SetPosition(Vec3{500.0f, 500.0f, -2.0f}, Vec3{600.0f, 600.0f, -2.0f}, ERotation::R0);
	m_ControlPanel.SetColor( { 1.0f, 1.0f, 1.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 0.5f } );
	
	m_PanelText.SetFont(0);
	m_PanelText.SetText(L"Pause");
	m_PanelText.SetAlign(EHorAlign::Center, EVertAlign::No);
	m_PanelText.SetPosition(Vec3{ 0.0f, 0.25f, -2.0f }, 0.6f*m_fControlHeightConst, m_fControlHeightConst);
	m_ControlPanel.AddControl(&m_PanelText);

	m_PanelButton.SetFont(0);
	m_PanelButton.SetText(L"Continue");
	m_PanelButton.SetAlign(EHorAlign::Center, EVertAlign::No);
	m_PanelButton.SetColor({ 1.0f, 1.0f, 1.0f, 0.5f }, { 0.5f, 0.5f, 0.5f, 0.5f });
	m_PanelButton.SetPosition(Vec3{ 0.0f, 0.35f, -2.0f }, 0.5f, m_fControlHeightConst, ERotation::R0);

	m_ControlPanel.AddControl(&m_PanelButton);
	m_ControlPanel.SetVisible(false);
	m_ControlPanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f }, 0.75f, 0.5f, ERotation::R0);
	m_ControlPanel.SetAlign(EHorAlign::Center, EVertAlign::Center);
	//
	//
	m_StartPanel.SetColor( { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } );
	m_StartPanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f },  0.95f, 0.95f, ERotation::R90);
	m_StartPanel.SetVisible(true);
	m_StartPanel.SetAlign(EHorAlign::Full, EVertAlign::Full);

	m_StartButton.SetFont(0);
	m_StartButton.SetText(L"Start Game");
	m_StartButton.SetAlign(EHorAlign::Center, EVertAlign::Center);
	m_StartButton.SetColor({ 0.5f, 0.5f, 0.5f, 1.0f }, { 0.5f, 0.5f, 0.5f, 0.5f });
	m_StartButton.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f },  0.5f, 0.1f,  ERotation::R0);

	m_StartPanel.AddControl(&m_StartButton);
	//
	m_ScoreControl.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	m_ScoreControl.SetPosition(Vec3{ 0.55f, 0.01f + m_fSmallFontConst, -3.0f }, 0.7f*m_fSmallFontConst, m_fSmallFontConst, m_fMaxHeight /*ERotation::R0*/);
	m_ScoreControl.SetNumber(0);
	m_GamePanel.AddControl(&m_ScoreControl);

	m_StageControl.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	m_StageControl.SetPosition(Vec3{ 0.06f, 0.02f + 2.0f*m_fSmallFontConst, -3.0f }, 0.7f*m_fSmallFontConst, m_fSmallFontConst, m_fMaxHeight/*ERotation::R0*/);
	m_StageControl.SetNumber(m_iStage);
	m_GamePanel.AddControl(&m_StageControl);

	m_LevelControl;
	m_LevelControl.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	m_LevelControl.SetPosition(Vec3{ 0.06f, 0.01f + m_fSmallFontConst, -3.0f }, 0.7f*m_fSmallFontConst, m_fSmallFontConst, m_fMaxHeight/*ERotation::R0*/);
	m_LevelControl.SetNumber(m_iLevel2);
	m_GamePanel.AddControl(&m_LevelControl);

	//
	CEnemyModel::Init(this);
	GLint maxTexture;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTexture);
	CEnumeratorTimer<timer_0>::PrintTimeStart();
	//
	CMouseNotifier::GetInstance().registerObserver(this);
	CKeyboardNotifier::GetInstance().registerObserver(this);
	//
	COGLControlManager::GetInstance().registerObserver(this);
	//
	CWindowResizeNotifier::GetInstance().registerObserver(this);
	//
	m_bPause = true;
}

COGLGame :: ~COGLGame()
{
}

size_t COGLGame::GetNewBackgroundNum()
{
	if (m_iCountBack == 0)
		return 0;
	//
	std::string strPath;
	size_t tc = GetTickCount();
	size_t num = GetTickCount() % (m_iCountBack - 1);
	size_t pos = num;
	for (; pos != m_iCountBack && m_vUsedBack[pos] != 0; ++pos);
	if (pos == m_iCountBack)
		for (pos = 0; pos != num && m_vUsedBack[pos] != 0; ++pos);
	m_vUsedBack[pos] += 1;
	num = pos;
	
	return num;

}

#ifndef OPENGL_SHADER
void	COGLGame::InitGL() noexcept
{
	glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// Black Background
	glClearDepth(1.0f);										// Depth Buffer Setup
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);										// Enable Blending
															//glBlendFunc(GL_ONE, GL_ONE);
															//glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Type Of Blending To Use
															//
	m_LightAmbient = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_LightDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_LightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_LightPosition = { 300.0f, 300.0f, 500.0f, 1.0f };
	//COGLDrawWithLight::Init();
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, m_LightAmbient.data());		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, m_LightDiffuse.data());		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition.data());		// Position The Light
	glLightfv(GL_LIGHT1, GL_SPECULAR, m_LightSpecular.data());
	glEnable(GL_LIGHT1);											// Enable Light One
																	//
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	//
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//
	CMatrix::setIdentityM(m_ProjMatrix);
}
#else
void	COGLGame::InitGLES() noexcept
{
	char vShaderStr[] =
		"attribute vec4 vPosition;"
		"attribute vec2 vtexCoord;"
		"attribute vec4 vColor;"
		"uniform mat4 u_ProjMatrix;"
		"uniform mat4 u_ModelMatrix;"
		"varying vec2 v_texCoord;"
		"varying vec4 v_texColor;"
		"void main()"
		"{"
		"   gl_Position = u_ProjMatrix*u_ModelMatrix*vPosition;"
		"   v_texCoord = vtexCoord;"
		"   v_texColor = vColor;"
		"}";
	
	char fShaderStr[] =
		//"precision mediump float;"
		"varying vec2 v_texCoord;"
		"varying vec4 v_texColor;"
		"vec4 v_textureColor;"
		"uniform sampler2D s_texture;"
		"uniform int b_UseTexture;"
		"void main()"
		"{"
		"   if( b_UseTexture != 0 ) { "
		"   v_textureColor = texture2D(s_texture, v_texCoord);"
		"   gl_FragColor = v_textureColor*v_texColor;"
		"   }  else "
		"       gl_FragColor = v_texColor;"
		//"   gl_FragColor = mix(v_textureColor, v_texColor, v_textureColor.a);"
		"}";
	
	GLuint vertexShader;
	GLuint fragmentShader;
	GLint linked;
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	// Load the vertex/fragment shaders
	m_Shader = new Shader();
	if( m_Shader->create_program(vShaderStr,fShaderStr) ) {
		// Store the program object
		auto	iprogramm = m_Shader->getShader();
		m_iVertShAttr = glGetAttribLocation(iprogramm, "vPosition");
		m_iTexCoordLoc = glGetAttribLocation(iprogramm, "vtexCoord");
		m_iColorLoc = glGetAttribLocation(iprogramm, "vColor");
		// Get the sampler location
		m_iSamplerLoc = glGetUniformLocation(iprogramm, "s_texture");
		//
		m_iMatrix = glGetUniformLocation(iprogramm, "u_Matrix");
	}
	m_DrawBuffer.SetShader(m_Shader);
	m_DrawBuffer.SetProjectMatrix(&m_ProjMatrix);
	//  подготовка буфера
	//m_DrawBuffer.Translate(-1.0f, -1.0f, -0.0f);
	//m_DrawBuffer.Scale( 2.0f/m_fMaxWidth, 2.0f / m_fMaxHeight, 1.0f);
//	CMatrix::setIdentityM(m_ProjMatrix);
//	CMatrix::translateM(m_ProjMatrix, -1.0f, -1.0f, -0.0f);
//	CMatrix::scaleM(m_ProjMatrix, 2.0f/m_fMaxWidth, 2.0f / m_fMaxHeight, 0.1f);
	//
	glEnable(GL_BLEND);										// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Type Of Blending To Use
}
#endif

void	COGLGame::FindSuitableBackground()
{
	while (true) {
		size_t num = GetNewBackgroundNum();
		if (!m_Background.CreateTexture(m_BackNames[num], GL_LINEAR, GL_LINEAR, 1))
			continue;
		auto backWdt = m_Background.Width();
		auto backHgt = m_Background.Height();
		if ((m_fMaxWidth / m_fMaxHeight >= 1.0f && backWdt / backHgt >= 1.0f) ||
			(m_fMaxWidth / m_fMaxHeight < 1.0f && backWdt / backHgt < 1.0f))
			break;
	}
}

void	COGLGame::ViewPanel(bool Visible, EPlayState State)
{
	switch (State) {
	case EPlayState::Normal: {
		m_PanelText.SetText(L"Pause");
		m_PanelText.GetPosition( m_fMaxWidth, m_fMaxHeight );//SetPosition( Vec3{ 150.0f, 300.0f, -2.0f }, 36.0f, 50.0f );
		break;
	}
	case EPlayState::Die : 
	{
		m_PanelText.SetText(L"Pause");
		m_PanelText.GetPosition(m_fMaxWidth, m_fMaxHeight);//SetPosition( Vec3{ 150.0f, 300.0f, -2.0f }, 36.0f, 50.0f );
		break;
	}
	case EPlayState::Gameover :
	{
		m_PanelText.SetText(L"Game over");
		m_PanelText.GetPosition(m_fMaxWidth, m_fMaxHeight);//SetPosition( Vec3{ 40.0f, 300.0f, -2.0f }, 36.0f, 50.0f );
		break;
	}
	case EPlayState::LevelComplete :
	{
		m_PanelText.SetText(L"Level Complete");
		m_PanelText.GetPosition(m_fMaxWidth, m_fMaxHeight);//SetPosition( Vec3{ 20.0f, 300.0f, -2.0f }, 36.0f, 50.0f );
		break;
	}
	}
	m_ControlPanel.SetVisible(Visible);
}

bool	COGLGame::Init(int Width, int Height) noexcept
{
	if (m_iCountBack == 0)
		return false;
	void InitAPI();
	InitAPI();

	//
	FindSuitableBackground();

	CalcCellsSize();
	//
	m_Line = CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Line);
	//strPath = m_AppPath + "Data\\Game\\line.png";
	//if (!m_Line.CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
	if( !m_Line->IsTexture() )
		return false;
	//
#ifdef OPENGL_SHADER
	InitGLES();
#else
	InitGL();
#endif
	//
	ResetObjects( true );
	//m_player.Init();
	return true;
}


void	COGLGame::GetWindowSize(int& Width, int& Height) noexcept
{
	Width = 130 + m_fGameFieldX + ( m_iHorSize -1 )*m_SellWidth;
	Height = 80 + m_fGameFieldY + (  m_iVertSize -1 )*m_SellHeight;
}

void	COGLGame::SetMaxSize(int Width, int Height)  noexcept {
	m_fMaxWidth = (GLfloat)Width;
	m_fMaxHeight = (GLfloat)Height;
	/*CMatrix::setIdentityM(m_ProjMatrix);
	CMatrix::translateM(m_ProjMatrix, -1.0f, -1.0f, -0.0f);
	CMatrix::scaleM(m_ProjMatrix, 2.0f / m_fMaxWidth, 2.0f / m_fMaxHeight, 0.1f);*/
};

void	COGLGame::CalcCellsSize()
{
	auto wdt = m_Background.Width();
	auto hgt = m_Background.Height();
	float h_w = (float)hgt / (float)wdt;
	auto maxWdt = m_fMaxWidth - 130 - m_fGameFieldX;
	auto maxHgt = m_fMaxHeight - 80 - m_fGameFieldY;
	if (maxWdt*h_w > maxHgt)
		maxWdt = int(maxHgt / h_w);
	else
		maxHgt = int(maxWdt*h_w);
	m_SellWidth = int(maxWdt / (m_iHorSize - 1));
	m_SellHeight = int(maxHgt / (m_iVertSize - 1));
}

void	COGLGame::CalcNewPosition()
{
	//  звездочки живут своей жизнью все время
	for (auto& lv : m_vLives)
		lv.Move();
	//
	/*m_Score.Move();
	m_Stage.Move();
	m_Level.Move();*/
	//

	if (m_ePlayState != EPlayState::Normal || m_bPause)
		return;
	for (size_t numEnemy = 0; numEnemy < m_vEnemy.size(); numEnemy++)	{	
		m_vEnemy[numEnemy].Check();
		m_vEnemy[numEnemy].Move();
		// 
		if ( abs( m_vEnemy[numEnemy].m_fFx - m_player.m_fFx ) < 10 && abs( m_vEnemy[numEnemy].m_fFy - m_player.m_fFy ) < 10 )	{
			m_iLives--;								// If So, Player Loses A Life
			m_vLives[m_iLives].m_State = EGameLiveState::Fall;  //  должна упасть
			m_ePlayState = EPlayState::Die;
			if (m_iLives == 0)
				m_ePlayState = EPlayState::Gameover;
			ViewPanel(true, m_ePlayState);
			//CPlaySound::GetInstance().playSound("Data/Die.wav", CPlaySound::EPlayType::Once);
			PlaySound("Data/Die.wav", NULL, SND_ASYNC/*SND_SYNC*/);	// Play The Death Sound
			//
			memset(m_keys, 0, sizeof(m_keys));
			//
			m_bPause = true;
			m_btnPause.SetPressed(true);
			break;
		}
	}
	//
	m_player.Move();
	//
	if (m_bFilled &&  m_ePlayState == EPlayState::Normal) {
		m_ePlayState = EPlayState::LevelComplete;
	 	ViewPanel( true, m_ePlayState);
		//CPlaySound::GetInstance().playSound("Data/Complete.wav", CPlaySound::EPlayType::Once);
		PlaySound("Data/Complete.wav", NULL, SND_ASYNC/*SND_SYNC*/);	// If So, Play The Level Complete Sound
		//
		m_iStage++;								// Increase The Stage
		if (m_iStage > 3) {						// Is The Stage Higher Than 3?
			m_iStage = 1;						// If So, Set The Stage To One
			m_iLevel++;							// Increase The Level
			m_iLevel2++;
			m_iLives++;
			if (m_iLives > 5)					// Does The Player Have More Than 5 Lives?
				m_iLives = 5;					// If So, Set Lives To Five
			ResetLives();
			if (m_iLevel > 3) 					// Is The Level Greater Than 3?
				m_iLevel = 3;					// If So, Set The Level To 3
		}
		//
		m_bPause = true;
		m_btnPause.SetPressed(true);

		m_ScoreControl.SetNumber(m_iScore);
		m_StageControl.SetNumber(m_iStage);
		m_LevelControl.SetNumber(m_iLevel2);
	}
	// If The Player Hits The Hourglass While It's Being Displayed On The Screen
	for( auto& hg : m_Prizes)
		hg->Move();
}

void		COGLGame::PlayStateTreatment()
{
 	if ( m_ePlayState == EPlayState::Normal) return;
	if (m_ePlayState == EPlayState::Die) {
		ResetObjects(false);
	}
	else if (m_ePlayState == EPlayState::Gameover) {
		m_iStage = 1;
		m_iLevel = 1;
		m_iLives = 5;
		m_iScore = 0;
		ResetLives();

		FindSuitableBackground();
		
		CalcCellsSize();
		//
		ResetObjects(true);
	}  

	if (m_ePlayState == EPlayState::LevelComplete) {
		m_bFilled = false;
		//
		FindSuitableBackground();
		CalcCellsSize();
		//
		ResetObjects(true);									// Reset Player / Enemy Positions
	}
	m_ePlayState = EPlayState::Normal;
  	for (auto& hd : m_HourglassData)
		StopProgressHourglass(hd.m_iId);
}


#ifndef OPENGL_SHADER
bool	COGLGame::DrawSceneFast()
{
	//  временная задержка, регулирующая скорость игры
	float start = TimerGetTime();
	while (TimerGetTime() < start + float(m_iSteps[m_iSpeed] * 2.0f)) {}
	
	//
	{
		timer_0	tt("DrawScene");

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		//
		glLoadIdentity();
		m_CurrentTime = std::chrono::steady_clock::now();
		
		//  рисование сцены
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer

		//														//   Рисуем фон
		CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Background)->Bind();

		GLfloat vertex[] = { 10.0f, 10.0f, -4.0f,		   m_fMaxWidth - 10.0f, 10.0f, -4.0f,
			10.0f, m_fMaxHeight - 10.0f, -4.0f, m_fMaxWidth - 10.0f, m_fMaxHeight - 10.0f, -4.0f };
		GLfloat tex[] = { 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f };
		unsigned int index[] = { 2, 1, 0, 2, 3, 1 };
		m_DrawBuffer.AddVertex(vertex, vertex + sizeof(vertex) / sizeof(GLfloat));
		m_DrawBuffer.AddTexture(tex, tex + sizeof(tex) / sizeof(GLfloat));
		m_DrawBuffer.AddIndex(index, index + sizeof(index) / sizeof(unsigned int));
		m_DrawBuffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.5f });
		m_DrawBuffer.DrawElements(GL_TRIANGLES);
		//
		{
			timer_0	tt("Draw Texts");
			CFontImage& fnt = CFontImageManager::GetInstance().GetFont(0);
			fnt.SetScale(0.35f);
			fnt.SetColor({1.0f, 0.5f, 1.0f, 1.0f});
			fnt.TextDraw( L"GRID CRAZY", 180, 54, 0.0f, m_DrawBuffer);

			fnt.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
			fnt.SetScale(0.25f);
			
			fnt.TextDraw(L"Level: ", 20, 40, 0.0f, m_DrawBuffer);
			fnt.TextDraw(L"Stage: ", 20, 70, 0.0f, m_DrawBuffer);
			fnt.TextDraw(L"Score: ", 700, 53, 0.0f, m_DrawBuffer);

			if (m_ePlayState == EPlayState::Gameover)		{
				fnt.SetColor({ ( rand() % 255 )/255.0f, ( rand() % 255) / 255.0f, ( rand() % 255) / 255.0f, 1.0f });
				fnt.TextDraw(L"GAME OVER", 480, 40, 0.0f, m_DrawBuffer);
				fnt.TextDraw(L"PRESS SPACE", 465, 60, 0.0f, m_DrawBuffer);
			}
			m_DrawBuffer.DrawElements(GL_TRIANGLES);
			//
			for (auto& lv : m_vLives)
				lv.Draw(m_DrawBuffer);
			m_DrawBuffer.DrawArray(GL_TRIANGLES);
		}

				
		m_bFilled = true;										// Set Filled To True Before Testing
		//glLoadIdentity();										// Reset The Current Modelview Matrix
																//glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping	
		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);						// Bright White Color
		m_Background.Bind();

		GLfloat  fSellHeight = m_SellHeight, fSellWidth = m_SellWidth;
		//
		{
		timer_0	tt("Draw Background");
		for (auto ix = 0; ix < m_iHorSize; ix++) {	// Loop From Left To Right
			for (auto iy = 0; iy < m_iVertSize; iy++) {// Loop From Top To Bottom

				if (ix < m_iHorSize - 1 && iy < m_iVertSize - 1)					// If In Bounds, Fill In Traced Boxes
				{
					// Are All Sides Of The Box Traced?
					if (m_vHorLine[ix][iy]() && m_vHorLine[ix][iy + 1]() && m_vVertLine[ix][iy]() && m_vVertLine[ix + 1][iy]())
					{
						
						m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fGameFieldY + iy * fSellHeight, -5.0F);
						m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
						m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fGameFieldY + iy * fSellHeight, -5.0F);
						m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
						m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fGameFieldY + (iy + 1) * fSellHeight, -5.0F);
						m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - (float(iy + 1) / m_iVertSize));

						m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fGameFieldY + iy * fSellHeight, -5.0F);
						m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
						m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fGameFieldY + (iy + 1) * fSellHeight, -5.0F);
						m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - (float(iy + 1) / m_iVertSize));
						m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fGameFieldY + (iy + 1) * fSellHeight, -5.0F);
						m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy + 1) / m_iVertSize);
					}
				}
			}
		}
		m_DrawBuffer.DrawArray(GL_TRIANGLES);
		}
		//
		{
		timer_0	tt("Draw Line"); 
		SOGLColor  lineColor;
		for (auto ix = 0; ix < m_iHorSize; ix++) {	// Loop From Left To Right
			for (auto iy = 0; iy < m_iVertSize; iy++) {// Loop From Top To Bottom

				if (ix < m_vHorLine.size() && iy < m_vHorLine[ix].size()) {								// Dont Draw To Far Right
					if (m_vHorLine[ix][iy]() == 0)				// If A Horizontal Line Isn't Filled
						m_bFilled = false;							// filled Becomes False
					//
					CalcGridLineColor(m_vHorLine[ix][iy]);
					m_vHorLine[ix][iy].Draw(m_DrawBuffer);
				}

				//lineColor = { 0.0f, 0.5f, 1.0f, 0.25f };					// Set Line Color To Blue

				if (ix < m_vVertLine.size() && iy < m_vVertLine[ix].size()) {		// Dont Draw To Far Down
					timer_0	tt("Draw Line");
					if (m_vVertLine[ix][iy]() == 0)			// If A Verticle Line Isn't Filled
						m_bFilled = false;						// filled Becomes False

					CalcGridLineColor(m_vVertLine[ix][iy]);
					m_vVertLine[ix][iy].Draw(m_DrawBuffer);
				}
			}
		}
		//
		glLineWidth(8);
		m_DrawBuffer.DrawArray(GL_LINES);
		
		}
		
		
//		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		//
		{
			timer_0	tt("Draw Prizes");
			for (auto& prize : m_Prizes)
				prize->Draw(m_DrawBuffer);
			m_DrawBuffer.DrawArray(GL_TRIANGLES);
			CGamePrize::m_ePrevType = CGamePrize::EType::None;
		}
		{
			timer_0	tt("Draw Enemy");
			for (auto& en : m_vEnemy)
				en.Draw(m_DrawBuffer);
			m_DrawBuffer.DrawArray(GL_TRIANGLES);
		}
		{
			timer_0	tt("Draw player");
			m_player.Draw(m_DrawBuffer);
			m_DrawBuffer.DrawArray(GL_TRIANGLES);
		}
		//
		m_Score.SetNumber(m_iScore);
		m_Score.Draw(m_DrawBuffer);

		m_Stage.SetNumber(m_iStage);
		m_Level.SetNumber(m_iLevel2);
		m_Stage.Draw(m_DrawBuffer);
		m_Level.Draw(m_DrawBuffer);
		m_DrawBuffer.DrawElements(GL_TRIANGLES);
		//
		glDisable(GL_TEXTURE_2D);
		//
		COGLControlManager::GetInstance().DrawControls(m_DrawBuffer);
		//
		CalcNewPosition();

	}
	return true;
}
#else
//
//void	COGLGame::AffineTransformation2D(const Vec3& Offset, GLfloat Angle, const Vec3& RotAxis, const Vec3& Scale) noexcept
//{
//	glhLoadIdentityf2(&m_ModelMatrix);
//	glhTranslatef2(&m_ModelMatrix, Offset.x, m_fMaxHeight - Offset.y, Offset.z);
//	glhScalef2(&m_ModelMatrix, Scale.x, Scale.y, Scale.z);
//	glhRotatef2(&m_ModelMatrix, Angle*M_PIF / 180.0f, RotAxis.x, RotAxis.y, RotAxis.z );
//	
//}
//
//void	COGLGame::AffineTransformation2D(const Vec3&& Offset, GLfloat Angle, const Vec3&& RotAxis, const Vec3&& Scale) noexcept
//{
//	glhLoadIdentityf2(&m_ModelMatrix);
//	glhTranslatef2(&m_ModelMatrix, Offset.x, m_fMaxHeight - Offset.y, Offset.z);
//	glhScalef2(&m_ModelMatrix, Scale.x, Scale.y, Scale.z);
//	glhRotatef2(&m_ModelMatrix, Angle*M_PIF / 180.0f, RotAxis.x, RotAxis.y, RotAxis.z);
//}
//
//void	COGLGame::AddVertices(const GLfloat* Vertises, int VertSize, const GLfloat* Texture, int TexSize, const SOGLColor& Color) noexcept
//{
//	GLfloat* verOut = new GLfloat[VertSize];
//	//MultiplyMatrices4(&m_ResultMatrix, &m_ProjMatrix, &m_ModelMatrix);
//	for (auto i = 0; i < VertSize; i += m_DrawBuffer.VertexDimension)
//		glhVec3TransformCoord(verOut + i, Vertises + i, &m_ModelMatrix);
//	m_DrawBuffer.AddVertex(verOut, verOut + VertSize);
//	m_DrawBuffer.AddTexture( Texture, Texture + TexSize);
//	m_DrawBuffer.SetColor(Color);
//	delete[] verOut;
//}


//void COGLGame::DrawBuffer( CGLTexture* Texture, CTextVertexColorBuffer& Buffer ) noexcept
//{
//	timer_0	tt("Buffer FlushES");
//	if (Buffer.VertexSize() == 0)
//		return;
//	glUseProgram(m_Shader->getShader());
//	auto err = glGetError();
//	// Load the vertex position
//	glVertexAttribPointer(m_iVertShAttr, Buffer.VertexDimension, GL_FLOAT, GL_FALSE, 0, Buffer.GetVertexData());
//	// Load the texture coordinate
//	glVertexAttribPointer(m_iTexCoordLoc, Buffer.TextureDimension, GL_FLOAT, GL_FALSE, 0, Buffer.GetTextureData());
//	// Load the texture coordinate
//	glVertexAttribPointer(m_iColorLoc, Buffer.ColorDimension, GL_FLOAT, GL_FALSE, 0, Buffer.GetColorData());
//
//	glEnableVertexAttribArray(m_iVertShAttr);
//	glEnableVertexAttribArray(m_iTexCoordLoc);
//	glEnableVertexAttribArray(m_iColorLoc);
//
//	// Bind the texture
//	glActiveTexture(GL_TEXTURE0);
//	Texture->Bind();
//
//	// Set the texture sampler to texture unit to 0
//	glUniform1i(m_iSamplerLoc, 0);
//	glUniformMatrix4fv (m_iMatrix, 1, false, &m_ProjMatrix._11);
//	// Draw quad with nearest sampling
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
//	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
//	if(Buffer.IndexSize() == 0)
//		glDrawArrays(GL_TRIANGLES, 0, Buffer.Count());
//	else {
//		glDrawElements(GL_TRIANGLES,			// режим
//			Buffer.IndexSize(),					// количество
//			GL_UNSIGNED_INT,					// тип
//			Buffer.GetIndexData()				// смещение в элементном буфере
//		);
//		auto err = glGetError();
//		if (err)
//			int f = 99;
//	}
//
//	glDisableVertexAttribArray(m_iVertShAttr);
//	glDisableVertexAttribArray(m_iTexCoordLoc);
//	glDisableVertexAttribArray(m_iColorLoc);
//
//	glDeleteProgram(m_Shader->getShader());
//	
//	Buffer.Clear();
//};


bool	COGLGame::DrawSceneES()  noexcept
{
	//  временная задержка, регулирующая скорость игры
	float start = TimerGetTime();
	while (TimerGetTime() < start + float(m_iSteps[m_iSpeed] * 2.0f)) {}
	//
	{
		timer_0	tt("DrawScene");

		glEnable(GL_TEXTURE_2D);
		//glLoadIdentity();
		m_CurrentTime = std::chrono::steady_clock::now();
		//  рисование сцены
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer
		//   Рисуем фон
		GLfloat vertex[] = { 10.0f, 10.0f, -4.0f,		   m_fMaxWidth - 10.0f, 10.0f, -4.0f,
			10.0f, m_fMaxHeight - 10.0f, -4.0f, m_fMaxWidth - 10.0f, m_fMaxHeight - 10.0f, -4.0f };
		GLfloat tex[] = { 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f };
		unsigned int index[] = { 2, 1, 0, 2, 3, 1 };
		m_DrawBuffer.AddVertex(vertex, vertex + sizeof(vertex) / sizeof(GLfloat));
		m_DrawBuffer.AddTexture(tex, tex + sizeof(tex) / sizeof(GLfloat));
		m_DrawBuffer.AddIndex(index, index + sizeof(index) / sizeof(unsigned int));
		m_DrawBuffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.5f });
		m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Background), GL_TRIANGLES);

		if (m_ePlayState == EPlayState::GameStart) {
			COGLControlManager::GetInstance().DrawControlsES(m_DrawBuffer);
			return true;
		}

		GLfloat		fSellHeight = m_SellHeight, fSellWidth = m_SellWidth;
		GLfloat		fMaxWidth = m_fMaxWidth, fMaxHeight = m_fMaxHeight;

		{
			timer_0	tt("Draw Texts");
//			LoadIdentity();
			/*CFontImage& fnt = CFontImageManager::GetInstance().GetFont(0);
			fnt.SetSize( 0.7f*m_fMediumFont, m_fMediumFont);
			fnt.SetColor({ 1.0f, 0.5f, 1.0f, 1.0f });
			fnt.TextDraw(L"GRID CRAZY", 180, m_fMaxHeight - 54, 0.0f, m_DrawBuffer);*/

			/*fnt.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
			fnt.SetSize(0.7f*m_fSmallFont, m_fSmallFont);

			fnt.TextDraw(L"Level: ", 20, m_fMaxHeight - 40, 0.0f, m_DrawBuffer);
			fnt.TextDraw(L"Stage: ", 20, m_fMaxHeight - 70, 0.0f, m_DrawBuffer);
			fnt.TextDraw(L"Score: ", 700, m_fMaxHeight - 53, 0.0f, m_DrawBuffer);*/

			/*if (m_ePlayState == EPlayState::Gameover) {
				fnt.SetColor({ (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, 0.80f });
				fnt.TextDraw(L"GAME OVER", 480, m_fMaxHeight - 40, 0.0f, m_DrawBuffer);
				fnt.TextDraw(L"PRESS SPACE", 465, m_fMaxHeight - 60, 0.0f, m_DrawBuffer);
			}
			m_DrawBuffer.DrawBuffer( fnt.GetTexture(), GL_TRIANGLES);*/
			//m_DrawBuffer.DrawElements(GL_TRIANGLES);
			//
			for (auto& lv : m_vLives)
				lv.DrawES(m_DrawBuffer);
			m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(m_vLives[0].GetTextute() ), GL_TRIANGLES );
		}


		m_bFilled = true;										// Set Filled To True Before Testing

		m_DrawBuffer.LoadIdentity();
		{
		timer_0	tt("Draw Background");
		for (size_t ix = 0; ix < m_iHorSize; ix++) {	// Loop From Left To Right
			for (size_t iy = 0; iy < m_iVertSize; iy++) {// Loop From Top To Bottom

				if (ix < m_iHorSize - 1 && iy < m_iVertSize - 1)					// If In Bounds, Fill In Traced Boxes
				{
					// Are All Sides Of The Box Traced?
					if (m_vHorLine[ix][iy]() && m_vHorLine[ix][iy + 1]() && m_vVertLine[ix][iy]() && m_vVertLine[ix + 1][iy]())
					{

						m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, fMaxHeight - ( m_fGameFieldY + iy * fSellHeight), -0.5f);
						m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
						m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, fMaxHeight - (m_fGameFieldY + iy * fSellHeight), -0.5f);
						m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
						m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight), -0.5f);
						m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - (float(iy + 1) / m_iVertSize));

						m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, fMaxHeight - (m_fGameFieldY + iy * fSellHeight), -0.5f);
						m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy) / m_iVertSize);
						m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight), -0.5f);
						m_DrawBuffer.AddTexture(float(ix + 1) / m_iHorSize, 1.0f - (float(iy + 1) / m_iVertSize));
						m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight), -0.5f);
						m_DrawBuffer.AddTexture(float(ix) / m_iHorSize, 1.0f - float(iy + 1) / m_iVertSize);
						m_DrawBuffer.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
			}
		}
		m_DrawBuffer.DrawBuffer(&m_Background, GL_TRIANGLES);
		}
		
		{
		timer_0	tt("Draw Line");
		GLfloat		texCoordHor[] = { 0.9f, 1.0f,	0.9f, 0.0f,		0.1f, 0.0f,		0.9f, 1.0f,		0.1f, 0.0f,		0.1f, 1.0f};
		GLfloat		texCoordVert[] = {0.1f, 1.0f,   0.9f, 0.0f,		0.1f, 0.0f,		0.1f, 1.0f,		0.9f, 1.0f,		0.9f, 0.0f};
		SOGLColor  lineColor;
		for (size_t ix = 0; ix < m_iHorSize; ix++) {	// Loop From Left To Right
			for (size_t iy = 0; iy < m_iVertSize; iy++) {// Loop From Top To Bottom
				//m_Line->Bind();
			  	lineColor = { 0.0f, 0.5f, 1.0f, 0.25f };

				if (ix < m_vHorLine.size() && iy < m_vHorLine[ix].size()) {								// Dont Draw To Far Right

					if (m_vHorLine[ix][iy]() == 1)					// Has The Horizontal Line Been Traced
						lineColor = { 1.0f, 1.0f, 1.0f, 0.25f };
					else if (m_vHorLine[ix][iy]() == 0)				// If A Horizontal Line Isn't Filled
						m_bFilled = false;							// filled Becomes False
																		
					m_DrawBuffer.AddTexture(texCoordHor, texCoordHor + sizeof(texCoordHor) / sizeof(GLfloat));
					m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight - 5), -0.4f);
					m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);

					m_DrawBuffer.AddVertex(m_fGameFieldX + (ix + 1) * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight - 5), -0.4f);
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight - 5), -0.4f);

					m_DrawBuffer.SetColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
				}

				lineColor = { 0.0f, 0.5f, 1.0f, 0.25f };					// Set Line Color To Blue

				if (ix < m_vVertLine.size() && iy < m_vVertLine[ix].size()) {		// Dont Draw To Far Down
					timer_0	tt("Draw Line");
					if (m_vVertLine[ix][iy]() == 1)				// Has The Horizontal Line Been Traced
						lineColor = { 1.0f, 1.0f, 1.0f, 0.25f };	// If So, Set Line Color To White
					else if (m_vVertLine[ix][iy]() == 0)			// If A Verticle Line Isn't Filled
						m_bFilled = false;						// filled Becomes False

					m_DrawBuffer.AddTexture(texCoordVert, texCoordVert + sizeof(texCoordVert) / sizeof(GLfloat));
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth + 5, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);	// Top Right
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth - 5, m_fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight - 5), -0.4f);	// Bottom Left
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth - 5, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);	// Top Left

					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth + 5, m_fMaxHeight - (m_fGameFieldY + iy * fSellHeight + 5), -0.4f);
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth + 5, m_fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight - 5), -0.4f);	// Bottom Right
					m_DrawBuffer.AddVertex(m_fGameFieldX + ix * fSellWidth - 5, m_fMaxHeight - (m_fGameFieldY + (iy + 1) * fSellHeight - 5), -0.4f);

					m_DrawBuffer.SetColor(lineColor.r, lineColor.g, lineColor.b, lineColor.a);
				}
			}
		}
		m_DrawBuffer.DrawBuffer(m_Line, GL_TRIANGLES);
		}
		//glDisable(GL_TEXTURE_2D);
		//		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		//
		{
		timer_0	tt("Draw Prizes");
		CTextureManager::EType type = CTextureManager::EType::None;
		for (auto& prize : m_Prizes) {
			if (prize->m_eActive != CGamePrize::EState::Activated)
				continue;
			
			if (prize->GetTextute() != type ) {
				if (type != CTextureManager::EType::None)
					m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(type), GL_TRIANGLES);
				type = prize->GetTextute();
			}
			prize->DrawES(m_DrawBuffer);
		}
		m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(type), GL_TRIANGLES);
		}
		{
		timer_0	tt("Draw Enemy");
		for (auto& en : m_vEnemy)
			en.DrawES(m_DrawBuffer);
		m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(m_vEnemy[0].GetTextute()/*CTextureManager::EType::Enemy*/), GL_TRIANGLES);
		}
		{
		timer_0	tt("Draw player");
		m_player.DrawES(m_DrawBuffer);
		m_DrawBuffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(m_player.GetTextute()/*CTextureManager::EType::Player*/), GL_TRIANGLES);
		}
		//
		m_ScoreControl.SetNumber(m_iScore);
		m_StageControl.SetNumber(m_iStage);
		m_LevelControl.SetNumber(m_iLevel2);
		//
		m_DrawBuffer.LoadIdentity();
		//
		COGLControlManager::GetInstance().DrawControlsES(m_DrawBuffer);
		//
		CalcNewPosition();
	}
	return true;
}

#endif

GLvoid	COGLGame::ProcessKeyboard() noexcept
{
	//if (m_keys['A'] && !m_bAp)	{							// If 'A' Key Is Pressed And Not Held
	//
	//	m_bAp = true;										// ap Becomes TRUE
	//	m_bAnti = !m_bAnti;									// Toggle Antialiasing
	//} else
	//if (!m_keys['A'])  {								// If 'A' Key Has Been Released
	//
	//	m_bAp = false;										// ap Becomes FALSE
	//}
	//
	m_player.ProcessKeyboard( &m_keys[0], 256 );
	//
	if (m_keys[' '] )	{									// If Spacebar Is Being Pressed
	
		if (m_ePlayState == EPlayState::Gameover) {
			m_ePlayState = EPlayState::Normal;				// gameover Becomes FALSE
			m_bFilled = true;								// filled Becomes TRUE
			m_iLevel = 1;									// Starting Level Is Set Back To One
			m_iLevel2 = 1;									// Displayed Level Is Also Set To One
			m_iStage = 1;									// Game Stage Is Set To Zero
			m_iLives = 5;									// Lives Is Set To Five
		} 
	}

}

void	COGLGame::SetControlPosition()  noexcept
{
	//m_GamePanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f }, Vec3{ 0.0f, 0.0f, -2.0f }, ERotation::R0);

	//m_ControlSpeed.SetPosition(Vec3{ Xdist_from(0.01f, EHorAlign::Right) - m_fControlHeight, sizeY(0.01f), -2.0f }, 
	//	Vec3{ Xdist_from(0.01f, EHorAlign::Right), sizeY(0.01f)+ m_fControlHeight , -2.0f }, ERotation::R0);
	//m_btnPause.SetPosition(Vec3{ Xdist_from(0.02f, EHorAlign::Right) - 2.0f*m_fControlHeight, sizeY(0.01f), -2.0f }, 
	//	Vec3{ Xdist_from(0.02f, EHorAlign::Right) - m_fControlHeight, sizeY(0.01f) + m_fControlHeight, -2.0f }, ERotation::R0);

	//m_GameNameContr.SetPosition(Vec3{ 180.0f, 54.0f, 0.0f }, 0.7f*m_fMediumFont, m_fMediumFont);
	//m_ScoreContr.SetPosition(Vec3{ 700.0f, 53.0f, 0.0f }, 0.7f*m_fSmallFont, m_fSmallFont);
	//m_StageContr.SetPosition(Vec3{ 20.0f, 70.0f, 0.0f }, 0.7f*m_fSmallFont, m_fSmallFont);
	//m_LeveContrl.SetPosition(Vec3{ 20.0f, 40.0f, 0.0f }, 0.7f*m_fSmallFont, m_fSmallFont);

	//m_ControlHourglass.SetPosition(Vec3{ 500.0f, 500.0f, -2.0f }, Vec3{ 600.0f, 600.0f, -2.0f }, ERotation::R0);
	////
	//m_ControlPanel.SetPosition(Vec3{ 100.0f, 100.0f, -2.0f }, Vec3{ 612.0f, 612.0f, -2.0f }, ERotation::R0);

	//m_PanelText.SetPosition(Vec3{ 120.0f, 300.0f, -2.0f }, 24.0f, 36.0f);
	//m_PanelButton.SetPosition(Vec3{ 50.0f, 350.0f, -2.0f }, Vec3{ 450.0f, 420.0f, -2.0f }, ERotation::R0);
	////
	//m_StartPanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f }, Vec3{ m_fMaxWidth, m_fMaxHeight, -2.0f }, ERotation::R90);

	//m_StartButton.SetPosition(Vec3{ 50.0f, 350.0f, -2.0f }, Vec3{ 450.0f, 420.0f, -2.0f }, ERotation::R0);
}


void	COGLGame::ResizeScene(GLsizei width, GLsizei height) noexcept
{
	if (height == 0)										// Prevent A Divide By Zero By
		height = 1;											// Making Height Equal One

	glViewport(0, 0, width, height);						// Reset The Current Viewport
#ifndef OPENGL_SHADER
	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
	glOrtho(0.0f, width, height, 0.0f, -5.0f, 5.0f);		// Create Ortho 640x480 View (0,0 At Top Left)
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 10.0f, -2000.0f);
	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
#else
	CMatrix::setIdentityM(m_ProjMatrix);
	CMatrix::orthoM( m_ProjMatrix, 0.0f, width, 0.0f, height, -5.0f, 5.0f );
	COGLControlManager::GetInstance().SetWindowSize(width, height);
#endif
	SetMaxSize(width, height);
	//
	auto sz = min(width, height);
	m_fSmallFont = m_fSmallFontConst*sz;
	m_fMediumFont = m_fMediumFontConst*sz;
	m_fLargeFont = m_fLargeFontConst*sz;
	m_fControlHeight = m_fControlHeightConst*sz;
	m_fGameObjectSize = m_fGameObjectSizeConst*sz;
	IO_TRACE("SmallFont = %f width = %f  height = %f \n", m_fSmallFont, width, height);
//	m_ControlPanel.SetPosition( Vec3{ width/2.0f - 256.0f, height / 2.0f - 256.0f, -2.0f }, Vec3{ width / 2.0f + 256.0f, height / 2.0f + 256.0f, -2.0f }, ERotation::R0 );
	//
	FindSuitableBackground();
	CalcCellsSize();
	ResetObjects(false);
	//m_StartPanel.SetPosition(Vec3{ 0.0f, 0.0f, -2.0f }, Vec3{ m_fMaxWidth, m_fMaxHeight, -2.0f }, ERotation::R90);
	SetControlPosition();
}

void  COGLGame::CheckPrize(int X, int Y)
{
	//IO_TRACE("CHECK LINE ( %d, %d )\n", X, Y);
	for (auto& hg : m_Prizes)
		if (hg->m_iX == X && hg->m_iY == Y) {
			if (hg->m_eActive == CGamePrize::EState::NotActive) {
				hg->m_eActive = CGamePrize::EState::Active;
				//IO_TRACE("hourglass in LINE ( %d, %d )\n", X, Y);
			}
		}
	
}

void COGLGame::ActivatePrize(int X, int Y, bool Horizontal)  noexcept // помечаем линию ( горизонтальную если Horizontal = true ) с коорд. X,Y как пройденную 
{
	//	часы лежат на линиях ( а не в прямоугольниках )
	CheckPrize(X, Y);
	if (Horizontal) {
		if (m_vHorLine[X][Y]() == 0)
			m_iScore += 1;

		bool horCondBefor1 = Y < m_iVertSize - 1 && m_vHorLine[X][Y]() && m_vHorLine[X][Y + 1]() && m_vVertLine[X][Y]() && m_vVertLine[X + 1][Y]();
		bool horCondBefor2 = Y > 0 && m_vHorLine[X][Y]() && m_vHorLine[X][Y - 1]() && m_vVertLine[X][Y - 1]() && m_vVertLine[X + 1][Y - 1]();

		m_vHorLine[X][Y]() = 1;
		
		if (Y < m_iVertSize - 1 && m_vHorLine[X][Y]() && m_vHorLine[X][Y + 1]() && m_vVertLine[X][Y]() && m_vVertLine[X + 1][Y]())
			if(!horCondBefor1)
				m_iScore += 10;
		if (Y > 0 && m_vHorLine[X][Y]() && m_vHorLine[X][Y - 1]() && m_vVertLine[X][Y - 1]() && m_vVertLine[X + 1][Y - 1]())
			if (!horCondBefor2)
				m_iScore += 10;
	}
	if (!Horizontal) {
		if (m_vVertLine[X][Y]() == 0)
			m_iScore += 1;

		bool vertCondBefor1 = X < m_iHorSize - 1 && m_vHorLine[X][Y]() && m_vHorLine[X][Y + 1]() && m_vVertLine[X][Y]() && m_vVertLine[X + 1][Y]();
		bool vertCondBefor2 = X > 0 && m_vHorLine[X - 1][Y]() && m_vHorLine[X - 1][Y + 1]() && m_vVertLine[X][Y]() && m_vVertLine[X - 1][Y]();

		m_vVertLine[X][Y]() = 1;
		if (X < m_iHorSize - 1 && m_vHorLine[X][Y]() && m_vHorLine[X][Y + 1]() && m_vVertLine[X][Y]() && m_vVertLine[X + 1][Y]())
			if(!vertCondBefor1)
				m_iScore += 10;
		if (X > 0 && m_vHorLine[X - 1][Y]() && m_vHorLine[X - 1][Y + 1]() && m_vVertLine[X][Y]() && m_vVertLine[X - 1][Y]())
			if (!vertCondBefor2)
				m_iScore += 10;
	}
	return;
	// и проверяем не нада ли открыть прямоугольник и нет ли там приза
	//if (X >= m_iHorSize || Y >= m_iVertSize) return;
	//if ( Horizontal && X  >= m_iHorSize -1 ) return;
	//if (!Horizontal && Y >= m_iVertSize - 1) return;
	//if (Horizontal) {
	//	m_vHorLine[X][Y] = 1;
	//	if (Y < m_iVertSize - 1 && m_vHorLine[X][Y] && m_vHorLine[X][Y + 1] && m_vVertLine[X][Y] && m_vVertLine[X + 1][Y])
	//		CheckPrize(X, Y);
	//	if (Y > 0 && m_vHorLine[X][Y] && m_vHorLine[X][Y - 1] && m_vVertLine[X][Y-1] && m_vVertLine[X + 1][Y-1])
	//		CheckPrize(X, Y-1);
	//}
	//if (!Horizontal) {
	//	m_vVertLine[X][Y] = 1;
	//	if (X < m_iHorSize - 1 && m_vHorLine[X][Y] && m_vHorLine[X][Y + 1] && m_vVertLine[X][Y] && m_vVertLine[X + 1][Y])
	//		CheckPrize(X, Y);
	//	if (X > 0 && m_vHorLine[X-1][Y] && m_vHorLine[X-1][Y + 1] && m_vVertLine[X][Y] && m_vVertLine[X - 1][Y])
	//		CheckPrize(X-1, Y);
	//}
}

void	COGLGame::ResetLives()
{
	for (auto i = 0; i != m_iLives/*m_vLives.size()*/; ++i) {
		m_vLives[i].SetPosition(0.25f + i * 0.035f, 0.065f);
		m_vLives[i].ResetObject(true);
	}
}


void COGLGame::ResetObjects(bool NewLvel)									// Reset Player And Enemies
{
	CEnumeratorTimer<timer_0>::PrintTimeClear();
	CEnumeratorTimer<timer_0>::PrintTimeStart();
	m_player.ResetObject(NewLvel);
	
	auto numEnemy =  m_iStage + m_iLevel;
//	numEnemy = 1;
	m_vEnemy.resize( numEnemy, CGameEnemy(this) );
	for (auto& enemy : m_vEnemy )				// Loop Through All The Enemies
	{
		enemy.ResetObject(NewLvel);
	}
	if (NewLvel) {
		//  персчитываем несработавшие призы и добаваляем их к новым
		auto nHour = 0, nBomb = 0;
		for (auto& pr : m_Prizes)
			if (pr->m_eActive != CGamePrize::EState::Stop) {
				if (pr->m_eType == CGamePrize::EType::Bomb) nBomb++;
				else if (pr->m_eType == CGamePrize::EType::Hourglass) nHour++;
			}
		//
		for (auto& pr : m_Prizes)
			delete pr;
		m_Prizes.clear();
		auto numPrizes = numEnemy * 2;
		m_Prizes.resize( numPrizes + nHour + nBomb, nullptr );
		float fnumHour = rand();
		float fnumBomb = rand();
		fnumHour = fnumHour / (fnumHour + fnumBomb)*numPrizes;
		int numHour = fnumHour + 0.5f,
			numBomb = min( numPrizes - numHour, numEnemy - 1 );
		if (m_iLevel == 1)   //  первый уровень - должен быть простым
			numBomb = numEnemy - 1;
		numHour = numPrizes - numBomb;
		//  добавляем призы, сохранившиеся с предыдущего уровня
		numBomb += nBomb;
		numHour += nHour;
		//  вычисляем случайным образом число разных призов
		// задаем случайные номера для ячеек, где спрятаны часы
		int num = 0;
		for (auto& hg : m_Prizes) {
			if(num < numHour)
				hg = new CGameHourglass(this);
			else if(num < numHour + numBomb )
				hg = new CGameBomb(this);
			num++;
			bool uniqueCell = true;
			int xcell = 0;
			int ycell = 0;
			do {
				uniqueCell = true;
				xcell = rand() % (m_iHorSize - 1);
				ycell = rand() % (m_iHorSize - 1);
				for (auto& hg1 : m_Prizes)
					if ( hg1 != nullptr && hg1->m_iX == xcell && hg1->m_iY == ycell)
						uniqueCell = false;
			} while (!uniqueCell);
			hg->m_iX = xcell;
			hg->m_iY = ycell;
			hg->ResetObject( NewLvel );
 		  	IO_TRACE("prize in SELL ( %d, %d )\n", hg->m_iX, hg->m_iY);
		}
		//
		m_HourglassData.clear();
		m_ControlHourglass.SetVisible(false);
		//		инициализация линий
		auto i = 0;
		for ( auto row = 0 ; row < m_iVertSize ; ++row )
			for( auto col = 0 ; col < m_iHorSize; col++ )   {
				if (row < m_iVertSize - 1) {
					m_vVertLine[col][row].Init(EDirection::Bottom, -3.0f, m_fGameFieldX + col*m_SellWidth, m_fGameFieldY + row*m_SellHeight, m_SellHeight);
					i++;
				}
				if (col < m_iHorSize - 1) {
					m_vHorLine[col][row].Init(EDirection::Left, -3.0f, m_fGameFieldX + col*m_SellWidth, m_fGameFieldY + row*m_SellHeight, m_SellWidth);
					i++;
				}
			}

	}
}

int		COGLGame::StartProgressHourglass(float MinValue, float MaxValue)
{
	m_ControlHourglass.SetVisible(true);
	unsigned int id = 0;
	std::for_each(m_HourglassData.begin(), m_HourglassData.end(), [&id](const SHourglassData& HG) { id = max(id,HG.m_iId); });
	id++;
	m_HourglassData.emplace_back(SHourglassData{id, MinValue, MaxValue, 0.0f});
	//   Устанавливаем MinValue, MaxValue и Value по часам которые будут дольше всего тикать.
	float vMax, vMin, Val, maxdVal = 0.0f;
	for (auto hg : m_HourglassData) {
		if( hg.m_fMax - hg.m_fVal > maxdVal ) {
			maxdVal = hg.m_fMax - hg.m_fVal;
			vMax = hg.m_fMax;
			vMin = hg.m_fMin;
			Val = hg.m_fVal;
		}
	}
	m_ControlHourglass.SetMaxMin(vMin, vMax); 
	m_ControlHourglass.SetValue(Val);
	IO_TRACE("START : Hourglass  Id  = %d vMin = %f vMax = %f Val = %f  count = %d\n", m_HourglassData.size() - 1, vMin, vMax, Val, m_HourglassData.size());
	//
	return id;
}

void	COGLGame::StopProgressHourglass(unsigned int Id)
{
	VHourglassData::iterator it = std::find_if(m_HourglassData.begin(), m_HourglassData.end(), [Id](const SHourglassData& HG)-> bool { return Id == HG.m_iId; });
	if (it != m_HourglassData.end())
		m_HourglassData.erase(it);
	if (m_HourglassData.empty())
		m_ControlHourglass.SetVisible(false);
	float vMax, vMin, Val = m_ControlHourglass.GetValue();
	m_ControlHourglass.GetMaxMin(vMax, vMin);
	IO_TRACE("STOP : ControlHourglass : Id = %d  vMin = %f vMax = %f Val = %f  count = %d\n", Id, vMin, vMax, Val, m_HourglassData.size());
}

void	COGLGame::SetValueProgressHourglass(unsigned int Id, float Value)
{
	VHourglassData::iterator it = std::find_if(m_HourglassData.begin(), m_HourglassData.end(), [Id](const SHourglassData& HG)-> bool { return Id == HG.m_iId; });
	if( it != m_HourglassData.end() ) {
		(*it).m_fVal = min((*it).m_fMax, Value );
		//   Устанавливаем MinValue, MaxValue и Value почасам которые будут дольше всего тикать.
		float vMax, vMin, Val, maxdVal = 0.0f;
		for (auto hg : m_HourglassData) {
			if (hg.m_fMax - hg.m_fVal >= maxdVal) {
				maxdVal = hg.m_fMax - hg.m_fVal;
				vMax = hg.m_fMax;
				vMin = hg.m_fMin;
				Val = hg.m_fVal;
			}
		}
		m_ControlHourglass.SetMaxMin(vMin, vMax);
		m_ControlHourglass.SetValue(Val);
		//IO_TRACE("SET : Hourglass  Id  = %d vMin = %f vMax = %f Val = %f  count = %d\n", Id, vMin, vMax, Val, m_HourglassData.size());
	}
}


void	COGLGame::onLMouseDown(int X, int Y) noexcept
{
	m_bMouseDown = true;
	m_iXprev = X;
	m_iYprev = Y;
	//COGLControl* contr = COGLControlManager::GetInstance().OnLMouseDown(Point);

}

void	COGLGame::onLMouseUp(int X, int Y) noexcept
{
	m_bMouseDown = false;
	m_player.m_bMouseRIGHT = false;
	m_player.m_bMouseLEFT = false;
	m_player.m_bMouseDOWN = false;
	m_player.m_bMouseUP = false;
}

void	COGLGame::onMouseMove(int X, int Y) noexcept
{
	int dX = m_iXprev - X;
	int dY = m_iYprev - Y;
	if (m_bMouseDown  && ( abs(dX) >= 4 || abs(dY) >= 4 )) {
		m_player.m_bMouseRIGHT = false;
		m_player.m_bMouseLEFT = false;
		m_player.m_bMouseDOWN = false;
		m_player.m_bMouseUP = false;
		
		
		if (abs(dX) > abs(dY)) {  //  двигаем по горизонтали
			if (dX < 0)
				m_player.m_bMouseRIGHT = true;
			else
				m_player.m_bMouseLEFT = true;
		}
		else {
			if (dY < 0)
				m_player.m_bMouseDOWN = true;
			else
				m_player.m_bMouseUP = true;
		}
		m_iXprev = X;
		m_iYprev = Y;
	}
	
}

//   IControlEvent

void	COGLGame::onButtonDown(const COGLButton* Control)  noexcept
{
	if (Control == &m_btnPause) {
		m_bPause = true;
		ViewPanel(m_bPause, m_ePlayState);
	}
}


void	COGLGame::onButtonUp(const COGLButton* Control)  noexcept
{
	if (Control == &m_btnPause || Control == &m_PanelButton) {
		m_bPause = false;
		ViewPanel(m_bPause, m_ePlayState);
		m_btnPause.SetPressed(m_bPause);
		PlayStateTreatment();
	}
	else if (Control == &m_StartButton) {
		m_bPause = false;
		m_ePlayState = EPlayState::Normal;
		m_StartPanel.SetVisible(false);
		m_GamePanel.SetVisible(true);
		m_ControlHourglass.SetVisible(false);
	}
}


void	COGLGame::onLButtonDown(const COGLControl* Control)  noexcept
{
	if (Control == &m_ControlSpeed) {
		m_iSpeed++;
		if (m_iSpeed == m_iNumSpeeds)
			m_iSpeed = 4;
		m_ControlSpeed.SetColor(m_SpeedColor[m_iSpeed], m_SpeedColor[m_iSpeed]);
		for (auto& enemy : m_vEnemy)				
			enemy.ResetObject(true);

	}
}


void	COGLGame::onLButtonUp(const COGLControl* Control)  noexcept
{
}

void	COGLGame::onMouseMove(const COGLControl* Control)  noexcept
{
	if (m_bMouseDown) return;
	/*CGamePlayer::m_bMouseRIGHT = false;
	CGamePlayer::m_bMouseLEFT = false;
	CGamePlayer::m_bMouseDOWN = false;
	CGamePlayer::m_bMouseUP = false;*/
	/*if (Control == &m_ControlArrowLeft)
		m_player.m_bMouseLEFT = true;
	else if (Control == &m_ControlArrowRight)
		m_player.m_bMouseRIGHT = true;
	else if (Control == &m_ControlArrowUp)
		m_player.m_bMouseUP = true;
	else if (Control == &m_ControlArrowDown)
		m_player.m_bMouseDOWN = true;*/
}

void	COGLGame::onMouseLeave(const COGLControl* Control)  noexcept
{
	if (m_bMouseDown) return;
	/*if (Control == &m_ControlArrowLeft)
		m_player.m_bMouseLEFT = false;
	else if (Control == &m_ControlArrowRight)
		m_player.m_bMouseRIGHT = false;
	else if (Control == &m_ControlArrowUp)
		m_player.m_bMouseUP = false;
	else if (Control == &m_ControlArrowDown)
		m_player.m_bMouseDOWN = false;*/
}

void	COGLGame::onResizeWindow(float Width, float Height)   noexcept
{
	ResizeScene(Width, Height);
}