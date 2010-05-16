// CreateDesktop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#pragma comment(lib,"User32.lib")
_TCHAR _Explorer[520]; 
void WINAPI CreateNewDesktop( LPVOID lpParam)
{
	HWINSTA winsta = GetProcessWindowStation();
	SetProcessWindowStation(winsta);
	_TCHAR * desktopname = (_TCHAR *)lpParam;
	HDESK newdesktop = CreateDesktop(desktopname,NULL,NULL,0,GENERIC_ALL,NULL);
	SetThreadDesktop(newdesktop);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si,0,sizeof(si));
	memset(&pi,0,sizeof(pi));
	si.cb = sizeof(si);
	si.lpDesktop = desktopname;
	CreateProcess(NULL,_Explorer,NULL,NULL,FALSE,0, NULL, NULL, &si, &pi);
	Sleep(4000);
	CloseDesktop(newdesktop);
	CloseWindowStation(winsta);
}
int _tmain(int argc, _TCHAR* argv[])
{
	if(argc != 2)
	{
		printf("Usage: CreateDesktop [DesktopName]\n");
		return -1;
	}
	lstrcpy(_Explorer,L"C:\\WINDOWS\\EXPLORER.exe");
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CreateNewDesktop,(LPVOID)argv[1],0,NULL);
	for( int i = 0 ; i < 10 ; i ++ )
	{
		printf(".");
		Sleep(500);
	}
	printf("\n");
	return 0;
}

