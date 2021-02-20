#pragma once
#include <assert.h>

class CGLTexture
{
	GLuint		m_iTexture = 0;
	bool		m_bCreated = false;
	//
	int			m_iWidth = 0;
	int			m_iHeight = 0;
	int			m_iImageSize = 0;
	GLenum		m_PicFormat;
	unsigned char*		m_pData = nullptr;
	bool	LoadBitmap(const std::string& FileName);
public :
	CGLTexture() {};
	bool glLoadImage(const std::string& FileName);
	void Assign( CGLTexture& Texture );
	bool CreateTexture( GLint MAGfilter, GLint MINfilter );
	bool CreateTexture( const std::string& FileName, GLint MAGfilter, GLint MINfilter, int VerticallyFlip );
	void DeleteTexture() { glDeleteTextures(1, &m_iTexture); m_iTexture = 0; m_bCreated = false; };
	bool IsTexture() { return m_bCreated; };
	bool Bind() {
		if (m_bCreated) {
			glBindTexture(GL_TEXTURE_2D, m_iTexture);
			auto err = glGetError();
			assert(err == 0);
			return err == 0;
		}
		else return false;
	};
	void Free() { free( m_pData ); };
	//
	inline unsigned char* Data() { return m_pData; };
	inline int GetDataSize() { return m_iImageSize; };
	inline int Width() { return m_iWidth; };
	inline int Height() { return m_iHeight; };
	inline GLenum	GetFormat()	{ return m_PicFormat; };
	inline GLvoid SetData( unsigned char* Data) { m_pData = Data; };
	inline GLvoid SetDataSize( int Size ) {  m_iImageSize = Size; };
	inline GLvoid SetWidth( int Width ) {  m_iWidth = Width; };
	inline GLvoid SetHeight( int Height ) {  m_iHeight = Height; };
	inline GLvoid SetFormat( GLenum Format ) {  m_PicFormat = Format; };
};

class CTextureManager
{
public:
	enum class EType : int {
		None,
		Player,
		Enemy,
		Hourglass,
		Bomb,
		Live,
		Line,
		Arrow,
		Font,
		Pause,
		Start,
		Speed,
		Digits,
		Background,
		PanelBtn,
		Panel,
		StartPanel,
		textureCount
	};
protected :
	CGLTexture		m_arrTextures[EType::textureCount];
	CTextureManager() { Init(); };
	bool Init();
public :
	static CTextureManager& GetInstance();
	CGLTexture*	GetTexture(EType  Texture);
};
