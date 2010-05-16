// SwitchDesktopEx.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include   <UNKNWN.h>   
#include   <objbase.h>
#include <wininet.h>
#include   <shlguid.h>   
#include   <shlobj.h> 

#pragma comment(lib,"User32.lib")
void SetDesktopWallPaper(LPWSTR wallpaper)
{
	CoInitialize(NULL);   
    
	IActiveDesktop   *pDesk;   
	if(   S_OK   !=   CoCreateInstance   (   CLSID_ActiveDesktop,   NULL,   CLSCTX_INPROC_SERVER,   IID_IActiveDesktop,   (void**)   &pDesk   )   )   
	{ 
	  pDesk->Release();
	  return ;
	}   
	pDesk->SetWallpaper(wallpaper,0);
	WALLPAPEROPT wallpaperopt;
	wallpaperopt.dwSize = sizeof(WALLPAPEROPT);
	wallpaperopt.dwStyle = WPSTYLE_CENTER;
	pDesk->SetWallpaperOptions(&wallpaperopt,0);
	pDesk->ApplyChanges(AD_APPLY_ALL);
	pDesk->Release();
	CoFreeUnusedLibraries();   
	CoUninitialize();  
}
int _tmain(int argc, _TCHAR* argv[])
{
	if(argc != 2)
	{
		printf("Usage : SwitchDesktop [DesktopName]\n");
		return -1;
	}
	HDESK dsk = OpenDesktop(argv[1],0,FALSE,GENERIC_ALL);
	if(!dsk)
	{
		printf("OpenDesktop Error! Error Code: %d\n",GetLastError());
		return -1;
	}
	_TCHAR wallpaper[MAX_PATH];
	lstrcpy(wallpaper,argv[1]);
	lstrcat(wallpaper,L".bmp");
	SetDesktopWallPaper(wallpaper);
	BOOL ret = SwitchDesktop(dsk);
	if(!ret)
	{
		printf("SwitchDesktop Error! Error Code: %d\n",GetLastError());
	}
	CloseDesktop(dsk);
	return 0;
}

