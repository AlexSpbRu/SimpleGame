#include "stdafx.h"
#include <io.h>
#include <fcntl.h>


#include "Math3D.h"

#include "utils.h"

#ifdef OPENGLES20
//typedef void (APIENTRYP PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
#pragma comment (lib,"libGLESv2.lib")
#endif
#ifdef OPENGL

//   OpenGL extensions support
PFNGLMULTITEXCOORD1FARBPROC		glMultiTexCoord1fARB		= NULL;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB		= NULL;
PFNGLMULTITEXCOORD3FARBPROC		glMultiTexCoord3fARB		= NULL;
PFNGLMULTITEXCOORD4FARBPROC		glMultiTexCoord4fARB		= NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB			= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB	= NULL;

//GLint maxTexelUnits = 1;												// Number Of Texel-Pipelines. This Is At Least 1.


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

PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;

PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
//PFNGLglBlendEquationPROC glBlendEquation = NULL;
//PFNGLglIsProgramPROC glIsProgram = NULL;
//PFNGLglIsBufferPROC glIsBuffer = NULL;

// Program

PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
//PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
//PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
//PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
//PFNGLUNIFORM3IVPROC glUniform3iv = NULL;
//PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
//PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
//PFNGLUNIFORM2FVPROC glUniform2fv = NULL;
//PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLUNIFORM4FVPROC glUniform4fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
//PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = NULL;
//PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = NULL;
//PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = NULL;
//PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = NULL;
//PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
//PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = NULL;
//PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;
//
//// Shader
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
//
//// VBO
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC	glBindBuffer = NULL;
PFNGLBUFFERDATAPROC	glBufferData = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLBINDVERTEXARRAYAPPLEPROC glBindVertexArray = NULL;
PFNGLDELETEVERTEXARRAYSAPPLEPROC glDeleteVertexArrays = NULL;
PFNGLBUFFERSUBDATAPROC  glBufferSubData = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;



void InitAPI()
{

//	glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
//	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
//	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
//	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	//glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	//glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
	//glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebuffer");
	//glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	//glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	//glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	//glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	//glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1D");
	//glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	//glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3D");
	//glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	//glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
	//glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	//glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)wglGetProcAddress("glBlitFramebuffer");
	//glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)wglGetProcAddress("glRenderbufferStorageMultisample");
	//glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)wglGetProcAddress("glFramebufferTextureLayer");

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	/*glBlendEquation = (PFNGLglBlendEquationPROC)wglGetProcAddress("glBlendEquation");
	glIsProgram = (PFNGLglIsProgramPROC)wglGetProcAddress("glIsProgram");
	glIsBuffer = (PFNGLglIsBufferPROC)wglGetProcAddress("glIsBuffer");*/

	// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	//glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	/*glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");*/
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	/*glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");*/
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"); 
	/*glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");*/
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	/*glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");*/

	// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

	// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYAPPLEPROC)wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSAPPLEPROC)wglGetProcAddress("glDeleteVertexArrays");
	glBufferSubData = ( PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");  
	glGenVertexArrays =(PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");  
}

#else

void InitAPI() {};

#endif
void _cdecl IO_TRACE(LPCSTR lpszFormat, ...)
{

	static HANDLE hFileIni = 0;
	if (hFileIni == 0)
	{
		char namefile[MAX_PATH];
		char nameproj[256];

		DWORD len = GetModuleFileName(NULL, namefile, MAX_PATH);
		char buff[256];
		DWORD buff_len;
		GetComputerName(buff, &buff_len);
		char* end_ = namefile + len;
		while (end_-- >= namefile && *end_ != '.');

		char* sn = end_;

		while (end_-- >= namefile && *end_ != '\\');
		char* en = end_;
		memcpy(nameproj, (void*)(en + 1), sn - en);
		nameproj[sn - en - 1] = 0;
		strcat(nameproj, buff);

		*end_ = 0;
		strcat(namefile, "\\TRACE\\");
		CreateDirectory(namefile, NULL);

		strcat(namefile, nameproj);


		strcat(namefile, ".TRACE");


		hFileIni = CreateFile(namefile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD ER = GetLastError();
	}

	va_list args;
	va_start(args, lpszFormat);



	int nBuf;
	char szBuffer[2512];
	nBuf = _vsnprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);

	//	IO_TRACE_FULL ( szBuffer);
	DWORD Written;

	WriteFile(hFileIni, (void*)szBuffer, nBuf, &Written, NULL);

	//	if( IsDebuggerPresent())
	OutputDebugString(szBuffer);
	va_end(args);
}

unsigned int	OpenAndReadFile(const char* FileName, char** Buffer)
{
	unsigned int Size = 0;
	int  read_ = 0;
	//IO_TRACE("OpenAndReadFile %s\n", FileName);
	int hFile = open(FileName, _O_RDONLY | _O_BINARY);

	if (hFile == -1) {
		if (errno == ETXTBSY) {
			IO_TRACE("open failed ETXTBSY %s %d\n", FileName, errno);
			int iTry = 0;
			while (++iTry < 20) {
				Sleep(10);
				hFile = open(FileName, _O_RDONLY | _O_BINARY);
			}
		}

		if (hFile == -1)
		{
			IO_TRACE("open failed %s error = %d\n", FileName, errno);
			return 0;
		}
	}

	IO_TRACE("fopen ok %s\n", FileName);

	Size = lseek(hFile, 0, SEEK_END);
	lseek(hFile, 0, SEEK_SET);
	if (*Buffer == 0)
		*Buffer = new char[Size];
	read_ = read(hFile, (LPVOID)*Buffer, Size);
	if (Size != read_)
	{
		if (EAGAIN == errno)
			read_ = read(hFile, (LPVOID)Buffer, Size);
		if (Size != read_)
		{
			IO_TRACE("read failed: h = %d error = %d \n", hFile, errno);
			return 0;
		}
	}

	close(hFile);
	return  Size;
}

int		OpenFile(const char* FileName, bool create, bool shared, bool write_mode)
{
	int	hFile = -1; //O_DIRECT

	int flag = O_RDONLY | O_BINARY;
	if (write_mode)
		flag = O_RDWR | O_BINARY;
	if (!shared) 	{
		IO_TRACE("OpenFile O_EXCL %s \n", FileName);
		flag |= O_EXCL;
	}

	if (create) 	{
		flag |= O_CREAT;
		hFile = open(FileName, flag, S_IREAD | S_IWRITE);
	}
	else hFile = open(FileName, flag);

	if (hFile == 0)
		hFile = open(FileName, flag);
	if (hFile == -1)	{
		if ( errno == ETXTBSY )	{
			IO_TRACE("open failed ETXTBSY %s %d\n", FileName, errno);
			int iTry = 0;
			while (++iTry < 20)		{
				Sleep(10);
				hFile = open(FileName, flag);
			}
		}

		if (hFile == -1)	{
			IO_TRACE("open failed %s %d\n", FileName, errno);
			return -1;
		}
	}
	return  hFile;
}

bool FileWrite(int hFile, void* Buffer, unsigned int Size)
{
	unsigned int read_ = 0;
	read_ = write(hFile, Buffer, Size);
	if (Size != read_)	{
		IO_TRACE(" write failed: h = %d error = %d \n", hFile, errno);
		return 0;
	}
	return  Size == read_;
}


bool	CloseFile(int hFile)
{
	return close(hFile) == 0;
}


//#ifdef OPENGL
void GetAppDirectory(std::string& strPath)
{
	static TCHAR	szRunPath[MAX_PATH];
	GetModuleFileName(NULL, szRunPath, MAX_PATH);
	strPath = szRunPath;
	int pos = strPath.find_last_of('\\');
	strPath = strPath.substr(0, pos + 1);
}


//#endif