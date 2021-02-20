#include "stdafx.h"

#ifdef OPENGLES20
#pragma comment ( lib, "libGLESv2.lib" )
#endif

#include "Shader.h"
#include "Math3D.h"
#include "OGLBuffers.h"
#include "Textures.h"
#include "FontImage.h"

#include "utils.h"
#include "OGLControl.h"


//   квадрат ( два треугольника ) единичного размера
GLfloat	Vertexes0[] = {	0.0f, 0.0f, 0.0f ,  1.0f, 0.0f, 0.0f , 
						1.0f, 1.0f, 0.0f ,  0.0f, 1.0f, 0.0f  };
GLfloat	Vertexes90[] = {	1.0f, 0.0f, 0.0f ,  1.0f, 1.0f, 0.0f ,
							0.0f, 1.0f, 0.0f  , 0.0f, 0.0f, 0.0f };
GLfloat	Vertexes270[] = {	1.0f, 1.0f, 0.0f ,  0.0f, 1.0f, 0.0f ,
							0.0f, 0.0f, 0.0f ,  1.0f, 0.0f, 0.0f };
GLfloat	Vertexes180[] = {	0.0f, 1.0f, 0.0f ,  0.0f, 0.0f, 0.0f,
							1.0f, 0.0f, 0.0f ,  1.0f, 1.0f, 0.0f };
//  текстурные координаты для этого квадрата
//#ifndef OPENGL_SHADER
GLfloat	TextureCoords[] = {	0.0f, 0.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f };
//#else
//GLfloat	TextureCoords[] = { 0.0f, 1.0f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f, 0.0f };
//#endif
unsigned int	Indexes[] = { 0, 1, 2,   0, 2, 3 };
///  это заклинание не очень понятно
template<typename T, size_t length>
constexpr size_t lengthof(T(&)[length])
{
	return length;
}

//   проверка, что размеры всехмассивов заданы правильно на шаге компиляции
constexpr size_t  Vertexes0_size = lengthof(Vertexes0) / 3;
constexpr size_t  Vertexes90_size = lengthof(Vertexes90) / 3;
constexpr size_t  Vertexes270_size = lengthof(Vertexes270) / 3;
constexpr size_t  Vertexes180_size = lengthof(Vertexes180) / 3;
constexpr size_t  TextureCoords_size = lengthof(TextureCoords) / 2;
constexpr size_t  Indexes_size = lengthof(Indexes);

template< size_t vert0, size_t vert90, size_t vert_90, size_t vert180, size_t tex >
struct CGetSize {
	using value_type = typename std::enable_if< vert0 == vert90 && vert90 == vert_90 && vert_90 == vert180 && vert180 == tex, size_t >::type;
	const value_type	iAmount = vert0;
	constexpr	CGetSize() = default;
	constexpr value_type GetAmount() { return iAmount; };
};


using GetSize = CGetSize<Vertexes0_size, Vertexes90_size, Vertexes270_size, Vertexes180_size, TextureCoords_size>;
using VertexSize = GetSize::value_type;

constexpr VertexSize iVerteAmount = GetSize().GetAmount();

using	VertexArrayType = GLfloat[iVerteAmount*3];

//const size_t	VertexAmount = iVerteAmount;


inline	GLfloat*  GetQuadVertecis(const Vec3&& Offset, const Vec2&& Size, ERotation Rotation, VertexArrayType& Vertecis ) {
	size_t	sz = 0;
	const GLfloat*  vert = nullptr;

	switch (Rotation) {
	case ERotation::R0:		vert = Vertexes0;	sz = lengthof(Vertexes0);	break;
	case ERotation::R90:	vert = Vertexes90;	sz = lengthof(Vertexes90);	break;
	case ERotation::R270:	vert = Vertexes270;	sz = lengthof(Vertexes270);	break;
	case ERotation::R180:	vert = Vertexes180;	sz = lengthof(Vertexes180);	break;
	default: __debugbreak(); break;
	}

	for (size_t i = 0; i < sz; i += 3) {
		Vertecis[i] = vert[i] * Size.x + Offset.x;
		Vertecis[i + 1] = vert[i + 1] * Size.y + Offset.y;
		Vertecis[i + 2] = Offset.z;
	}

	return Vertecis;
}

//float	COGLControl::m_fWidthWindow = 0.0f;
//float	COGLControl::m_fHeightWindow = 0.0f;


COGLControl::COGLControl() noexcept
{
	m_CurrentColor = m_Color;
	COGLControlManager::GetInstance().AddControl(this);
}

COGLControl::COGLControl(CTextureManager::EType  Type) noexcept : m_iTextureType(Type)
{
	m_CurrentColor = m_Color;
	COGLControlManager::GetInstance().AddControl(this);
}


COGLControl::~COGLControl() noexcept
{
}

void	COGLControl::GetPosition( GLfloat Width, GLfloat Height )  noexcept
{
 	m_visLeftTop.x = m_LeftTop.x*Width;
	m_visLeftTop.y = m_LeftTop.y*Height;
	//
	if (m_eHorAlign != EHorAlign::No || m_eVertAlign != EVertAlign::No) {
		auto	Xparent = m_pParent != nullptr ? m_pParent->Left() : 0.0f;
		auto	Yparent = m_pParent != nullptr ? m_pParent->Top() : 0.0f;
		auto	Wparent = m_pParent != nullptr ? m_pParent->Right() - Xparent : 1.0f;
		auto	Hparent = m_pParent != nullptr ? m_pParent->Bottom() - Yparent : 1.0f;
		auto	Z = m_pParent != nullptr ? m_pParent->Z() + 0.1f : m_LeftTop.z;
		//
		auto Xoff = m_LeftTop.x + Xparent, Yoff = m_LeftTop.y + Yparent;
		switch (m_eHorAlign) {
			case EHorAlign::Full:	m_fWidth = Wparent;
			case EHorAlign::Left:	Xoff = m_LeftTop.x = Xparent; break;
			case EHorAlign::Center:	Xoff = m_LeftTop.x = Xparent + (Wparent - m_fWidth) / 2.0f; break;
			case EHorAlign::Right:	Xoff = m_LeftTop.x = Xparent + Wparent - m_fWidth; break;
			case EHorAlign::RightBind : Xoff = Xparent + Wparent - m_LeftTop.x; break;
		}
		//
		switch (m_eVertAlign) {
			case EVertAlign::Full:	m_fHeight = Hparent;
			case EVertAlign::Up:	Yoff = m_LeftTop.y = Yparent; break;
			case EVertAlign::Center:Yoff = m_LeftTop.y = Yparent + (Hparent - m_fHeight) / 2.0f; break;
			case EVertAlign::Down:	Yoff = m_LeftTop.y = Yparent + Hparent - m_fHeight; break;
			case EVertAlign::DownBind :	Yoff = Yparent + Hparent - m_LeftTop.y; break;
		}
		//
		m_visLeftTop.x = Xoff*Width;
		m_visLeftTop.y = Yoff*Height;
		
	}
	m_fvisWidth = Width*m_fWidth;		//  размер контрола
	if(m_fHeight > 0.0f )
		m_fvisHeight = Height*m_fHeight;
	else m_fvisHeight = m_fvisWidth;
};

#ifndef OPENGL_SHADER
void	COGLControl::Draw(CTextVertexColorBuffer& Buffer) noexcept
{
	if (m_iTextureType != CTextureManager::EType::None) {
		glEnable(GL_TEXTURE_2D);
		CTextureManager::GetInstance().GetTexture(m_iTextureType)->Bind();
	}
	else
		glDisable(GL_TEXTURE_2D);

	auto	Xparent = 0.0f, Yparent = 0.0f, Z = -3.0f;
	if ( m_pParent != nullptr ) {
		Xparent = m_pParent->m_LeftTop.x;
		Yparent = m_pParent->m_LeftTop.y;
		Z = m_pParent->m_LeftTop.z + 0.1f;
	}

	VertexArrayType verOut;
	GetQuadVertecis(Vec3{ m_LeftTop.x + Xparent, m_LeftTop.y + Yparent, Z }, Vec2{ m_RightBottom.x - m_LeftTop.x, m_RightBottom.y - m_LeftTop.y },
		m_eRottation, verOut);
	
	Buffer.AddVertex(verOut, verOut + iVerteAmount * Buffer.VertexDimension);
	if (m_iTextureType != CTextureManager::EType::None)
		Buffer.AddTexture( TextureCoords, TextureCoords + iVerteAmount * Buffer.TextureDimension);
	Buffer.SetColor(m_CurrentColor);
	Buffer.AddIndex(Indexes, Indexes + Indexes_size);
}
#else

void	COGLControl::DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept
{
	if (m_iTextureType != CTextureManager::EType::None) {
		glEnable(GL_TEXTURE_2D);
		CTextureManager::GetInstance().GetTexture(m_iTextureType)->Bind();
	}
	else
		glDisable(GL_TEXTURE_2D);

	//
	VertexArrayType verOut;
	GetQuadVertecis(Vec3{ m_visLeftTop.x, m_visLeftTop.y, m_visLeftTop.z/*Z*/ }, Vec2{ m_fvisWidth, m_fvisHeight },
		m_eRottation, verOut);
	for (auto i = 1; i < iVerteAmount * 3; i += 3 ) verOut[i] = Height - verOut[i];
	Buffer.AddVertex(verOut, verOut + iVerteAmount * Buffer.VertexDimension);
	//Buffer.AddVerticesAndTransform(verOut, iVerteAmount * Buffer.VertexDimension, TextureCoords, iVerteAmount * Buffer.TextureDimension, m_CurrentColor);
	if (m_iTextureType != CTextureManager::EType::None)
		Buffer.AddTexture( TextureCoords, TextureCoords + iVerteAmount * Buffer.TextureDimension);
	Buffer.SetColor(m_CurrentColor);
	Buffer.AddIndex(Indexes, Indexes + Indexes_size);
}
#endif


GLvoid	COGLControl::OnLMouseDown() noexcept
{
}

GLvoid	COGLControl::OnLMouseUp() noexcept
{
}

GLvoid	COGLControl::OnMouseMove() noexcept
{
	m_CurrentColor = m_ActiveColor;
}

GLvoid	COGLControl::OnMouseLeave() noexcept
{
	m_CurrentColor =  m_Color;
}


void	COGLButton::SetTextPosition() {
	if (!m_sText.empty()) {
		// определение размера области, занимаемой текстом
		CFontImage&	fnt = CFontImageManager::GetInstance().GetFont(m_iFont);
		fnt.SetSize(m_fvisHeight*0.4f, m_fvisHeight*0.7f);
		float txtWidth = 0.0f, txtHeight = 0.0f;
		fnt.GetTextSize(m_sText, txtWidth, txtHeight);
		//
		m_TextPos.z = m_LeftTop.z;
		
		switch (m_eHorAlignText) {
			case EHorAlign::Left:	m_TextPos.x = m_visLeftTop.x; break;
			case EHorAlign::Right:	m_TextPos.x = m_visLeftTop.x + m_fvisWidth; break;
			default : // во всех остальных случаях центровка по середине
				m_TextPos.x = m_visLeftTop.x + (m_fvisWidth - txtWidth) / 2.0f; break;
		}
		//
		switch (m_eVertAlign) {
			case EVertAlign::Up:	m_TextPos.y = m_visLeftTop.y; break;
			case EVertAlign::Down:	m_TextPos.y = m_visLeftTop.y + m_fvisHeight - txtHeight; break;
			default: // во всех остальных случаях центровка по середине
				m_TextPos.y = m_visLeftTop.y  + (m_fvisHeight - txtHeight) / 2.0f + txtHeight; break;
		}
	}
}


#ifndef OPENGL_SHADER
void	COGLButton::Draw(CTextVertexColorBuffer& Buffer) noexcept
{
	CTextureManager::EType type = m_iTextureType;
	if (m_bPressed)
		m_iTextureType = m_iTextureTypePressed;
	COGLControl::Draw(Buffer);
	Buffer.DrawElements(GL_TRIANGLES);
	m_iTextureType = type;
	//
	if (!m_sText.empty()) {
		auto	Xparent = 0.0f, Yparent = 0.0f, Z = -3.0f;
		if (m_pParent != nullptr) {
			Xparent = m_pParent->Left();
			Yparent = m_pParent->Top();
			Z = m_pParent->Z() + 0.1f;
		}
		CFontImage&	fnt = CFontImageManager::GetInstance().GetFont(m_iFont);
		auto	scale = fnt.GetScale();
		auto yoff = (m_RightBottom.y - m_LeftTop.y)*0.15f, xoff = (m_RightBottom.x - m_LeftTop.x)*0.15f;
		fnt.SetHeight((m_RightBottom.y - m_LeftTop.y)*0.7f);
		fnt.TextDraw(m_sText, m_LeftTop.x + xoff + Xparent, m_RightBottom.y - yoff + Yparent, m_LeftTop.z + Z, Buffer);
		fnt.SetScale(scale);
	}
}

#else
void	COGLButton::DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept
{
	CTextureManager::EType type = m_iTextureType;
	if (m_bPressed)
		m_iTextureType = m_iTextureTypePressed;
	COGLControl::DrawES(Buffer, Height);
	Buffer.DrawBuffer( CTextureManager::GetInstance().GetTexture(m_iTextureType), GL_TRIANGLES);
	m_iTextureType = type;
	//
	
	if (!m_sText.empty()) {
		CFontImage&	fnt = CFontImageManager::GetInstance().GetFont(m_iFont);

		auto yoff = m_fvisHeight*0.15f, xoff = m_fvisWidth*0.15f;
		fnt.SetSize(m_fvisHeight*0.4f, m_fvisHeight*0.7f);
		fnt.TextDraw(m_sText, m_TextPos.x, Height - m_TextPos.y, m_TextPos.z , Buffer);

		Buffer.DrawBuffer(fnt.GetTexture(), GL_TRIANGLES);
	}
}
#endif

void	COGLStaticText::CalculateControlSize()  noexcept
{
	if (!m_sText.empty() && m_iFont >= 0 && m_fvisGlypthWdth > 0.0f) {
		CFontImage&	fnt = CFontImageManager::GetInstance().GetFont(m_iFont);
		fnt.SetSize(m_fvisGlypthWdth, m_fvisGlypthHeight);
		GLfloat wdt, hgt;
		fnt.GetTextSize(m_sText, wdt, hgt);
		m_fvisWidth = wdt;		//  размер контрола
		m_fvisHeight = hgt;
	}
}

void	COGLStaticText::SetPosition(const Vec3& LeftDown, GLfloat GlyphWidth, GLfloat GlyphHeight)  noexcept
{
	m_fGlypthWdth = GlyphWidth;
	m_fGlypthHeight = GlyphHeight;
	m_LeftTop = LeftDown;
	
	CalculateControlSize();
}


void	COGLStaticText::SetFont(int Font)  noexcept
{ 
	m_iFont = Font; 
	CalculateControlSize();
}


void	COGLStaticText::SetText(const std::wstring& Text)   noexcept
{ 
	m_sText = Text; 
	CalculateControlSize();
}

void	COGLStaticText::GetPosition(GLfloat Width, GLfloat Height)  noexcept
{
	m_fvisGlypthWdth = m_fGlypthWdth*min(Height,Width);
	m_fvisGlypthHeight = m_fGlypthHeight*min(Height, Width);
	CalculateControlSize();
	m_fWidth = m_fvisWidth/Width;		// относительный размер контрола
	m_fHeight = m_fvisHeight/Height;
	COGLControl::GetPosition(Width, Height);
}

#ifndef OPENGL_SHADER
void	COGLStaticText::Draw(CTextVertexColorBuffer& Buffer) noexcept 
{
	auto	Xparent = 0.0f, Yparent = 0.0f, Z = -3.0f;
	if (m_pParent != nullptr) {
		Xparent = m_pParent->Left();
		Yparent = m_pParent->Top();
		Z = m_pParent->Z() + 0.1f;
	}
	CFontImage&	fnt = CFontImageManager::GetInstance().GetFont(m_iFont);
	auto	scale = fnt.GetScale();
	fnt.SetHeight(m_RightBottom.y - m_LeftTop.y);
	fnt.TextDraw(m_sText, m_LeftTop.x + Xparent, m_RightBottom.y + Yparent, m_LeftTop.z + Z, Buffer);
	fnt.SetScale(scale);
}
#else
void	COGLStaticText::DrawES(CTextVertexColorBuffer& Buffer, GLfloat Height) noexcept
{

	CFontImage&	fnt = CFontImageManager::GetInstance().GetFont(m_iFont);

	fnt.SetColor(m_Color);
	fnt.SetSize(m_fvisGlypthWdth, m_fvisGlypthHeight);
	fnt.TextDraw(m_sText, m_visLeftTop.x, Height - m_visLeftTop.y, m_visLeftTop.z, Buffer);

	Buffer.DrawBuffer(fnt.GetTexture(), GL_TRIANGLES);
}
#endif
//---------------------------------------------------------------------------------------------------
//template < RoundProgressBar >
void	CProgressBarDraw< RoundProgressBar > ::DrawPB(CTextVertexColorBuffer& Buffer) noexcept {
	glDisable(GL_TEXTURE_2D);

	auto xCenter = (m_Control->visLeft() + m_Control->visRight()) / 2.0f,
		yCenter = (m_Control->visTop() + m_Control->visBottom()) / 2.0f,
		zCenter = m_Control->Z();
	auto Rad = min(m_Control->visRight() - m_Control->visLeft(), m_Control->visBottom() - m_Control->visTop())/2.0f;
	//
	auto value = m_Control->GetValue();
	if (value > 990)
		int gg = 999;
	float MinVal, MaxVal;
	m_Control->GetMaxMin(MinVal, MaxVal);
	auto angle = 0.0f/*M_PIF / 2.0f*/, stopAngle = - M_PIF*2.0f *( value - MinVal )/(MaxVal - MinVal);

	auto dAng = M_PIF / 25.0f;
	auto count = fabs(stopAngle/dAng);
	auto alpha = max(0.0f, 1.0 - count*0.05f);
	// вариант с индексным массивом
	unsigned int index = 0, startIndex = Buffer.Count();
	
	for ( /*angle -= M_PIF / 90.0f*/ ; angle > stopAngle; angle -= dAng ) {
		if (angle - dAng <= stopAngle)
			int fff = 999;
		index = Buffer.Count();
		if (index - startIndex >= 2)
			Buffer.AddIndex(index, index - 1, index - 2,		//  первый треугольник
							index - 1, index, index + 1);		//	второй треугольник
		auto x11 = xCenter + 0.5f*Rad*cos(angle), y11 = yCenter - 0.5f*Rad*sin(angle), x12 = xCenter + Rad*cos(angle), y12 = yCenter - Rad*sin(angle);
		Buffer.AddVertex(x11, y11, zCenter);
		Buffer.AddVertex(x12, y12, zCenter);
		auto color = m_Control->GetCurrentColor();
		color.a = alpha;
		alpha += 0.05f;
		Buffer.SetColor(color);
	}
	//
	//Buffer.SetColor(m_Control->GetCurrentColor());
	
}



//----------------------------------------------------------------------------------------------------
//std::vector<COGLControl*>		COGLControlManager::m_vControls;
//COGLControl*					COGLControlManager::m_pCurrentControl = nullptr;

COGLControlManager::COGLControlManager() : Observable< IControlEvent >()//, Singleton< COGLControlManager >()
{
	CMouseNotifier::GetInstance().registerObserver(this);
	CWindowResizeNotifier::GetInstance().registerObserver(this);
}

COGLControlManager& COGLControlManager :: GetInstance() noexcept
{
	static COGLControlManager ControlManager;
	return ControlManager;
}


#ifndef OPENGL_SHADER
//  новое рисование через буферы
void	COGLControlManager::DrawControls(CTextVertexColorBuffer& Buffer) noexcept
{
	if (m_vControls.empty())   return;
	CTextureManager::EType type = CTextureManager::EType::None;
	for (auto contr : m_vControls) {
		if (!contr->GetVisible())
			continue;
		if (type != contr->GetTexType()) {
			type = contr->GetTexType();
			Buffer.DrawElements(GL_TRIANGLES);
		}
		contr->Draw(Buffer);
	}
	Buffer.DrawElements(GL_TRIANGLES);
}
#else
//   Рисование шейдерами
void	COGLControlManager::DrawControlsES(CTextVertexColorBuffer& Buffer) noexcept
{
	
	if (m_vControls.empty())   return;
	CTextureManager::EType type = CTextureManager::EType::None;
	for (auto contr : m_vControls) {
		if (!contr->GetVisible())
			continue;
		if (type != contr->GetTexType()) {
			Buffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(type), GL_TRIANGLES);
			type = contr->GetTexType();
		}
		contr->DrawES(Buffer, m_iHeight);
	}
	Buffer.DrawBuffer(CTextureManager::GetInstance().GetTexture(type), GL_TRIANGLES);
	//Buffer.DrawElements(GL_TRIANGLES);
}
#endif




void	COGLControlManager::onMouseMove(int X, int Y)  noexcept
{
	COGLControl* control = FindControl(POINTS{ (short)X, (short)Y });

	if( control != nullptr )
		notifyObservers(&IControlEvent::onMouseMove, control);
	if (m_pCurrentControl != nullptr && m_pCurrentControl != control) {
		m_pCurrentControl->OnMouseLeave();
		notifyObservers(&IControlEvent::onMouseLeave, m_pCurrentControl);
	}

	m_pCurrentControl = control;

	if (control != nullptr) {

		control->OnMouseMove();
	}

}

void	COGLControlManager::onLMouseDown(int X, int Y)  noexcept
{
	COGLControl* control = FindControl(POINTS{ (short)X, (short)Y });
	if (control != nullptr) {
		control->OnLMouseDown();
		notifyObservers(&IControlEvent::onLButtonDown, control);
	}


	COGLButton* butt = dynamic_cast<COGLButton*>(control);
	if (butt != nullptr) {
		if( butt->GetPressed() )
			notifyObservers(&IControlEvent::onButtonDown, butt);
		else
			notifyObservers(&IControlEvent::onButtonUp, butt);
	}

}

void	COGLControlManager::onLMouseUp(int X, int Y)  noexcept
{
	COGLControl* control = FindControl(POINTS{ (short)X, (short)Y });
	if (control != nullptr) {
		control->OnLMouseUp();
		notifyObservers(&IControlEvent::onLButtonUp, control);
	}


	COGLButton* butt = dynamic_cast<COGLButton*>(control);
	if (butt != nullptr) {
		if (butt->GetPressed())
			notifyObservers(&IControlEvent::onButtonDown, butt);
		else
			notifyObservers(&IControlEvent::onButtonUp, butt);
	}
}

//
void	COGLControlManager::onResizeWindow(float Width, float Height)  noexcept
{
	SortControls();
	//  вызывать 
	for (auto& contr : m_vControls) contr->OnResizeWindow(Width, Height);
}
