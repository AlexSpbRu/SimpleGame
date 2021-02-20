#pragma once

#ifdef OPENGL
#include "gl\glext.h"
#endif

#ifdef OPENGLES20
//#include <gles20\gles2\gl2.h>	
#endif

enum struct EDirection {
	None /*= 0*/,
	Right/* = 1*/,
	Left/* = 2*/,
	Top/* = 4*/,
	Bottom/* = 8*/
};

enum struct ERotation {
	R0,		//		без поворота
	R90,	//		поворот на 90 градусов
	R180,	//		поворот на 180 градусов
	R270,	//		поворот на 270 градусов
};

	
#ifdef OPENGL
//PFNGLISRENDERBUFFERPROC						glIsRenderbuffer = NULL;
//PFNGLBINDRENDERBUFFERPROC					glBindRenderbuffer = NULL;
//PFNGLDELETERENDERBUFFERSPROC				glDeleteRenderbuffers = NULL;
//PFNGLGENRENDERBUFFERSPROC					glGenRenderbuffers = NULL;
//PFNGLRENDERBUFFERSTORAGEPROC				glRenderbufferStorage = NULL;
//PFNGLGETRENDERBUFFERPARAMETERIVPROC			glGetRenderbufferParameteriv = NULL;
//PFNGLISFRAMEBUFFERPROC						glIsFramebuffer = NULL;
//PFNGLBINDFRAMEBUFFERPROC					glBindFramebuffer = NULL;
//PFNGLDELETEFRAMEBUFFERSPROC					glDeleteFramebuffers = NULL;
//PFNGLGENFRAMEBUFFERSPROC					glGenFramebuffers = NULL;
//PFNGLCHECKFRAMEBUFFERSTATUSPROC				glCheckFramebufferStatus = NULL;
//PFNGLFRAMEBUFFERTEXTURE1DPROC				glFramebufferTexture1D = NULL;
//PFNGLFRAMEBUFFERTEXTURE2DPROC				glFramebufferTexture2D = NULL;
//PFNGLFRAMEBUFFERTEXTURE3DPROC				glFramebufferTexture3D = NULL;
//PFNGLFRAMEBUFFERRENDERBUFFERPROC			glFramebufferRenderbuffer = NULL;
//PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = NULL;
//PFNGLGENERATEMIPMAPPROC						glGenerateMipmap = NULL;
//PFNGLBLITFRAMEBUFFERPROC					glBlitFramebuffer = NULL;
//PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC		glRenderbufferStorageMultisample = NULL;
//PFNGLFRAMEBUFFERTEXTURELAYERPROC			glFramebufferTextureLayer = NULL;

extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

extern  PFNGLACTIVETEXTUREPROC glActiveTexture;
//PFNGLglBlendEquationPROC glBlendEquation = NULL;
//PFNGLglIsProgramPROC glIsProgram = NULL;
//PFNGLglIsBufferPROC glIsBuffer = NULL;

// Program

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
//PFNGLDETACHSHADERPROC glDetachShader = NULL;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
//PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
//PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
//PFNGLUNIFORM3IVPROC glUniform3iv = NULL;
//PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
extern PFNGLUNIFORM1FPROC glUniform1f;
//PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
//PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
//PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
//PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = NULL;
//PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = NULL;
//PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = NULL;
//PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = NULL;
//PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = NULL;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
//PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
//PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;
//
//// Shader
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
//
//// VBO
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC	glBindBuffer;
extern PFNGLBUFFERDATAPROC	glBufferData;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLBINDVERTEXARRAYAPPLEPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSAPPLEPROC glDeleteVertexArrays;
extern PFNGLBUFFERSUBDATAPROC  glBufferSubData;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;

#endif

//#define INVALID_HANDLE_VALUE ((int)(LONG_PTR)-1)

int		OpenFile(const char* FileName, bool create, bool shared, bool write_mode);
bool FileWrite(int hFile, void* Buffer, unsigned int Size);
unsigned int	OpenAndReadFile(const char* FileName, char** Buffer);
bool	CloseFile(int hFile);

void GetAppDirectory(std::string& strPath);
void _cdecl IO_TRACE(LPCSTR lpszFormat, ...);
