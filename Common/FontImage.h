#pragma once
#include "GLFonts.h"

struct Snode;
using VGraph = std::vector< Snode >;

struct Snode {
	int		m_iRow = 0;
	int		m_iXstart = 0;
	int		m_iXstop = 0;
	bool	m_bVisit = false;
	std::vector< int >	m_pNext;
	std::vector< int >	m_pPrev;
	Snode(int Row, int Xstart, int Xstop) : m_iRow(Row), m_iXstart(Xstart), m_iXstop(Xstop) {}
	bool CalcBoundBox(int& Left, int& Top, int& Right, int& Bottom, VGraph& Graph ) {
		if (m_bVisit) return false;
		m_bVisit = true;
		Left = min(Left, m_iXstart);
		Right = max(Right, m_iXstop);
		Top = min(m_iRow, Top);
		Bottom = max(m_iRow, Bottom);
		//		IO_TRACE("row = %d  start = %d  stop = %d  \n", m_iRow, m_iXstart, m_iXstop);
		for (auto& segm : m_pNext) {
			//IO_TRACE("row = %d ", m_iRow);
			Graph[segm].CalcBoundBox(Left, Top, Right, Bottom, Graph);
		}
		for (auto& segm : m_pPrev) {
			//IO_TRACE("row = %d ", m_iRow);
			Graph[segm].CalcBoundBox(Left, Top, Right, Bottom, Graph);
		}

		return true;
	}
};

class CFontImage
{
	std::string					m_sFileName;
	CGLTexture					m_Image;
	int							m_iWidth = 0;
	int							m_iHeight = 0;
	int							m_iXoff = 0;
	int							m_iYoff = 50;
	int							m_iGlyphHeight = 20;
	int							m_iGlyphWidth = 0;
	//
	std::vector< Snode >		m_Graph;
	std::vector<SGLGlyph>		m_Glyphs;

	float						m_fXScale = 1.0f;
	float						m_fYScale = 1.0f;
	SOGLColor					m_Color = {1.0f, 1.0f, 1.0f, 1.0f };
public:
	CFontImage();
	~CFontImage();
	//void		InitGL() noexcept;
	//bool		Init(int Width, int Height) noexcept;
	//GLvoid		ResizeScene(GLsizei width, GLsizei height) noexcept;
	bool		DrawScene( CTextVertexColorBuffer& DrawBuffer );

	bool		ScanImageForFont(const std::string& ImageFile);
	bool		MakeFontFile();
	bool		MakeFontSerial( const SFontSerial& Data );
	void		GetSize(int& Width, int& Height) { Width = m_iWidth; Height = m_iHeight; };

	CGLTexture*	GetTexture() { 	return &m_Image;  };
	// Возвращает новое смещение символа по оси Х 
#ifndef OPENGL_SHADER
	GLfloat		GetSymbol(wchar_t Symb,				//	выводимый символ 
		GLfloat Xoff, GLfloat Yoff, GLfloat Zoff,	//	смещение символа
		CTextVertexColorBuffer& DrawBuffer);		// при выводе в одну строчку , возвращается значение смещения Y для выводв следующего символа
#else
	float		GetSymbolES(wchar_t Symb,			//	выводимый символ 
		GLfloat Xoff, GLfloat Yoff, GLfloat Zoff,	//	смещение символа
		CTextVertexColorBuffer& DrawBuffer);		// при выводе в одну строчку , возвращается значение смещения X для выводв следующего символа
#endif

	void		GetTextSize(const std::wstring& Text, GLfloat& Width, GLfloat& Height);

	void		TextDraw( const std::wstring& Text, GLfloat X, GLfloat Y, GLfloat Zoff, CTextVertexColorBuffer& DrawBuffer );

	void		SetColor(const SOGLColor& Color) { m_Color = Color; };
	void		SetColor(const SOGLColor&& Color) { m_Color = Color; };
	void		SetColor(float R, float G, float B, float A) { m_Color = SOGLColor{R,G,B,A}; };

	void		SetSize(GLfloat Width, GLfloat  Height);
};


class CFontImageManager
{
	int								m_iWidth = 0;
	int								m_iHeight = 0;

	std::vector< CFontImage >		m_vFonts;
	int								m_iCurFont = -1;
	CTextVertexColorBuffer			m_DrawBuffer;

public :
	static CFontImageManager& GetInstance() noexcept {
		static CFontImageManager	instance;
		return instance;
	};
	void		InitGL() noexcept;
	bool		Init(int Width, int Height) noexcept;
	GLvoid		ResizeScene(GLsizei width, GLsizei height) noexcept;
	bool		DrawScene();
	//
	void		InitFonts( const SFontSerial* Fonts, int	Count );
	void		AddFont(const CFontImage& Font) { m_vFonts.push_back(Font); };
	void		AddFont(const CFontImage&& Font) { m_vFonts.emplace_back(Font); } ;
	void		AddFontSerial(const SFontSerial& Data);
	int			GetFontsCount() { return m_vFonts.size(); };
	CFontImage&	GetFont(int Num) { 
		assert( Num >= 0 && Num < m_vFonts.size());
		return m_vFonts[Num];
	};
	void	SetCurFont(int Num) { m_iCurFont = Num; };
	int		GetCurFont() { return m_iCurFont; };
};

