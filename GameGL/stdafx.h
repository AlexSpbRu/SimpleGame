// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

#define OPENGL
//#define OPENGLES20
#define OPENGL_SHADER

#ifdef OPENGL
#include <gl\gl.h>	
#include <gl\glu.h>	
#include "gl\glext.h"
#endif

#ifdef OPENGLES20

//#define GL_ES_VERSION_2_0
//#include <GLES3/gl3platform.h> //OpenGL ES 3.0 Platform - Dependent Macros.
#include <EGL/egl.h>
#include <GLES3/gl3.h>// OpenGL ES 3.0 Header File
#include <GLES2/gl2ext.h> //OpenGL ES Extension Header File.

#endif
#define USE_TIMER
#include "timer_0.h"
void _cdecl IO_TRACE(LPCSTR lpszFormat, ...);

extern const std::string appPath;

#pragma warning ( disable : 4996 )
