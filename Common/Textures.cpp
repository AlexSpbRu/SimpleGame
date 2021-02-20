#include "stdafx.h"
#include "Textures.h"

#include "Math3D.h"
#include "image.h"
#include "utils.h"

bool CGLTexture::LoadBitmap( const std::string& FileName )
{
	unsigned char header[54];	// ������ BMP ���� ���������� � 54�������� ���������
	unsigned int dataPos;		// ������� � ����� ��� ���� ������ ����������
	//unsigned int width, height;
	//unsigned int imageSize;		// = ������*������*3
								// ���� RGB ������
	// ��������� ����
	FILE * file = fopen( FileName.c_str(), "rb" );
	if (!file) {
		printf("Image could not be opened\n"); return 0;
	}

	//	BMP ���� ���������� � 54 �������� ���������.���� ��������� �������� ����� ���������� ��� : ���� ������������� BMP ���� ? �, ������ �����������, ���������� ����� �� �������, ���.������� ������� ������� ��������� :

	if (fread(header, 1, 54, file) != 54) { // � ��� ��������, ���� �� ������ ��������� 54 �����
		printf("Not a correct BMP file\n");
		return false;
	}
	//   � ��� ��� ����� ��������� ������ ��� ������� :
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return false;
	}

	// ������ int �� ������� ������
	dataPos = *(int*)&(header[0x0A]);
	m_iImageSize = *(int*)&(header[0x22]);
	m_iWidth = *(int*)&(header[0x12]);
	m_iHeight = *(int*)&(header[0x16]);

	// � ��������� BMP ������ ��� ������ ����������, ������� �� � ������� ����
	if ( m_iImageSize == 0 )   
		m_iImageSize = m_iWidth*m_iHeight * 3;	// 3 : ���� ���� �� ������ Red, Green, Blue ����������
	if ( dataPos == 0 )     
		dataPos = 54;					// ��� ������������� ���������, � �� ����, ������ �������� ������

															// ������� �����
	m_pData =  new unsigned char[m_iImageSize];

	// ������ ������ �� ����� � �����
	fread(m_pData, 1, m_iImageSize, file);
	//������ ��� ������ � ������, � ����� ������� ����
	fclose(file);
	//
	for (auto* pos = m_pData; pos < m_pData + m_iImageSize; pos += 3)	{
		std::swap(*pos, *(pos + 2));
	}
	return true;
}

bool CGLTexture::glLoadImage(const std::string& FileName)
{
	if (m_iTexture != 0) {
		glBindTexture(GL_TEXTURE_2D, m_iTexture);
		m_iTexture = 0;
		m_bCreated = false;
		if (m_pData) free(m_pData);
		m_pData = nullptr;
	} 

	return LoadImageGL(FileName.c_str(), m_iWidth, m_iHeight, m_PicFormat, &m_pData, m_iImageSize);
}

void CGLTexture::Assign(CGLTexture& Texture)
{
	if (m_iTexture != 0) {
		glBindTexture(GL_TEXTURE_2D, m_iTexture);
		m_iTexture = 0;
		m_bCreated = false;
	}
		//
	m_iWidth = Texture.m_iWidth;
	m_iHeight = Texture.m_iHeight;
	m_iImageSize = Texture.m_iImageSize;
	m_PicFormat = Texture.m_PicFormat;
	m_pData = Texture.m_pData;
}

bool CGLTexture :: CreateTexture(GLint MAGfilter, GLint MINfilter)

{
	if (m_iTexture != 0) {
		glBindTexture(GL_TEXTURE_2D, m_iTexture);
		m_iTexture = 0;
	}
	if (m_pData == nullptr)
		return false;
	glGenTextures(1, &m_iTexture);

	glBindTexture(GL_TEXTURE_2D, m_iTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MINfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAGfilter);
#ifndef OPENGLES20
	if (MINfilter == GL_NEAREST_MIPMAP_NEAREST || MINfilter == GL_LINEAR_MIPMAP_NEAREST || MINfilter == GL_NEAREST_MIPMAP_LINEAR || MINfilter == GL_LINEAR_MIPMAP_LINEAR)
		gluBuild2DMipmaps(GL_TEXTURE_2D, m_PicFormat, m_iWidth, m_iHeight, m_PicFormat, GL_UNSIGNED_BYTE, m_pData);
	else
#endif
		glTexImage2D(GL_TEXTURE_2D, 0, m_PicFormat, m_iWidth, m_iHeight, 0, m_PicFormat, GL_UNSIGNED_BYTE, m_pData);

	int err = glGetError();
	if (err)
		IO_TRACE("glBufferData   failed=%d\n", err);

	m_bCreated = true;
	return true;
}
bool CGLTexture :: CreateTexture(const std::string& FileName, GLint MAGfilter, GLint MINfilter, int VerticallyFlip)
{
	if (m_iTexture != 0) {
		glBindTexture(GL_TEXTURE_2D, m_iTexture);
		m_iTexture = 0;
	} 
	return m_bCreated = CreateTextureFromFileGL(FileName.c_str() , m_iTexture, m_iWidth, m_iHeight, m_PicFormat,  MAGfilter,  MINfilter, VerticallyFlip);
}

CTextureManager& CTextureManager::GetInstance()
{
	static CTextureManager	textureManager;
	return textureManager;
}

bool CTextureManager::Init()
{
	bool ret = true;
	std::string		AppPath;
	GetAppDirectory( AppPath);

	std::string  strPath = AppPath + "Data\\Game\\player1.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Player)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\enemy1.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Enemy)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\star.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Live)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\hourglass2.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Hourglass)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\line.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Line)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\bomb1.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Bomb)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\ArrowLeft.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Arrow)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Font.bmp";
	if (!m_arrTextures[static_cast<size_t>(EType::Font)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
		ret = false;
	strPath = AppPath + "Data\\Game\\pause.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Pause)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
		ret = false;
	strPath = AppPath + "Data\\Game\\start.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Start)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
		ret = false;
	strPath = AppPath + "Data\\Game\\speed.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Speed)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\digits1.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Digits)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
		ret = false;
	strPath = AppPath + "Data\\Game\\background.jpg";
	if (!m_arrTextures[static_cast<size_t>(EType::Background)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
		ret = false;
	strPath = AppPath + "Data\\Game\\PanelBackground.png";
	if (!m_arrTextures[static_cast<size_t>(EType::Panel)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	strPath = AppPath + "Data\\Game\\StartPanel.png";
	if (!m_arrTextures[static_cast<size_t>(EType::StartPanel)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false; 
	strPath = AppPath + "Data\\Game\\PanelButton.png";
	if (!m_arrTextures[static_cast<size_t>(EType::PanelBtn)].CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 0))
		ret = false;
	return ret;
}

CGLTexture*	CTextureManager::GetTexture(EType  Texture)
{

	return &m_arrTextures[static_cast<size_t>(Texture)];
}