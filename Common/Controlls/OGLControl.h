#pragma once

#include "Observable.h"
#include "Patterns.h"

enum class EHorAlign {
	No,
	Left,
	Center,
	Right,
	Full,
	RightBind,
};

enum class EVertAlign {
	No,
	Up,
	Center,
	Down,
	Full,
	DownBind
};

class COGLControl
{
protected :
	bool		m_bIsClicable = false;
	bool		m_bVisible = true;
	bool		m_bDisable = false;
	//
	EHorAlign	m_eHorAlign = EHorAlign::No;
	EVertAlign	m_eVertAlign = EVertAlign::No;
	//
	Vec3		m_LeftTop;				// относительные  координаты левого верхнего угла контрола ( для текста линия над которой расположен текст )
	GLfloat		m_fWidth = 0.0f;		// относительный размер контрола
	GLfloat		m_fHeight = 0.0f;
	//
	Vec3		m_visLeftTop;			//  фактическое положение контрола  - для рисования
	GLfloat		m_fvisWidth = 0.0f;		//  размер контрола
	GLfloat		m_fvisHeight = 0.0f;
	//
	SOGLColor	m_Color = { 1.0f, 1.0f, 1.0f, 1.0 };
	SOGLColor	m_ActiveColor = { 1.0f, 1.0f, 1.0f, 1.0 };
	SOGLColor	m_CurrentColor;
	ERotation	m_eRottation = ERotation::R0;
	CTextureManager::EType		m_iTextureType = CTextureManager::EType::None;
	//
	COGLControl*				m_pParent = nullptr;
	//std::vector<COGLControl*>	m_vChilds;
	//static float	m_fWidthWindow;
	//static float	m_fHeightWindow;
public:
	//static void		SetWindowSize(float	Width, float Height) { m_fWidthWindow = Width; 	m_fHeightWindow = Height; };

	COGLControl() noexcept;
	COGLControl(CTextureManager::EType  Type) noexcept;
	virtual ~COGLControl() noexcept;
	//
	void SetAlign(EHorAlign HorAlign, EVertAlign	VertAlign) {
		m_eHorAlign = HorAlign;
		m_eVertAlign = VertAlign;
	};
	//
	void	SetParent(COGLControl* Parent) { m_pParent = Parent; };
	COGLControl* const	GetParent() const noexcept {
		return m_pParent;
	};
	//
	void	SetPosition( const Vec3&& LeftTop, float Width, float Height, ERotation  Rotation ) noexcept {
		m_LeftTop = LeftTop;
		m_fWidth = Width;
		m_fHeight = Height;
		m_eRottation = Rotation;
	};
	//
	void	SetPosition(const Vec3&& LeftTop, float Size,  ERotation  Rotation) noexcept {
		m_LeftTop = LeftTop;
		m_fWidth = Size;
		m_fHeight = -1.0f;
		m_eRottation = Rotation;
	};
	void	SetColor(const SOGLColor&& Color, const SOGLColor&& ActiveColor = { 1.0f, 1.0f, 1.0f, 1.0 } ) noexcept {
		m_Color = Color;
		m_CurrentColor = Color;
		m_ActiveColor = ActiveColor;
	};
	void	SetColor(const SOGLColor& Color, const SOGLColor& ActiveColor = { 1.0f, 1.0f, 1.0f, 1.0 })  noexcept {
		m_Color = Color;
		m_CurrentColor = Color;
		m_ActiveColor = ActiveColor;
	};
	//
	virtual void	SetVisible(bool Vis) noexcept { m_bVisible = Vis; };
	virtual bool	GetVisible() noexcept { return m_bVisible; };
	//
	void		SetDisable(bool Disable) noexcept { m_bDisable = Disable; };
	bool		GetDisable() noexcept { return m_bDisable; };
	//
	GLfloat Left() noexcept { return m_LeftTop.x;	};
	GLfloat Right()  noexcept { return m_LeftTop.x + m_fWidth;  };
	GLfloat Top() noexcept { return m_LeftTop.y; };
	GLfloat Bottom() noexcept { return m_LeftTop.y + m_fHeight; };

	GLfloat Width() noexcept { return  m_fWidth; };
	GLfloat Height() noexcept { return  m_fHeight; };

	GLfloat visLeft() noexcept { return m_visLeftTop.x;	};
	GLfloat visRight() noexcept { return m_visLeftTop.x + m_fvisWidth;  };
	GLfloat visTop() noexcept { return m_visLeftTop.y; };
	GLfloat visBottom() noexcept { return m_visLeftTop.y + m_fvisHeight; };
	//
	GLfloat Z() noexcept { return m_LeftTop.z; };
	virtual void	GetPosition(GLfloat Width, GLfloat Height )  noexcept;
	//
	ERotation	GetRotation()  noexcept { return m_eRottation; }
	SOGLColor	GetCurrentColor()  noexcept { return m_CurrentColor; };
	//
	CTextureManager::EType	GetTexType()  noexcept { return m_iTextureType; };
	//
	bool	IsClickable() noexcept { return m_bIsClicable; };

	virtual void	OnLMouseDown() noexcept;
	virtual void	OnLMouseUp() noexcept;
	virtual void	OnMouseMove() noexcept;
	virtual void	OnMouseLeave() noexcept;
	//
#ifndef OPENGL_SHADER
	virtual void	Draw(CTextVertexColorBuffer& Buffer) noexcept;
#else
	virtual void	DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height ) noexcept;
#endif
	//
	virtual void OnResizeWindow(float Width, float Height)   noexcept {
		GetPosition(Width, Height);
	};
};


class COGLButton : public COGLControl
{
protected:
	std::wstring	m_sText;
	int				m_iFont = 0;
	EHorAlign		m_eHorAlignText = EHorAlign::Center;
	EVertAlign		m_eVertAlignText = EVertAlign::Center;
	Vec3			m_TextPos;  //  координаты левого нижнего угла прямоугольника, занимаемого текстом
	//
	CTextureManager::EType		m_iTextureTypePressed = CTextureManager::EType::None;
	bool			m_bPressed = false;
	bool			m_bFixedButton = false;
	//
	void	SetTextPosition();
public:
	COGLButton() noexcept : COGLControl(CTextureManager::EType::None) { m_bIsClicable = true; };
	COGLButton(CTextureManager::EType  Type, CTextureManager::EType  Pressed) noexcept : COGLControl(Type), m_iTextureTypePressed(Pressed) { m_bIsClicable = true; };
	GLvoid	SetFixed(bool Fixed)   noexcept { m_bFixedButton = Fixed; };
	bool	GetFixed()   noexcept { return m_bFixedButton; };
	//
	GLvoid	SetPressed(bool Pressed)  noexcept {
		m_bPressed = Pressed; 
		m_CurrentColor = m_bPressed ? m_ActiveColor : m_Color;
	};

	bool	GetPressed()   noexcept { return m_bPressed; };
	//
	void SetTextAlign(EHorAlign HorAlign, EVertAlign VertAlign) {
		m_eHorAlignText = HorAlign;
		m_eVertAlignText = VertAlign;
		//SetTextPosition();
	}
	//
	void	GetPosition(GLfloat Width, GLfloat Height)  noexcept {
		COGLControl::GetPosition(Width, Height);
		SetTextPosition();
	}
	//
	void	OnLMouseDown() noexcept {
		if (!m_bFixedButton)
			m_bPressed = true;
		else
			m_bPressed = !m_bPressed;
		m_CurrentColor = m_bPressed ? m_ActiveColor : m_Color;
	};

	void	OnLMouseUp() noexcept {
		if( !m_bFixedButton )
			m_bPressed = false;
		m_CurrentColor = m_bPressed ? m_ActiveColor : m_Color;
	};

	void	OnMouseMove() noexcept {};
	void	OnMouseLeave() noexcept {};
	//
	void	SetFont(int Font)   noexcept { 
		m_iFont = Font;
		//SetTextPosition();
	};
	void	SetTextSize(GLfloat  Width, GLfloat  Height )  noexcept {
		CFontImageManager::GetInstance().GetFont(m_iFont).SetSize( Width, Height );
		//SetTextPosition();
	};
	void	SetText(const std::wstring& Text)   noexcept {
		m_sText = Text;
		//SetTextPosition();
	};
	const std::wstring&	GetText()  noexcept { return m_sText; };
	//
#ifndef OPENGL_SHADER
	void	Draw(CTextVertexColorBuffer& Buffer) noexcept;
#else
	void	DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept;
#endif
};


class COGLStaticText : public COGLControl
{
	std::wstring			m_sText;
	int						m_iFont = -1;
	GLfloat					m_fGlypthWdth = -1.0f;
	GLfloat					m_fGlypthHeight = -1.0f;
	float					m_fvisGlypthWdth = -1.0f;
	float					m_fvisGlypthHeight = -1.0f;
	void		CalculateControlSize()   noexcept;    //  вычисления прямоугольника занимаемого текстом
public :
	COGLStaticText() noexcept : COGLControl() {};
	COGLStaticText(CTextureManager::EType  Type) noexcept = delete;// : COGLControl(Type) {};
	//
	void	SetFont(int Font)  noexcept;
	//
	void	SetPosition(const Vec3& LeftDown, GLfloat GlyphWidth, GLfloat GlyphHeight )   noexcept;
	void	SetText(const std::wstring& Text)   noexcept;
	const std::wstring&	GetText()   noexcept { return m_sText; };
	void	GetPosition(GLfloat Width, GLfloat Height)  noexcept;
#ifndef OPENGL_SHADER
	void	Draw(CTextVertexColorBuffer& Buffer) noexcept;
#else
	void	DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept;
#endif
};


//----------------------------------------------------------------------------------------------------------------
// контролы имеющие собственную отрисовку

//class CSelfDraw
//{
//public:
//	CSelfDraw() noexcept;
//	CSelfDraw(CTextureManager::EType  Type) noexcept;
//	void	DrawSelf(CTextVertexColorBuffer& Buffer, float Height) noexcept;
//	void	SetPositionSelf(const Vec3& LeftDown, GLfloat Width, GLfloat Height, float WinHeight)  noexcept;
//};
//
//template < class SelfDraw = CSelfDraw >
//class COGLSelfDrawingControl : public COGLControl
//{
//protected :
//	SelfDraw		m_DrawClass;
//public:
//	COGLSelfDrawingControl() noexcept : COGLControl(CTextureManager::EType::None), m_DrawClass(this) {  };
//	COGLSelfDrawingControl(CTextureManager::EType  Type) noexcept : COGLControl(Type), m_DrawClass(this) { };
//
//	void	DrawES(CTextVertexColorBuffer& Buffer, float Height) noexcept {
//		m_DrawClass.Draw(Buffer, Height);
//	};
//	void	GetPosition(GLfloat Width, GLfloat Height)  noexcept {
//		COGLControl::GetPosition(Width, Height);
//		m_DrawClass.SetPosition(m_visLeftTop, m_fvisWidth, m_fvisHeight, Height);
//	};
//};

//--------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------
//  имитация вращения барабана с цифрами

//struct SSlidingTextureContr
//{
//	int			m_iNumSign = 11;
//	float		m_fWidth = 0.01f;
//	float		m_fHeight = 0.01f;
//	float		m_fX = 0.0f;     //  положение верхней левой точки
//	float		m_fY = 0.0f;
//	float		m_fZ = 0.0f;
//	//
//	float		m_fSlider = 1.0f - 1.0f / m_iNumSign * 10;
//	float		m_fStopSlider = 0.0f;
//	bool		m_bDown = false;
//	//
//	int			m_iSymbol = 0;		// 0 - 0, 1 -1 , .... -1 - пустое поле
//
//	SSlidingTextureContr() {}
//	SSlidingTextureContr(float Height, float Width, float X, float Y, float Z) : m_fWidth(Width),
//		m_fX(X), m_fY(Y), m_fZ(Z), m_fHeight(Height) {}
//
//	void SetPosition(float Height, float Width, float X, float Y, float Z)	noexcept {
//		m_fWidth = Width;
//		m_fHeight = Height;
//		m_fX = X;     
//		m_fY = Y;
//		m_fZ = Z;
//	}
//
//	void SetSymbol(int Symbol)	noexcept {
//		assert(m_iSymbol >= 0 && m_iSymbol < m_iNumSign);
//		m_iSymbol = Symbol;
//		m_fStopSlider = 1.0f - 1.0f / m_iNumSign*m_iSymbol;
//		if (m_fSlider < m_fStopSlider)
//			m_bDown = true;
//	}
//
//	CTextureManager::EType GetTexture() { return CTextureManager::EType::Digits; };
//
//	void GetVertexTextureES( std::vector<float>&  Vertex, std::vector<float>&  Tex, unsigned int IndexStart, std::vector<unsigned int>&  Index) noexcept
//	{
//		Vertex = { m_fX, m_fY,  m_fZ,
//			m_fX + m_fWidth, m_fY, m_fZ,
//			m_fX, m_fY + m_fHeight, m_fZ,
//			m_fX + m_fWidth, m_fY + m_fHeight, m_fZ };
//		Tex = { 0.0f, m_fSlider - 1.0f / m_iNumSign, 1.0f, m_fSlider - 1.0f / m_iNumSign, 0.0f, m_fSlider, 1.0f, m_fSlider };
//		Index = { IndexStart, IndexStart + 2, IndexStart + 1, IndexStart + 2, IndexStart + 3, IndexStart + 1 };
//	}
//
//	void	Move()	noexcept {
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
//	}
//};
//
//template < int NumDigit, class DrumType >
//struct CNumberDrumDraw : public CSelfDraw
//{
//	DrumType			m_Drums[NumDigit];
//	GLfloat				m_fWidthDigit = 0.01f;
//	GLfloat				m_fHeightDigit = 0.015f;
//	CNumberDrumDraw(  ) noexcept /*: m_pControl(Control)*/ {
//		auto x = 0.0f;
//		for (auto& dr : m_Drums) {
//			dr = std::move( DrumType{ m_fHeightDigit, m_fWidthDigit, x, 0.0f, 0.0f } );
//			x += m_fWidthDigit;
//		}
//	};
//
//	void SetPosition(   Vec3  LeftTop,			  //  координаты нижнего левого угла (как при выводе текста )
//						float Width, float Height, float WindHeight )	noexcept {
//		auto x = LeftTop.x;
//
//		for ( auto& dr : m_Drums ) {
//			dr.SetPosition(Height, Width, x, WindHeight - LeftTop.y, LeftTop.z);
//			x += Width;
//		}
//	};
//
//	void SetNumber(int Num)	noexcept {
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
//	//   Рисование шейдерами
//	void	Draw(CTextVertexColorBuffer& Buffer, float Height) noexcept {
//		std::vector< float>	vertex, tex;
//		std::vector< unsigned int >		index;
//		//
//		for ( auto& dr : m_Drums ) {
//			dr.GetVertexTextureES(vertex, tex, Buffer.Count(), index);
//			Buffer.AddVertex(&vertex[0], &vertex[0] + vertex.size());
//			Buffer.AddTexture(&tex[0], &tex[0] + tex.size());
//			Buffer.AddIndex(&index[0], &index[0] + index.size());
//			Buffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.8f });
//			dr.Move();
//		}
//	};
//};
//
//
//
//template < int NumDigit, class DrumType >
//class   CNumberDrumControl : public COGLSelfDrawingControl< CNumberDrumDraw< NumDigit, DrumType> >
//{
//public :
//	CNumberDrumControl() noexcept : COGLSelfDrawingControl() {  };
//	CNumberDrumControl(CTextureManager::EType  Type) noexcept : COGLSelfDrawingControl(Type) { };
//	void SetNumber(int Num)	noexcept {
//		m_DrawClass.SetNumber(Num);
//	};
//};
//
//
//template < int NumDigit > class CSlideNumberDrumControl : public CNumberDrumControl< NumDigit, SSlidingTextureContr > {
//public :
//	CSlideNumberDrumControl(CTextureManager::EType  Type) noexcept : CNumberDrumControl(Type) { };
//};

//------------------------------------------------------------------------------
//   Типы рисования ProgressBar


enum  EProgressBarType {
	RoundProgressBar
};

template< int Type >
class COGLProgressBar;

//  абстрактный шаблонный клас рисования ProgressBar
template < int Type >
struct CProgressBarDraw {
	CProgressBarDraw(COGLProgressBar<Type>*);
	void	DrawPB(CTextVertexColorBuffer&) noexcept;
};
// специализации  абстрактного шаблонного класса рисования ProgressBar

template < >
struct CProgressBarDraw< RoundProgressBar > {
	COGLProgressBar<RoundProgressBar>*			m_Control;
	CProgressBarDraw(COGLProgressBar<RoundProgressBar>* Control) : m_Control(Control) {};
	void	DrawPB(CTextVertexColorBuffer& Buffer) noexcept;
};



template< int Type >
class COGLProgressBar : public COGLControl, public CProgressBarDraw< Type >
{
protected:
	float				m_MinValue = 0.0f;
	float				m_MaxValue = 1.0f;
	float				m_Value = 0.0f;
	//	EProgressBarType	m_Type = EProgressBarType::Round;
public:
	COGLProgressBar() noexcept : COGLControl(CTextureManager::EType::None), CProgressBarDraw< Type >(this)/*, COGLProgressBar<Type> >(this)*//*, m_Type(Type)*/ {};
	void	SetMaxMin(float MinVal, float MaxVal) noexcept { m_MinValue = MinVal; m_MaxValue = MaxVal; };
	void	GetMaxMin(float& MinVal, float& MaxVal) noexcept { MinVal = m_MinValue; MaxVal = m_MaxValue; };
	void	SetValue(float Val)	noexcept { m_Value = Val; };
	float	GetValue() noexcept { return m_Value; };
	//
	virtual GLvoid	OnLMouseDown() noexcept {};
	virtual GLvoid	OnLMouseUp() noexcept {};
	virtual GLvoid	OnMouseMove() noexcept {};
	virtual GLvoid	OnMouseLeave() noexcept {};
	//
#ifndef OPENGL_SHADER
	void	Draw(CTextVertexColorBuffer& Buffer) noexcept {
		DrawPB(Buffer);
	};
#else
	void	DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept {
		DrawPB(Buffer);
	};
#endif
};


using COGLRoundProgressBar = COGLProgressBar<RoundProgressBar>;


//--------------------------------------------------------------------------------------------------------------
//  другой вариант архитектуры классов

//  имитация вращения барабана с цифрами


struct SSlidingTextureData
{
	int			m_iNumSign = 11;
	float		m_fWidth = 0.01f;
	float		m_fHeight = 0.01f;
	float		m_fX = 0.0f;     //  положение верхней левой точки
	float		m_fY = 0.0f;
	float		m_fZ = 0.0f;
	//
	float		m_fSlider = 1.0f - 1.0f / m_iNumSign * 10;
	float		m_fStopSlider = 0.0f;
	bool		m_bDown = false;
	//
	int			m_iSymbol = 0;		// 0 - 0, 1 -1 , .... -1 - пустое поле

	SSlidingTextureData() {}
	SSlidingTextureData(float Height, float Width, float X, float Y, float Z) : m_fWidth(Width),
		m_fX(X), m_fY(Y), m_fZ(Z), m_fHeight(Height) {}
};

template < int NumDigit > class CNumberDrumControl1 : public COGLControl
{
protected :
	SSlidingTextureData		m_Drums[NumDigit];
	float					m_fWidthDigit = 0.01f;
	float					m_fHeightDigit = 0.015f;
public:
	CNumberDrumControl1(CTextureManager::EType  Type) noexcept : COGLControl(Type) {
		auto x = 0.0f;
		for (auto& dr : m_Drums) {
			dr = std::move( SSlidingTextureData{ m_fHeightDigit, m_fWidthDigit, x, 0.0f, 0.0f } );
			x += m_fWidthDigit;
		}
	};

	void SetPosition(Vec3  LeftTop,			  //  координаты нижнего левого угла (как при выводе текста )
		float Width, float Height, float WindHeight)	noexcept {
		m_LeftTop = LeftTop;
		m_fWidthDigit = Width;
		m_fHeightDigit = Height;
	};

	void SetNumber(int Num)	noexcept {
		int number = Num;
		for (auto i = 0; i < NumDigit; ++i) {
			auto nn = number - (number / 10) * 10;
			if (number == 0)
				nn = 10;
			
			SSlidingTextureData& dr = m_Drums[NumDigit - i - 1];
			dr.m_iSymbol = nn;
			dr.m_fStopSlider = 1.0f - 1.0f / dr.m_iNumSign*dr.m_iSymbol;
			if (dr.m_fSlider < dr.m_fStopSlider)
				dr.m_bDown = true;

			number = number / 10;
		}
	};


	void	GetPosition(GLfloat Width, GLfloat Height)  noexcept {
		COGLControl::GetPosition(Width, Height);
		//....... /// здесь доделать перевод координат из относительных в абсолютные
		auto x = m_LeftTop.x*Width;

		for (auto& dr : m_Drums) {
			dr.m_fWidth = m_fWidthDigit*Width;
			dr.m_fHeight = m_fHeightDigit*Height;
			dr.m_fX = x;
			dr.m_fY = (1.0f - m_LeftTop.y)*Height;
			dr.m_fZ = m_LeftTop.z;
			//dr.SetPosition(Height, Width, x, WindHeight - LeftTop.y, LeftTop.z);
			x += m_fWidthDigit*Width;
		}
	}
	
};

class CSlidingDraw {
public :
	void GetVertexTextureES(SSlidingTextureData& SlideData, std::vector<float>&  Vertex, std::vector<float>&  Tex, 
							unsigned int IndexStart, std::vector<unsigned int>&  Index) noexcept
	{
		Vertex = { SlideData.m_fX, SlideData.m_fY,  SlideData.m_fZ,
			SlideData.m_fX + SlideData.m_fWidth, SlideData.m_fY, SlideData.m_fZ,
			SlideData.m_fX, SlideData.m_fY + SlideData.m_fHeight, SlideData.m_fZ,
			SlideData.m_fX + SlideData.m_fWidth, SlideData.m_fY + SlideData.m_fHeight, SlideData.m_fZ };
		Tex = { 0.0f, SlideData.m_fSlider - 1.0f / SlideData.m_iNumSign, 1.0f, SlideData.m_fSlider - 1.0f / SlideData.m_iNumSign, 0.0f, SlideData.m_fSlider, 1.0f, SlideData.m_fSlider };
		Index = { IndexStart, IndexStart + 2, IndexStart + 1, IndexStart + 2, IndexStart + 3, IndexStart + 1 };
	}

	void	Move( SSlidingTextureData& SlideData )	noexcept {
		if (SlideData.m_bDown) {
			SlideData.m_fSlider -= 0.01f;
			if (SlideData.m_fSlider < 1.0f - 1.0f / SlideData.m_iNumSign * 10) {
				SlideData.m_fSlider = 1.0f;
				SlideData.m_bDown = false;
			}
		}
		else if (SlideData.m_fSlider > SlideData.m_fStopSlider) {
			SlideData.m_fSlider -= 0.01f;
			SlideData.m_fSlider = max(SlideData.m_fSlider, SlideData.m_fStopSlider);
		}
	}
};

template < int NumDigit > class CNumberDrumControlSlide : public  CNumberDrumControl1< NumDigit >, public CSlidingDraw
{
public :
	CNumberDrumControlSlide( CTextureManager::EType  Type) noexcept : CNumberDrumControl1(Type) {};
	//   Рисование шейдерами
	void	DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept {
		std::vector< float>	vertex, tex;
		std::vector< unsigned int >		index;
		//
		for (auto& dr : m_Drums) {
			GetVertexTextureES(dr, vertex, tex, Buffer.Count(), index);
			Buffer.AddVertex(&vertex[0], &vertex[0] + vertex.size());
			Buffer.AddTexture(&tex[0], &tex[0] + tex.size());
			Buffer.AddIndex(&index[0], &index[0] + index.size());
			Buffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.8f });
			Move(dr);
		}
	};
};

//-------------------------------------------------------------------------------------------------------------------

class COGLPanel : public COGLControl
{
protected:
	std::vector< COGLControl* >			m_vChild;
public :
	COGLPanel() noexcept : COGLControl(CTextureManager::EType::None) {};
	COGLPanel(CTextureManager::EType  Type) noexcept : COGLControl(Type) {};
	//
	void	AddControl(COGLControl* Control)  noexcept {
		m_vChild.push_back(Control); 
		Control->SetParent(this);
		Control->SetVisible(m_bVisible);
	};
	GLvoid	SetVisible(bool Vis)  noexcept {
		m_bVisible = Vis; 
		for (auto ch : m_vChild) ch->SetVisible(Vis);
	};
	//
	void	DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept {
		if(m_iTextureType != CTextureManager::EType::None)
			COGLControl::DrawES(Buffer, Height);
	};
};



/////----------------------------------------------------------------------------------------------------------------

class COGLControlManager :  public Observable< IControlEvent >,	public IKeyboardEvent, public IMouseEvent , public IResizeWindow 
{
protected :
#ifdef OPENGL_SHADER
	int									m_iWidth = 0;
	int									m_iHeight = 0;
#endif
	std::vector<COGLControl*>	m_vControls;
	COGLControlManager();
	COGLControl*  FindControl(const POINTS& Point) {
		for (auto contr : m_vControls) {
			if (contr->IsClickable() && Point.x >= contr->visLeft() && Point.x <= contr->visRight() && Point.y >= contr->visTop() && Point.y <= contr->visBottom())
				return contr;
		}
		return nullptr;
	};		
	COGLControl*					m_pCurrentControl;
	//  сортировка списка контролов в порядке их зависимости друг от друга  
	void SortControls() {
		std::sort(m_vControls.begin(), m_vControls.end(), [](COGLControl* Contr1, COGLControl* Contr2) -> bool {
			bool ret = false;
			auto pc1 = Contr1->GetParent(), pc2 = Contr2->GetParent();
			if (pc1 == nullptr && pc2 == nullptr) ret = pc1 < pc2;
			else if( pc1 != nullptr && pc2 == nullptr ) ret = false;
			else if( pc1 == nullptr && pc2 != nullptr ) ret = true;
			else {   //  у обоих контролов есть родители
				if (pc1 == Contr2) ret = false;
				else if(pc2 == Contr1 ) ret = true;
				else Contr1 < Contr2;
			}
			return ret; } );
	};
public:
	
	static COGLControlManager& GetInstance() noexcept;
	inline GLvoid	AddControl( COGLControl*  Control) noexcept {
		std::vector<COGLControl*>::iterator it;
		for (it = m_vControls.begin(); it != m_vControls.end() && Control->GetTexType() < (*it)->GetTexType(); ++it);
		m_vControls.insert( it, Control); 
	};
#ifndef OPENGL_SHADER
	//  новое рисование через буферы
	void	DrawControls( CTextVertexColorBuffer& Buffer) noexcept;
#else
	//   Рисование шейдерами
	void	DrawControlsES( CTextVertexColorBuffer& Buffer ) noexcept;
	void	SetWindowSize(int Width, int Height) {  m_iWidth = Width;	m_iHeight = Height; }
#endif
	//
	void	onKeyDown(char Key)  noexcept { }
	void	onKeyUp(char Key)  noexcept { }
	//
	void	onMouseMove(int X, int Y)  noexcept;
	void	onLMouseDown(int X, int Y)  noexcept;
	void	onLMouseUp(int X, int Y)  noexcept;
	//
	void	onResizeWindow(float Width, float Height)  noexcept;
};


