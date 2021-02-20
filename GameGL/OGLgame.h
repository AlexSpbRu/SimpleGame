#pragma once

#include <thread>
#include <future>

#include "Observable.h"


//-------------------------------------------------------------------------------------

class CPlaySound : public Singleton< CPlaySound >
{
	std::vector<  std::future<int> >	m_fuThreads;
public:
	enum class EPlayType {
		Once,
		Loop
	};
protected :
	void	play_sound(const std::string& FileName, EPlayType  Type);
public :

	int playSound(const std::string& FileName, EPlayType  Type);
	void stopPlay(int Id);
};

class COGLGame;

struct	CGameObject												
{
	//	буферы вертексов и текстур
	static constexpr float m_fVertex[] = { -1.0f , +1.0f, 0.0f,
		-1.0f , -1.0f, -0.0f,
		+1.0f , +1.0f, -0.0f,
		-1.0f , -1.0f, -0.0f,
		+1.0f , -1.0f, -0.0f,
		+1.0f , +1.0f, -0.0f,
	};
	static constexpr float m_fTexCoord[] = { 0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	static constexpr size_t	m_iVertAmount = sizeof(m_fVertex) / (3 * sizeof(GLfloat));
	//
	COGLGame*		m_pGame = nullptr;
	GLfloat			m_fFx = 0;				// Fine Movement Position
	GLfloat			m_fFy = 0;
	int				m_iX = 0;				// Current Player Position
	int				m_iY = 0;				
	float			m_fSpin = 0.0f;			// Spin Direction
	EDirection		m_eDirection = EDirection::None;
	CTextureManager::EType		m_eTextureType = CTextureManager::EType::None;
	// для рисования с помощью буферов
	size_t			m_iStartPos = 0;
	size_t			m_iAmount = 0;
	//
	CGameObject(COGLGame* Game) : m_pGame(Game) {};
	virtual ~CGameObject() {};
	virtual GLvoid	ProcessKeyboard()  noexcept {};
	//
	virtual GLvoid	Move()  noexcept {};
	virtual void ResetObject(bool NewLvel) = 0;
	CTextureManager::EType	GetTextute()   noexcept { return m_eTextureType; };
};




struct CGamePlayer : public CGameObject
{
	bool			m_bKeyRIGHT = false;
	bool			m_bKeyLEFT = false;
	bool			m_bKeyDOWN = false;
	bool			m_bKeyUP = false;
	bool			m_bMouseRIGHT = false;
	bool			m_bMouseLEFT = false;
	bool			m_bMouseDOWN = false;
	bool			m_bMouseUP = false;
	//
	GLvoid	ProcessKeyboard(const bool* Keys, size_t CountKeys);
	//
	CGamePlayer(COGLGame* Game) : CGameObject(Game)  {	};
	virtual ~CGamePlayer() {};
	GLvoid	Move()  noexcept;
	void ResetObject(bool NewLvel)  noexcept;

};


struct CGamePlayerImage : public CGamePlayer
{
protected :
public :
	CGamePlayerImage(COGLGame* Game) : CGamePlayer(Game) { m_eTextureType = CTextureManager::EType::Player;  };
	virtual ~CGamePlayerImage();
#ifndef OPENGL_SHADER
	//  новое рисование через буферы
	void	Draw(CTextVertexColorBuffer& Buffer) const noexcept;
#else
	//   Рисование шейдерами
	void	DrawES(CTextVertexColorBuffer& Buffer) const noexcept;
#endif
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
class CEnemyModelPhysical;

class CEnemyModel
{
public :
	enum struct EModelType {
		Simple,
		Physical
	};
protected :
	static COGLGame*		m_pGame;
	CEnemyModel() {};
	using MapModels = std::map< EModelType, CEnemyModel* >;
	static MapModels		m_MapModels;
public :
	static	void		Init(COGLGame* Game)  noexcept;
	static	CEnemyModel*	GetModel( EModelType  Type)  noexcept;
	virtual EDirection	CalcDirection( int& X, int& Y, GLfloat Fx, GLfloat Fy )  noexcept;
};

class CEnemyModelPhysical : public CEnemyModel
{
	friend CEnemyModel;
protected:
	CEnemyModelPhysical() {};
public:
	EDirection	CalcDirection(int& X, int& Y, GLfloat Fx, GLfloat Fy)  noexcept;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CGameEnemy : public CGameObject
{
	enum class State {
		Active,
		NotActive,
		Dead
	};
	State				m_State = State::Active;
	size_t				m_iSpeed = 0;   // определяет скорость передвижения
	CEnemyModel*		m_pModel = nullptr;
	//
	void Check() const noexcept {
		if (m_State == CGameEnemy::State::Dead && (m_fFx >= 0 || m_fFy >= 0 || m_iX >= 0 || m_iY >= 0 ) )
			__debugbreak();
		if (m_State != CGameEnemy::State::Dead && (m_fFx < 0 || m_fFy < 0 || m_iX < 0 || m_iY < 0))
			__debugbreak();
	};
	CGameEnemy(COGLGame* Game) : CGameObject(Game) { 
		m_pModel = CEnemyModel::GetModel(CEnemyModel::EModelType::Simple);
		m_eTextureType = CTextureManager::EType::Enemy;
	};
	virtual ~CGameEnemy() {/* m_Image.DeleteTexture();*/ };
	GLvoid	Move() noexcept;
	bool	Deactivate() noexcept;
	void	ResetObject(bool NewLvel) noexcept;
#ifndef OPENGL_SHADER
	//  новое рисование через буферы
	void	Draw(CTextVertexColorBuffer& Buffer) const noexcept;
#else
	//   Рисование шейдерами
	void	DrawES(CTextVertexColorBuffer& Buffer) const noexcept;
#endif
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CGamePrize : public CGameObject
{
	enum class EType {
		None,
		Hourglass,
		Bomb
	};
	enum struct EState {
		NotActive,
		Active,
		Activated,
		Start,
		Stop
	};
//	CGLTexture*			m_Image;
	//size_t				m_iLifetime = 0;
	EState				m_eActive = EState::NotActive;
	EType				m_eType;
//	static EType		m_ePrevType;// = EType::None;
	CGamePrize(COGLGame* Game, EType Type ) : CGameObject(Game), m_eType(Type) { };
	virtual ~CGamePrize() {};
	virtual GLvoid	Move()  noexcept = 0;
	virtual void	ResetObject(bool NewLvel) {};
#ifndef OPENGL_SHADER
	//  новое рисование через буферы
	void	Draw(CTextVertexColorBuffer& Buffer) const noexcept;
#else
	//   Рисование шейдерами
	void	DrawES(CTextVertexColorBuffer& Buffer) const noexcept;
#endif
};

struct CGameHourglass : public CGamePrize
{
	using steady_time = std::chrono::time_point<std::chrono::steady_clock>;
	steady_time		m_StartTime;
	GLfloat			m_fDuration = 1000.0f;      // продолжительность действия в млсек
	GLfloat			m_fDeceleration = 1.0f;		//  замедление движения врагов ( от 1.0 до ... )
	unsigned int	m_HourglassId = 0xffff0000;
	CGameHourglass(COGLGame* Game) : CGamePrize(Game, EType::Hourglass) { m_eTextureType = CTextureManager::EType::Hourglass; };
	GLvoid	Move() noexcept;
	void	ResetObject(bool NewLvel) noexcept;
};

struct CGameBomb : public CGamePrize
{
	CGameBomb(COGLGame* Game) : CGamePrize(Game, EType::Bomb) {
		m_eTextureType = CTextureManager::EType::Bomb;
	};
	GLvoid	Move() noexcept;
	void	ResetObject(bool NewLvel) {};
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////

enum struct EGameLiveState {
	Active,
	Fall,
	NotActive
};

struct CGameLive : public CGameObject
{
	//
	EGameLiveState				m_State = EGameLiveState::Active;
	GLfloat						m_iAlpha = 1.0f;
	GLfloat						m_fScale = 1.0f;

	CGameLive(COGLGame* Game);
	virtual ~CGameLive() {};
	virtual GLvoid	Move()  noexcept;
	//  задаются относительные координаты
	void	SetPosition(GLfloat X, GLfloat Y)  noexcept { m_fFx = X; 	m_fFy = Y; };   
	void	ResetObject(bool NewLvel) noexcept;
#ifndef OPENGL_SHADER
	//  новое рисование через буферы
	void	Draw(CTextVertexColorBuffer& Buffer) const noexcept;
#else
	//   Рисование шейдерами
	void	DrawES(CTextVertexColorBuffer& Buffer) const noexcept;
#endif
};

//  Вращающийся барабан
//struct CSymbolDrum
//{
//	int			m_iNumSign = 11;
//	GLfloat		m_fRad = 1.0f;
//	GLfloat		m_fWidth = 1.0f;
//	GLfloat		m_fHeight = 1.0f;
//	GLfloat		m_fX = 1.0f;     //  положение оси барабана - левая точка
//	GLfloat		m_fY = 1.0f;
//	GLfloat		m_fZ = 1.0f;
//	//
//	float		m_fAngle = -10.5f *M_PIF*2.0f / m_iNumSign;// -M_PIF / m_iNumSign;   //  соответствует - 0
//	float		m_fStopAngle = 0.0f;
//	//
//	int			m_iSymbol = 0;		// 0 - 0, 1 -1 , .... -1 - пустое поле
//	CSymbolDrum() {};
//	CSymbolDrum(GLfloat Height, GLfloat Width, GLfloat X, GLfloat Y, GLfloat Z) : m_fRad(Height/(2.0f*sin(M_PIF/m_iNumSign))), m_fWidth(Width),
//								m_fX(X), m_fY(Y+Height/2.0f), m_fZ(-m_fRad/*Z*/), m_fHeight(Height) {};
//	void SetSymbol(int Symbol) { 
//		assert(m_iSymbol >= 0 && m_iSymbol < m_iNumSign ); 
//		m_iSymbol = Symbol; 
//		auto newStop = - M_PIF*2.0f / m_iNumSign*(0.5f + m_iSymbol);
//		if (m_fStopAngle != newStop) {
//			m_fStopAngle = newStop;
//			while (m_fAngle < m_fStopAngle)
//				m_fAngle += 2.0f*M_PIF;
//		}
//	};
//
//	void GetVertexTexture( std::vector<GLfloat>&  Vertex,  std::vector<GLfloat>&  Tex,  std::vector<unsigned int>&  Index ) const;
//
//	void	Move() { 
//		if( m_fAngle > m_fStopAngle )
//			m_fAngle -=  0.1f; 
//		else {
//			m_fAngle = m_fStopAngle;
//		}
//	};
//};
//
//struct SlidingTexture
//{
//	int			m_iNumSign = 11;
//	GLfloat		m_fWidth = 1.0f;
//	GLfloat		m_fHeight = 1.0f;
//	GLfloat		m_fX = 1.0f;     //  положение верхней левой точки
//	GLfloat		m_fY = 1.0f;
//	GLfloat		m_fZ = 1.0f;
//	//
//	float		m_fSlider = 1.0f - 1.0f / m_iNumSign*10;
//	float		m_fStopSlider = 0.0f;
//	bool		m_bDown = false;
//	//
//	int			m_iSymbol = 0;		// 0 - 0, 1 -1 , .... -1 - пустое поле
//
//	SlidingTexture() {};
//	SlidingTexture(GLfloat Height, GLfloat Width, GLfloat X, GLfloat Y, GLfloat Z) :  m_fWidth(Width),
//		m_fX(X), m_fY(Y), m_fZ(Z), m_fHeight(Height) {};
//	void SetSymbol(int Symbol) {
//		assert(m_iSymbol >= 0 && m_iSymbol < m_iNumSign);
//		m_iSymbol = Symbol;
//		m_fStopSlider = 1.0f - 1.0f / m_iNumSign*m_iSymbol;
//		if (m_fSlider < m_fStopSlider)
//			m_bDown = true;
//	};
//
//	CTextureManager::EType GetTexture() { return CTextureManager::EType::Digits; };
//
//	void GetVertexTexture(std::vector<GLfloat>&  Vertex, std::vector<GLfloat>&  Tex, unsigned int IndexStart,  std::vector<unsigned int>&  Index) const;
//	void GetVertexTextureES( float Width, float Height, std::vector<GLfloat>&  Vertex, std::vector<GLfloat>&  Tex, unsigned int IndexStart, std::vector<unsigned int>&  Index) const;
//
//	void	Move() {
//		if (m_bDown) {
//			m_fSlider -= 0.01f;
//			if (m_fSlider < 1.0f - 1.0f / m_iNumSign * 10) {
//				m_fSlider = 1.0f;
//				m_bDown = false;
//			}
//		}
//		else if (m_fSlider > m_fStopSlider) {
//			m_fSlider -= 0.01f;
//			m_fSlider = max(m_fSlider, m_fStopSlider);
//		}
//	};
//};
//
//template < int NumDigit, class DrumType >
//struct CNumberDrum : public CGameObject
//{
//	DrumType			m_Drums[NumDigit];
//	GLfloat				m_fWidthDigit = 10.0f;
//	GLfloat				m_fHeightDigit = 16.0f;
//	CNumberDrum(COGLGame* Game) : CGameObject(Game) { m_eTextureType = m_Drums[0].GetTexture(); };
//
//	void SetPos(GLfloat X, GLfloat Y, GLfloat Z,  //  координаты нижнего левого угла (как при выводе текста )
//				GLfloat Width, GLfloat Height) {
//		auto x = X;
//#ifdef OPENGL_SHADER
//		Y = /*m_pGame->GetMaxHeight()*/1.0f - Y;
//#endif
//		for (auto& dr : m_Drums) {
//			dr = std::move(DrumType{Height, Width, x, Y, Z });
//			x += Width;
//		}
//	};
//
//	void SetNumber(int Num) {
//		int number = Num;
//		for (auto i = 0; i < NumDigit; ++i) {
//			auto nn = number - (number / 10) * 10;
//			if (number == 0)
//				nn = 10;
//			m_Drums[NumDigit - i - 1].SetSymbol(nn);
//			number = number / 10;
//		}
//	};
//
//	void	ResetObject(bool NewLvel) noexcept {};
//	GLvoid	Move()  noexcept { for (auto& dr : m_Drums) dr.Move(); };
//#ifndef OPENGL_SHADER
//	//  новое рисование через буферы
//	void	Draw(CTextVertexColorBuffer& Buffer) const noexcept {
//		CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Digits)->Bind();
//		//
//		std::vector< GLfloat>	vertex, tex;
//		std::vector< unsigned int >		index;
//		//
//		for (auto& dr : m_Drums) {
//			dr.GetVertexTexture(vertex, tex, Buffer.Count(), index);
//			Buffer.AddVertex(&vertex[0], &vertex[0] + vertex.size() );
//			Buffer.AddTexture(&tex[0], &tex[0] + tex.size());
//			Buffer.AddIndex(&index[0], &index[0] + index.size());
//			Buffer.SetColor(SOGLColor{1.0f, 1.0f, 1.0f, 1.0f });
//			//Buffer.DrawElements(GL_TRIANGLES);
//		}
//		
//	}
//#else
//	//   Рисование шейдерами
//	void	DrawES(CTextVertexColorBuffer& Buffer) const noexcept {
//		std::vector< GLfloat>	vertex, tex;
//		std::vector< unsigned int >		index;
//		//
//		for (auto& dr : m_Drums) {
//			dr.GetVertexTextureES(m_pGame->GetMaxWidth(), m_pGame->GetMaxHeight(),vertex, tex, Buffer.Count(), index);
//			Buffer.AddVertex(&vertex[0], &vertex[0] + vertex.size());
//			Buffer.AddTexture(&tex[0], &tex[0] + tex.size());
//			Buffer.AddIndex(&index[0], &index[0] + index.size());
//			Buffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.8f });
//		}
//	};
//#endif
//};

struct SWayPart : CGameObject
{
	bool			m_bChecked = false;
	GLfloat			m_fZ = -3.0f;
	GLfloat			m_fXstop = 0.0f;	// координаты конечной точки отрезка
	GLfloat			m_fYstop = 0.0f;	//  координаты начальной точки лежат в CGameObject - m_fFx,	m_fFy
	SOGLColor		m_StartColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	SOGLColor		m_StopColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	explicit SWayPart(COGLGame* Game) : CGameObject( Game ) {};
	void Init(EDirection	Direction, GLfloat Z, GLfloat Xstart, GLfloat Ystart, GLfloat Length ) {
		m_bChecked = false;
		m_eDirection = Direction;
		m_fZ = Z; 
		m_fFx = Xstart;		m_fFy = Ystart;
		switch (Direction) {
			case EDirection::Bottom	: m_fXstop = m_fFx;	m_fYstop = m_fFy + Length; break;
			case EDirection::Left	: m_fXstop = m_fFx + Length;	m_fYstop = m_fFy; break;
			case EDirection::Right	: m_fXstop = m_fFx - Length;	m_fYstop = m_fFy; break;
			case EDirection::Top	: m_fXstop = m_fFx;	m_fYstop = m_fFy - Length; break;
			case EDirection::None	: assert(false); break;
		}
	};
	void ResetObject(bool NewLvel) {};
	bool& operator () () {  return m_bChecked; };
#ifndef OPENGL_SHADER
	//  новое рисование через буферы
	void	Draw(CTextVertexColorBuffer& Buffer) const noexcept {
		Buffer.AddVertex(m_fFx, m_fFy, m_fZ); 
		Buffer.AddVertex(m_fXstop, m_fYstop, m_fZ);
		Buffer.AddColor(m_StartColor);
		Buffer.AddColor(m_StopColor);
	};
#else
	//   Рисование шейдерами
	void	DrawES() const noexcept;
#endif
};



class COGLGame : public IKeyboardEvent, public IMouseEvent,  public IControlEvent, public IResizeWindow
{
protected :
	friend CGameObject;
	friend CGamePlayer;
	friend CGameEnemy;
	friend CGamePrize;
	friend CGameHourglass;
	friend CGameBomb;
	friend CGamePlayerImage;
	friend CGameLive;
	friend CEnemyModel;
	friend CEnemyModelPhysical;
	//
	enum struct EPrize {
		HourGlass
	};

	enum struct EPlayState {
		Normal,
		Die,
		Gameover,
		LevelComplete,
		GameStart
	};


	volatile EPlayState		m_ePlayState = EPlayState::GameStart;
	void		PlayStateTreatment();
	//	относительные размеры элементов интерфейса и игровых объектов ( относительно размеров окна / экрана	).
	const	float		m_fSmallFontConst = 0.025f;
	const	float		m_fMediumFontConst = 0.035f;
	const	float		m_fLargeFontConst = 0.07f;

	const	float		m_fControlHeightConst = 0.07f;

	const	float		m_fGameObjectSizeConst = 0.025f;
	//			конкретные размеры элементов
	float		m_fSmallFont;
	float		m_fMediumFont;
	float		m_fLargeFont;
	float		m_fControlHeight;
	float		m_fGameObjectSize;
	//
public :
	inline  float	SmallFont() { return m_fSmallFont; };
	inline  float	MediumFont() { return m_fMediumFont; };
	inline  float	LargeFont() { return m_fLargeFont; };
	inline  float	ControlHeight() { return m_fControlHeight; };
	inline  float	GameObjectSize() { return m_fGameObjectSize; };

	inline	float	SizeX(float X) { return m_fMaxWidth*X; };
	inline	float	SizeY(float Y) { return m_fMaxHeight*Y; };
protected:
	//
	std::string		m_AppPath;
	bool			m_keys[256];					// Array Used For The Keyboard Routine
	//
	std::vector<GLfloat>	m_LightAmbient = { 0.9f, 0.9f, 0.9f, 1.0f };
	std::vector<GLfloat>	m_LightDiffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
	std::vector<GLfloat>	m_LightSpecular = { 1.0f, 1.0f,  1.0f, 1.0f };
	std::vector<GLfloat>	m_LightPosition = { 500.0f, 500.0f, 2000.0f, 1.0f };
	//
	CGLTexture		m_Background;
	size_t			m_iCountBack = 28;
	std::vector< char >	m_vUsedBack;
	size_t			GetNewBackgroundNum();
	std::vector< std::string >		m_BackNames;
	using steady_time = std::chrono::time_point<std::chrono::steady_clock>;
	steady_time		m_CurrentTime;
	//
	CGLTexture*		m_Line;
	//
	int				m_iHorSize = 11;
	int				m_iVertSize = 11;
	GLfloat			m_SellHeight = 45.0f;
	GLfloat			m_SellWidth = 75.0f;
	GLfloat			m_fGameFieldX = 50.0f;
	GLfloat			m_fGameFieldY = 120.0f;
	std::vector< std::vector<SWayPart> >	m_vVertLine;// [11][10];		// Keeps Track Of Verticle Lines
	std::vector< std::vector<SWayPart> >	m_vHorLine;// [10][11];			// Keeps Track Of Horizontal Lines
	inline	void CalcGridLineColor(SWayPart& Line) {
		SOGLColor lineColorStart = { 0.0f, 0.5f, 1.0f, 0.25f }, lineColorStop = { 0.0f, 0.5f, 1.0f, 0.25f };
		if ( Line() == 1)					
			lineColorStart = lineColorStop = { 1.0f, 1.0f, 1.0f, 0.5f };
		float minStartDist = 1.0e15f, minStopDist = 1.0e15f;
		for( auto& en : m_vEnemy ) {
			if (en.m_State == CGameEnemy::State::Active) {
				minStartDist = min(minStartDist, (Line.m_fFx - en.m_fFx - m_fGameFieldX)*(Line.m_fFx - en.m_fFx - m_fGameFieldX) + 
					(Line.m_fFy - en.m_fFy - m_fGameFieldY)*(Line.m_fFy - en.m_fFy - m_fGameFieldY));
				minStopDist = min(minStopDist, (Line.m_fXstop - en.m_fFx - m_fGameFieldX)*(Line.m_fXstop - en.m_fFx - m_fGameFieldX) + 
					(Line.m_fYstop - en.m_fFy - m_fGameFieldY)*(Line.m_fYstop - en.m_fFy - m_fGameFieldY));
			}
		}
		//
		auto dist = max(m_SellWidth, m_SellHeight);
		auto red = dist*dist / minStartDist;
		if (red > 1.0f) {
			lineColorStart.b /= (lineColorStart.r + red);
			lineColorStart.g /= (lineColorStart.r + red);
			lineColorStart.r = 1.0f;
			lineColorStart.a = 0.75f;
		}
		red = dist*dist / minStopDist;
		if (red > 1.0f) {
			lineColorStop.b /= (lineColorStop.r + red);
			lineColorStop.g /= (lineColorStop.r + red);
			lineColorStop.r = 1.0f;
			lineColorStop.a = 0.75f;
		}
		//
		Line.m_StartColor = lineColorStart;
		Line.m_StopColor = lineColorStop;
	};
	int									m_iVertCount = 0;
	int									m_iHorCount = 0;
	//std::vector< SWayPart >				m_vLines;
	//bool			m_bGameover = false;
	int				m_iLives = 5;										// Player Lives
	int				m_iLevel = 1;										// Internal Game Level
	int				m_iLevel2 = 1;										// Displayed Game Level
	int				m_iStage = 1;										// Game Stage
	int				m_iDelay = 0;										// Enemy Delay
	bool			m_bFilled = true;									// Done Filling In The Grid?
	//bool			m_bLevelComplete = false;
	bool			m_bPause = false;
	//
	GLfloat			m_fMaxWidth = 1280.0f;		//  размер окна
	GLfloat			m_fMaxHeight = 1024.0f;
	//
	std::vector< int >			m_iSteps = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	const size_t				m_iNumSpeeds = m_iSteps.size();
	std::vector< SOGLColor >	m_SpeedColor = { {0.0f, 1.0f, 0.0f, 1.0f },
												{ 0.3f, 1.0f, 0.0f, 1.0f }, 
												{ 0.5f, 1.0f, 0.0f, 1.0f }, 
												{ 0.7f, 1.0f, 0.0f, 1.0f }, 
												{ 0.0f, 1.0f, 0.0f, 1.0f },
												{ 0.3f, 1.0f, 0.0f, 1.0f },
												{ 0.7f, 1.0f, 0.0f, 1.0f },
												{ 1.0f, 0.7f, 0.0f, 1.0f },
												{ 0.6f, 0.0f, 0.0f, 1.0f },
												{ 1.0f, 0.0f, 0.0f, 1.0f }
	};
	size_t			m_iSpeed = 5;
	//
	bool			m_bAp = true;										
	bool			m_bAnti = false;
	//
	size_t			m_iScore = 0;
	//
	bool			m_bMouseDown = false;
	//
	bool			m_btextureEnable;
	bool			m_balphaEnable;
	//
	CTextVertexColorBuffer	m_DrawBuffer;
	const size_t			m_iVertexSize	= 3;  //   размерности
	const size_t			m_iColorSize	= 4;
	const size_t			m_iTextureSize	= 3;
	//
	SOGLMatrix				m_ProjMatrix;
	SOGLMatrix				m_ModelMatrix;
	SOGLMatrix				m_ResultMatrix;
	//
	//CFontImage				m_FontES;
	//CFontImageManager		m_FontsManager;
	//
	Shader*					m_Shader = nullptr;
	GLuint					m_iVertShAttr = 0;		//  координата вершины
	GLuint					m_iTexCoordLoc = 0;		//	текстурная координгата
	GLuint					m_iColorLoc = 0;		//	цвет вершины
	GLuint					m_iSamplerLoc = 0;		//	номер текстуры
	GLuint					m_iMatrix = 0;			//	матрица преобразования кординат координгата
	//   для управления движением Игроком при помощи мыши
	int						m_iXprev = -1;
	int						m_iYprev = -1;
	//
	CGamePlayerImage				m_player;
	std::vector< CGameEnemy >		m_vEnemy;			// Enemy Information
	std::vector< CGamePrize* >		m_Prizes;
	std::vector< CGameLive >		m_vLives;
	//   элементы управления
	/*COGLControl				m_ControlArrowLeft;
	COGLControl				m_ControlArrowRight;
	COGLControl				m_ControlArrowUp;
	COGLControl				m_ControlArrowDown;*/
	COGLButton				m_ControlSpeed;
	COGLRoundProgressBar	m_ControlHourglass;
	//
	COGLPanel				m_SplashPanel;				//  заставка
	//
	COGLPanel				m_StartPanel;				//  стартовая панель с кнопками управления
	COGLButton				m_StartButton;				//  элементы управления на панели
	//
	COGLPanel				m_GamePanel;				//  фиктивная панель, на которой расположены контролы на игровом поле

	COGLStaticText			m_GameNameContr;
	COGLStaticText			m_ScoreContr;
	COGLStaticText			m_StageContr;
	COGLStaticText			m_LeveContrl;
	//
	COGLPanel				m_ControlPanel;
	COGLStaticText			m_PanelText;				// элементы управления на панели
	COGLButton				m_PanelButton;
	//
	/*CSlideNumberDrumControl*/CNumberDrumControlSlide< 6 > 		m_ScoreControl;
	/*CSlideNumberDrumControl*/CNumberDrumControlSlide< 2 > 		m_StageControl;
	/*CSlideNumberDrumControl*/CNumberDrumControlSlide< 2 > 		m_LevelControl;
	
	COGLButton				m_btnPause;
	//
	void		CalcNewPosition();
	void		ResetObjects( bool NewLvel );
	void		CheckPrize(int X, int Y);
	void		CalcCellsSize();
	void		ResetLives();
#ifndef OPENGL_SHADER
	void		InitGL() noexcept;
#else
	void		InitGLES() noexcept;
	//void		DrawBuffer(CGLTexture* Texture, CTextVertexColorBuffer& Buffer ) noexcept;
#endif
	struct SHourglassData {
		unsigned int m_iId = 0xffff0000;
		float	m_fMin = 0.0f;
		float	m_fMax = 0.0f;
		float	m_fVal = 0.0f;
		SHourglassData(unsigned int Id, float Min, float Max, float Val) : m_iId(Id), m_fMin(Min), m_fMax(Max), m_fVal(Val) {};
	};
	using  VHourglassData = std::vector<SHourglassData>;
	VHourglassData		m_HourglassData;
	int			StartProgressHourglass(float MinValue, float MaxValue );
	void		StopProgressHourglass(unsigned int Id);
	void		SetValueProgressHourglass(unsigned int Id, float Value );
	void		FindSuitableBackground();
	void		ViewPanel(bool Visible, EPlayState State);
public:
	COGLGame( int Width, int Height );
	~COGLGame();
	bool		Init( int Width, int Height ) noexcept ;
	void		GetWindowSize(int& Width, int& Height)  noexcept;
	void		SetMaxSize( int Width, int Height )  noexcept;

	inline GLfloat		sizeX(GLfloat X) { return X*m_fMaxWidth; };
	inline GLfloat		sizeY(GLfloat Y) { return Y*m_fMaxHeight; };
	//inline GLfloat		Xdist_from(GLfloat X, EHorAlign  Bind ) {
	//	switch (Bind) {
	//	case EHorAlign::Right: return ( 1.0f - X )*m_fMaxWidth;
	//	default : return X*m_fMaxWidth;
	//	}
	//};
	//inline GLfloat		Ydist_from(GLfloat Y, EVertAlign  Bind ) {
	//	switch (Bind) {
	//	case EVertAlign::Down: return (1.0f - Y)*m_fMaxHeight;
	//	default: return Y*m_fMaxHeight;
	//	}
	//};

	float		GetMaxHeight() { return m_fMaxHeight; };
	float		GetMaxWidth() { return m_fMaxWidth; };
#ifndef OPENGL_SHADER
	bool		DrawSceneFast() ;
#else
	//void		LoadIdentity()  noexcept { glhLoadIdentityf2(&m_ProjMatrix); };
	//void		AffineTransformation2D(const Vec3& Offset, GLfloat Angle, const Vec3& RotAxis, const Vec3& Scale) noexcept;
	//void		AffineTransformation2D(const Vec3&& Offset, GLfloat Angle, const Vec3&& RotAxis, const Vec3&& Scale) noexcept;
	//void		AddVertices(const GLfloat* Vertises, int VertSize, const GLfloat* Texture, int TexSize, const SOGLColor& Color) noexcept;
	bool		DrawSceneES() noexcept;
#endif
	void		ProcessKeyboard() noexcept;
	//
	void		SetControlPosition()  noexcept;			//  задание положения контролов в зависимости от размера окна программы
	void		ResizeScene(GLsizei width, GLsizei height) noexcept ;
	//
	void		ActivatePrize( int X, int Y, bool Horizontal ) noexcept;  // помечаем линию ( горизонтальную если Horizontal = true ) с коорд. X,Y как пройденную 
					// и проверяем не надo ли открыть прямоугольник и нет ли там приза
	//
	void	onKeyDown(char Key)  noexcept {
		m_keys[Key] = true;
		if ( Key == ' ' && ( m_ePlayState == EPlayState::Normal || m_bPause ) ) {									// If Spacebar Is Being Pressed
		   // Pause
			m_bPause = !m_bPause;
			ViewPanel(m_bPause, m_ePlayState);
			if( !m_bPause )
				PlayStateTreatment();
			m_btnPause.SetPressed( m_bPause );
		}
		//IO_TRACE("Key Down : %d\n", Key);
	}
	void	onKeyUp(char Key)  noexcept {
		m_keys[Key] = false;
		//IO_TRACE("Key Up : %d\n", Key);
	}
	//	 IMouseEvent
	void	onMouseMove(int X, int Y)  noexcept;
	void	onLMouseDown(int X, int Y)  noexcept;
	void	onLMouseUp(int X, int Y)  noexcept;
	//   IButtonEvent
	void	onButtonDown(const COGLButton* Control)  noexcept;
	void	onButtonUp(const COGLButton* Control)  noexcept;
	//   IControlEvent
	void	onLButtonDown(const COGLControl* Control)  noexcept;
	void	onLButtonUp(const COGLControl* Control)  noexcept;
	void	onMouseMove(const COGLControl* Control)  noexcept;
	void	onMouseLeave(const COGLControl* Control)  noexcept;
	//	IResizeWindow
	void	onResizeWindow(float Width, float Height)  noexcept;
};
