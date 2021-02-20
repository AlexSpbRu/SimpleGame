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
#include <algorithm>

#include <gl\gl.h>	
#include <gl\glu.h>	
#include "gl\glext.h"

struct timer_0 {
	timer_0(const char*) {};
};

#define OPENGL

void _cdecl IO_TRACE(LPCSTR lpszFormat, ...);

using GLfloat = float;

// TODO: reference additional headers your program requires here
