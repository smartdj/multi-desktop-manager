//main.cpp
//--------------------------------------------------------------------
// File include
//--------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include "WindowMonitor.h"
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <tlhelp32.h>
#include "CommonWindows.h"
//--------------------------------------------------------------------
// Load library
//--------------------------------------------------------------------
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "winmm.lib" )
//--------------------------------------------------------------------
// //Type define
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// Global variables define
//--------------------------------------------------------------------
static const std::wstring c_strFilename;
static std::wstring s_strMainDirectory;
//XML config
CXMLParser*		m_pConfig = 0;
XMLLog			m_Log;
//--------------------------------------------------------------------
// Function declare
//--------------------------------------------------------------------
int __stdcall wWinMain(	HINSTANCE,
						HINSTANCE,
						LPWSTR,
						int )
{
	if ( CreateUniqueInstance( L"WindowMonitor.exe" ) == 0 )
	{
		return 0;
	}
	WindowMonitor* pMonitor = new WindowMonitor;

	if ( pMonitor )
	{
		if ( pMonitor->Initialize() )
		{
			for ( ; ; )
			{
				Sleep( 10000 );
			}
			//WaitForSingleObject( TRUE, INFINITE );
		}
	}
	Safe_Delete( pMonitor );
	//
	//int* param = 0;
	//HANDLE hMonitor = 
	//CreateThread(	0,
	//				0,
	//				( LPTHREAD_START_ROUTINE )MonitorThread,
	//				param,
	//				0,
	//				0 );
	//WaitForSingleObject( hMonitor, 0xffffffff );
	//CloseHandle( hMonitor );
	//Clean();
	return 0;
}