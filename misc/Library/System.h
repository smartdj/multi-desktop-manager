#include <stdio.h>
#include <stdlib.h>
#include <atlpath.h>
#include <shlwapi.h>
#include <windows.h>
#include <time.h>
namespace System
{
	int GetSystemLanguage()
	{
		return  GetSystemDefaultLangID();
	}
	int CurrentTime(tm * t)
	{
		time_t tt = time(0);
		localtime_s(t,&tt);
		return 0;
	}
	void pause()
	{
		system("pause");
	}
	namespace Console
	{
		int HelloWorld()
		{
			printf("Hello\n");
			return 0;
		}
		int print(int Array[],int Length)
		{
			printf("[");
			for(int i = 0 ; i < Length - 1 ; i ++ )
			{
				printf("%d,",Array[i]);
			}
			printf("%d]\n",Array[Length - 1]);																
			return 0;
		}
		int print(WCHAR Array[][MAX_PATH],int Length)
		{
			wprintf(L"[");
			for( int i = 0 ; i < Length - 1; i ++ )
			{
				wprintf(L"\"%s\",",Array[i]);
			}
			wprintf(L"\"%s\"]\n",Array[Length - 1]);
			return 0;
		}
	}
	namespace Window
	{
		SIZE GetScreenSize()
		{
			SIZE size;
			size.cx = GetSystemMetrics(SM_CXSCREEN);
			size.cy = GetSystemMetrics(SM_CYSCREEN);
			return size;
		}
	}
	namespace IO
	{
		namespace File
		{
			
		}
		namespace Directory
		{
			
		}
	}
	namespace Path
	{
		char * Join(char p[],char q[])
		{
			char *path = (char*)malloc(MAX_PATH);
			sprintf_s(path,MAX_PATH,"%s\\%s",p,q);
			/*strcpy_s(path,MAX_PATH,p);
			strcat_s(path,"\\");
			strcat_s(path,MAX_PATH,q);*/
			return path;
		}
		wchar_t* Join(wchar_t * p,wchar_t* q)
		{
			wchar_t *path = (wchar_t*)malloc(MAX_PATH);
			wsprintf(path,L"%s\\%s",p,q);
			/*lstrcpyW(path,p);
			lstrcatW(path,L"\\");
			lstrcatW(path,q);*/
			return path;
		}
		BOOL Exsits(const char* pszPath)
		{
			return PathFileExistsA(pszPath);
		}
		BOOL Exsits(const wchar_t* pszPath )
		{
			return PathFileExistsW(pszPath);
		}
		BOOL IsDir(const char* pszPath)
		{
			return PathIsDirectoryA(pszPath);
		}
		BOOL IsDir(const wchar_t* pszPath)
		{
			return PathIsDirectoryW(pszPath);
		}
	}
}