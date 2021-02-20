//GlobManage.cpp
#include "stdafx.h"		    // Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library


//bool LoadGLTexturesAUX(CStringArray * sArray);             // Создание текстур из изображений, которые в файлах
CString GetDataFile(CString fName, BOOL bInform);
bool m_bFirstDraw = TRUE;
extern CString m_strNum;
int DrawGLSceneN(void);


//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////