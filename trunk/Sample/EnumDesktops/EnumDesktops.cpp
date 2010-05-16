// EnumDesktops.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#pragma comment(lib,"User32.lib")
void WINAPI EnumDesktopCallBack(LPTSTR lpszDesktop,LPARAM lParam)
{
	wprintf(L"%s\n",lpszDesktop);
}
int _tmain(int argc, _TCHAR* argv[])
{
	HWINSTA winsta = GetProcessWindowStation();
	EnumDesktops(winsta,(DESKTOPENUMPROCW)EnumDesktopCallBack,NULL);
	CloseWindowStation(winsta);
	system("pause");
	return 0;
}

