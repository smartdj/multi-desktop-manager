#include <string>
#include <windows.h>
#pragma comment(lib,"User32.lib")
class DesktopManager
{
public:
	static BOOL Create(std::string desktopname)
	{
		return Create(desktopname.c_str());
	}

	static BOOL Create(std::wstring desktopname)
	{
		return Create(desktopname.c_str());
	}

	static BOOL Create(char * desktopname)
	{
		return CreateDesktopA(desktopname,NULL,NULL,0,GENERIC_ALL,NULL);
	}

	static BOOL Create(WCHAR * desktopname)
	{
		return CreateDesktopA(desktopname,NULL,NULL,0,GENERIC_ALL,NULL);
	}

	static bool Switch(HDESK desktop)
	{
		BOOL result = SwitchDesktop(desktop);
		return true;
	}
	static bool GetAllDesktops(
		__in          HWINSTA hwinsta,
		__in          DESKTOPENUMPROC lpEnumFunc,
		__in          LPARAM lParam)
	{
		EnumDesktops(hwinsta,lpEnumFunc,lParam);
		return true;
	}
	static bool GetDesktopWindows(
		__in          HDESK hDesktop,
		__in          WNDENUMPROC lpfn,
		__in          LPARAM lParam)
	{
		EnumDesktopWindows(hDesktop,lpfn,lParam);
		return true;
	}

	static BOOL CALLBACK EnumDesktopProc(
		LPTSTR lpszDesktop,
		LPARAM lParam)
	{
		return TRUE;
	}

	static BOOL CALLBACK EnumWindowsProc(
		HWND hwnd,
		LPARAM lParam)
	{
		return TRUE;
	}

	static BOOL Close(HDESK hDesktop)
	{
		return CloseDesktop(hDesktop);
	}

	static HDESK Open(std::string desktopname)
	{
		return Open(desktopname.c_str());
	}

	static HDESK Open(std::wstring desktopname)
	{
		return Open(desktopname.c_str());
	}

	static HDESK Open(char * lpszDesktop)
	{
		return OpenDesktopA(lpszDesktop,0,FALSE,GENERIC_ALL);
	}

	static HDESK Open(WCHAR * lpszDesktop)
	{
		return OpenDesktopW(lpszDesktop,0,FALSE,GENERIC_ALL);
	}

	static HDESK OpenInput()
	{
		return OpenInputDesktop(0,FALSE,GENERIC_ALL);
	}

	static BOOL SetThread(HDESK hDesktop)
	{
		return SetThreadDesktop(hDesktop);
	}

	static HDESK GetThread(DWORD dwThreadId)
	{
		return GetThreadDesktop(dwThreadId);
	}
};

class WindowStation
{
public:
	static BOOL Close(HWINSTA hWinSta)
	{
		return CloseWindowStation(hWinSta);
	}
	static BOOL GetAllWindowStation(
		__in          WINSTAENUMPROC lpEnumFunc,
		__in          LPARAM lParam)
	{
		return EnumWindowStations(lpEnumFunc,lParam);
	}

	static BOOL CALLBACK EnumWindowStationProc(
		LPTSTR lpszWindowStation,
		LPARAM lParam)
	{
		return TRUE;
	}

	static HWINSTA GetProcess()
	{
		return GetProcessWindowStation();
	}

	static HWINSTA Open(std::wstring stationname)
	{
		return Open(stationname.c_str());
	}

	static HWINSTA Open(std::string stationname)
	{
		return Open(stationname.c_str());
	}

	static HWINSTA Open(char * lpszWinSta)
	{
		return OpenWindowStationA(lpszWinSta,FALSE,GENERIC_ALL);
	}

	static HWINSTA Open(WCHAR * lpszWinSta)
	{
		return OpenWindowStationW(lpszWinSta,FALSE,GENERIC_ALL);
	}

	static BOOL WINAPI SetProcess(
		__in          HWINSTA hWinSta)
	{
		return SetProcessWindowStation(hWinSta);
	}
};