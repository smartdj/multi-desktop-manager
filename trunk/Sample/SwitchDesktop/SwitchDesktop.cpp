// SwitchDesktop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#pragma comment(lib,"User32.lib")
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
	BOOL ret = SwitchDesktop(dsk);
	if(!ret)
	{
		printf("SwitchDesktop Error! Error Code: %d\n",GetLastError());
	}
	CloseDesktop(dsk);
	return 0;
}

