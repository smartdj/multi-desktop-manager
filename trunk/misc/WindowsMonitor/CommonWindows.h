//CommonWindows.h
#pragma once
//--------------------------------------------------------------------
// File include
//--------------------------------------------------------------------
#include "CommonMisc.h"
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
//--------------------------------------------------------------------
// Inline function PointInRect
//--------------------------------------------------------------------
inline bool PointInRect( const RECT& rect, const POINT& point )
{
	return 
		point.x >= rect.left && 
		point.x <= rect.right &&
		point.y >= rect.top &&
		point.y <= rect.bottom;
}
//--------------------------------------------------------------------
// Inline function GetMainDirectory
//--------------------------------------------------------------------
inline std::wstring GetMainDirectory()
{
	wchar_t* pCurrentPath = new wchar_t[ MAX_PATH ];
	
	GetModuleFileName(	0, 
						pCurrentPath, 
						MAX_PATH );
	wchar_t* pTChar = wcsrchr(pCurrentPath, L'\\');
	*pTChar = L'\0';
	lstrcat( pCurrentPath, L"\\" );

	std::wstring main = pCurrentPath;

	Safe_Delete_Array( pCurrentPath );

	return main;
}
//--------------------------------------------------------------------
// Inline function OutputInfo
//--------------------------------------------------------------------
#ifndef OUTPUT_MODE_ODS
#define OUTPUT_MODE_ODS		0x0001
#endif
#ifndef OUTPU_MODE_MB
#define OUTPUT_MODE_MB		0x0002
#endif
#ifndef OUTPU_MODE_CSL
#define OUTPUT_MODE_CSL		0x0003
#endif 

inline void OutputInfo(	const std::wstring& info, 
						int mode = OUTPUT_MODE_ODS)
{
	switch ( mode )
	{
	case OUTPUT_MODE_ODS:
		OutputDebugString( info.c_str() );
		OutputDebugString( L"\n" );
		break;
	case OUTPUT_MODE_MB:
		MessageBox( 0, L"Exception", info.c_str(), 0 );
		break;
	case OUTPUT_MODE_CSL:
		wprintf( ( info + L"\n" ).c_str() );
		break;
	default:
		OutputDebugString( info.c_str() );
		OutputDebugString( L"\n" );
		break;
	}
}
//--------------------------------------------------------------------
// Inline function CreateUniqueInstance
//--------------------------------------------------------------------
inline int CreateUniqueInstance( const std::wstring& mutex )
{
	if ( mutex.empty() )
	{
		return 0;
	}
	int iReturn = 1;
	HANDLE hMutex = CreateMutex( NULL, FALSE, mutex.c_str() );
	if ( hMutex != NULL )
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{					
			iReturn = 0;
			goto Exit;
		}
	}
	else
	{
		iReturn = 0;
	}
Exit:
	//CloseHandle(hMutex); This should never close if you want to keep unique instance;
	return iReturn;
}