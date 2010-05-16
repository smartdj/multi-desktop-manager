//WindowMonitor.h
#pragma once
//--------------------------------------------------------------------
// File include
//--------------------------------------------------------------------
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <shlwapi.h>
#include <Psapi.h>

#include "XMLLog.h"
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Psapi.lib")
//--------------------------------------------------------------------
// Class WindowMonitor define
//--------------------------------------------------------------------

class WindowMonitor
{
public:
	//Constants/Static members
	static const unsigned int c_uiEnumFunctionReturn = 0x0001;
public:
	//Constructor/Destructor
	WindowMonitor();
	virtual ~WindowMonitor();
public:
	//Type define
	typedef std::map< HWND, bool > TitleMap;
public:
	//Structure
	struct WindowAttribute
	{
	public:
		std::wstring		strAttachDesktop;
		std::wstring		strWindowTitle;
	};
	struct DesktopAttribute
	{
	public:
		HDESK			hDesktop;
		std::wstring	strDesktopName;
		TitleMap		PreviousTitles;
		TitleMap		CurrentTitles;
	};
public:
	//Type define
//	typedef vector< HDESK > DesktopVec;
	typedef std::vector< DesktopAttribute > DesktopVec;
	typedef std::vector< WindowAttribute > WindowAttributeVec;
public:
	//Methods
	bool Initialize();
private:
	void Clean();
	int Transport2Server();
	int EnumNewWindows(	const std::wstring& name,
						TitleMap& cur, 
						TitleMap& pre );
	int EnumDesktop();
	int EnumNewDesktop();
	int EnumWindowByDesktop();
	int RefreshWindowMap();

public:
	//Core thread
	static unsigned int __stdcall MonitorThread( void* param );
	static unsigned int __stdcall ShowPopupWindowThread(LPVOID param);
public:
	//System callback
	static unsigned int __stdcall EnumDesktopCallback( wchar_t* name, unsigned int param );
	static unsigned int __stdcall EnumNewDesktopCallback( wchar_t* name, unsigned int param );
	static unsigned int __stdcall EnumStationCallback( wchar_t* name, unsigned int param );
	static unsigned int __stdcall EnumDesktopWindowsCallback( HWND hwnd, unsigned int param );
	static void __stdcall TimerCallback( HWND, UINT, UINT_PTR, DWORD );
public:
	//
	void SetEnumInterval( unsigned int interval )	{ m_uiEnumInterval = interval; }

	unsigned int GetEnumInterval() const	{ return m_uiEnumInterval; }
	XMLLog& GetLog()	{ return m_Log; }
	DesktopVec& GetDesktopVec()	{ return m_DesktopVec; }
protected:
	//Data members
	WindowAttributeVec	m_WindowAttributeVec;
	DesktopVec			m_DesktopVec;
	HWINSTA				m_hWindowStation;
	XMLLog				m_Log;
	unsigned int		m_uiEnumInterval;
};