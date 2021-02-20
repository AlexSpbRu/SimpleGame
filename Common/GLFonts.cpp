#include "stdafx.h"

#include "Math3D.h"
#include "Shader.h"
#include "OGLBuffers.h"

#include "Textures.h"
#include "GLFonts.h"

//COGLFont :: COGLFont(HDC hDc, FontType  Type) : m_Type( Type ), m_hDc( hDc ), m_CountSymbols(256)
//{
//
//}
//
//COGLFont :: ~COGLFont()
//{
//	Delete();
//}
//
//
//GLvoid	COGLFont :: Delete() 
//{ 
//	glDeleteLists(m_fontList, m_CountSymbols);
//}
//
//GLvoid COGLFont::SetLOGFONT(GLint Height, const char* FontName, LOGFONT& LogFont)
//{
//	LogFont ={ Height,							// Height Of Font
//		0,								// Width Of Font
//		0,								// Angle Of Escapement
//		0,								// Orientation Angle
//		FW_BOLD,						// Font Weight
//		FALSE,							// Italic
//		FALSE,							// Underline
//		FALSE,							// Strikeout
//		ANSI_CHARSET,					// Character Set Identifier
//		OUT_TT_PRECIS,					// Output Precision
//		CLIP_DEFAULT_PRECIS,			// Clipping Precision
//		ANTIALIASED_QUALITY,			// Output Quality
//		FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
//		' ' };						   // Font Name
//	strncpy(LogFont.lfFaceName, FontName, LF_FACESIZE);
//}
//
//
//COGLRasterFont :: COGLRasterFont(HDC hDc) : COGLFont( hDc, FontType::Raster)
//{ }
//
//COGLRasterFont :: ~COGLRasterFont()
//{
//
//}
//
//GLvoid	COGLRasterFont::glCreatefont(const  LOGFONT& LogFont)
//{
//	HFONT	oldfont;
//
//	m_fontList = glGenLists(m_CountSymbols);
//
//	HFONT font = CreateFontIndirect(&LogFont);
//
//	oldfont = (HFONT)SelectObject(m_hDc, font);
//	wglUseFontBitmaps(m_hDc, 0, 256, m_fontList);
//	SelectObject(m_hDc, oldfont);
//	DeleteObject(font);
//}
//
//GLvoid	COGLRasterFont::glCreatefont(GLint Height, const char* FontName)
//{
//	LOGFONT LogFont;
//	SetLOGFONT(Height, FontName, LogFont);
//	glCreatefont(LogFont);
//}
//
//
//
//GLvoid	COGLRasterFont::Print(const char *fmt, ...)
//{
//	if (fmt == NULL)
//		return;
//	char		text[256];
//	va_list		ap;
//	va_start(ap, fmt);
//	vsprintf(text, fmt, ap);
//	va_end(ap);
//	glPushAttrib(GL_LIST_BIT);
//	glListBase(m_fontList);
//	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
//	glPopAttrib();
//}
//
//
//
//COGLVectorFont::COGLVectorFont(HDC hDc) : COGLFont(hDc, FontType::Vector)
//{
//	m_gmf.resize(m_CountSymbols);
//}
//
//
//COGLVectorFont :: ~COGLVectorFont()
//{
//
//}
//
//GLvoid	COGLVectorFont::glCreatefont(const  LOGFONT& LogFont)
//{
//	m_fontList = glGenLists(256);								// Storage For 256 Characters
//
//	HFONT	font = CreateFontIndirect(&LogFont);
//	HFONT	oldfont;
//	oldfont = (HFONT)SelectObject(m_hDc, font);							// Selects The Font We Created
//
//	wglUseFontOutlines(m_hDc,			// Select The Current DC
//		0,								// Starting Character
//		m_CountSymbols,					// Number Of Display Lists To Build
//		m_fontList,						// Starting Display Lists
//		0.0f,							// Deviation From The True Outlines
//		0.2f,							// Font Thickness In The Z Direction
//		WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
//		&m_gmf[0]);						// Address Of Buffer To Recieve Data
//
//	SelectObject(m_hDc, oldfont);
//	DeleteObject(font);
//}
//
//
//GLvoid	COGLVectorFont::glCreatefont(GLint Height, const char* FontName)
//{
//	LOGFONT LogFont;
//	SetLOGFONT(Height, FontName, LogFont);
//	glCreatefont(LogFont);
//}
//
//GLvoid	COGLVectorFont::Print(const char *fmt, ...)
//{
//	if (fmt == NULL)
//		return;
//	char		text[256];
//	va_list		ap;
//	va_start(ap, fmt);
//	vsprintf(text, fmt, ap);
//	va_end(ap);
//
//	float		length = 0;								// Used To Find The Length Of The Text
//	for (unsigned int i = 0; i < (strlen(text)); i++)	// Loop To Find Text Length
//	{
//		if (text[i] >= m_gmf.size()) {
//			__debugbreak();
//			break;
//		}
//		length += m_gmf[text[i]].gmfCellIncX;			// Increase Length By Each Characters Width
//	}
//
//	glTranslatef(-length / 2, 0.0f, 0.0f);					// Center Our Text On The Screen
//
//	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
//	glListBase(m_fontList);									// Sets The Base Character to 0
//	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
//	glPopAttrib();										// Pops The Display List Bits
//
//}
//
//
//COGL2DFont :: COGL2DFont( HDC hDc ) : COGLFont( hDc, FontType::Texture )
//{
//	m_CountSymbols = 256;
//}
//
//COGL2DFont :: ~COGL2DFont()
//{
//
//}
//
////GLvoid	COGL2DFont::glCreatefont(const  LOGFONT& LogFont)
////{
////	GLuint fboId, textureId;
////	// Генерируем текстуру, задаем ей размер 1024x1024 RGBA8888, резервируем память но не инициализируем.
////	glGenTextures(1, &textureId);
////	glBindTexture(GL_TEXTURE_2D, textureId);
////	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
////	// Poor filtering.Needed !
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
////	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
////	// Создаем фреймбуффер, прикрепляем к нему нашу текстуру
////	glGenFramebuffers(1, &fboId);
////	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
//////	glBindFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
////	// ...
////	// Отрисовываем сцену в фреймбуффер
////	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
//////	glDrawElements(...);
////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	// Отрисовываем сцену, используя полученную текстуру в задний буффер
////}
//
////GLvoid	COGL2DFont::glCreatefont(GLint Height, const char* FontName)
////{
////
////}
//
//GLvoid	COGL2DFont::glCreatefont( const char* ImageName )
//{
//	if (!m_Texture.CreateTexture(ImageName, GL_LINEAR, GL_LINEAR, 1))
//		return;
//	float	cx;											// Holds Our X Character Coord
//	float	cy;											// Holds Our Y Character Coord
//
//	m_fontList = glGenLists(m_CountSymbols);			// Creating 256 Display Lists
//	m_Texture.Bind();
//	for (auto pos = 0; pos < m_CountSymbols; pos++)		
//	{
//		cx = float(pos % 16) / 16.0f;					// X Position Of Current Character
//		cy = float(pos / 16) / 16.0f;					// Y Position Of Current Character
//
//		glNewList(m_fontList + pos, GL_COMPILE);		// Start Building A List
//		glBegin(GL_QUADS);								// Use A Quad For Each Character
//		glTexCoord2f(cx, 1 - cy - 0.0625f);				// Texture Coord (Bottom Left)
//		glVertex2i(0, 0);								// Vertex Coord (Bottom Left)
//		glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);	// Texture Coord (Bottom Right)
//		glVertex2i(16, 0);								// Vertex Coord (Bottom Right)
//		glTexCoord2f(cx + 0.0625f, 1 - cy);				// Texture Coord (Top Right)
//		glVertex2i(16, 16);								// Vertex Coord (Top Right)
//		glTexCoord2f(cx, 1 - cy);						// Texture Coord (Top Left)
//		glVertex2i(0, 16);								// Vertex Coord (Top Left)
//		glEnd();										// Done Building Our Quad (Character)
//		glTranslated(10, 0, 0);							// Move To The Right Of The Character
//		glEndList();									
//	}												
//
//}
//
//
//GLvoid COGL2DFont::Print( GLint x, GLint y, const char* string, int Set )
//{
//	if ( Set > 1 ) 
//		Set = 1;
//	bool isTextEnabled = glIsEnabled(GL_TEXTURE_2D);
//	if( !isTextEnabled )
//		glEnable(GL_TEXTURE_2D);
//	m_Texture.Bind();
//	GLboolean depthTest = true;
//	glGetBooleanv( GL_DEPTH_TEST, &depthTest);
//	if ( depthTest )
//		glDisable(GL_DEPTH_TEST);						// Disables Depth Testing
//	float matrix[4][4];
//	glGetFloatv(GL_PROJECTION_MATRIX, &matrix[0][0]);
//	bool isOrtho = true;
//	for (auto i = 0; i < 3; ++i)
//		for (auto j = 0; j < 3; ++j)
//			if (i != j && matrix[i][i] != 0.0f)
//				isOrtho = false;
//	if (!isOrtho) {
//		glMatrixMode(GL_PROJECTION);					// Select The Projection Matrix
//		glPushMatrix();									// Store The Projection Matrix
//
//		glLoadIdentity();									// Reset The Projection Matrix
//		glOrtho(0, 640, 0, 480, -1, 1);						// Set Up An Ortho Screen
//
//		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
//		glPushMatrix();										// Store The Modelview Matrix
//	}
//
//	glLoadIdentity();									// Reset The Modelview Matrix
//	glTranslated(x, y, 0);								// Position The Text (0,0 - Bottom Left)
//	glListBase(m_fontList - 32 + (128 * Set));						// Choose The Font Set (0 or 1)
//	if (Set == 0)												// If Set 0 Is Being Used Enlarge Font
//		glScalef(1.5f, 2.0f, 1.0f);							// Enlarge Font Width And Height
//	glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);// Write The Text To The Screen
//	if (!isOrtho) {
//		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
//		glPopMatrix();										// Restore The Old Projection Matrix
//		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
//		glPopMatrix();										// Restore The Old Projection Matrix
//	}
//	if (depthTest)
//		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
//	if( !isTextEnabled )
//		glEnable(GL_TEXTURE_2D);
//}
//
//GLvoid COGL2DFont::glPrint(GLint x, GLint y, int set, const char *fmt, ...)	// Where The Printing Happens
//{
//	char		text[256];									// Holds Our String
//	va_list		ap;											// Pointer To List Of Arguments
//
//	if (fmt == NULL)										// If There's No Text
//		return;												// Do Nothing
//
//	va_start(ap, fmt);										// Parses The String For Variables
//	vsprintf(text, fmt, ap);								// And Converts Symbols To Actual Numbers
//	va_end(ap);												// Results Are Stored In Text
//
//	Print(x, y, text, set);
//}

//GLuint CreateRastrFont(HDC	hDC)
//{
//	HFONT	font;
//	HFONT	oldfont;
//
//	GLuint	fontList = glGenLists(256);
//
//	font = CreateFont(-15,				// Height Of Font
//		0,								// Width Of Font
//		0,								// Angle Of Escapement
//		0,								// Orientation Angle
//		FW_BOLD,						// Font Weight
//		FALSE,							// Italic
//		FALSE,							// Underline
//		FALSE,							// Strikeout
//		RUSSIAN_CHARSET,				// Character Set Identifier
//		OUT_TT_PRECIS,					// Output Precision
//		CLIP_DEFAULT_PRECIS,			// Clipping Precision
//		ANTIALIASED_QUALITY,			// Output Quality
//		FF_DONTCARE | DEFAULT_PITCH,	// Family And Pitch
//		"Courier New");					// Font Name
//
//	oldfont = (HFONT)SelectObject(hDC, font);
//	wglUseFontBitmaps(hDC, 0, 256, fontList);
//	SelectObject(hDC, oldfont);
//	DeleteObject(font);
//	return fontList;
//}

//GLuint CreateVectorFont(HDC	hDC)								// Build Our Bitmap Font
//{
//	HFONT	font;										// Windows Font ID
//
//	GLuint	fontList = glGenLists(256);								// Storage For 256 Characters
//
//	font = CreateFont(-12,							// Height Of Font
//		0,								// Width Of Font
//		0,								// Angle Of Escapement
//		0,								// Orientation Angle
//		FW_BOLD,						// Font Weight
//		FALSE,							// Italic
//		FALSE,							// Underline
//		FALSE,							// Strikeout
//		ANSI_CHARSET,					// Character Set Identifier
//		OUT_TT_PRECIS,					// Output Precision
//		CLIP_DEFAULT_PRECIS,			// Clipping Precision
//		ANTIALIASED_QUALITY,			// Output Quality
//		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
//		"Comic Sans MS");				// Font Name
//
//	SelectObject(hDC, font);							// Selects The Font We Created
//
//	wglUseFontOutlines(hDC,				// Select The Current DC
//		0,								// Starting Character
//		255,							// Number Of Display Lists To Build
//		fontList,							// Starting Display Lists
//		0.0f,							// Deviation From The True Outlines
//		0.2f,							// Font Thickness In The Z Direction
//		WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
//		gmf);							// Address Of Buffer To Recieve Data
//	return fontList;
//}

//////-----------------------------------------------------------------------------------------------------------

COGLESFont::COGLESFont() : m_Texture(CTextureManager::GetInstance().GetTexture(CTextureManager::EType::Font))
{

}

#ifndef OPENGL_SHADER

GLvoid COGLESFont :: glPrint(GLfloat X, GLfloat Y, CTextVertexColorBuffer& Bufer, const char *fmt, ...)
{
	char		text[512];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	GLfloat  xpos = X, ypos = Y;
	m_Texture->Bind();
	for (auto pos = text; *pos != 0; ++pos) {
		auto chPos = *pos - 32;
		float cx = float(chPos % 16) / 16.0f;						// X Position Of Current Character
		float cy = float(chPos / 16) / 16.0f;						// Y Position Of Current Character
		Bufer.AddTexture(cx, 1.0f - cy - 0.0625f);
		Bufer.AddVertex(xpos, ypos + m_fSize, 3.0f);
		Bufer.AddTexture(cx, 1.0f - cy);
		Bufer.AddVertex(xpos, ypos, 3.0f);
		Bufer.AddTexture(cx + 0.0625f, 1.0f - cy - 0.0625f);
		Bufer.AddVertex(xpos + m_fSize, ypos + m_fSize, 3.0f);

		Bufer.AddTexture(cx + 0.0625f, 1.0f - cy);
		Bufer.AddVertex(xpos + m_fSize, ypos, 3.0f);
		Bufer.AddTexture(cx + 0.0625f, 1.0f - cy - 0.0625f);
		Bufer.AddVertex(xpos + m_fSize, ypos + m_fSize, 3.0f);
		Bufer.AddTexture(cx, 1.0f - cy);
		Bufer.AddVertex(xpos, ypos, 3.0f);
		xpos += m_fSize;
	}
	Bufer.SetColor(m_Color.r, m_Color.g, m_Color.b, m_Color.a );

}

#else
GLvoid	COGLESFont::glPrintES(GLfloat X, GLfloat Y, const char *fmt, ...)
{
	char		text[512];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);
	//
	auto sz = strlen(text);
	m_vVertexBuf.reserve(m_vVertexBuf.size()+sz*6);
	m_vTextureBuf.reserve(m_vTextureBuf.size()+sz*6);
	m_vColorBuf.reserve(m_vColorBuf.size()+sz*6);
	//
	GLfloat  xpos = X, ypos = Y;
	m_Texture->Bind();
	for (auto pos = text; *pos != 0; ++pos) {
		auto chPos = *pos - 32;
		float cx = float(chPos % 16) / 16.0f;						// X Position Of Current Character
		float cy = float(chPos / 16) / 16.0f;						// Y Position Of Current Character
		m_vTextureBuf.emplace_back(cx, 1.0f - cy - 0.0625f);
		m_vVertexBuf.emplace_back(xpos, ypos - m_fSize, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx, 1.0f - cy);
		m_vVertexBuf.emplace_back(xpos, ypos, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx + 0.0625f, 1.0f - cy - 0.0625f);
		m_vVertexBuf.emplace_back(xpos + m_fSize, ypos - m_fSize, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx + 0.0625f, 1.0f - cy);
		m_vVertexBuf.emplace_back(xpos + m_fSize, ypos, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx + 0.0625f, 1.0f - cy - 0.0625f);
		m_vVertexBuf.emplace_back(xpos + m_fSize, ypos - m_fSize, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);

		m_vTextureBuf.emplace_back(cx, 1.0f - cy);
		m_vVertexBuf.emplace_back(xpos, ypos, -0.3f);
		m_vColorBuf.emplace_back(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
		xpos += m_fSize;
	}
}

void	COGLESFont::AddToBuffer(CTextVertexColorBuffer& Bufer)
{
	const GLfloat*	vert = GetVertexBufData();
	const GLfloat*	tex = GetTextureBufData();
	const GLfloat*	col = GetColorBufData();
	Bufer.AddVertex(vert, vert + GetVertexBufSize());
	Bufer.AddTexture(tex, tex + GetTextureBufSize());
	Bufer.AddColor(col, col + GetColorBufSize());
	m_vVertexBuf.clear();
	m_vTextureBuf.clear();
	m_vColorBuf.clear();
}

#endif