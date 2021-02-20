#include "stdafx.h"

#include <fcntl.h>
//#include <sys\types.h>
//#include <sys\stat.h>
#include <io.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

#include "Math3D.h"
#include "utils.h"

void _cdecl IO_TRACE(LPCSTR lpszFormat, ...);

#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF


inline int IntToPow2(int a)
{
	if (a <= 1) return 1;
	int hh = 1; a--;
	while (a >>= 1)hh++;
	return 1 << hh;
}



bool LoadImageGL(LPCTSTR pSrcFile, int& W, int& H, GLenum& PicFormat, unsigned char** pSrcData, int& SrcDataSize, int VerticallyFlip = 0)
{
	char* buffer = 0;
	size_t insize = OpenAndReadFile(pSrcFile, &buffer);
	if (insize == 0)
		return false;
	int  component;

	stbi__vertically_flip_on_load = VerticallyFlip;
	*pSrcData = stbi_load_from_memory((const stbi_uc *)buffer, insize, &W, &H, &component, 0);
//	IO_TRACE("stbi_load_from_memory SrcDataSize=%d w=%d h=%d ", SrcDataSize, W, H);
	if (*pSrcData == 0)
	{
//		IO_TRACE(" stbi_load_from_memory  %s  ERROR %s \n", pSrcFile, stbi_failure_reason());
		return false;
	}

	if (component == 4)
		PicFormat = GL_RGBA;
	else if (component == 3)
		PicFormat = GL_RGB;
	else if (component == 1)
		PicFormat = GL_ALPHA;
	else __debugbreak();//RM_ASSERT(__FILE__, __LINE__)
	delete[] buffer;
	return true;
}

bool CreateTextureFromFileInMemory(LPCVOID pSrcData, UINT SrcDataSize, GLuint& Texture, int& W, int& H, GLenum& Format, GLint MAGfilter, GLint MINfilter, int VerticallyFlip )
{
	/*if(glActiveTexture == nullptr )
		glActiveTexture = (PFNGLglActiveTexturePROC)wglGetProcAddress("glActiveTexture");*/
	bool r = true;
	
	//timer_0 t("****** CreateTextureFromFileInMemory");
	unsigned char *pBits;
	int w, h, component;
	stbi__vertically_flip_on_load = VerticallyFlip;
	pBits = stbi_load_from_memory((const stbi_uc *)pSrcData, SrcDataSize, &w, &h, &component, 0);
	IO_TRACE("stbi_load_from_memory SrcDataSize=%d w=%d h=%d \n", SrcDataSize, w, h);
	if (pBits == 0)
	{
//		IO_TRACE(" stbi_load_from_memory ERROR %s \n", stbi_failure_reason());
		Texture = 0;
		return false;
	}

	W = w;// IntToPow2(w);
	H = h;// IntToPow2(h);

	if (component == 4) 
		Format = GL_RGBA;
	else if (component == 3) 
		Format = GL_RGB;
	else if (component == 1) 
		Format = GL_ALPHA;
	else
		__debugbreak();//RM_ASSERT(__FILE__, __LINE__)

	if (component == 1)
		for (int i = 0; i < w*h; i++)pBits[i] = 255 - pBits[i];
	if (component == 4 && W == 1024 && h == 1024)
		for (UINT i = 0; i != w*h; i++)
		{
			UINT& p = ((UINT*)pBits)[i];
			if (p == 0xff000000)
				p = 0xff040404;
		}
//	IO_TRACE("stbi_load_from_memory 1\n");
	//if( component == 3 )
	//for (UINT i=0;i!= 3*w*h;)
	//{
	//	UCHAR& r=pBits[i++];
	//	UCHAR& g=pBits[i++];
	//	UCHAR& b=pBits[i++];
	//	if(r+g+b==0)r=g=b=1;		
	//}
	try {
		glGenTextures(1, &Texture);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MINfilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAGfilter);
		//if (W == w && H == h)
#ifndef OPENGLES20
		if (MINfilter == GL_NEAREST_MIPMAP_NEAREST || MINfilter == GL_LINEAR_MIPMAP_NEAREST || MINfilter == GL_NEAREST_MIPMAP_LINEAR || MINfilter == GL_LINEAR_MIPMAP_LINEAR)
			gluBuild2DMipmaps(GL_TEXTURE_2D, Format, w, h, Format, GL_UNSIGNED_BYTE, pBits);
		else
#endif
			glTexImage2D(GL_TEXTURE_2D, 0, Format, w, h, 0, Format, GL_UNSIGNED_BYTE, pBits);
		//else
		//{

		//}
		//{
		//	glTexImage2D(GL_TEXTURE_2D, 0, format, W, H, 0, format, GL_UNSIGNED_BYTE, 0);
		//	for (int i = 0; i<h; i++)
		//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i, w, 1, format, GL_UNSIGNED_BYTE, pBits + i*w*component);
		//}
//		IO_TRACE("stbi_load_from_memory 2\n");
		free(pBits);

		int err = glGetError();
		if (err)
			IO_TRACE("glBufferData   failed=%d\n", err);

		IO_TRACE("t=%d ok\n", Texture);
		W = w; H = h;
//		IO_TRACE("stbi_load_from_memory 3\n");
	}
	catch (...) {
		r = false;
		free(pBits);
	}
	return r;
}


bool CreateTextureFromFileGL(LPCTSTR pSrcFile, GLuint& Texture, int& W, int& H, GLenum& Format, GLint MAGfilter, GLint MINfilter, int VerticallyFlip)
{
	char* buffer = 0;
	bool r = true;
	size_t insize = OpenAndReadFile(/*"d:\\Projects\\OpenGL_tests\\Lesson01\\Bin\\Data\\Game\\Girls\\warning1.jpg"*/pSrcFile, &buffer);
	if (insize == 0)
		return false;
	//if (std::string(pSrcFile).find("error") != std::string::npos || std::string(pSrcFile).find("warning") != std::string::npos )
	//	__debugbreak();
	r = CreateTextureFromFileInMemory(buffer, insize, Texture, W, H, Format, MAGfilter,  MINfilter, VerticallyFlip);
	delete[] buffer;
	return r;
}