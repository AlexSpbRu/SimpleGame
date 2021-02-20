//GlobMyProc.cpp 

#include "stdafx.h"
#include <math.h>           
#include "io.h"
#include <stdio.h>          
#include <atlimage.h>
#include <direct.h>


#include <gl\gl.h>          
#include <gl\glu.h>         
#include "mainfrm.h"

BOOL bTestPrint = TRUE;

BOOL DirUpToLessons(void);
FILE * MyFopen( CString fName, CString sMode);

CString strFormat(CString frmt, ...);
BOOL isFileExist( CString fileName);
CBitmap * GetImgBitmap(CImage * pImg);
extern CString m_strNum;                  
extern char * CommonCommand[];
static char str[1024];
void TestMessageBox(CString wString);

extern BOOL m_bRus;
extern CString m_strLesson; 

extern CStringArray m_strEngArray;
extern CStringArray m_strRusArray;

#if _MSC_VER >= 1400
int ch_dir( const char *dirname )
{
return _chdir( dirname );
}
#endif

void MakeBWMask(CImage * pImg, CDC *pDC, int w, int h, COLORREF eColor, int BPP)
{
	pImg->Create(w,h, BPP);
	CDC * tDC = CDC::FromHandle(pImg->GetDC());
    for( int i = 0; i < w; i++)
		for(int j = 0; j < h; j++)
		{
			COLORREF rgb = pDC->GetPixel(i,j);
			if (rgb == eColor)
				tDC->SetPixel(i,j, RGB(255, 255, 255));
			else
				tDC->SetPixel(i,j, RGB(0, 0, 0));
		}

}
////////////////////////////////////////////////////////////////////////////////////////
void ChangeColor(CImage * pImg,  COLORREF tColor, COLORREF sColor)
{
	if(tColor == sColor)
		return;
	CDC * tDC = CDC::FromHandle(pImg->GetDC());
	for( int i = 0; i < pImg->GetWidth(); i++)
		for(int j = 0; j < pImg->GetHeight(); j++)
		{
			COLORREF rgb = tDC->GetPixel(i,j);
			if (rgb == tColor)
				tDC->SetPixel(i,j, sColor);
		}

}
////////////////////////////////////////////////////////////////////////////////////////
 CMenu * MakeDubleMenu(CMenu * pMenu)
 {
    CMenu * pNewMenu = new CMenu;
	pNewMenu->CreateMenu();
	if(pMenu->GetMenuItemCount())
	for(int i = 0; i < (int)pMenu->GetMenuItemCount(); i++)
	{
	CMenu * tMenu = pMenu->GetSubMenu(i);
	if(tMenu == NULL)
		continue;
    CMenu * nMenu = MakeDubleMenu(tMenu);
	CString rString;
    pMenu->GetMenuString(i, rString, MF_BYPOSITION);
	if(tMenu->GetMenuItemCount())
	  pNewMenu->AppendMenu( MF_POPUP,(UINT_PTR) tMenu->m_hMenu,rString);
	else 
    	pNewMenu->AppendMenu( 0,tMenu->GetMenuItemID(i),rString);
 	}

	return pNewMenu;
 }
 /////////////////////////////////////////////////////////////////////////////
COLORREF ColorRandom(void)
{
//	return RGB( rand()%255, rand()%255, rand()%255); 
//	return RGB( rand()%127, rand()%127, rand()%127); 
	return RGB( rand()%196, rand()%196, rand()%196); 
}
/////////////////////////////////////////////////////
COLORREF ColorRandom(int rMin, int rMax)
{
	int rr = rMax - rMin;
	return RGB( rMin + rand()%rr, rMin + rand()%rr, rMin + rand()%rr); 
}
/////////////////////////////////////////////////////
void SetCurselLesson(CListBox * pBx, CString nStr)
{
	for(int i = 0; i < pBx->GetCount(); i++)
	{
		CString s;
		pBx->GetText(i, s);
		if(nStr == s)
		{
			pBx->SetCurSel(i);
			break;
		}//if(strN == s.Left(2))
		else
		{
		if(nStr == (CString)strtok(strcpy(str,s), m_strtok))
		{
			pBx->SetCurSel(i);
			break;
		}//if(strN == s.Left(2))
		}
	}//for(int i = 0; i < pBx->GetCount(); i++)




}
/////////////////////////////////////////////////////////////////////
void SetCurselOfCommand(CString aString, CListBox * pBx)
{
	CString tString;
	for(int i = 0; i < pBx->GetCount(); i++)
	{
	pBx->GetText(i, tString);
	if(tString.Left(aString.GetLength()) == aString)
	{
		pBx->SetCurSel(i);
		return;
	}
	}
}
/////////////////////////////////////////////////////////////////////////
void ResetListFromFile(CListBox * pBx, CString fName, BOOL bCmd)
{
	_getcwd(str, MAX_PATH);
//TestMessageBox("ResetListFromFile\n'" + fName + "'\n"+ (CString)str);
if(pBx == NULL)
	  return;
	pBx->ResetContent();
	if(bCmd)
	{
     int i = 0;
 		while (CommonCommand[i][0])
		  pBx->AddString(CommonCommand[i++]);
	}

	if(isFileExist(fName))
	{
//TestMessageBox("1 ResetListFromFile\n");
    FILE *f = MyFopen(fName, "rt");
	while(!feof(f))
	{
		fgets(str,256,f);
		CString s = str;
		if(!s.GetLength())
			continue;
		s = s.Left(s.GetLength()-1);
		if(!s.GetLength())
			continue;
		if(!feof(f))
			pBx->AddString(s);
	}
	fclose(f);
	}//if(isFileExist(fName))
    SetCurselLesson(pBx, m_strNum);
}
/////////////////////////////////////////////////////////////////////
CString OpenFileExt( CString strExt, CString strFname, CString strInfo, BOOL bRead)
{
	if(!strFname.GetLength())
       strFname = "*";
strcpy(str, strFname+"."+strExt);
CFileDialog dlg( bRead,"" + strExt +"", str, 
				bRead ? OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_CREATEPROMPT :
                  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT, 
                 "" + strInfo +"|" + (CString) str  + "| All files(*.*) | *.* ||",  NULL ); 
                   
if ( dlg.DoModal()==IDOK)
    return dlg.GetPathName(); 
else
   return "";
}
//////////////////////////////////////////////////////////////////////
CString strFormat(CString frmt, ...)
{
	 char    text[256];      // Место для нашей строки
  va_list    ap;          // Указатель на список аргументов
  if (!frmt.GetLength())     // Если нет текста
    return "*** Error Format ***";            // Ничего не делать
  va_start(ap, frmt);           // Разбор строки переменных
      vsprintf(text, frmt, ap); // И конвертирование символов в реальные коды

	  return text;
}
//////////////////////////////////////////////////////////////////////////////
BOOL isFileExist( CString fileName)
{
	_getcwd(str, _MAX_PATH);
//	TestMessageBox("isFileExist\n" + fileName + "\n" + (CString)str);
//	strcpy(str, fileName);

struct _finddata_t fInfo;
intptr_t handle = _findfirst( fileName, &fInfo );
_findclose(handle);
 return (handle != -1);
 
}
///////////////////////////////////////////
CString strRect(CRect rect)
{
	return strFormat("left = %d top = %d right = %d bottom = %d\nw = %d  h = %d\n",
		rect.left,rect.top,rect.right,rect.bottom,rect.Width(),rect.Height());
}
/////////////////////////////////////////////////////////////////////////////////
void TestMessageBox(CString wString)
{
	if(bTestPrint)
	{
		int rslt = AfxMessageBox(wString, MB_YESNOCANCEL);
		if(rslt == IDNO)
			bTestPrint = FALSE;
		else
			if(rslt == IDCANCEL)
				exit(0);
	}	

}
///////////////////////////////////////////////////////////////////////////////////
CString GetDataFile(CString fName, BOOL bInform)
{
	if(isFileExist(fName))
		return fName;

	CString sName = "../../data/" + fName;

	if(isFileExist(sName))
		return sName;


	sName = "../data/" + fName;

	if(isFileExist(sName))
		return sName;

	sName = "data/" + fName;

	if(isFileExist(sName))
		return sName;


if(bInform)
	MessageBox(NULL, "No file exist:\n" + fName, "Error:", MB_OK | MB_ICONERROR);
	return "";
}
/////////////////////////////////////////////////////////////////
unsigned long GetMyTime(void)
{
	time_t tt;
	time(&tt);
	return (unsigned long)tt;
}
////////////////////////////////////////////////////////////////////
CString GetNameStr(CString fName, CString nStr)
{
if(isFileExist(fName))
	
	if(!isFileExist(fName))
		return "";

CString tString = "";

   FILE *f = MyFopen(fName, "rt");
	while(!feof(f))
	{
		fgets(str,256,f);
		CString s = str;
		if(!s.GetLength())
			continue;
		s = s.Left(s.GetLength()-1);
		if(!s.GetLength())
			continue;
//TestMessageBox("GetNameStr\n" + nStr + "\n" + (CString)s);

		if(nStr == (CString)strtok(strcpy(str,s), m_strtok))
		{
			tString = s;
			break;
		}
	}
	fclose(f);
   return tString;
}
/////////////////////////////////////////////////////////////////////
CString GetLessonName(CString nStr)
{
	if(!m_strEngArray.GetSize())
		return "";
	CStringArray * pSarray = m_bRus ? &m_strRusArray : &m_strEngArray;

	for(int i = 0; i < pSarray->GetSize(); i++)
	{
		CString strT = strtok(strcpy(str, pSarray->GetAt(i)), m_strtok);
		if(strT == nStr)
		  return m_strLesson = pSarray->GetAt(i);
	}
	return "";

}
////////////////////////////////////////////////////////////////////
BOOL StartFirstExe(CString strD)
{

	_getcwd(str, _MAX_PATH);
	CString remD = str;
	ch_dir( strD);
	struct _finddata_t fInfo;
intptr_t handle = _findfirst("*.exe*", &fInfo );
if(handle != -1)
{
//TestMessageBox("StartFirstExe\n" + strD + "\n" + (CString)fInfo.name);	
ShellExecute(NULL , NULL, fInfo.name,   NULL , strD, SW_SHOWDEFAULT );
	return TRUE;
}

handle = _findfirst("*.*", &fInfo );

while(!_findnext(handle, &fInfo))
{
if( fInfo.attrib == _A_SUBDIR)
if(fInfo.name[0] != '.')
	if(StartFirstExe(fInfo.name))
	{
        _findclose(handle);
		return TRUE;
	}
}//while(!_findnext(handle, &finfo)

_findclose(handle);
ch_dir(remD);
return FALSE;
}
////////////////////////////////////////////////////////////////////
BOOL StartExeOfNum(CString nStr)
{
	CString strL = "lesson" + nStr;
	if(!DirUpToLessons())
	{
		MessageBox(NULL,"Failed to Find Dir of Lessons\n", "Sorry:",
			MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	_getcwd(str, MAX_PATH);
	CString remDir = str;
	if(!StartFirstExe(strL))
	{
		MessageBox(NULL, "No *.exe File Available in Path:\n" + strL, "Error:", MB_OK | MB_ICONINFORMATION );
		ch_dir(remDir);
		return FALSE;
	}
//TestMessageBox("StartExeOfNum\n" + strL);
	return TRUE;
}
//////////////////////////////////////////////////////////////////
char * GetDataFile(CString fName)              //To Find Data file from Current Dir
{
//TestMessageBox("1 GetDataFile\n" + fName + "\n");
	if(isFileExist(fName))
	{
//TestMessageBox("2 GetDataFile\n" + fName + "\nOk");
		return strcpy(str,fName);
	}
//	fName = "../" + fName;
	ch_dir("../");
	if(isFileExist(fName))
	{
//TestMessageBox("3 GetDataFile\n" + fName + "\n");

		return strcpy(str,fName);
	}
	else
	{
		_getcwd(str,_MAX_PATH);
		TestMessageBox("no File: GetDataFile\n" + fName + "\n" + (CString)str);
	}

	return "";
}
//////////////////////////////////////////////////////////////////
void ResetListFromFile(CListBox * pAx, CListBox * pBx, CString fName)
{
	_getcwd(str, MAX_PATH);
//TestMessageBox("ResetListFromFile\n'" + fName + "'\n"+ (CString)str);
if(pBx == NULL)
	  return;
	pAx->ResetContent();
	pBx->ResetContent();

	int i = 0;
 		while (CommonCommand[i][0])
		{
          CString s = CommonCommand[i++];
          strcpy(str, s);
		CString aString = strtok(str, ":");
		CString bString = s.Mid(aString.GetLength()+1);
		while(aString.Right(1)== " ")
			aString = aString.Left(aString.GetLength()-1);
//        TestMessageBox(  "ResetListFromFile\n" + aString + "\n" + bString);

		pAx->AddString(aString);
		pBx->AddString(bString);
		
		}

	if(!isFileExist(fName))
		return;
//TestMessageBox("1 ResetListFromFile\n");
	int nCount = 0;
    FILE *f = MyFopen(fName, "rt");
	while(!feof(f))
	{
		fgets(str,256,f);
		CString s = str;
		if(!s.GetLength())
			continue;
      if(nCount >= 2)
	  {
		str[strlen(str)-1] = '\0';
		s = s.Left(s.GetLength()-1);
		if(!s.GetLength())
			continue;
		CString aString = strtok(str, ":");
		CString bString = s.Mid(aString.GetLength()+1);
		while(aString.Right(1)== " ")
			aString = aString.Left(aString.GetLength()-1);
           
		if(!feof(f))
		{
			pAx->AddString(aString);
			pBx->AddString(bString);
		}
	  }
		nCount++;
	}
	fclose(f);
}
/////////////////////////////////////////////////////////////////////
void AppendListLessons(CString fName)
{
//TestMessageBox("AppendSummaryListLessons\n" + fName);
	FILE * f = MyFopen(fName, "rt");
    for(int i = 0; i < 2; i++)
	{
		fgets(str, 128, f);
		str[strlen(str)-1] = '\0';
		if(!strlen(str))
			continue;
		i ? m_strRusArray.Add(str) : m_strEngArray.Add(str);
 //       TestMessageBox("AppendListLessons\n" + (CString)str + "\n" + strD);
	}//for(int i = 0; i < 2; i++)
    fclose(f);
}
/////////////////////////////////////////////////
void AppendListLessonsDir(CString strD)
{
  //   TestMessageBox("AppendListLessons\n" + strD);

	if(strD.Find("GLSummary")>= 0)
		return;
	_getcwd(str, _MAX_PATH);
	CString remD = str;
	ch_dir(strD);
struct _finddata_t fInfo;

intptr_t handle = _findfirst("*_GLhelp.txt", &fInfo );
if(handle != -1)
{
   AppendListLessons(fInfo.name);
   ch_dir(remD);
	return;
}

handle = _findfirst("*.*", &fInfo );
while(!_findnext(handle, &fInfo))
{
if( fInfo.attrib == _A_SUBDIR)
if(fInfo.name[0] != '.')
	AppendListLessonsDir(fInfo.name);
}//while(!_findnext(handle, &finfo)

_findclose(handle);
	
	ch_dir(remD);

}
/////////////////////////////////////////////////////////
BOOL DirUpToLessons(void)
{
struct _finddata_t fInfo;
intptr_t handle = _findfirst("lesson*.*", &fInfo );
if( handle != -1 && fInfo.attrib == _A_SUBDIR)
  return TRUE;
_getcwd(str, _MAX_PATH);
//TestMessageBox("DirUpToLessons\n" + (CString)str);
if(strlen(str) <4)
   return FALSE;
 ch_dir("../");
return DirUpToLessons();

}

////////////////////////////////////////////////////////
void CreateListLessons(void)
{
	_getcwd(str, _MAX_PATH);
	CString remD = str;

	if(!DirUpToLessons())
	{
		MessageBox(NULL,"Failed to Create List of Lessons\n", "Sorry:",
			MB_OK | MB_ICONINFORMATION);
		return;
	}

struct _finddata_t fInfo;
intptr_t handle = _findfirst("*.*", &fInfo );

while(!_findnext(handle, &fInfo))
{
if( fInfo.attrib == _A_SUBDIR)
if(fInfo.name[0] != '.')
	AppendListLessonsDir(fInfo.name);
}//while(!_findnext(handle, &finfo)

_findclose(handle);

ch_dir(remD);
}
///////////////////////////////////////////////////////////////////
void CheckMyDir()
{
	_getcwd(str, _MAX_PATH);
	CString tString = str; 
	if(tString.Find("Release")>= 0)
		ch_dir("../");
}
//////////////////////////////////////////////////////////////////
void SummaryListLessons(void)
{
	_getcwd(str, _MAX_PATH);
	CString remD = str;

	ch_dir("HelpTexT");

struct _finddata_t fInfo;

intptr_t handle = _findfirst("*_GLhelp.txt", &fInfo );
if(handle == -1)
{
  MessageBox(NULL,"Failed to Create List of Lessons\n", "Sorry:",
	MB_OK | MB_ICONINFORMATION);
 return;
}//if(handle == -1)

AppendListLessons(fInfo.name);
while(!_findnext(handle, &fInfo))
	AppendListLessons(fInfo.name);

_findclose(handle);

ch_dir(remD);
}
/////////////////////////////////////////////////////////////////////////////
FILE * MyFopen( CString fName, CString sMode)
{
	FILE * f = NULL;
	errno_t rslt = fopen_s(&f, fName, sMode);
	if(!rslt) return f; 
	else 
	    return NULL;
}
/////////////////////////////////////////////////////////////////////////////




