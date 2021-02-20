#include "stdafx.h"
#include <limits>

#include "Math3D.h"
#include "Shader.h"
#include "OGLBuffers.h"
#include "Textures.h"

#include "FontImage.h"

#include "utils.h"


CFontImage::CFontImage()
{
}


CFontImage::~CFontImage()
{
}

#ifndef OPENGLES20
bool	CFontImage::DrawScene( CTextVertexColorBuffer& DrawBuffer )
{
	auto im_height = m_Image.Height();
	auto im_width = m_Image.Width();
	m_Image.Bind();
	GLfloat vertex[] = { 0.0f, m_iYoff, -4.0f,		   im_width , m_iYoff, -4.0f,
		0.0f, im_height + m_iYoff, -4.0f, im_width, im_height + m_iYoff, -4.0f };
	GLfloat tex[] = { 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f };
	unsigned int index[] = { 2, 1, 0, 2, 3, 1 };
	DrawBuffer.AddVertex(vertex, vertex + sizeof(vertex) / sizeof(GLfloat));
	DrawBuffer.AddTexture(tex, tex + sizeof(tex) / sizeof(GLfloat));
	DrawBuffer.AddIndex(index, index + sizeof(index) / sizeof(unsigned int));
	DrawBuffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.5f });
	DrawBuffer.DrawElements(GL_TRIANGLES);
	//
	std::vector<GLfloat> Vertexes;
	for (auto& gl : m_Glyphs) {
		gl.GetVertexes(Vertexes, m_iXoff, m_iYoff, -3.0f);
		DrawBuffer.AddVertex(&Vertexes[0], &Vertexes[0] + Vertexes.size());
	}
	DrawBuffer.SetColor(SOGLColor{ 1.0f, 1.0f, 1.0f, 0.5f });

	return true;
}


#endif

bool LoadImageGL(LPCTSTR pSrcFile, int& W, int& H, GLenum& PicFormat, unsigned char** pSrcData, int& SrcDataSize, int VerticallyFlip);

bool	CFontImage::ScanImageForFont(const std::string& ImageFile)
{
	int  SrcDataSize = 0;
	GLenum PicFormat = 0;
	unsigned char* pSrcData = nullptr;
	bool ret = LoadImageGL(ImageFile.c_str(), m_iWidth, m_iHeight, PicFormat, &pSrcData, SrcDataSize, 0 );
	assert(PicFormat == GL_RGBA);

	int* pix = (int*)pSrcData;

	std::vector< int >	prevRow, curRow;

	auto xStart = -1;
	for (auto row = 0; row < m_iHeight; ++row) {
		//IO_TRACE("  %d  %d ", row, Graph.size());
		curRow.clear();
		for (auto col = 0; col < m_iWidth; ++col) {
			auto alpha = *pix >> 24;
			if (/*alpha < 10 &&*/ alpha > 0 || *pix)
				int aa = 444;
			if (*pix*alpha == 0/*|| alpha < 150*/) {
				if (xStart >= 0) {   // конец сегмента
					m_Graph.emplace_back(Snode{ row, xStart, col - 1 });
					//IO_TRACE(" ( %d - %d ) ", xStart, col-1);
					xStart = -1;
					Snode& last = m_Graph.back();
					int ilast = m_Graph.size() - 1;
					curRow.push_back(ilast);
					for (auto& isegm : prevRow) {
						auto& segm = m_Graph[isegm];
						auto xminStop = min(segm.m_iXstop, last.m_iXstop);
						auto xmaxStart = max(segm.m_iXstart, last.m_iXstart);
						if (xmaxStart < xminStop) {
							segm.m_pNext.push_back(ilast);
							last.m_pPrev.push_back(isegm);
						}
					}
				}
			}
			else if (xStart < 0)
				xStart = col;
			pix++;
		}
		prevRow.swap(curRow);
		//IO_TRACE("\n");
	}
	free(pSrcData);
	//  обход графа в глубину и расчет boundbox
	for (auto& segm : m_Graph) {
		int Left = 0xeffffff, Top = 0xeffffff, Right = -1, Bottom = -1;
		if (segm.CalcBoundBox(Left, Top, Right, Bottom, m_Graph)) {
			if (Left != Right && Top != Bottom) {
				m_Glyphs.emplace_back(SGLGlyph{ 0, (GLfloat)Left, (GLfloat)Top, GLfloat(Right - Left), GLfloat(Bottom - Top) });
				IO_TRACE(" Left : %d  Top : %d  Width : %d  Height : %d \n", Left, Top, Right - Left, Bottom - Top);
			}
		}
	}
	IO_TRACE("Count %d  \n", m_Glyphs.size());
	//	вычисляем средний размер
	auto Smin = 0x7fffffff, Smax = 0, Saver = 0;
	auto Wmin = 0x7fffffff, Wmax = 0, Waver = 0;
	auto Hmin = 0x7fffffff, Hmax = 0, Haver = 0;

	for (auto& glyph : m_Glyphs) {
		auto s = glyph.m_fHeight*glyph.m_fWidth;
		Smin = min(s, Smin);
		Smax = max(s, Smax);
		Saver += s;

		Wmin = min(glyph.m_fWidth, Wmin);
		Wmax = max(glyph.m_fWidth, Wmax);
		Waver += glyph.m_fWidth;

		Hmin = min(glyph.m_fHeight, Hmin);
		Hmax = max(glyph.m_fHeight, Hmax);
		Haver += glyph.m_fHeight;
	}
	Saver = Saver / m_Glyphs.size();
	Waver = Waver / m_Glyphs.size();
	Haver = Haver / m_Glyphs.size();

	IO_TRACE("Smin %d  Smax %d  Saver %d  \n", Smin, Smax, Saver);
	IO_TRACE("Wmin %d  Wmax %d  Waver %d  \n", Wmin, Wmax, Waver);
	IO_TRACE("Hmin %d  Hmax %d  Haver %d  \n", Hmin, Hmax, Haver);

	//std::vector<SGLGlyph>	glyphs_filter;
	//for (auto& glyph : m_Glyphs) {
	//	if (glyph.m_fWidth > Waver / 5 && glyph.m_fHeight > Haver / 5)
	//		glyphs_filter.push_back(glyph);
	//}
	if (!m_Glyphs.empty()) {
		m_Image.CreateTexture(ImageFile, GL_LINEAR, GL_LINEAR, 1);
		m_sFileName = ImageFile;
		return true;
	}
	else return false;
}

bool	CFontImage::MakeFontFile()
{
	bool ret = false;
	std::string		fText;
	std::string		fName = m_sFileName.substr(0, m_sFileName.find_last_of('.'));
	std::string	fontName = fName.substr(fName.find_last_of("/\\")+1 );
	int		txtFont = OpenFile((fName + ".h").c_str(), false, false, false);
	if (txtFont == -1) {		//  создаем файл
		txtFont = OpenFile((fName + ".h").c_str(), true, false, true);
		char buff[1024];
		sprintf(buff,"	{  \"%s\", \"%s\",\n		 %d, %d, %d, %d, %d, %d,\n		{\n", 
			fontName.c_str(), m_sFileName.c_str(), m_iWidth, m_iHeight, m_iXoff, m_iYoff, m_iGlyphHeight, m_Glyphs.size());
		fText += buff;
		for( auto& gl : m_Glyphs ){
			sprintf(buff, "			{ 0, %ff, %ff, %ff, %ff, %ff, %ff },\n", gl.m_fXpos, gl.m_fYpos, gl.m_fWidth, gl.m_fHeight,gl.m_fXoff, gl.m_fYoff);
			fText += buff;
		}
		sprintf(buff, "		}\n	},\n", fontName.c_str(), m_Glyphs.size());
		fText += buff;
		bool FileWrite(int hFile, void* Buffer, unsigned int Size);
		ret = FileWrite(txtFont, (void *)fText.c_str(), fText.size());
		CloseFile(txtFont);
	}
	else {
		CloseFile(txtFont);
		txtFont = -1;
	}
	//
	fText.clear();
	txtFont = OpenFile((fName + ".java").c_str(), false, false, true);
	if (txtFont == -1) //{		//  создаем файл
		txtFont = OpenFile((fName + ".java").c_str(), true, false, true);

		char buff[1024];
		sprintf(buff, "package com.game.alex.androidgame.OGLUtils;\n\nimport com.game.alex.androidgame.R;\n\n");
		fText += buff;
		sprintf(buff, 
"public class EngSimpleFont   extends FontImage {\n\
	static EngSimpleFont    font = new EngSimpleFont()\n\
		{ {\n\
			m_iResID = R.drawable.%s;\n", fontName.c_str());
		fText += buff;
		sprintf(buff,
"           m_Image = TextureManager.GetInstance().GetTexture(TextureManager.EType.%s);\n\
           m_sName = \"%s\";\n\
           m_iWidth = %d;\n\
           m_iHeight = %d;\n\
           m_iXoff = %d;\n\
           m_iYoff = %d;\n\
           m_iGlyphHeight = %d;\n\
           m_Glyphs = new GLGlyph[%d];\n", fontName.c_str(), fontName.c_str(), m_iWidth, m_iHeight, m_iXoff, m_iYoff, m_iGlyphHeight, m_Glyphs.size() );
		fText += buff;

		int numGlypth = 0;
		for (auto& gl : m_Glyphs) {
			sprintf(buff, "           m_Glyphs[%d] = new GLGlyph('\u0020', %ff, %ff, %ff, %ff, %ff, %ff);\n",
				numGlypth, gl.m_fXpos, gl.m_fYpos, gl.m_fWidth, gl.m_fHeight, gl.m_fXoff, gl.m_fYoff);
			fText += buff;
			numGlypth++;
		}

		sprintf(buff, 
"        } };\n\
    static public  EngSimpleFont GetInstance() { return font; };\n\
}");
		fText += buff;
		bool FileWrite(int hFile, void* Buffer, unsigned int Size);
		ret = FileWrite(txtFont, (void *)fText.c_str(), fText.size());
		CloseFile(txtFont);
//	}
	return ret;
}

bool	CFontImage::MakeFontSerial(const SFontSerial& Data)
{
	m_sFileName = appPath + Data.m_sFileName;
	m_iWidth = Data.m_iWidth;
	m_iHeight = Data.m_iHeight;
	m_iXoff = Data.m_iXoff;
	m_iYoff = Data.m_iYoff;
	m_iGlyphHeight = Data.m_iGlyphHeight;
	auto num = 0;
	for (auto& gl : Data.m_Gliph) {
		if (gl.m_cSymbol != 0)
			m_Glyphs.emplace_back(gl);
	}

	if (!m_Glyphs.empty())
		return m_Image.CreateTexture(m_sFileName, GL_LINEAR, GL_LINEAR, 1);

	return false;
}

//#ifndef OPENGL_SHADER
//GLfloat	CFontImage::GetSymbol(wchar_t Symb, GLfloat Xoff, GLfloat Yoff, GLfloat Zoff, CTextVertexColorBuffer& DrawBuffer)
//{
//	SGLGlyph* pGl = nullptr;
//	
//	for_each(m_Glyphs.begin(), m_Glyphs.end(), [&pGl, Symb](SGLGlyph& glyph) { if (glyph.m_cSymbol == Symb)  pGl = &glyph; });
//	if (pGl != nullptr) {
//		auto indVert = DrawBuffer.Count();
//		DrawBuffer.AddVertex( std::vector<GLfloat>{ Xoff + pGl->m_fXoff*m_fXScale, Yoff - pGl->m_fYoff*m_fYScale, Zoff,
//			Xoff + pGl->m_fWidth*m_fXScale + pGl->m_fXoff*m_fXScale, Yoff - pGl->m_fYoff*m_fYScale, Zoff,
//			Xoff + pGl->m_fWidth*m_fXScale + pGl->m_fXoff*m_fXScale, Yoff - pGl->m_fHeight*m_fYScale - pGl->m_fYoff*m_fYScale, Zoff,
//			Xoff + pGl->m_fXoff*m_fXScale, Yoff - pGl->m_fHeight*m_fYScale - pGl->m_fYoff*m_fYScale, Zoff } );
// 
//		DrawBuffer.AddTexture( std::vector<GLfloat>{ (pGl->m_fXpos) / m_iWidth, (m_iHeight - pGl->m_fYpos - pGl->m_fHeight) / m_iHeight,
//			(pGl->m_fXpos + pGl->m_fWidth) / m_iWidth, (m_iHeight - pGl->m_fYpos - pGl->m_fHeight) / m_iHeight,
//			(pGl->m_fXpos + pGl->m_fWidth) / m_iWidth, (m_iHeight - pGl->m_fYpos) / m_iHeight,
//			(pGl->m_fXpos) / m_iWidth, (m_iHeight - pGl->m_fYpos) / m_iHeight});
//		
//		DrawBuffer.AddIndex( std::vector<unsigned int>{ indVert, indVert +1, indVert + 2, indVert, indVert + 2, indVert + 3 });
//
//		DrawBuffer.SetColor(m_Color);
//
//		return	Xoff + pGl->m_fWidth*m_fXScale;
//	}
//	return	Xoff;
//}
//#else
GLfloat	CFontImage::GetSymbolES(wchar_t Symb, GLfloat Xoff, GLfloat Yoff, GLfloat Zoff, CTextVertexColorBuffer& DrawBuffer)
{
	SGLGlyph* pGl = nullptr;

	for_each(m_Glyphs.begin(), m_Glyphs.end(), [&pGl, Symb](SGLGlyph& glyph) { if (glyph.m_cSymbol == Symb)  pGl = &glyph; });
	if (pGl != nullptr) {
		auto indVert = DrawBuffer.Count();
		/*DrawBuffer.AddVertices(std::vector<float>{ Xoff, Yoff, Zoff, Xoff + pGl->m_fWidth*m_fXScale, Yoff, Zoff,
			Xoff + pGl->m_fWidth*m_fXScale, Yoff - pGl->m_fHeight*m_iScale, Zoff, Xoff, Yoff - pGl->m_fHeight*m_iScale, Zoff }.data(), 12,
			std::vector<float>{ (pGl->m_fXpos) / m_iWidth, (m_iHeight - pGl->m_fYpos - pGl->m_fHeight) / m_iHeight,
			(pGl->m_fXpos + pGl->m_fWidth) / m_iWidth, (m_iHeight - pGl->m_fYpos - pGl->m_fHeight) / m_iHeight,
			(pGl->m_fXpos + pGl->m_fWidth) / m_iWidth, (m_iHeight - pGl->m_fYpos) / m_iHeight,
			(pGl->m_fXpos) / m_iWidth, (m_iHeight - pGl->m_fYpos) / m_iHeight}.data(), 8, 
			std::vector<unsigned int>{ indVert, indVert + 1, indVert + 2, indVert, indVert + 2, indVert + 3 }.data(), 6, m_Color);*/
		DrawBuffer.AddVertex(std::vector<GLfloat>{ Xoff, Yoff, Zoff, Xoff + pGl->m_fWidth*m_fXScale, Yoff, Zoff,
			Xoff + pGl->m_fWidth*m_fXScale, Yoff + pGl->m_fHeight*m_fYScale, Zoff, Xoff, Yoff + pGl->m_fHeight*m_fYScale, Zoff });

		DrawBuffer.AddTexture(std::vector<GLfloat>{ (pGl->m_fXpos) / m_iWidth, (m_iHeight - pGl->m_fYpos - pGl->m_fHeight) / m_iHeight,
			(pGl->m_fXpos + pGl->m_fWidth) / m_iWidth, (m_iHeight - pGl->m_fYpos - pGl->m_fHeight) / m_iHeight,
			(pGl->m_fXpos + pGl->m_fWidth) / m_iWidth, (m_iHeight - pGl->m_fYpos) / m_iHeight,
			(pGl->m_fXpos) / m_iWidth, (m_iHeight - pGl->m_fYpos) / m_iHeight});

		DrawBuffer.AddIndex(std::vector<unsigned int>{ indVert, indVert + 1, indVert + 2, indVert, indVert + 2, indVert + 3 });

		DrawBuffer.SetColor(m_Color);

		return	Xoff + pGl->m_fWidth*m_fXScale;
	}
	return	Xoff;
}
//#endif

void	CFontImage::SetSize(GLfloat Width, GLfloat  Height) {
	m_fYScale = Height / m_iGlyphHeight;
	if (m_iGlyphWidth == 0) {   //  вычисляем среднюю ширину символа
		for (SGLGlyph gl : m_Glyphs) {
			m_iGlyphWidth += gl.m_fWidth;
		}
		m_iGlyphWidth /= m_Glyphs.size();
	}
	if (m_iGlyphWidth != 0)
		m_fXScale = Width / m_iGlyphWidth;
	else
		m_fXScale = m_fYScale;
}

void	CFontImage::GetTextSize(const std::wstring& Text, GLfloat& Width, GLfloat& Height)
{
	Width = 0.0f;
	float ymax = std::numeric_limits<float>::lowest();
	float ymin = -std::numeric_limits<float>::lowest(); //std::numeric_limits<float>::max();
	for (auto wch : Text) {
		SGLGlyph* pGl = nullptr;
		for_each(m_Glyphs.begin(), m_Glyphs.end(), [&pGl, wch](SGLGlyph& glyph) { if (glyph.m_cSymbol == wch)  pGl = &glyph; });
		if (pGl != nullptr) {
			Width += (pGl->m_fXoff + pGl->m_fWidth)*m_fXScale;
			ymin = min(ymin, pGl->m_fYoff*m_fYScale);
			ymax = max(ymax, (pGl->m_fYoff + pGl->m_fHeight)*m_fYScale);
		}
	}
	Height = ymax - ymin;
}

void	CFontImage::TextDraw( const std::wstring& Text, GLfloat X, GLfloat Y, GLfloat Z, CTextVertexColorBuffer& DrawBuffer)
{
#ifndef OPENGL_SHADER
	m_Image.Bind();
	std::for_each(Text.begin(), Text.end(), [&X, Y, Z, &DrawBuffer, this](const wchar_t Symb) {
		auto xoff = this->GetSymbol(Symb, X, Y, Z, DrawBuffer);
#else
//	DrawBuffer.AffineTransformation2D(Vec3{ 0.0f, 0.0f, 0.0f }, 0.0f, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ 1.0f, 1.0f, 1.0f });
 	std::for_each(Text.begin(), Text.end(), [&X, Y, Z, &DrawBuffer, this](const wchar_t Symb) {
		auto xoff = this->GetSymbolES(Symb, X, Y, Z, DrawBuffer);
#endif
		X = xoff;
	} );
}

#ifndef OPENGLES20

void	CFontImageManager::InitGL() noexcept
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
	//m_LightAmbient = { 0.5f, 0.5f, 0.5f, 1.0f };
	//m_LightDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	//m_LightSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	//m_LightPosition = { 300.0f, 300.0f, 500.0f, 1.0f };
	////COGLDrawWithLight::Init();
	//glEnable(GL_LIGHTING);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, m_LightAmbient.data());		// Setup The Ambient Light
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, m_LightDiffuse.data());		// Setup The Diffuse Light
	//glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition.data());		// Position The Light
	//glLightfv(GL_LIGHT1, GL_SPECULAR, m_LightSpecular.data());
	//glEnable(GL_LIGHT1);											// Enable Light One
	//																//
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	//
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//
	//CMatrix::setIdentityM(m_ProjMatrix);
}


bool	CFontImageManager::Init(int Width, int Height) noexcept
{
	void InitAPI();
	InitAPI();

	InitGL();

	return true;
}

GLvoid	CFontImageManager::ResizeScene(GLsizei width, GLsizei height) noexcept
{
	m_iWidth = width;
	m_iHeight = height;
	if (height == 0)										// Prevent A Divide By Zero By
		height = 1;											// Making Height Equal One

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
	glOrtho(0.0f, width, height, 0.0f, -5.0f, 5.0f);		// Create Ortho 640x480 View (0,0 At Top Left)
															//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 10.0f, -2000.0f);
	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
															//glLoadIdentity();										// Reset The Modelview Matrix
}

bool	CFontImageManager::DrawScene()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	//
	glLoadIdentity();
	//  рисование сцены
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_iCurFont >= 0 && m_iCurFont < m_vFonts.size()) {
		m_vFonts[m_iCurFont].DrawScene(m_DrawBuffer);
		glLineWidth(2);
		m_DrawBuffer.DrawArray(GL_LINES);
	}

	int Width, Height;
	m_vFonts[m_iCurFont].GetSize(Width, Height);
	m_vFonts[m_iCurFont].SetSize(15.0f, 25.0f);
	m_vFonts[m_iCurFont].SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_vFonts[m_iCurFont].TextDraw( std::wstring(L"QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"), 10.0f, Height + 100, 0.0f, m_DrawBuffer );
	m_DrawBuffer.DrawElements(GL_TRIANGLES);
	
	return true;
}

#endif

void	CFontImageManager::AddFontSerial(const SFontSerial& Data)
{
	m_vFonts.emplace_back();
	CFontImage& font = m_vFonts.back();
	font.MakeFontSerial(Data);
}

void  CFontImageManager::InitFonts(const SFontSerial* Fonts, int Count)
{
	for (auto i = 0; i != Count; ++i)
		AddFontSerial(*(Fonts + i));
}
