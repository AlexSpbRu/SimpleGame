#pragma once

//class COGLFont
//{
//public :
//	enum class FontType { Raster, Vector, Texture };
//protected :
//	GLuint		m_fontList = 0;
//	FontType	m_Type;
//	HDC			m_hDc = 0;
//	GLuint		m_CountSymbols = 0;
//	//
//	GLvoid SetLOGFONT( GLint Height, const char* FontName, LOGFONT& LogFont );
//public :	
//	COGLFont( HDC hDc, FontType  Type );
//	virtual ~COGLFont();
//	virtual GLvoid	glCreatefont(const  LOGFONT& LogFont) = 0;
//	virtual GLvoid	glCreatefont(  GLint Height, const char* FontName ) = 0;
//	GLvoid	Delete();
//	virtual GLvoid	Print( const char *fmt, ... ) = 0;
//	virtual GLvoid glPrint(GLint x, GLint y, char *string) {};
//};
//
//
//class COGLRasterFont : public COGLFont
//{
//public:
//	COGLRasterFont(HDC hDc);
//	~COGLRasterFont();
//	virtual GLvoid	glCreatefont(const  LOGFONT& LogFont)  override;
//	virtual GLvoid	glCreatefont(GLint Height, const char* FontName)  override;
//	virtual GLvoid	Print(const char *fmt, ...)  override;
//};
//
//
//class COGLVectorFont : public COGLFont
//{
//	std::vector< GLYPHMETRICSFLOAT >	m_gmf;
//public:
//	COGLVectorFont(HDC hDc);
//	~COGLVectorFont();
//	virtual GLvoid	glCreatefont(const  LOGFONT& LogFont)  override;
//	virtual GLvoid	glCreatefont(GLint Height, const char* FontName)  override;
//	virtual GLvoid	Print(const char *fmt, ...)  override;
//};
//
//
//class COGL2DFont : public COGLFont
//{
//	CGLTexture			m_Texture;
//public:
//	COGL2DFont(HDC hDc);
//	~COGL2DFont();
//	virtual GLvoid	glCreatefont(const  LOGFONT& LogFont)  override {};
//	virtual GLvoid	glCreatefont(GLint Height, const char* FontName)  override {};
//	GLvoid	glCreatefont(const char* ImageName);
//	virtual GLvoid	Print(const char *fmt, ...)  override {};
//	GLvoid Print(GLint x, GLint y, const char* string, int Set);
//	GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);
//};


class COGLESFont
{
	CGLTexture*			m_Texture;
	GLfloat				m_fSize = 16.0f;
	SOGLColor			m_Color;// = { 0.0f, 0.0f, 0.0f, 1.0f };   // черный по умолчанию
#ifdef OPENGL_SHADER
	std::vector< Vec3 >			m_vVertexBuf;
	std::vector< Vec2 >			m_vTextureBuf;
	std::vector< SOGLColor >	m_vColorBuf;
#endif
public :
	COGLESFont();
	CGLTexture*	GetTexture() { return m_Texture; };
	inline GLvoid	SetSize(GLfloat Size) noexcept { m_fSize = Size; };
	inline GLvoid	SetColor(const SOGLColor&	Color) noexcept { m_Color = Color; };
#ifndef OPENGL_SHADER
	GLvoid	glPrint(GLfloat X, GLfloat Y, CTextVertexColorBuffer& Bufer, const char *fmt, ...);
#else
	inline size_t	GetVertexBufSize()		{ return m_vVertexBuf.size()*sizeof(Vec3)/sizeof(GLfloat); };
	inline size_t	GetTextureBufSize()		{ return m_vTextureBuf.size()*sizeof(Vec2)/sizeof(GLfloat); };
	inline size_t	GetColorBufSize()		{ return m_vColorBuf.size()*sizeof(SOGLColor)/sizeof(GLfloat); };

	inline const GLfloat*	GetVertexBufData()		{ return (const GLfloat*)m_vVertexBuf.data(); };
	inline const GLfloat*	GetTextureBufData()		{ return (const GLfloat*)m_vTextureBuf.data(); };
	inline const GLfloat*	GetColorBufData()		{ return (const GLfloat*)m_vColorBuf.data(); };
	void	AddToBuffer(CTextVertexColorBuffer& Bufer);
	GLvoid	glPrintES(GLfloat X, GLfloat Y, const char *fmt, ...);
#endif

};


//		Класс сопоставляющий части текстуры с символами, изображенными на текстуре ( достаточно произвольной )

//using uchar = unsigned short;		//   предполагается, что символы юникодные ( как в Android и Java ).

//	Структура описывающая положение символа на текстуре и при его рисование

struct SGLGlyph
{
	wchar_t		m_cSymbol = 0;
	float		m_fXpos = 0.0f;		//	положение символа на текстуре ( верхний левый угол )
	float		m_fYpos = 0.0f;
	float		m_fWidth;			//	размер прямоугольной области, занимаемой символом
	float		m_fHeight;
	float		m_fXoff = 0.0f;		//	смещение символа при выводе текста
	float		m_fYoff = 0.0f;
	SGLGlyph() = default;
	SGLGlyph( wchar_t Symbol, float Xpos, float Ypos, float Width, float Height, float fXoff = 0.0f, float Yoff = 0.0f) :
		m_cSymbol(Symbol), m_fXpos(Xpos), m_fYpos(Ypos), m_fWidth(Width), m_fHeight(Height), m_fXoff(fXoff), m_fYoff(Yoff)  {}
	void GetVertexes( std::vector<float>& Vertexes,int Xoff, int Yoff, float Z ) {
		Vertexes.resize(3*2*4);
		Vertexes[0] = m_fXpos + Xoff; Vertexes[1] = m_fYpos + Yoff; Vertexes[2] = Z;
		Vertexes[3] = m_fXpos + Xoff + m_fWidth; Vertexes[4] = m_fYpos + Yoff; Vertexes[5] = Z;

		Vertexes[6] = m_fXpos + Xoff + m_fWidth; Vertexes[7] = m_fYpos + Yoff; Vertexes[8] = Z;
		Vertexes[9] = m_fXpos + Xoff + m_fWidth; Vertexes[10] = m_fYpos + Yoff + m_fHeight; Vertexes[11] = Z;

		Vertexes[12] = m_fXpos + Xoff + m_fWidth; Vertexes[13] = m_fYpos + Yoff + m_fHeight; Vertexes[14] = Z;
		Vertexes[15] = m_fXpos + Xoff; Vertexes[16] = m_fYpos + Yoff + m_fHeight; Vertexes[17] = Z;

		Vertexes[18] = m_fXpos + Xoff; Vertexes[19] = m_fYpos + Yoff + m_fHeight; Vertexes[20] = Z;
		Vertexes[21] = m_fXpos + Xoff; Vertexes[22] = m_fYpos + Yoff; Vertexes[23] = Z;
	}
};


struct SFontSerial
{
	char*		m_hFontName;
	char*		m_sFileName;
	int			m_iWidth;
	int			m_iHeight;
	int			m_iXoff;
	int			m_iYoff;
	int			m_iGlyphHeight;
	size_t		m_iSize;
	SGLGlyph	m_Gliph[256];

	void setFileName(const char* fileName) {
		m_sFileName = new char[strlen(fileName) + 1];
		strncpy(m_sFileName, fileName, strlen(fileName)+1);
 	}
};


class CGLTextureFont
{
	std::string							m_FontName;
	std::map< wchar_t, SGLGlyph >		m_mSymbols;				//   набор символов в шрифте
public :
};
